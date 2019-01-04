#include "Camera.hpp"

using namespace std;

// Constructeur:
Camera::Camera() {
  m_pos_x = 0.0f;
  m_pos_y = 0.0f;
  m_pos_z = 0.0f;
  m_angle_x = 0.0f;
  m_angle_y = 0.0f;
  m_angle_z = 0.0f;
}

// Renvoie la matrice de rotation de la caméra:
mat4 Camera::getMat4() {
  mat4 rotation_x = matrice_rotation(m_angle_x, 1.0f, 0.0f, 0.0f);
  mat4 rotation_y = matrice_rotation(m_angle_y, 0.0f, 1.0f, 0.0f);
  mat4 rotation_z = matrice_rotation(m_angle_z, 0.0f, 0.0f, 1.0f);
  mat4 rotation = rotation_x * rotation_y * rotation_z;
  return rotation;
}

// Translations:
void Camera::translation_x(float d) { m_pos_x += d; }
void Camera::translation_y(float d) { m_pos_y += d; }
void Camera::translation_z(float d) { m_pos_z += d; }

// Rotations:
void Camera::rotate_x(float theta) { m_angle_x += theta; }
void Camera::rotate_y(float theta) { m_angle_y += theta; }
void Camera::rotate_z(float theta) { m_angle_z += theta; }

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

// Output:
// ostream& operator<<(ostream& os, Camera& c)
void Camera::print() {
  cout << getX() << '/' << getY() << '/' << getZ() << endl;
  cout << getAngleX() << '/' << getAngleY() << '/' << getAngleZ() << endl;
  cout << endl;
}
