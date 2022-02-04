--32bit Full Adder logic
--This can be done easily as we have the '+' operator in VHDL

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

--interface for 32bit adder
entity int32add is 
	port (
		a: IN integer;
		b: IN integer;
		sum: OUT integer
	);
end int32add;

--specification of 32bit adder
architecture add32_type0 of int32add is
begin
	sum <= a + b;
end add32_type0;