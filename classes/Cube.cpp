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

void Cube::render(GLuint shader_program_id) {
  glUniformMatrix4fv(get_uni_loc(shader_program_id, "rotation"), 1, false, pointeur(getMat4()));  PRINT_OPENGL_ERROR();
  glUniform4f(get_uni_loc(shader_program_id, "rotation_center"), 0.0f, 0.0f, 0.0f, 0.0f);  PRINT_OPENGL_ERROR();
  glUniform4f(get_uni_loc(shader_program_id, "translation"), getCoord().x, getCoord().y, getCoord().z, 0.0f);  PRINT_OPENGL_ERROR();

  // Affichage:
  glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, 0);  PRINT_OPENGL_ERROR();
}
