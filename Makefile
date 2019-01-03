CXXFLAGS= -g -Wall -Wextra -O2
LDFLAGS = -lm -lglut -lGLEW -lGLU -lGL -pthread

all: executable

executable: main.o tools/glutils.o tools/mat3.o tools/mat4.o tools/vec2.o tools/vec3.o tools/triangle_index.o classes/Camera.o classes/Cube.o classes/Path.o classes/Perlin.o
	g++ $^ -o executable $(LDFLAGS)

main.o: main.cpp tools/glutils.hpp tools/mat3.hpp tools/mat4.hpp tools/vec3.hpp tools/vec2.hpp tools/triangle_index.hpp classes/Camera.hpp classes/Cube.hpp classes/Path.hpp classes/Perlin.hpp
glutils.o: tools/glutils.cpp tools/glutils.hpp
mat3.o: tools/mat3.cpp tools/mat3.hpp
mat4.o: tools/mat4.cpp tools/mat4.hpp
vec2.o: tools/vec2.cpp tools/vec2.hpp
vec3.o: tools/vec3.cpp tools/vec3.hpp
triangle_index.o: tools/triangle_index.cpp tools/triangle_index.hpp

Camera.o: classes/Camera.cpp classes/Camera.hpp
Cube.o: classes/Cube.cpp classes/Cube.hpp
Path.o: classes/Path.cpp classes/Path.hpp
Perlin.o: classes/Perlin.cpp classes/Perlin.hpp

.PHONY: clean
clean: 
	rm -f *.o executable *~ 
	rm -f */*.o executable *~ 