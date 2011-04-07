TESTS := $(patsubst %.c,%,$(wildcard *.c))

%: %.c

.PHONY: all
all: $(TESTS)

.PHONY: clean
clean:
	rm -f $(TESTS) *.o

