#Makefile tutorial usage for project

#Declaring variable to input as our vhdl target code 
#vhdl_code1 := "vhdl.c" 
vhdl := a c d s g

all: v1 v2 v3 v4 v5 v6 s1

#First we declare the final target
v1: $(vhdl) vhdl.c
	gcc $(vhdl) vhdl.c -o v1

v2: $(vhdl) vhdlex2.c 
	gcc $(vhdl) vhdlex2.c -o v2

v3: $(vhdl) vhdlex3.c 
	gcc $(vhdl) vhdlex3.c -o v3

v4: $(vhdl) vhdlex4.c 
	gcc $(vhdl) vhdlex4.c -o v4

v5: $(vhdl) vhdlex5.c 
	gcc $(vhdl) vhdlex5.c -o v5

v6: $(vhdl) vhdlex6.c 
	gcc $(vhdl) vhdlex6.c -o v6

s1: $(vhdl) shm1.c 
	gcc $(vhdl) shm1.c -o shm1

s2: $(vhdl) shm2.c 
	gcc $(vhdl) shm2.c -o s2

s3: $(vhdl) shm3.c 
	gcc $(vhdl) shm3.c -o s3

s4: $(vhdl) shm4.c 
	gcc $(vhdl) shm4.c -o s4

s5: $(vhdl) shm5.c 
	gcc $(vhdl) shm5.c -o s5

s6: $(vhdl) shm6.c 
	gcc $(vhdl) shm6.c -o s6

s7: $(vhdl) shm7.c 
	gcc $(vhdl) shm7.c -o s7

st: $(vhdl) shm_total.c
	gcc $(vhdl) shm_total.c -o st

#Now the dependencies
a: ast.c 
	gcc -c ast.c -o a

c: cdfg.c 
	gcc -c cdfg.c -o c

d: dpath.c 
	gcc -c dpath.c -o d

s: schd.c 
	gcc -c schd.c -o s

g: codegen.c 
	gcc -c codegen.c -o g 


#Cleanup files and restart
clean: 
	rm -f a c s d g v vhdl