CFLAGS := -Wall -Wextra -ggdb -O3

SRC     := ./src
BUILD   := ./build
SRC_OBJ := $(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(wildcard $(SRC)/*.c))

THIRDPARTY_INCLUDE_DIR := ./thirdparty

RAYLIB         := $(THIRDPARTY_INCLUDE_DIR)/raylib-5.5_linux_amd64
RAYLIB_INCLUDE := $(RAYLIB)/include
RAYLIB_LIB     := $(RAYLIB)/lib
RAYLIB_RPATH   := -Wl,-rpath=$(RAYLIB_LIB)
RAYLIB_LINK    := raylib

UTILS_INCLUDE := $(THIRDPARTY_INCLUDE_DIR)/utils/c

INCLUDES      := -I$(RAYLIB_INCLUDE) -I$(UTILS_INCLUDE)
LIBS          := -L$(RAYLIB_LIB) -l$(RAYLIB_LINK) -lm $(RAYLIB_RPATH)

TARGET        := $(BUILD)/visualize_sorting

.PHONY: all clean run bear

# default action. Builds target
all: $(TARGET)

$(TARGET): $(SRC_OBJ)
	mkdir -p $(BUILD)
	@echo "Linking $^..."
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

$(BUILD)/%.o: $(SRC)/%.c
	mkdir -p $(BUILD)
	@echo "Linking $^..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "Cleaning..."
	rm -rf $(BUILD)

# Builds if needed
run: $(TARGET)
	./$(TARGET)

bear:
	@echo "Creating compile_commands.json..."
	bear -- make all
