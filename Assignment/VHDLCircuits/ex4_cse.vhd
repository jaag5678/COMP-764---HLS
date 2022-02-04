library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity ex4_cse is 
port ( 
clk: IN std_logic;
q: OUT integer
);
end ex4_cse;

architecture hls of ex4_cse is 

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

signal sel_A, sel_W, sel_C: integer; 
signal we_A, we_W, we_C: std_logic;
signal ar_A, aw_A, ar_W, aw_W, ar_C, aw_C: std_logic_vector (31 downto 0);
signal din_A, dout_A, din_W, dout_W, din_C, dout_C: integer;
signal r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r12, r15, r16, r17, r19, r20, r23, r26, r27, r28, r30, r31, r34, r37, r38, r39, r41, r42, r43: integer; 

signal sel_A0, sel_A1: integer; 

signal sel_M0, sel_M1, sel_M2: integer; 

signal iA0_0, iA0_1, iA1_0, iA1_1: integer; 

signal oA0, oA1: integer; 

signal iM0_0, iM0_1, iM1_0, iM1_1, iM2_0, iM2_1: integer; 

signal oM0, oM1, oM2: integer; 

signal state: integer := 0;


begin
r0 <= dout_A when sel_A <= 0;
r2 <= dout_A when sel_A <= 1;
r6 <= dout_A when sel_A <= 2;
r17 <= dout_A when sel_A <= 3;
r28 <= dout_A when sel_A <= 4;
r39 <= dout_A when sel_A <= 5;
A: int32dualportRAM port map(clk, we_A, aw_A, ar_A, din_A, dout_A);

r1 <= dout_W when sel_W <= 0;
r3 <= dout_W when sel_W <= 1;
r7 <= dout_W when sel_W <= 2;
W: int32dualportRAM port map(clk, we_W, aw_W, ar_W, din_W, dout_W);

C: int32dualportRAM port map(clk, we_C, aw_C, ar_C, din_C, dout_C);

iA0_0 <= r4 when sel_A0 <= 0 else 
r8 when sel_A0 <= 1 else 
r19 when sel_A0 <= 2 else 
r30 when sel_A0 <= 3 else 
r41 when sel_A0 <= 4 ;
iA0_1 <= r5 when sel_A0 <= 0 else 
r9 when sel_A0 <= 1 else 
r20 when sel_A0 <= 2 else 
r31 when sel_A0 <= 3 else 
r42 when sel_A0 <= 4 ;
r8 <= oA0 when sel_A0 <= 0;
r12 <= oA0 when sel_A0 <= 1;
r23 <= oA0 when sel_A0 <= 2;
r34 <= oA0 when sel_A0 <= 3;
r43 <= oA0 when sel_A0 <= 4;
iA1_0 <= r15 when sel_A1 <= 0 else 
r26 when sel_A1 <= 1 else 
r37 when sel_A1 <= 2 ;
iA1_1 <= r16 when sel_A1 <= 0 else 
r27 when sel_A1 <= 1 else 
r38 when sel_A1 <= 2 ;
r19 <= oA1 when sel_A1 <= 0;
r30 <= oA1 when sel_A1 <= 1;
r41 <= oA1 when sel_A1 <= 2;
A0: int32add port map(iA0_0, iA0_1, oA0);
A1: int32add port map(iA1_0, iA1_1, oA1);
iM0_0 <= r0 when sel_M0 <= 0 else 
r2 when sel_M0 <= 1 else 
r6 when sel_M0 <= 2 else 
r17 when sel_M0 <= 3 else 
r28 when sel_M0 <= 4 else 
r39 when sel_M0 <= 5 ;
iM0_1 <= r1 when sel_M0 <= 0 else 
r3 when sel_M0 <= 1 else 
r7 when sel_M0 <= 2 else 
r7 when sel_M0 <= 3 else 
r7 when sel_M0 <= 4 else 
r7 when sel_M0 <= 5 ;
r4 <= oM0 when sel_M0 <= 0;
r5 <= oM0 when sel_M0 <= 1;
r9 <= oM0 when sel_M0 <= 2;
r20 <= oM0 when sel_M0 <= 3;
r31 <= oM0 when sel_M0 <= 4;
r42 <= oM0 when sel_M0 <= 5;
iM1_0 <= r2 when sel_M1 <= 0 else 
r6 when sel_M1 <= 1 else 
r17 when sel_M1 <= 2 else 
r28 when sel_M1 <= 3 ;
iM1_1 <= r1 when sel_M1 <= 0 else 
r3 when sel_M1 <= 1 else 
r3 when sel_M1 <= 2 else 
r3 when sel_M1 <= 3 ;
r15 <= oM1 when sel_M1 <= 0;
r16 <= oM1 when sel_M1 <= 1;
r27 <= oM1 when sel_M1 <= 2;
r38 <= oM1 when sel_M1 <= 3;
iM2_0 <= r6 when sel_M2 <= 0 else 
r17 when sel_M2 <= 1 ;
iM2_1 <= r1 when sel_M2 <= 0 else 
r1 when sel_M2 <= 1 ;
r26 <= oM2 when sel_M2 <= 0;
r37 <= oM2 when sel_M2 <= 1;
M0: int32mult port map(iM0_0, iM0_1, oM0);
M1: int32mult port map(iM1_0, iM1_1, oM1);
M2: int32mult port map(iM2_0, iM2_1, oM2);


