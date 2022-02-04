library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity ex1 is 
port ( 
clk: IN std_logic;
q: OUT integer
);
end ex1;

architecture hls of ex1 is 

component int32dualportRAM is 
generic (addr_width: integer := 32);
port (
clk: IN std_logic; 
we: IN std_logic; 
addr_w: IN std_logic_vector (addr_width - 1 downto 0); 
addr_r: IN std_logic_vector (addr_width - 1 downto 0); 
d_in: IN integer; 
 d_out: OUT integer 
);
end component;

component int32add is 
port (
a: IN integer;
b: IN integer;
sum: OUT integer
);
end component;

component int32mult is 
port (
a: IN integer;
b: IN integer;
prod: OUT integer
);
end component;

signal sel_mem: integer; 
signal we_mem: std_logic;
signal ar_mem, aw_mem: std_logic_vector (31 downto 0);
signal din_mem, dout_mem: integer;
signal r0, r1, r2, r3, r4: integer; 

signal sel_A0: integer; 

signal sel_M0: integer; 

signal iA0_0, iA0_1: integer; 

signal oA0: integer; 

signal iM0_0, iM0_1: integer; 

signal oM0: integer; 

signal state: integer := 0;


begin
r0 <= dout_mem when sel_mem <= 0;
r1 <= dout_mem when sel_mem <= 1;
r3 <= dout_mem when sel_mem <= 2;
mem: int32dualportRAM port map(clk, we_mem, aw_mem, ar_mem, din_mem, dout_mem);

iA0_0 <= r2 when sel_A0 <= 0 ;
iA0_1 <= r3 when sel_A0 <= 0 ;
r4 <= oA0 when sel_A0 <= 0;
A0: int32add port map(iA0_0, iA0_1, oA0);
iM0_0 <= r0 when sel_M0 <= 0 ;
iM0_1 <= r1 when sel_M0 <= 0 ;
r2 <= oM0 when sel_M0 <= 0;
M0: int32mult port map(iM0_0, iM0_1, oM0);


process(clk)
begin

if (clk'event and clk='1') then 
if (state = 0) then 
ar_mem <= std_logic_vector(to_unsigned(0, 32));
sel_mem <= 0;
state <= 1;

elsif (state = 1) then 
ar_mem <= std_logic_vector(to_unsigned(1, 32));
sel_mem <= 1;
state <= 2;

elsif (state = 2) then 
sel_M0 <= 0;
ar_mem <= std_logic_vector(to_unsigned(2, 32));
sel_mem <= 2;
state <= 3;

elsif (state = 3) then 
sel_A0 <= 0;
state <= 4;

elsif (state = 4) then 
aw_mem <= std_logic_vector(to_unsigned(3, 32));
we_mem <= '1';
state <= 5;
else 
 
end if; 


end if;
end process;

q <= 1;

end hls;
