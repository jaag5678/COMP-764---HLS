#Makefile tutorial usage for project

#Declaring variable to input as our vhdl target code 
#vhdl_code1 := "vhdl.c" 
vhdl := a c d s g

all: v1 v2 v3 v4 v5

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