# ---------------------------------------------------------
#	Make file using the arduino-cli compiler to compile and
#	link the project
#
#	Author: Wesley Campbell
#	Date: 	2026-01-16
#	Version: 1.0.2
#
#	Part of the lightTrackingRobot project
# ---------------------------------------------------------

# Board Configuration
BOARD_FQBN = arduino:avr:nano:cpu=atmega328old
PORT       = /dev/ttyACM1
# PORT       = $(shell arduino-cli board list | grep -Eo '/dev/\w*')

# Filetree Configuration
SRC_DIR   = src
INC_DIR   = include
BUILD_DIR = build
TARGET    = $(BUILD_DIR)/lightTrackingRobot.ino.hex

SRC_FILES := $(wildcard $(SRC_DIR)/*.c) \
			 $(wildcard $(SRC_DIR)/*.cpp) \
			 $(wildcard *.ino) \
			 $(wildcard $(SRC_DIR)/*.ino)

# Compiler Configuration
CFLAGS   := -I$(INC_DIR) -I$(SRC_DIR) 
ARDUINO = arduino-cli

# Rules

all: $(TARGET)

$(TARGET): $(SRC_FILES) 
	@echo "Beginning compile process..."
	@mkdir -p $(BUILD_DIR)
	$(ARDUINO) compile \
		--fqbn $(BOARD_FQBN) \
		--port $(PORT) \
		--build-path ./$(BUILD_DIR) \
		--build-property "build.extra_flags=$(CFLAGS)" \
		$(PWD)
	@echo "Compile process finished"

upload: all
	@echo "Uploading code to board $(BOARD_FQBN)..."
	$(ARDUINO) upload \
		--port $(PORT) \
		--fqbn $(BOARD_FQBN) \
		--input-dir $(BUILD_DIR)
	@echo "Upload successfull"

clangd:
	@echo "Generating Clangd database..."
	$(ARDUINO) compile \
		--fqbn $(BOARD_FQBN) \
		--build-path ./$(BUILD_DIR) \
		--build-property "build.extra_flags=$(CFLAGS)" \
		--only-compilation-database \
		$(PWD)

clean:
	@echo "Cleaning build artifacts..."
	@find build -mindepth 1 -maxdepth 1 ! -name 'venv' ! -name 'compile_commands.json' -exec rm -rf {} +
	@echo "Finished cleaning"
