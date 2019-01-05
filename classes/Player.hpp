#ifndef PLAYER_H
#define PLAYER_H

// Outil:
#include "../tools/mat4.hpp"

// Classe:
#include "PathAgent.hpp"

class Player : public PathAgent {
 public:
  // Constructeur:
  Player(Path* path);

  // Getters:
  mat4 getRotationMatrix();
  vec3 getRotationCenter();

  // Setters:
  void setDesiredAngle(float);
  void setRotationCenter(vec3 p);
  void setRenderProgram(GLuint program);
  void updateModelAngle(float dangle);  // Animation

  // Update:
  bool update(int path_points_deleted = 0);
  void smoothRotation();  // Animation

  // Affichage:
  void render();
  void init_model();

 private:
  // Paramètre du joueur:
  int m_life;
  float m_angle, m_desired_angle;

  // Transformation:
  vec3 m_rotation_center;
  vec3 m_model_angle_offset;

  // Rendu:
  GLuint m_render_program;
  GLuint m_vbo, m_vboi, m_texture_id;
  int m_nbr_triangles;

  void load_texture(const char* filename,GLuint *texture_id);

};

#endif