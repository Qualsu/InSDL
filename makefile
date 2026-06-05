CXX ?= g++
SRC ?= src/main.cpp
BUILD_DIR ?= build

INCLUDE_DIRS := include
LIB_DIRS := lib
LDLIBS := -lSDL3 -lSDL3_ttf -lSDL3_image
CXXFLAGS ?= -g

ifeq ($(OS),Windows_NT)
	EXE_EXT := .exe
	MKDIR_P := if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	RUN_PREFIX :=
	CLEAN_CMD := if exist "$(TARGET)" del /Q "$(TARGET)"
else
	EXE_EXT :=
	MKDIR_P := mkdir -p "$(BUILD_DIR)"
	RUN_PREFIX := ./
	CLEAN_CMD := rm -f "$(TARGET)"
endif

SRC_NAME := $(basename $(notdir $(SRC)))
TARGET ?= $(BUILD_DIR)/$(SRC_NAME)$(EXE_EXT)

CPPFLAGS += $(addprefix -I,$(INCLUDE_DIRS))
LDFLAGS += $(addprefix -L,$(LIB_DIRS))

.PHONY: default all compile run clean print-vars

default: all

all: compile run

compile: $(TARGET)

$(TARGET): $(SRC)
	$(MKDIR_P)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS) $(LDLIBS)

run: $(TARGET)
	$(RUN_PREFIX)$(TARGET)

clean:
	-$(CLEAN_CMD)

print-vars:
	@echo SRC=$(SRC)
	@echo TARGET=$(TARGET)
