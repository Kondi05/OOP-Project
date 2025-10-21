CXX = g++
CXXFLAGS = -I/opt/homebrew/opt/sfml/include -std=c++17 -Wall -Wextra #need to change this to -I/opt/homebrew/opt/sfml/include for macOS
LDFLAGS = -L/opt/homebrew/opt/sfml/lib #need to change this to -L/opt/homebrew/opt/sfml/lib for macOS
LIBS = -lsfml-graphics -lsfml-window -lsfml-system  #remove -s and all -LX11 stuff for macOS
TARGET = space_invaders
SOURCES = main.cpp GameObject.cpp Entity.cpp Player.cpp Bullet.cpp Enemy.cpp GameManager.cpp FileHandler.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS) game_state.txt

.PHONY: all clean