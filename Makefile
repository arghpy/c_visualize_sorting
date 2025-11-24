CFLAGS := -Wall -Wextra -ggdb

SRC     := ./src
BUILD   := ./build
SRC_OBJ := $(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(wildcard $(SRC)/*.c))

OTHER_SRC := $(SRC)/other
OTHER_OBJ := $(patsubst $(OTHER_SRC)/%.c,$(BUILD)/%.o,$(wildcard $(OTHER_SRC)/*.c))

ALGORITHMS_SRC := $(SRC)/algorithms
ALGORITHMS_OBJ := $(patsubst $(ALGORITHMS_SRC)/%.c,$(BUILD)/%.o,$(wildcard $(ALGORITHMS_SRC)/*.c))

OBJS := $(SRC_OBJ) $(OTHER_OBJ) $(ALGORITHMS_OBJ)

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

$(TARGET): $(OBJS)
	mkdir -p $(BUILD)
	@echo "Linking $^..."
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

$(BUILD)/%.o: $(SRC)/%.c
	mkdir -p $(BUILD)
	@echo "Linking $^..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD)/%.o: $(OTHER_SRC)/%.c
	mkdir -p $(BUILD)
	@echo "Linking $^..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD)/%.o: $(ALGORITHMS_SRC)/%.c
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
