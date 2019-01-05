
// Header:
#include "vec3.hpp"

// Librairies:
#include <cassert>
#include <cmath>
#include <iostream>

// Outil:
#include "mat3.hpp"

vec3::vec3() : x(0.0f), y(0.0f), z(0.0f) {}

vec3::vec3(float x_param, float y_param, float z_param)
    : x(x_param), y(y_param), z(z_param) {}

vec3& vec3::operator+=(const vec3& v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

vec3& vec3::operator-=(const vec3& v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

vec3& vec3::operator*=(float s) {
  x *= s;
  y *= s;
  z *= s;
  return *this;
}

vec3& vec3::operator/=(float s) {
  //assert(fabs(s) > 10e-6);
  x /= s;
  y /= s;
  z /= s;
  return *this;
}

std::ostream& operator<<(std::ostream& sout, const vec3& v) {
  sout << v.x << "," << v.y << "," << v.z;
  return sout;
}

float norm(const vec3& v) {
  return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float dot(const vec3& v0, const vec3& v1) {
  return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

vec3 cross(const vec3& v0, const vec3& v1) {
  return vec3(v0.y * v1.z - v0.z * v1.y, v0.z * v1.x - v0.x * v1.z,
              v0.x * v1.y - v0.y * v1.x);
}

vec3 normalize(const vec3& v) {
  vec3 temp = v;
  temp /= norm(v);
  return temp;
}

vec3 operator+(const vec3& v0, const vec3& v1) {
  vec3 temp = v0;
  temp += v1;
  return temp;
}

vec3 operator-(const vec3& v0, const vec3& v1) {
  vec3 temp = v0;
  temp -= v1;
  return temp;
}

vec3 operator*(const vec3& v0, float s) {
  vec3 temp = v0;
  temp *= s;
  return temp;
}

vec3 operator*(float s, const vec3& v0) { return v0 * s; }

vec3 operator/(const vec3& v0, float s) {
  vec3 temp = v0;
  temp /= s;
  return temp;
}

// Rotation d'un vecteur autour d'un axe:
//   Inspiré de : 'Rotating vectors in space and high precision in C++'
//      https://stackoverflow.com/questions/45591017/rotating-vectors-in-space-and-high-precision-in-c
vec3 rotateVectorAroundAxis(vec3 vec, vec3 axis, float angle) {
  // Normalisation du vecteur de l'axe de rotation:
  axis = normalize(axis);

  // Valeur constantes:
  float cos = std::cos(angle), sin = std::sin(angle);
  float mcos = 1 - cos;

  // Matrice de rotation:
  mat3 rotation;

  // Remplie la matrice de rotation:
  rotation(0, 0) = cos + axis.x * axis.x * mcos;
  rotation(1, 0) = axis.x * axis.y * mcos - axis.z * sin;
  rotation(2, 0) = axis.x * axis.z * mcos - axis.y * sin;

  rotation(0, 1) = axis.x * axis.y * mcos + axis.z * sin;
  rotation(1, 1) = cos + axis.y * axis.y * mcos;
  rotation(2, 1) = axis.x * axis.z * mcos - axis.x * sin;

  rotation(0, 2) = axis.x * axis.z * mcos - axis.y * sin;
  rotation(1, 2) = axis.z * axis.y * mcos - axis.x * sin;
  rotation(2, 2) = cos - axis.z * axis.z * mcos;

  // Rotation du vecteur:
  vec = rotation * vec;

  return vec;
}
