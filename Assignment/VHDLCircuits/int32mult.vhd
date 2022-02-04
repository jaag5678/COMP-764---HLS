--32-bit multiplication logic 

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_signed.all;
use IEEE.numeric_std.all;

--interface for 32bit adder
entity int32mult is 
	port (
		a: IN integer;
		b: IN integer;
		prod: OUT integer
	);
end int32mult;

--specification of 32bit adder
architecture mult32_type0 of int32mult is
signal i: integer;
begin
		prod <= a * b;
end mult32_type0;