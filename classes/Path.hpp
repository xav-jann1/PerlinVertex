#ifndef PATH_H
#define PATH_H

// Librairies:
#include <GL/glew.h>
#include <cmath>
#include <vector>

// Outils:
#include "../tools/mat3.hpp"
#include "../tools/triangle_index.hpp"
#include "../tools/vec3.hpp"

/**
 * class Path: Synthèse de fonctionnement
 *  - Chemin:
 *    - 'm_points' : liste de points représentant un chemin
 *    - 'pushPoint()': ajout d'un point à 'm_points'
 *
 *  - Tube:
 *    - 'm_render_radius': rayon du tube
 *    - 'm_render_res': nombre de points dans les cercles constituant le tube
 *
 *  - Pré-rendu:
 *    - 'm_render_sommets' : sommets composant le tube
 *    - 'm_render_indices' : indices des sommets représentant les triangles
 *
 *  - Rendu:
 *    - 'render()' : affiche les triangles contenus dans 'm_render_sommets' et
 *                   'm_render_indices'
 *
 *  Lors de l'ajout d'un point avec la méthode 'pushPoint()', la portion du tube
 *  créé par l'ajout du point est ajouté dans les variables de rendu
 *  'm_render_sommets' et 'm_render_indices'
 *    --> Le tube est ainsi construit progressivement (~ pré-rendu)
 */

class Path {
 public:
  // Constructeur:
  Path();

  /**
   * Chemin:
   */

  // Ajoute un point à la fin du chemin:
  void pushPoint(vec3);

  // Supprime le premier point du chemin:
  void removeFirst();

  /**
   * Méthodes pour paramètres du tube:
   */

  // Définit les shaders utilisé par render():
  void setRenderProgram(GLuint program);

  // Définit la résolution du tube (= nombre de points par cercle):
  void setRenderRes(int);

  // Définit le rayon du tube:
  void setRenderRadius(float r);

  // Modifie relativement le rayon du tube:
  void updateRadius(float dr);

  /**
   * Affichage:
   */

  // Affichage du chemin par un tube:
  void render();

  // Ré-initialisation du pré-rendu du tube:
  void resetRender();

  /**
   * Getters:
   */
  int getLength();     // Nombre de points composant le chemin
  vec3 getPoint(int);  // Position d'un point du chemin
  float getRadius();   // Rayon du tube

 private:
  /**
   * Chemin:
   */

  // Liste des points du chemin:
  std::vector<vec3> m_points;

  // Référence au shaders utilisée lors du rendu:
  GLuint m_render_program;

  /**
   * Paramètres du tube:
   */

  // Nombre de points constituant les cercles composant le tube:
  int m_render_res;

  // Rayon des cercles composant le tube:
  float m_render_radius;

  /**
   * Variables pour le rendu:
   */

  // Points composant un cercle de référence pour composer le tube:
  std::vector<vec3> m_render_cercle;

  // Tous les points composant le tube:
  std::vector<vec3> m_render_sommets;

  // Tous les indices des triangles du tube:
  std::vector<triangle_index> m_render_indices;

  /**
   * Méthodes pour le pré-rendu:
   */

  // Ajoute le rendu d'une partie du tube:
  void addRender(int idx);

  // Ajoute les points du premier cercle du tube:
  void addFirstRender();

  // Ajoute les indices pour former le tube entre les points <i> et <i-1>:
  void addTubeIndices(int i);

  // Création d'un cercles de N points et de rayon r sur le plan (x,y):
  std::vector<vec3> generateCercle();

  // Oriente les points d'un cercle selon sa normale puis les déplace dans une
  // direction:
  std::vector<vec3> moveCercle(std::vector<vec3> points, vec3 dir,
                               vec3 normale);

  // Ajoute des sommets à la liste de sommets:
  void insertSommets(std::vector<vec3> vec);
};

#endif