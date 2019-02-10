// Header:
#include "Cube.hpp"

// Librairies:
#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>

// Outil:
#include "../tools/glutils.hpp"

using namespace std;

// Constructeur:
Cube::Cube(Path* path) : PathAgent(path) {
  // Paramètres du cube:
  m_dim = 1.0f; // Taille

  setSpeed(-1);

  // Angle aléatoire:
  float angle = ((float)rand() / RAND_MAX) * 2.0f * M_PI;
  setAngle(angle);

  setAngleSpeed(0);

  // Place la cube à la fin du chemin::
  int position = path->getLength() - 2;
  setPointsAB(position);

  // Rotations:
  m_angleX = 0.0f;
  m_angleY = M_PI;
  m_angleZ = ((float)rand() / RAND_MAX) * 2.0f * M_PI;
  m_rotation_center = 0.5 * vec3(m_dim, m_dim, m_dim);

  // Initialisation points et couleurs du cube:
  initPoints();
  initColors();
}

/**
 * Setters:
 */

// Rotations:
void Cube::rotateX(float theta) { m_angleX += theta; }
void Cube::rotateY(float theta) { m_angleY += theta; }
void Cube::rotateZ(float theta) { m_angleZ += theta; }

// Shaders utilisés par render():
void Cube::setRenderProgram(GLuint program) { m_render_program = program; }

/**
 * Getter:
 */

// Matrice de rotation du cube:
mat4 Cube::getRotationMatrix() {

  // Rotation en direction du centre du tube:
  vec3 position = getPosition();
  vec3 path_position = getPathPosition();
  vec3 orientation = position - path_position;
  mat3 rotation3 = mat3_rotation_from_vec3_z(orientation * -1);
  mat4 rotation4 = mat4_from_mat3(rotation3);

  // Offset de rotation du modèle:
  mat4 rotation_x = matrice_rotation(m_angleX, 1.0f, 0.0f, 0.0f);
  mat4 rotation_y = matrice_rotation(m_angleY, 0.0f, 1.0f, 0.0f);
  mat4 rotation_z = matrice_rotation(m_angleZ, 0.0f, 0.0f, 1.0f);
  mat4 model_offset = rotation_x * rotation_y * rotation_z;

  // Ajout des rotations:
  return rotation4 * model_offset;
}

/**
 * Mise à jour de la position du cube:
 * @param int path_points_deleted  Nombre de points supprimé du chemin depuis la
 *                                 dernière mise à jour de l'agent
 * @return bool canDelete  Averti si le cube peut être supprimé
 *                         (car en dehors du chemin)
 */
bool Cube::update(int path_points_deleted) {
  // Mise à jour de la position du cube sur le chemin:
  bool canDelete = PathAgent::update(path_points_deleted);

  // Rotation du cube sur lui-même:
  m_angleZ += 0.05f;

  return canDelete;
}

/**
 * Méthodes:
 */

// Points du cube:
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

// Couleurs de chaque point du cube:
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

void Cube::render() {
  glUniformMatrix4fv(get_uni_loc(m_render_program, "rotation"), 1, false, pointeur(getRotationMatrix()));  PRINT_OPENGL_ERROR();
  glUniform4f(get_uni_loc(m_render_program, "rotation_center"), 0.0f, 0.0f, 0.0f, 0.0f);  PRINT_OPENGL_ERROR();
  glUniform4f(get_uni_loc(m_render_program, "translation"), getPosition().x, getPosition().y, getPosition().z, 0.0f);  PRINT_OPENGL_ERROR();

  // Affichage:
  glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, 0);  PRINT_OPENGL_ERROR();
}
