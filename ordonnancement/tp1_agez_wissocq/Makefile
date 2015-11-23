all:display_stack try_mul

try.o:try.c
	gcc -c try.c -m32 -g

display_stack:display_stack.c
	gcc -o display_stack display_stack.c -m32

try_mul:try_mul.c try.o
	gcc -o try_mul try.c try_mul.c -m32 -g

clean:
	rm -f *~ *.o *#
	rm display_stack
	rm try_mul
