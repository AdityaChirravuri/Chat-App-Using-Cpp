# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall -Wextra -O2

# Target binary name
TARGET = chat_app

# Directories
SRCDIR = src
INCDIR = include

# Source and object files
SRCS = chat_app.cpp $(SRCDIR)/ChatClient.cpp $(SRCDIR)/ChatServer.cpp
OBJS = $(SRCS:.cpp=.o)

# Default rule
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -o $@ $^

# Compiling
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean