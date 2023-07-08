CC = g++

# c++ compile option
CXXFLAGS = 

# linker option
LDFLAGS = 

# header file directory path
INCLUDE_DIR = -Iinclude/

# src file directory path
SRC_DIR = ./src

# object file directory path
OBJ_DIR = ./obj

# executable file name
TARGET = replay

SRCS = $(notdir $(wildcard $(SRC_DIR)/*.cc))
OBJS = $(SRCS:.cc=.o)
DEPS = $(SRCS:.cc=.d)

OBJECTS = $(patsubst %.o,$(OBJ_DIR)/%.o,$(OBJS))
DEPS = (OBJECTS:.o=.d)

all: replay

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cc
	$(CC) $(CXXFLAGS) $(INCLUDE_DIR) -c $< -o $@ -MD $(LDFLAGS)

$(TARGET) : $(OBJECTS)
	$(CC) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

.PHONY: clean all
clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET)

-include $(DEPS)
	