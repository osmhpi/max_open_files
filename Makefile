
CFLAGS = -Wall -Wextra -O2 -g

BIN = max_open_files

.PHONY: all clean

all: $(BIN)

clean:
	@$(RM) $(BIN) files
