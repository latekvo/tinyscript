BUILD_DIR=build/
SOURCE_DIR=src/

default: tinyscript

tinyscript: src/main.o src/conversions.o src/separators.o
	gcc -g -o tinyscript $^ 
	rm -f *.o

clean:
	rm -f *.o tinyscript
