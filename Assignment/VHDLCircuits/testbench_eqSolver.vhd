--testbench for equation solver

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity testbench_eqSolver is
--empty
end testbench_eqSolver;

architecture tb of testbench_eqSolver is
	
	component eqSolver is
		port (
			clk: IN std_logic;
			lr0: IN std_logic_vector (1 downto 0);
			lr1: IN std_logic_vector (1 downto 0);
			lr2: IN std_logic_vector (1 downto 0);
			lw: IN std_logic_vector (1 downto 0);
			q: OUT integer 
		);
	end component;
	
	signal clk_in: std_logic;
	signal r0, r1, r2, w : std_logic_vector(1 downto 0);
	signal res: integer;
	
	begin
	
		DUT: eqSolver PORT MAP(clk_in, r0, r1, r2, w, res);
		process 
		begin 
		
		r0 <= "00";
		r1 <= "01";
		r2 <= "10";
		w <= "11";
		
		--one clock cycle
		clk_in <= '0';
		wait for 0.5 ns;
		clk_in <= '1';
		wait for 0.5 ns;
		
		--second clock cycle
		clk_in <= '0';
		wait for 0.5 ns;
		clk_in <= '1';
		wait for 0.5 ns;
		
		--third clock cycle
		clk_in <= '0';
		wait for 0.5 ns;
		clk_in <= '1';
		wait for 0.5 ns;
		
		--fourth clock cycle
		clk_in <= '0';
		wait for 0.5 ns;
		clk_in <= '1';
		wait for 0.5 ns;
		
		--fifth clock cycle
		clk_in <= '0';
		wait for 0.5 ns;
		clk_in <= '1';
		wait for 0.5 ns;
		
		--sixth clock cycle
		clk_in <= '0';
		wait for 0.5 ns;
		clk_in <= '1';
		wait for 0.5 ns;
		
		--seventh clock cycle
		clk_in <= '0';
		wait for 0.5 ns;
		clk_in <= '1';
		wait for 0.5 ns;
		
		assert(res = 20) report "Failed" severity error;
		
		wait;
		end process;
		
end tb;