--testbench for dual port RAM int 32 bits

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity testbench_int32dualportRAM is 
--empty;
end testbench_int32dualportRAM;


architecture tb of testbench_int32dualportRAM is 

	component int32dualportRAM is 
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
	end component;
	
	signal clk_in, we_in: std_logic;
	signal addr_w_in, addr_r_in: std_logic_vector (2 downto 0);
	signal d_in_in: integer;
	signal d_out_out: integer;
	signal reg: integer;
	
	begin
		
		reg <= d_out_out when addr_r_in <= "001";
	
		DUT: int32dualportRAM PORT MAP(clk_in, we_in, addr_w_in, addr_r_in, d_in_in, d_out_out);
	
		process 
		begin
			
			clk_in <='0';
			we_in <= '0';
			d_in_in <= 20;
			addr_w_in <= "001";
			addr_r_in <= "010";
			wait for 0.5 ns;
			assert(d_out_out = -2147483648) report "Fail init" severity error; 
			
			clk_in <='1';
			wait for 0.5 ns;
			assert(d_out_out = -2147483648) report "Fail init" severity error; 
			
			clk_in <= '0';
			we_in <= '1';
			wait for 0.5 ns;
			assert(d_out_out = -2147483648) report "Fail init" severity error; 
			
			clk_in <= '1';
			wait for 0.5 ns;
			assert(d_out_out = -2147483648) report "Fail init" severity error; 
			
			clk_in <= '0';
			addr_r_in <= "001";
			assert(reg = 20) report "Fail to read" severity error; 
			
			clk_in <='1';
			wait for 0.5 ns;
			
			assert false report "testing done" severity note; 
			wait; 
		end process;
end tb;