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

  // Translations:
  void translate(vec3 t);
  void translateX(float x);
  void translateY(float y);
  void translateZ(float z);

  // Rotations:
  void rotateX(float theta);
  void rotateY(float theta);
  void rotateZ(float theta);

  // Getters:
  mat4 getMat4();
  vec3 getCoord();
  float getX();
  float getY();
  float getZ();
  vec3* getGeometrie();
  triangle_index* getIndex();

  // Mise à jour de la position du cube:
  bool update(int path_points_deleted = 0);

  // Chargement d'un cube sur la carte graphique:
  static void loadCube(float);

  // Affichage du cube:
  void render(GLuint shader_program_id);

 private:
  // Paramètre du cube:
  float m_dim;
  
  // Initialisations:
  void initPoints();
  void initColors();

  // Transformation:
  vec3 m_coord;
  vec3 m_rot_center;
  float m_angleX, m_angleY, m_angleZ;

  // Données GPU:
  vec3 m_points[8];
  // vec3[8] normales;
  vec3 m_colors[8];
  triangle_index m_index[8];
};

#endif
