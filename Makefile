CXXFLAGS= -g -Wall -Wextra -O2
LDFLAGS = -lm -lglut -lGLEW -lGLU -lGL -pthread

all: executable

executable: main.o \
            tools/callbacks.o tools/glutils.o tools/mat3.o tools/mat4.o tools/vec2.o tools/vec3.o tools/triangle_index.o \
						classes/Camera.o classes/Cube.o classes/Path.o classes/PathAgent.o classes/Perlin.o classes/Player.o \
						modele/vertex_opengl.o modele/tga.o modele/mesh.o modele/format/mesh_io_obj.o modele/format/mesh_io_off.o
	g++ $^ -o executable $(LDFLAGS)

main.o: main.cpp \
        tools/callbacks.hpp tools/glutils.hpp tools/mat3.hpp tools/mat4.hpp tools/vec3.hpp tools/vec2.hpp tools/triangle_index.hpp \
			  classes/Camera.hpp classes/Cube.hpp classes/Path.hpp classes/PathAgent.hpp classes/Perlin.hpp classes/Player.hpp

callbacks.o: tools/callbacks.cpp tools/callbacks.hpp
glutils.o: tools/glutils.cpp tools/glutils.hpp
mat3.o: tools/mat3.cpp tools/mat3.hpp
mat4.o: tools/mat4.cpp tools/mat4.hpp
vec2.o: tools/vec2.cpp tools/vec2.hpp
vec3.o: tools/vec3.cpp tools/vec3.hpp
triangle_index.o: tools/triangle_index.cpp tools/triangle_index.hpp

Camera.o: classes/Camera.cpp classes/Camera.hpp
Cube.o: classes/Cube.cpp classes/Cube.hpp
Path.o: classes/Path.cpp classes/Path.hpp
PathAgent.o: classes/PathAgent.cpp classes/PathAgent.hpp
Perlin.o: classes/Perlin.cpp classes/Perlin.hpp
Player.o: classes/Player.cpp classes/Player.hpp

vertex_opengl.o: modele/vertex_opengl.cpp modele/vertex_opengl.hpp tools/vec2.hpp tools/vec3.hpp
tga.o: modele/tga.cpp modele/image.hpp
mesh.o: modele/mesh.cpp modele/mesh.hpp tools/mat4.hpp modele/format/mesh_io_obj.hpp modele/format/mesh_io_off.hpp
format/mesh_io_obj.o: modele/format/mesh_io_obj.cpp modele/format/mesh_io_obj.hpp
format/mesh_io_off.o: modele/format/mesh_io_off.cpp modele/format/mesh_io_off.hpp


.PHONY: clean
clean: 
	rm -f *.o executable *~ 
	rm -f */*.o executable *~ 