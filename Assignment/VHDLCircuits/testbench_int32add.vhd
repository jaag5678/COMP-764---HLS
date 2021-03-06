--testbench for adding 32-bit integers
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_signed.all;
use IEEE.numeric_std.all;

entity testbench_int32add is
--empty
end testbench_int32add;

architecture tb of testbench_int32add is
	
	component int32add is
		port (
			a: IN integer;
			b: IN integer;
			sum: OUT integer
		);
	end component;
	
	signal a_in, b_in, sum_in: integer;
	
	begin
	
		DUT: int32add PORT MAP(a_in, b_in, sum_in);
		process 
		begin 
		
		a_in <= 2147483647;
		b_in <= 1;
		wait for 1 ns;
		assert (sum_in = -2147483648) report "Fail 25/32" severity error;
		
		wait;
		end process;
		
end tb;