## Constants

CC := gcc
CFLAGS := -Wall -O2 -MMD -MP

## Variables

program := main
test-program := test-main

src-dir := src
build-dir := build

test-sources := $(wildcard $(src-dir)/*_test.c)
test-objects := $(addprefix $(build-dir)/,$(notdir $(test-sources:.c=.o)))
test-target-sources := $(test-sources:_test.c=.c)
test-target-objects := $(addprefix $(build-dir)/,$(notdir $(test-target-sources:.c=.o)))

sources := $(filter-out $(test-sources), $(wildcard $(src-dir)/*.c))
objects := $(addprefix $(build-dir)/,$(notdir $(sources:.c=.o)))

deps := $(test-objects:.o=.d) $(objects:.o=.d)

## Dependencies

$(program): $(objects)
	$(CC) $(LDFLAGS) $(objects) $(LDLIBS) -o $@

$(test-program): LDLIBS += -lcriterion
$(test-program): $(test-objects) $(test-target-objects)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

-include $(deps)

## Task targets

.DEFAULT_GOAL := all
.PHONY: all
all: $(program)

.PHONY: clean
clean:
	-$(RM) $(program) $(test-program) $(build-dir)/*.o $(build-dir)/*.d

.PHONY: test
test: $(test-program) run-test

# private
.PHONY: run-test
run-test:
	./$(test-program) --verbose

# Pattern rules
$(build-dir)/%.o: $(src-dir)/%.c
	@mkdir -p $(build-dir)
	$(CC) $(CFLAGS) -c $< -o $@

