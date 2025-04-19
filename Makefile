BUILD_DIR=build/
SOURCE_DIR=src/

default: tinyscript
.PHONY: clean run

tinyscript: build/main.o build/conversions.o build/separators.o
	gcc -g -o tinyscript $^ 

build:
	mkdir $(BUILD_DIR)

build/%.o: build
	gcc -g -c -o $@ $(subst $(BUILD_DIR),$(SOURCE_DIR),$(subst .o,.c,$@))


clean:
	rm -r $(BUILD_DIR) tinyscript

run: tinyscript
	./tinyscript test_files/simple.js
	make clean
