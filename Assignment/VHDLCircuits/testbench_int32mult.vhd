--testbench for multiplication
--testbench for adding 32-bit integers
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity testbench_int32mult is
--empty
end testbench_int32mult;

architecture tb of testbench_int32mult is
	
	component int32mult is
		port (
			a: IN integer;
			b: IN integer;
			prod: OUT integer
		);
	end component;
	
	signal a_in, b_in, prod_in: integer;
	
	begin
	
		DUT: int32mult PORT MAP(a_in, b_in, prod_in);
		process 
		begin 
		
		a_in <= 1073741824;
		b_in <= 2;
		wait for 1 ns;
		assert (prod_in = -2147483648) report "Fail 25/32" severity error;
		
		wait;
		end process;
		
end tb;