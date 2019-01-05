#ifndef CUBE_H
#define CUBE_H

// Librairies:
#include <iostream>
#include <string>

// Outils:
#include "../tools/mat4.hpp"
#include "../tools/triangle_index.hpp"
#include "../tools/vec3.hpp"

// Classes:
#include "Path.hpp"
#include "PathAgent.hpp"

class Cube : public PathAgent {
 public:
  // Constructeur:
  Cube(Path*);

  // Rotations:
  void rotateX(float theta);
  void rotateY(float theta);
  void rotateZ(float theta);

  // Définit les shaders utilisé par render():
  void setRenderProgram(GLuint program);

  // Getters:
  mat4 getMat4();
  float getX();
  float getY();
  float getZ();

  // Mise à jour de la position du cube:
  bool update(int path_points_deleted = 0);

  // Chargement d'un cube sur la carte graphique:
  static void loadCube(float);

  // Affichage du cube:
  void render();

 private:
  // Paramètre du cube:
  float m_dim;
  
  // Initialisations:
  void initPoints();
  void initColors();

  // Transformation:
  vec3 m_rotation_center;
  float m_angleX, m_angleY, m_angleZ;

  // Rendu:
  vec3 m_points[8], m_colors[8];
  triangle_index m_index[8];
  GLuint m_render_program;
};

#endif
