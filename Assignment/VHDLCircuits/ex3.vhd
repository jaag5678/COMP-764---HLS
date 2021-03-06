library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity ex3 is 
port ( 
clk_f: IN std_logic;
q: OUT integer
);
end ex3;

architecture hls of ex3 is 

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

signal sel_A, sel_B, sel_C: integer; 
signal we_A, we_B, we_C: std_logic;
signal ar_A, aw_A, ar_B, aw_B, ar_C, aw_C: std_logic_vector (31 downto 0);
signal din_A, dout_A, din_B, dout_B, din_C, dout_C: integer;
signal r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14: integer; 

signal sel_A0: integer; 

signal sel_M0: integer; 

signal iA0_0, iA0_1: integer; 

signal oA0: integer; 

signal iM0_0, iM0_1: integer; 

signal oM0: integer; 

signal state: integer := 0;


begin
r0 <= dout_A when sel_A <= 0;
r2 <= dout_A when sel_A <= 1;
r6 <= dout_A when sel_A <= 2;
r10 <= dout_A when sel_A <= 3;
A: int32dualportRAM port map(clk_f, we_A, aw_A, ar_A, din_A, dout_A);

r1 <= dout_B when sel_B <= 0;
r3 <= dout_B when sel_B <= 1;
r7 <= dout_B when sel_B <= 2;
r11 <= dout_B when sel_B <= 3;
B: int32dualportRAM port map(clk_f, we_B, aw_B, ar_B, din_B, dout_B);

C: int32dualportRAM port map(clk_f, we_C, aw_C, ar_C, din_C, dout_C);

iA0_0 <= r4 when sel_A0 <= 0 else 
r8 when sel_A0 <= 1 else 
r12 when sel_A0 <= 2 ;
iA0_1 <= r5 when sel_A0 <= 0 else 
r9 when sel_A0 <= 1 else 
r13 when sel_A0 <= 2 ;
r8 <= oA0 when sel_A0 <= 0;
r12 <= oA0 when sel_A0 <= 1;
r14 <= oA0 when sel_A0 <= 2;
A0: int32add port map(iA0_0, iA0_1, oA0);
iM0_0 <= r0 when sel_M0 <= 0 else 
r2 when sel_M0 <= 1 else 
r6 when sel_M0 <= 2 else 
r10 when sel_M0 <= 3 ;
iM0_1 <= r1 when sel_M0 <= 0 else 
r3 when sel_M0 <= 1 else 
r7 when sel_M0 <= 2 else 
r11 when sel_M0 <= 3 ;
r4 <= oM0 when sel_M0 <= 0;
r5 <= oM0 when sel_M0 <= 1;
r9 <= oM0 when sel_M0 <= 2;
r13 <= oM0 when sel_M0 <= 3;
M0: int32mult port map(iM0_0, iM0_1, oM0);


process(clk_f)
begin

if (clk_f'event and clk_f='1') then 
if (state = 0) then 
ar_A <= std_logic_vector(to_unsigned(0, 32));
sel_A <= 0;
ar_B <= std_logic_vector(to_unsigned(0, 32));
sel_B <= 0;
state <= 1;

elsif (state = 1) then 
sel_M0 <= 0;
ar_A <= std_logic_vector(to_unsigned(1, 32));
sel_A <= 1;
ar_B <= std_logic_vector(to_unsigned(1, 32));
sel_B <= 1;
state <= 2;

elsif (state = 2) then 
sel_M0 <= 1;
ar_A <= std_logic_vector(to_unsigned(2, 32));
sel_A <= 2;
ar_B <= std_logic_vector(to_unsigned(2, 32));
sel_B <= 2;
state <= 3;

elsif (state = 3) then 
sel_A0 <= 0;
sel_M0 <= 2;
ar_A <= std_logic_vector(to_unsigned(3, 32));
sel_A <= 3;
ar_B <= std_logic_vector(to_unsigned(3, 32));
sel_B <= 3;
state <= 4;

elsif (state = 4) then 
sel_A0 <= 1;
sel_M0 <= 3;
state <= 5;

elsif (state = 5) then 
sel_A0 <= 2;
state <= 6;

elsif (state = 6) then 
aw_C <= std_logic_vector(to_unsigned(0, 32));
we_C <= '1';
state <= 7;
else 
 
end if; 


end if;
end process;

q <= 1;

end hls;
