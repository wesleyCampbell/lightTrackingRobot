# ---------------------------------------------------------
#	Make file using the arduino-cli compiler to compile and
#	link the project
#
#	Author: Wesley Campbell
#	Date: 	2026-01-16
#	Version: 1.0
#
#	Part of the lightTrackingRobot project
# ---------------------------------------------------------

# Board Configuration
BOARD_FQBN = arduino:avr:nano:cpu=atmega328old
PORT       = /dev/ttyACM0
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

clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR)/* $(TARGET)
	@echo "Finished cleaning"
