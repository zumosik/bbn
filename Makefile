BUILD_DIR := build
RESULT_NAME := bnn # name of executable 

clean:
	@ rm -rf $(BUILD_DIR)

test:
	# TODO

debug:
	@ $(MAKE) -f src/makefile NAME=$(RESULT_NAME) MODE=debug SOURCE_DIR=src 

release:
	@ $(MAKE) -f src/makefile NAME=$(RESULT_NAME) MODE=release SOURCE_DIR=src 
