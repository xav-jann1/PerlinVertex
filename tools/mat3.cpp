// Librairies:
#include <cmath>
#include <cstdlib>

// Header:
#include "mat3.hpp"

mat3::mat3() {
  M[0] = 1.0f;  M[3] = 0.0f;  M[6] = 0.0f;
  M[1] = 0.0f;  M[4] = 1.0f;  M[7] = 0.0f;
  M[2] = 0.0f;  M[5] = 0.0f;  M[8] = 1.0f;
}

mat3::mat3(float x00, float x01, float x02, float x10, float x11,
           float x12, float x20, float x21, float x22) {
  M[0] = x00;  M[3] = x01;  M[6] = x02;
  M[1] = x10;  M[4] = x11;  M[7] = x12;
  M[2] = x20;  M[5] = x21;  M[8] = x22; 
}

float mat3::operator()(int x, int y) const {
  if (x >= 0 && x < 3 && y >= 0 && y < 3)
    return M[x + 3 * y];

  else {
    // gestion d'erreur
    std::cout << "Indices de matrices incorrects (" << x << "," << y << ")" << std::endl;
    abort();
  }
}

float& mat3::operator()(int x, int y) {
  if (x >= 0 && x < 3 && y >= 0 && y < 3)
    return M[x + 3 * y];

  else {
    // Gestion d'erreur:
    std::cout << "Indices de matrices incorrects (" << x << "," << y << ")" << std::endl;
    abort();
  }
}

mat3 operator*(const mat3& m1, const mat3& m2) {
  mat3 res = mat3_zeros();

  for (int kx = 0; kx < 3; ++kx) {
    for (int ky = 0; ky < 3; ++ky) {
      for (int kz = 0; kz < 3; ++kz) res(kx, ky) += m1(kx, kz) * m2(kz, ky);
    }
  }

  return res;
}

vec3 operator*(const mat3& m, const vec3& v){
  vec3 res = vec3();

  res.x = m(0,0) * v.x + m(1,0) * v.y + m(2,0) * v.z;
  res.y = m(0,1) * v.x + m(1,1) * v.y + m(2,1) * v.z;
  res.z = m(0,2) * v.x + m(1,2) * v.y + m(2,2) * v.z;

  return res;
}

const float* pointeur(const mat3& m) { return m.M; }

mat3 transpose(const mat3& m) {
  return mat3(m(0, 0), m(1, 0), m(2, 0),
              m(0, 1), m(1, 1), m(2, 1),
              m(0, 2), m(1, 2), m(2, 2));
}

mat3 mat3_rotation(float angle, float axe_x, float axe_y, float axe_z) {
  const float n = std::sqrt(axe_x * axe_x + axe_y * axe_y + axe_z * axe_z);
  const float epsilon = 1e-5f;

  mat3 m;
  if (n < epsilon) {
    std::cout << "Attention, norme de vecteur vaut 0 pour matrice de rotation" << std::endl;
    return m;
  }

  const float x = axe_x / n;
  const float y = axe_y / n;
  const float z = axe_z / n;

  const float cost = cos(angle);
  const float sint = sin(angle);

  m(0, 0) = cost + x * x * (1.0f - cost);
  m(0, 1) = x * y * (1.0f - cost) - z * sint;
  m(0, 2) = x * z * (1.0f - cost) + y * sint;

  m(1, 0) = y * x * (1.0f - cost) + z * sint;
  m(1, 1) = cost + y * y * (1.0f - cost);
  m(1, 2) = y * z * (1.0f - cost) - x * sint;

  m(2, 0) = z * x * (1.0f - cost) - y * sint;
  m(2, 1) = z * y * (1.0f - cost) + x * sint;
  m(2, 2) = cost + z * z * (1.0f - cost);

  return m;
}


mat3 mat3_rotation_from_vec3_z(vec3 p){
  vec3 direction = normalize(p);
  vec3 rotationZ = direction;

  // Si pas de rotation: rotationZ == (0,0,1) --> matrice identité
  if (rotationZ.x == 0 && rotationZ.y == 0) return mat3();

  vec3 rotationX = normalize( cross( vec3( 0, 0, 1 ), rotationZ ) );
  vec3 rotationY = normalize( cross( rotationZ, rotationX ) );

  mat3 rotation(rotationX.x, rotationX.y, rotationX.z, rotationY.x, rotationY.y, rotationY.z, rotationZ.x, rotationZ.y, rotationZ.z);
  
  return rotation;
}

mat3 mat3_rotation_from_vec3_y(vec3 p){
  vec3 direction = normalize(p);
  vec3 rotationZ = direction;

  // Si pas de rotation: rotationY == (0,1,0) --> matrice identité
  if (rotationZ.x == 0 && rotationZ.z == 0) return mat3();

  vec3 rotationX = normalize( cross( vec3( 0, 1, 0 ), rotationZ ) );
  vec3 rotationY = normalize( cross( rotationZ, rotationX ) );

  mat3 rotation(rotationX.x, rotationX.y, rotationX.z, rotationY.x, rotationY.y, rotationY.z, rotationZ.x, rotationZ.y, rotationZ.z);
  
  return rotation;
}

mat3 mat3_zeros() {
  return mat3(0.0f, 0.0f, 0.0f,
              0.0f, 0.0f, 0.0f,
              0.0f, 0.0f, 0.0f);
}

std::ostream& operator<<(std::ostream& sout, const mat3& m) {
  sout << m(0, 0) << "," << m(0, 1) << "," << m(0, 2) << std::endl;
  sout << m(1, 0) << "," << m(1, 1) << "," << m(1, 2) << std::endl;
  sout << m(2, 0) << "," << m(2, 1) << "," << m(2, 2) << std::endl;

  return sout;
}
