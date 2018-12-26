#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "../tools/glutils.hpp"

// Header:
#include "Cube.hpp"

using namespace std;

Cube::Cube() {
  m_dim = 1.0f;
  m_speed = 0.1f;
  m_coord = vec3(0.0f, 0.0f, 0.0f);
  m_angleX = 0.0f;
  m_angleY = 0.0f;
  m_angleZ = 0.0f;
  m_rot_center = 0.5 * vec3(m_dim, m_dim, m_dim);
  Cube::initPoints();
  Cube::initColors();
}

vec3 Cube::getCoord() { return m_coord; }

void Cube::initPoints() {
  m_points[0] = vec3(-0.2f, -0.2f, -0.2f);
  m_points[1] = vec3(0.2f, -0.2f, -0.2f);
  m_points[2] = vec3(0.2f, 0.2f, -0.2f);
  m_points[3] = vec3(-0.2f, 0.2f, -0.2f);
  m_points[4] = vec3(-0.2f, -0.2f, 0.2f);
  m_points[5] = vec3(0.2f, -0.2f, 0.2f);
  m_points[6] = vec3(0.2f, 0.2f, 0.2f);
  m_points[7] = vec3(-0.2f, 0.2f, 0.2f);
}

void Cube::initColors() {
  m_colors[0] = vec3(0.0f, 0.0f, 0.0f);
  m_colors[1] = vec3(1.0f, 0.0f, 0.0f);
  m_colors[2] = vec3(0.0f, 1.0f, 0.0f);
  m_colors[3] = vec3(1.0f, 1.0f, 0.0f);
  m_colors[4] = vec3(1.0f, 0.0f, 1.0f);
  m_colors[5] = vec3(1.0f, 0.0f, 0.0f);
  m_colors[6] = vec3(0.0f, 1.0f, 0.0f);
  m_colors[7] = vec3(1.0f, 1.0f, 0.0f);
}

void Cube::translate(vec3 t) { m_coord += t; }

void Cube::translateX(float x) { m_coord.x += x; }

void Cube::translateY(float y) { m_coord.y += y; }

void Cube::translateZ(float z) { m_coord.z += z; }

void Cube::rotateX(float theta) { m_angleX += theta; }

void Cube::rotateY(float theta) { m_angleY += theta; }

void Cube::rotateZ(float theta) { m_angleZ += theta; }

mat4 Cube::getMat4() {
  mat4 rotation_x = matrice_rotation(m_angleX, 1.0f, 0.0f, 0.0f);
  mat4 rotation_y = matrice_rotation(m_angleY, 0.0f, 1.0f, 0.0f);
  mat4 rotation_z = matrice_rotation(m_angleZ, 0.0f, 0.0f, 1.0f);
  mat4 rotation = rotation_x * rotation_y * rotation_z;
  return rotation;
}

void Cube::update() { translateZ(m_speed); }

// Chargement d'un cube sur la carte graphique:
void Cube::loadCube(float dim) {
  GLuint vbo, vboi;

  // Tableau entrelacant coordonnees-normales:
  vec3 geometrie[] = {dim * vec3(-0.2f, -0.2f, -0.2f), vec3(0.0f, 0.0f, 0.0f),
                      dim * vec3(0.2f, -0.2f, -0.2f),  vec3(1.0f, 0.0f, 0.0f),
                      dim * vec3(0.2f, 0.2f, -0.2f),   vec3(0.0f, 1.0f, 0.0f),
                      dim * vec3(-0.2f, 0.2f, -0.2f),  vec3(1.0f, 1.0f, 0.0f),
                      dim * vec3(-0.2f, -0.2f, 0.2f),  vec3(1.0f, 0.0f, 1.0f),
                      dim * vec3(0.2f, -0.2f, 0.2f),   vec3(1.0f, 0.0f, 0.0f),
                      dim * vec3(0.2f, 0.2f, 0.2f),    vec3(0.0f, 1.0f, 0.0f),
                      dim * vec3(-0.2f, 0.2f, 0.2f),   vec3(1.0f, 1.0f, 0.0f)};

  // Indice des triangles:
  triangle_index tri0 = triangle_index(0, 1, 2);
  triangle_index tri1 = triangle_index(0, 2, 3);
  triangle_index tri2 = triangle_index(1, 2, 5);
  triangle_index tri3 = triangle_index(5, 2, 6);
  triangle_index tri4 = triangle_index(0, 4, 3);
  triangle_index tri5 = triangle_index(4, 3, 7);
  triangle_index tri6 = triangle_index(4, 7, 5);
  triangle_index tri7 = triangle_index(5, 7, 6);
  triangle_index tri8 = triangle_index(2, 3, 6);
  triangle_index tri9 = triangle_index(3, 6, 7);
  triangle_index tri10 = triangle_index(0, 1, 5);
  triangle_index tri11 = triangle_index(0, 5, 4);

  triangle_index index[] = {tri0, tri1, tri2, tri3, tri4,  tri5,
                            tri6, tri7, tri8, tri9, tri10, tri11};

  // Attribution d'un buffer de donnees (1 indique la création d'un buffer):
  glGenBuffers(1, &vbo);  PRINT_OPENGL_ERROR();
  // Affectation du buffer courant:
  glBindBuffer(GL_ARRAY_BUFFER, vbo);  PRINT_OPENGL_ERROR();
  // Copie des donnees des sommets sur la carte graphique:
  glBufferData(GL_ARRAY_BUFFER, sizeof(geometrie), geometrie, GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();

  // Active l'utilisation des données de positioncenter_views:
  glEnableClientState(GL_VERTEX_ARRAY);  PRINT_OPENGL_ERROR();
  // Indique que le buffer courant (désigné par la variable vbo) est utilisé pour les positions de sommets:
  glVertexPointer(3, GL_FLOAT, 2 * sizeof(vec3), 0);  PRINT_OPENGL_ERROR();

  // Active l'utilisation des données de couleurs:
  glEnableClientState(GL_COLOR_ARRAY);  PRINT_OPENGL_ERROR();
  // Indique que le buffer courant (désigné par la variable vbo) est utilisé pour les couleurs:
  glColorPointer(3, GL_FLOAT, 2 * sizeof(vec3), buffer_offset(sizeof(vec3)));  PRINT_OPENGL_ERROR();

  // Attribution d'un autre buffer de données:
  glGenBuffers(1, &vboi);  PRINT_OPENGL_ERROR();
  // Affectation du buffer courant (buffer d'indice):
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboi);  PRINT_OPENGL_ERROR();
  // Copie des indices sur la carte graphique:
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();
}

void Cube::render(GLuint shader_program_id) {
  glUniformMatrix4fv(get_uni_loc(shader_program_id, "rotation"), 1, false, pointeur(getMat4()));  PRINT_OPENGL_ERROR();
  glUniform4f(get_uni_loc(shader_program_id, "rotation_center"), 0.0f, 0.0f, 0.0f, 0.0f);  PRINT_OPENGL_ERROR();
  glUniform4f(get_uni_loc(shader_program_id, "translation"), getCoord().x, getCoord().y, getCoord().z, 0.0f);  PRINT_OPENGL_ERROR();

  // Affichage:
  glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, 0);  PRINT_OPENGL_ERROR();
}