process(clk)
begin

if (clk'event and clk='1') then 
if (state = 0) then 
ar_A <= std_logic_vector(to_unsigned(0, 32));
sel_A <= 0;
ar_W <= std_logic_vector(to_unsigned(0, 32));
sel_W <= 0;
state <= 1;

elsif (state = 1) then 
sel_M0 <= 0;
ar_A <= std_logic_vector(to_unsigned(1, 32));
sel_A <= 1;
ar_W <= std_logic_vector(to_unsigned(1, 32));
sel_W <= 1;
state <= 2;

elsif (state = 2) then 
sel_M0 <= 1;
ar_A <= std_logic_vector(to_unsigned(2, 32));
sel_A <= 2;
ar_W <= std_logic_vector(to_unsigned(2, 32));
sel_W <= 2;
sel_M1 <= 0;
state <= 3;

elsif (state = 3) then 
sel_A0 <= 0;
sel_M0 <= 2;
sel_M1 <= 1;
ar_A <= std_logic_vector(to_unsigned(3, 32));
sel_A <= 3;
sel_M2 <= 0;
state <= 4;

elsif (state = 4) then 
sel_A0 <= 1;
sel_A1 <= 0;
sel_M0 <= 3;
sel_M1 <= 2;
ar_A <= std_logic_vector(to_unsigned(4, 32));
sel_A <= 4;
sel_M2 <= 1;
state <= 5;

elsif (state = 5) then 
aw_C <= std_logic_vector(to_unsigned(0, 32));
we_C <= '1';
sel_A0 <= 2;
sel_A1 <= 1;
sel_M0 <= 4;
sel_M1 <= 3;
ar_A <= std_logic_vector(to_unsigned(5, 32));
sel_A <= 5;
state <= 6;

elsif (state = 6) then 
aw_C <= std_logic_vector(to_unsigned(1, 32));
we_C <= '1';
sel_A0 <= 3;
sel_A1 <= 2;
sel_M0 <= 5;
state <= 7;

elsif (state = 7) then 
aw_C <= std_logic_vector(to_unsigned(2, 32));
we_C <= '1';
sel_A0 <= 4;
state <= 8;

elsif (state = 8) then 
aw_C <= std_logic_vector(to_unsigned(3, 32));
we_C <= '1';
state <= 9;

elsif (state = 9) then 
state <= 10;
else 
 
end if; 


end if;
end process;

q <= 1;

end hls;
