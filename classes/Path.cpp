#include "Path.hpp"

#include "../tools/glutils.hpp"
#include "../tools/mat4.hpp"

using namespace std;

// Constructeur:
Path::Path() {}

/** Chemin **/

// Ajoute un point à la liste:
void Path::pushPoint(vec3 point) { m_points.push_back(point); }

// Enlève le premier point de la iste:
void Path::removeFirst() { m_points.erase(m_points.begin()); }

/** Affichage **/

// Définit les shaders utilisés par render():
void Path::setRenderProgram(GLuint program) { m_render_program = program; }

// Définit la résolution du tube (= nombre de points par cercle):
void Path::setRenderRes(int N) { m_render_res = N; }

// Définit le rayon du tube:
void Path::setRenderRadius(float r) { m_render_radius = r; }

// Création d'un cercles de N points et de rayon r sur le plan (x,y):
vector<vec3> createCircle(int N, float r) {
  vector<vec3> sommets;

  // Ajoute le centre du cercle:
  sommets.push_back(vec3(0.0f, 0.0f, 0.0f));

  // Ajoute les sommets:
  for (int k = 0; k < N; ++k) {
    sommets.push_back(vec3(r * cos(float(2.0f * k / N) * M_PI),
                           r * sin(float(2.0f * k / N) * M_PI), 0));
  }

  return sommets;
}

// Oriente les points d'un cercle selon sa normale puis les déplace dans une direction:
vector<vec3> moveCercle(vector<vec3> points, vec3 dir, vec3 normale) {
  // Matrice de rotation pour orienter correctement chaque point du cercle:
  mat3 orient = mat3_rotation_from_vec3(normale);

  // Pour chaque point:
  for (int i = 0; i < (int)points.size(); i++)
    // Ajoute la rotation puis la translation:
    points[i] = orient * points[i] + dir;

  return points;
}

// Concatène 2 vectors de vec3:
vector<vec3> concatVect(vector<vec3> vec1, vector<vec3> vec2) {
  vec1.insert(vec1.end(), vec2.begin(), vec2.end());
  return vec1;
}

// Affichage du chemin par un tube:
void Path::render() {
  // Tous les points du tube:
  vector<vec3> points;

  // Tous les indices des triangles du tube:
  vector<triangle_index> indices;

  // Chargement d'un cercle de référence:
  vector<vec3> cercle = createCircle(m_render_res, m_render_radius);

  // Points du cercle autour du premier point ("vers" le deuxième point):
  vec3 pos = m_points[0];
  vec3 normale = m_points[1] - pos;
  vector<vec3> cercle1 = moveCercle(cercle, pos, normale);

  // Ajoute les points à l'ensemble de points:
  points = concatVect(points, cercle1);


  // Pour chaque point du chemin:
  for (int i = 1; i < (int) m_points.size(); ++i) {
    // Récupère les points du cercle formés en direction du point précédent:
    vec3 pos = m_points[i];
    vec3 normale = pos - m_points[i - 1];  // Normale en direction du point précédent
    vector<vec3> cercle1 = moveCercle(cercle, pos, normale);

    // Ajoute les points à l'ensemble de points:
    points = concatVect(points, cercle1);

    // Ajoute les triangles, entre le triangle précédent et en cours, pour former le tube:
    int iCercle1 = (i-1) * (m_render_res + 1) + 1;  // Indice du premier point du cercle 1 (précédent)
    int iCercle2 = iCercle1 + m_render_res + 1;     // Indice du premier point du cercle 2 (en cours)
    for (int j = 0; j < m_render_res-1; ++j) {
      indices.push_back(triangle_index(iCercle1 + j, iCercle2 + j, iCercle2 + j + 1));
      indices.push_back(triangle_index(iCercle1 + j, iCercle1 + j + 1, iCercle2 + j + 1));
    }
    indices.push_back(triangle_index(iCercle1, iCercle1 + m_render_res - 1, iCercle2));
    indices.push_back(triangle_index(iCercle1 + m_render_res - 1 , iCercle2, iCercle2 + m_render_res -1 ));

  }

  // Affiche le tube:
  renderTube(points, indices);

  return;
}


// Affiche le tube à partir des points des triangles et les indices qui forment les triangles:
void Path::renderTube(vector<vec3> points, vector<triangle_index> indices) {
  GLuint vbo, vboi = 0;

  // Attribution d'un buffer de donneés:
  glGenBuffers(1, &vbo);  PRINT_OPENGL_ERROR();
  // Affectation du buffer courant:
  glBindBuffer(GL_ARRAY_BUFFER, vbo);  PRINT_OPENGL_ERROR();
  // Copie des donnees des sommets sur la carte graphique:
  glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(vec3), points.data(), GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();

  // Active l'utilisation des données de positioncenter_views:
  glEnableClientState(GL_VERTEX_ARRAY);  PRINT_OPENGL_ERROR();
  // Indique que le buffer courant (désigné par la variable vbo) est utilisé pour les positions de sommets
  glVertexPointer(3, GL_FLOAT, 1 * sizeof(vec3), 0);  PRINT_OPENGL_ERROR();

  // Attribution d'un autre buffer de données:
  glGenBuffers(1, &vboi);  PRINT_OPENGL_ERROR();
  // Affectation du buffer courant (buffer d'indice):
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboi);  PRINT_OPENGL_ERROR();
  // Copie des indices sur la carte graphique:
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(triangle_index), indices.data(), GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();

  
  glUniformMatrix4fv(get_uni_loc(m_render_program, "rotation"), 1, false, pointeur(mat4()));  PRINT_OPENGL_ERROR();
  glUniform4f(get_uni_loc(m_render_program, "rotation_center"), 0.0f, 0.0f, 0.0f, 0.0f);  PRINT_OPENGL_ERROR();  
  glUniform4f(get_uni_loc(m_render_program, "translation"), 0.0f, 0.0f, 0.0f, 0.0f);  PRINT_OPENGL_ERROR();

  glDrawElements(GL_TRIANGLES, indices.size() * 3 , GL_UNSIGNED_INT, 0);  PRINT_OPENGL_ERROR();

  return;
}