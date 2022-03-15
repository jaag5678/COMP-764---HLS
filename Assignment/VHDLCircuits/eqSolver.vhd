--Building a circuit for the program 
	-- mem = malloc(int * 4);
	-- mem[3] = mem[0] * mem[1] + mem[2];
	
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

--interface of circuit
	--this is only solving equation 
	--so not required input values
	--input can then be from where to read the values using which we want to solve equation
entity eqSolver is
	port (
	--since the solver works on clock cycles, we need clock
	clk_eq: IN std_logic;
	--inputs can be the address locations from where to read/write 
	lr0: IN std_logic_vector (1 downto 0);
	lr1: IN std_logic_vector (1 downto 0);
	lr2: IN std_logic_vector (1 downto 0);
	lw: IN std_logic_vector (1 downto 0);
	q: OUT integer 
	);
end eqSolver;

--specification
architecture calc_circuit of eqSolver is 

	--First we need a RAM component 
	component int32dualportRAM is 
		generic (addr_width: integer := 2);
		port (
			clk: IN std_logic;
			we: IN std_logic;
			addr_w: IN std_logic_vector (1 downto 0);
			addr_r: IN std_logic_vector (1 downto 0);
			d_in: IN integer;
			d_out: OUT integer
		);
	end component;
	
	--Next we require an adder of 2 integers
	component int32add is
		port (
			a: IN integer;
			b: IN integer;
			sum: OUT integer
		);
	end component;
	
	--We also require a multiplier of 2 integers
	component int32mult is
		port (
		a: IN integer;
		b: IN integer;
		prod: OUT integer
	);
	end component;
	
	--all the temp signals (wires) for our circuits 
	
	--for ram
	signal we0: std_logic;
	signal aw, ar: std_logic_vector (1 downto 0);
	signal ri, ro: integer;
	
	--for adder
	signal prod01, e2, res_sum: integer;

	--for multiplier
	signal e0, e1, res_prod: integer;
	
	--type can be perhaps of a better one to save space
	signal state: integer := 0;
	
	begin
	
		ram: int32dualportRAM port map(clk_eq, we0, aw, ar, ri, ro);
		mult0: int32mult port map(e0, e1, res_prod);
		adder0: int32add port map(prod01, e2, res_sum);
	
		process (clk_eq)
		begin	
		
		if (clk_eq'event and clk_eq='1') then
			if(state = 0) then
				--read from location 0
				ar <= std_logic_vector(signed(lr0));
				state <= 1;	
			elsif(state = 1) then
				--read from location 1 and also set the value of e0 
				e0 <= ro;
				--e0 <= 5;
				ar <= lr1;
				state <= 2;
			elsif (state = 2) then
				--perform the product by setting the valye of e1
				e1 <= ro;
				--e1 <= 2;
				state <= 3;
			elsif (state = 3) then
				--set prod to the first input for adder
				prod01 <= res_prod;
				--read from location 2
				ar <= lr2;
				state <= 4;
			elsif (state = 4) then
				-- perform addition by setting value of e2
				e2 <= ro;
				--e2 <= 10;
				state <= 5;
				--also set the ram to be ready to write to location 3
			elsif (state = 5) then
				--set the ram input to the expression solution
				ri <= res_sum;
				--set the write enable to enable the ram to write
				we0 <= '1';
				aw <= lw;
				ar <= lw;
				--perform the store
				--do not need to do anything here
			else 
				-- do nothing
			end if;
		end if;
	
		end process;
		
		q <= ro;
		
end calc_circuit;

