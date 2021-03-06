library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity ex4 is 
port ( 
clk_f: IN std_logic;
q: OUT integer
);
end ex4;

architecture hls of ex4 is 

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
signal r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r20, r21, r22, r23, r24, r25, r26, r27, r28, r29, r30, r31, r32, r33, r34, r35, r36, r37, r38, r39, r40, r41, r42, r43: integer; 

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
r13 <= dout_A when sel_A <= 4;
r17 <= dout_A when sel_A <= 5;
r21 <= dout_A when sel_A <= 6;
r24 <= dout_A when sel_A <= 7;
r28 <= dout_A when sel_A <= 8;
r32 <= dout_A when sel_A <= 9;
r35 <= dout_A when sel_A <= 10;
r39 <= dout_A when sel_A <= 11;
A: int32dualportRAM port map(clk_f, we_A, aw_A, ar_A, din_A, dout_A);

r1 <= dout_W when sel_W <= 0;
r3 <= dout_W when sel_W <= 1;
r7 <= dout_W when sel_W <= 2;
r11 <= dout_W when sel_W <= 3;
r14 <= dout_W when sel_W <= 4;
r18 <= dout_W when sel_W <= 5;
r22 <= dout_W when sel_W <= 6;
r25 <= dout_W when sel_W <= 7;
r29 <= dout_W when sel_W <= 8;
r33 <= dout_W when sel_W <= 9;
r36 <= dout_W when sel_W <= 10;
r40 <= dout_W when sel_W <= 11;
W: int32dualportRAM port map(clk_f, we_W, aw_W, ar_W, din_W, dout_W);

C: int32dualportRAM port map(clk_f, we_C, aw_C, ar_C, din_C, dout_C);

iA0_0 <= r4 when sel_A0 <= 0 else 
r8 when sel_A0 <= 1 else 
r15 when sel_A0 <= 2 else 
r19 when sel_A0 <= 3 else 
r26 when sel_A0 <= 4 else 
r30 when sel_A0 <= 5 else 
r37 when sel_A0 <= 6 else 
r41 when sel_A0 <= 7 ;
iA0_1 <= r5 when sel_A0 <= 0 else 
r9 when sel_A0 <= 1 else 
r16 when sel_A0 <= 2 else 
r20 when sel_A0 <= 3 else 
r27 when sel_A0 <= 4 else 
r31 when sel_A0 <= 5 else 
r38 when sel_A0 <= 6 else 
r42 when sel_A0 <= 7 ;
r8 <= oA0 when sel_A0 <= 0;
r12 <= oA0 when sel_A0 <= 1;
r19 <= oA0 when sel_A0 <= 2;
r23 <= oA0 when sel_A0 <= 3;
r30 <= oA0 when sel_A0 <= 4;
r34 <= oA0 when sel_A0 <= 5;
r41 <= oA0 when sel_A0 <= 6;
r43 <= oA0 when sel_A0 <= 7;

A0: int32add port map(iA0_0, iA0_1, oA0);

iM0_0 <= r0 when sel_M0 <= 0 else 
r2 when sel_M0 <= 1 else 
r6 when sel_M0 <= 2 else 
r10 when sel_M0 <= 3 else 
r13 when sel_M0 <= 4 else 
r17 when sel_M0 <= 5 else 
r21 when sel_M0 <= 6 else 
r24 when sel_M0 <= 7 else 
r28 when sel_M0 <= 8 else 
r32 when sel_M0 <= 9 else 
r35 when sel_M0 <= 10 else 
r39 when sel_M0 <= 11 ;
iM0_1 <= r1 when sel_M0 <= 0 else 
r3 when sel_M0 <= 1 else 
r7 when sel_M0 <= 2 else 
r11 when sel_M0 <= 3 else 
r14 when sel_M0 <= 4 else 
r18 when sel_M0 <= 5 else 
r22 when sel_M0 <= 6 else 
r25 when sel_M0 <= 7 else 
r29 when sel_M0 <= 8 else 
r33 when sel_M0 <= 9 else 
r36 when sel_M0 <= 10 else 
r40 when sel_M0 <= 11 ;
r4 <= oM0 when sel_M0 <= 0;
r5 <= oM0 when sel_M0 <= 1;
r9 <= oM0 when sel_M0 <= 2;
r15 <= oM0 when sel_M0 <= 3;
r16 <= oM0 when sel_M0 <= 4;
r20 <= oM0 when sel_M0 <= 5;
r26 <= oM0 when sel_M0 <= 6;
r27 <= oM0 when sel_M0 <= 7;
r31 <= oM0 when sel_M0 <= 8;
r37 <= oM0 when sel_M0 <= 9;
r38 <= oM0 when sel_M0 <= 10;
r42 <= oM0 when sel_M0 <= 11;

