CXX = g++
CXXFLAGS = -I/usr/local/include -std=c++17 -Wall -Wextra
LDFLAGS = -L/usr/local/lib
LIBS = -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lX11 -lXrandr -lXi -ludev -lXcursor -lGL -lfreetype -lpthread
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