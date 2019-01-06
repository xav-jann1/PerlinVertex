
#ifndef MAT3_HPP
#define MAT3_HPP

#include <iostream>
#include "vec3.hpp"

/** Une structure de matrice de taille 3x3 */

struct mat3 {
  /** Initialise la matrice a l'identitee */
  mat3();
  /** Initialisation par valeur */
  mat3(float x00, float x01, float x02,
       float x10, float x11, float x12,
       float x20, float x21, float x22);

  /** Obtention des valeurs de la matrice sous la forme m(x,y) */
  float operator()(int x, int y) const;

  /** Modification des valeurs de la matrice sous la forme m(x,y)=... */
  float& operator()(int x, int y);

  /** Données de la matrice sous forme d'un tableau */
  float M[3 * 3];
};

/** Produit de matrice */
mat3 operator*(const mat3& m1, const mat3& m2);

/** Produit de matrice avec vecteur */
vec3 operator*(const mat3& m, const vec3& v);

/** Récupère un pointeur sur les données de la matrice */
const float* pointeur(const mat3& m);

/** Calcule la transposee d'une matrice */
mat3 transpose(const mat3& m);

/** Construit une matrice de rotation ayant pour axe: (axe_x,axe_y,axe_z) et l'angle donné */
mat3 mat3_rotation(float angle, float axe_x, float axe_y, float axe_z);

/** Matrice de rotation orienter selon un vecteur */
mat3 mat3_rotation_from_vec3_y(vec3 p);
mat3 mat3_rotation_from_vec3_z(vec3 p);

/** Construit une matrice n'ayant que des zéros */
mat3 mat3_zeros();

/** Affichage d'une matrice sur la ligne de commande */
std::ostream& operator<<(std::ostream& sout, const mat3& m);

#endif