M0: int32mult port map(iM0_0, iM0_1, oM0);



process(clk_f)
begin

if (clk_f'event and clk_f='1') then 
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
state <= 3;

elsif (state = 3) then 
sel_A0 <= 0;
sel_M0 <= 2;
ar_A <= std_logic_vector(to_unsigned(1, 32));
sel_A <= 3;
ar_W <= std_logic_vector(to_unsigned(0, 32));
sel_W <= 3;
state <= 4;

elsif (state = 4) then 
sel_A0 <= 1;
sel_M0 <= 3;
ar_A <= std_logic_vector(to_unsigned(2, 32));
sel_A <= 4;
ar_W <= std_logic_vector(to_unsigned(1, 32));
sel_W <= 4;
state <= 5;

elsif (state = 5) then 
din_C <= r12;
aw_C <= std_logic_vector(to_unsigned(0, 32));
we_C <= '1';
sel_M0 <= 4;
ar_A <= std_logic_vector(to_unsigned(3, 32));
sel_A <= 5;
ar_W <= std_logic_vector(to_unsigned(2, 32));
sel_W <= 5;
state <= 6;

elsif (state = 6) then 
sel_A0 <= 2;
sel_M0 <= 5;
ar_A <= std_logic_vector(to_unsigned(2, 32));
sel_A <= 6;
ar_W <= std_logic_vector(to_unsigned(0, 32));
sel_W <= 6;
state <= 7;

elsif (state = 7) then 
sel_A0 <= 3;
sel_M0 <= 6;
ar_A <= std_logic_vector(to_unsigned(3, 32));
sel_A <= 7;
ar_W <= std_logic_vector(to_unsigned(1, 32));
sel_W <= 7;
state <= 8;

elsif (state = 8) then 
din_C <= r23;
aw_C <= std_logic_vector(to_unsigned(1, 32));
we_C <= '1';
sel_M0 <= 7;
ar_A <= std_logic_vector(to_unsigned(4, 32));
sel_A <= 8;
ar_W <= std_logic_vector(to_unsigned(2, 32));
sel_W <= 8;
state <= 9;

elsif (state = 9) then 
sel_A0 <= 4;
sel_M0 <= 8;
ar_A <= std_logic_vector(to_unsigned(3, 32));
sel_A <= 9;
ar_W <= std_logic_vector(to_unsigned(0, 32));
sel_W <= 9;
state <= 10;

elsif (state = 10) then 
sel_A0 <= 5;
sel_M0 <= 9;
ar_A <= std_logic_vector(to_unsigned(4, 32));
sel_A <= 10;
ar_W <= std_logic_vector(to_unsigned(1, 32));
sel_W <= 10;
state <= 11;

elsif (state = 11) then 
din_C <= r34;
aw_C <= std_logic_vector(to_unsigned(2, 32));
we_C <= '1';
sel_M0 <= 10;
ar_A <= std_logic_vector(to_unsigned(5, 32));
sel_A <= 11;
ar_W <= std_logic_vector(to_unsigned(2, 32));
sel_W <= 11;
state <= 12;

elsif (state = 12) then 
sel_A0 <= 6;
sel_M0 <= 11;
state <= 13;

elsif (state = 13) then 
sel_A0 <= 7;
state <= 14;

elsif (state = 14) then 
din_C <= r43;
aw_C <= std_logic_vector(to_unsigned(3, 32));
we_C <= '1';
state <= 15;
else 
 
end if; 


end if;
end process;

q <= state;

end hls;
