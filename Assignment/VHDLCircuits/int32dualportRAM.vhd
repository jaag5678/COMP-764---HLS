--dual port RAM for 32bit integers 

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

--interface 
	-- inputs 
		-- clk 
		-- we - write enable
		-- addr_w - address to write to
		-- addr_r - address to read from and assign to output
		-- addr_width - number of width bits (to denote number of addresses)
		-- d_in - if we == 1 then set value at addr_w to d_in
		-- d_out - set value read from addr_r to d_out
		
entity int32dualportRAM is 
	generic (
		addr_width: integer := 3
		);
	port (
		clk: IN std_logic;
		we: IN std_logic;
		addr_w: IN std_logic_vector (addr_width - 1 downto 0);
		addr_r: IN std_logic_vector (addr_width - 1 downto 0);
		d_in: IN integer;
		d_out: OUT integer
	);
end int32dualportRAM;

--specification 
architecture dpRAM of int32dualportRAM is
	
	type ram_type is array (2 ** addr_width - 1 downto 0) of integer;
	signal ram_dual_port: ram_type;

begin
	process(clk)
	begin
		if(clk'event and clk='1') then
			if(we = '1') then
				ram_dual_port(to_integer(unsigned(addr_w))) <= d_in;
			end if;
		end if;
	end process;

	--why this assignment is not dependant on clock event with rising edge? 
	d_out <= ram_dual_port(to_integer(unsigned(addr_r)));

end dpRAM;