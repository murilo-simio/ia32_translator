# Software Basico 2/2022
# Aline
# Murilo
# --------------------
# MAKEFILE para o tradutor de IA-32

CXX := -gcc
CXXFLAGS := -Wall -std=c++17
LDFLAGS := -lm -lstdc++
BUILD := ./build
OBJ_DIR := $(BUILD)/objects
APP_DIR := $(BUILD)/apps
TARGET := translator
INCLUDE := -Iinclude/
SRC := $(wildcard src/*.cpp)
OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEPENDECIES := $(OBJECTS:.o=.d)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

-include $(DEPENDECIES)

.PHONY: all build clean debug release info

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*

info:
	@echo "[*] Application dir: ${APP_DIR}     "
	@echo "[*] Object dir:      ${OBJ_DIR}     "
	@echo "[*] Sources:         ${SRC}         "
	@echo "[*] Objects:         ${OBJECTS}     "
	@echo "[*] Dependencies:    ${DEPENDENCIES}"