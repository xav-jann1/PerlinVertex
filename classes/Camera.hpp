// Librairie:
#include <string>

// Outil:
#include "../tools/mat4.hpp"

class Camera {
 public:
  // Constructeur:
  Camera();

  // Translations:
  void translation_x(float d);
  void translation_y(float d);
  void translation_z(float d);

  // Rotations:
  void rotate_x(float theta);
  void rotate_y(float theta);
  void rotate_z(float theta);

  // Getters:
  mat4 getMat4();
  float getX();
  float getY();
  float getZ();
  float getAngleX();
  float getAngleY();
  float getAngleZ();

  // Setters:
  void setX(float);
  void setY(float);
  void setZ(float);

  // Console Output:
  void print();

 private:
  float m_pos_x, m_pos_y, m_pos_z;
  float m_angle_x, m_angle_y, m_angle_z;
};
