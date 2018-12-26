#include <GL/glew.h>
#include <cmath>
#include <vector>
#include "../tools/vec3.hpp"
#include "../tools/mat3.hpp"
#include "../tools/triangle_index.hpp"

class Path {
 public:
  // Constructeur:
  Path();

  /** Chemin **/

  // Ajoute un point à la fin du chemin:
  void pushPoint(vec3);

  // Supprime le premier point du chemin:
  void removeFirst();

  /** Affichage **/

  // Affichage du chemin par un tube:
  void render();

  // Définit les shaders utilisé par render():
  void setRenderProgram(GLuint program);

  // Définit la résolution du tube (= nombre de points par cercle):
  void setRenderRes(int);

  // Définit le rayon du tube:
  void setRenderRadius(float);



 private:
  // Liste des points du chemin:
  std::vector<vec3> m_points;

  // Référence au shaders utilisée lors du rendu:
  GLuint m_render_program;

  // Nombre de points constituant les cercles composant le tube:
  int m_render_res;

  // Rayon des cercles composant le tube
  float m_render_radius;

  void renderTube(std::vector<vec3> points, std::vector<triangle_index> indices);
};