// Header:
#include "Camera.hpp"

// Outil:
#include "../tools/glutils.hpp"

using namespace std;

// Constructeur:
Camera::Camera() {
  m_pos_x = 0.0f;
  m_pos_y = 0.0f;
  m_pos_z = 0.0f;
  m_angle_x = 0.0f;
  m_angle_y = M_PI;
  m_angle_z = 0.0f;

  m_smoothness = 0.08f;
}

// Renvoie la matrice de rotation de la caméra:
mat4 Camera::getRotationMatrix() {

  // Rotation (linéaire) vers la direction de déplacement du joueur:
  m_desiredDirection = m_player->getDirection();
  m_direction = m_direction + m_smoothness * (m_desiredDirection - m_direction);  // lerp
  mat3 rotation3 = mat3_rotation_from_vec3_y(m_direction * -1);
  mat4 rotation4 = mat4_from_mat3(rotation3);

  // Rotation du joueur:
  mat4 player_rotation = matrice_rotation(-m_player->getAngle(), 0.0f, 0.0f, 1.0f);

  // Rotation de la caméra:
  mat4 rotation_x = matrice_rotation(m_angle_x, 1.0f, 0.0f, 0.0f);
  mat4 rotation_y = matrice_rotation(m_angle_y, 0.0f, 1.0f, 0.0f);
  mat4 rotation_z = matrice_rotation(m_angle_z, 0.0f, 0.0f, 1.0f);
  mat4 model_offset = rotation_x * rotation_y * rotation_z;

  // Ajout des rotations:
  return rotation4 * player_rotation * model_offset;
}

// Translations:
void Camera::translation_x(float d) { m_pos_x += d; }
void Camera::translation_y(float d) { m_pos_y += d; }
void Camera::translation_z(float d) { m_pos_z += d; }

// Rotations:
void Camera::rotate_x(float theta) { m_angle_x += theta; }
void Camera::rotate_y(float theta) { m_angle_y += theta; }
void Camera::rotate_z(float theta) { m_angle_z += theta; }

// Envoie des paramètres caméra sur la carte graphique:
void Camera::sendDataToGPU() {
  //float cam_translation_x = 
  glUniformMatrix4fv(get_uni_loc(m_program, "cam_rotation"), 1, false, pointeur(getRotationMatrix()));  PRINT_OPENGL_ERROR();
  glUniform4f(get_uni_loc(m_program, "cam_rotation_center"), getX() + m_player->getPathPosition().x, getY() + m_player->getPathPosition().y, getZ() + m_player->getPathPosition().z, 0.0f);  PRINT_OPENGL_ERROR();
  //glUniform4f(get_uni_loc(m_program, "cam_translation"), getX() + m_player->getPathPosition().x, getY() + m_player->getPathPosition().y, getZ() + m_player->getPathPosition().z, 0.0f);  PRINT_OPENGL_ERROR();
  float cam_translation_x = getX() + m_player->getPathPosition().x; 
  float cam_translation_y = getY() + m_player->getPathPosition().y;
  float cam_translation_z = getZ() + m_player->getPathPosition().z + m_player->getSpeed() / 15.0f;
  glUniform4f(get_uni_loc(m_program, "cam_translation"), cam_translation_x, cam_translation_y, cam_translation_z , 0.0f);  PRINT_OPENGL_ERROR();
}

// Getters:
float Camera::getX() { return m_pos_x; }
float Camera::getY() { return m_pos_y; }
float Camera::getZ() { return m_pos_z; }
float Camera::getAngleX() { return m_angle_x; }
float Camera::getAngleY() { return m_angle_y; }
float Camera::getAngleZ() { return m_angle_z; }

// Setters:
void Camera::setX(float x) { m_pos_x = x; }
void Camera::setY(float y) { m_pos_y = y; }
void Camera::setZ(float z) { m_pos_z = z; }
void Camera::setRenderProgram(GLuint program) { m_program = program; }
void Camera::setPlayer(Player* player) { 
  m_player = player;
  m_desiredDirection = m_player->getDirection();
}

// Output:
void Camera::print() {
  cout << getX() << '/' << getY() << '/' << getZ() << endl;
  cout << getAngleX() << '/' << getAngleY() << '/' << getAngleZ() << endl;
  cout << endl;
}
