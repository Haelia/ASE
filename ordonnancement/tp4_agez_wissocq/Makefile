all:main


%.o : %.c %.h
	gcc -c $<

main : main.c
	gcc -o main hw.c semaphore.c ordonnancement.c main.c -m32 -g

clean:
	rm -f *~ *.o *#
	rm -f main
