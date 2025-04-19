BUILD_DIR=build/
SOURCE_DIR=src/
TARGET=tinyscript
CC=gcc
CFLAGS=-g
MEMCHECK_CMD=valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt


OBJECTS=$(subst $(SOURCE_DIR),$(BUILD_DIR),$(subst .c,.o,$(wildcard $(SOURCE_DIR)*.c)))

default: $(TARGET) 
.PHONY: clean test 

$(TARGET): $(OBJECTS) 
	gcc $(CFLAGS) -o $@ $^ 

$(BUILD_DIR)%.o: $(SOURCE_DIR)$(subst .o,.c,$(notdir $@))
	mkdir -p $(BUILD_DIR)
	gcc $(CFLAGS) -c -o $@ $(SOURCE_DIR)$(notdir $(subst .o,.c,$@))


clean:
	rm -r $(BUILD_DIR) $(TARGET)

test: $(TARGET)
	# temporary testing solution, will add more robust testing in future
	./$(TARGET) test_files/simple.js  

memcheck: $(TARGET)
	$(MEMCHECK_CMD) ./$(TARGET) test_files/simple.js 
