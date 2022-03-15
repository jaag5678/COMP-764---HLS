--testbench for ex4 circuit

--testbench for equation solver

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity test_ex4 is
--empty
end test_ex4;

architecture tb of test_ex4 is
	
	component ex4 is
		port (
			clk_f: IN std_logic;
			q: OUT integer 
		);
	end component;
	
	signal clk_in: std_logic;
	signal res: integer;
	
	begin
	
		DUT: ex4 PORT MAP(clk_in, res);
		
		process
		begin 
		
		clk_in <= '0';
		--one clock cycle
		wait for 1 ns;
		clk_in <= '1';
		wait for 1 ns; 
		--second clock cycle
		
		end process;
		
end tb;