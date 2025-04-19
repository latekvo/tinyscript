default: tinyscript

tinyscript: main.o conversions.o separators.o
	gcc -g -o tinyscript main.o conversions.o separators.o

main.o:
	gcc -g -c main.c

conversions.o:
	gcc -g -c conversions.c

separators.o:
	gcc -g -c separators.c 


clean:
	rm -f *.o
