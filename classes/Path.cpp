// Header:
#include "Path.hpp"

// Outils:
#include "../tools/glutils.hpp"
#include "../tools/mat4.hpp"

using namespace std;

// Constructeur:
Path::Path() {
  // Paramètres par défaut du tube:
  m_render_res = 10;
  m_render_radius = 1;

  // Chargement d'un cercle de référence:
  m_render_cercle = generateCercle();
}

/**
 * Chemin:
 */

// Ajoute un point à la liste du chemin et continue le tube:
void Path::pushPoint(vec3 point) {
  m_points.push_back(point);

  // Ajoute le rendu de la nouvelle partie du tube:
  addRender(m_points.size() - 1);
}

// Enlève le premier point de la liste:
void Path::removeFirst() {
  m_points.erase(m_points.begin());

  // Nouveau rendu du tube:
  resetRender();

  // TODO: à optimiser, en enlevant directement les valeurs utilisé par
  //       le point dans m_render_sommets et m_render_indices
}

/**
 * Paramètres du tube:
 */

// Définit les shaders utilisés par render():
void Path::setRenderProgram(GLuint program) { m_render_program = program; }

// Définit la résolution du tube (= nombre de points par cercle):
void Path::setRenderRes(int N) {
  m_render_res = N;

  // Nouveau rendu du tube:
  resetRender();
}

// Définit le rayon du tube:
void Path::setRenderRadius(float r) {
  // Si le rayon est trop faible:
  if (r < 0.1) {
    cerr << "setRenderRadius: rayon donné trop faible -> " << r << endl;
    return;
  }

  m_render_radius = r;

  // Nouveau rendu du tube:
  resetRender();
}

// Modifie le rayon du tube:
void Path::updateRadius(float dr) { setRenderRadius(m_render_radius + dr); }

/**
 * Méthodes:
 */

// Création d'un cercles de N points et de rayon r sur le plan (x,y):
vector<vec3> Path::generateCercle() {
  int N = m_render_res;       // Nombre de points constituant le cercle
  float r = m_render_radius;  // Rayon du cercle

  // Liste des sommets:
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
vector<vec3> Path::moveCercle(vector<vec3> points, vec3 dir, vec3 normale) {
  // Matrice de rotation pour orienter correctement chaque point du cercle:
  mat3 orient = mat3_rotation_from_vec3(normale);

  // Pour chaque point:
  for (int i = 0; i < (int)points.size(); i++)
    // Ajoute la rotation puis la translation:
    points[i] = orient * points[i] + dir;

  return points;
}

// Ajoute les sommets à la liste de sommets:
void Path::insertSommets(vector<vec3> sommets) {
  m_render_sommets.insert(m_render_sommets.end(), sommets.begin(), sommets.end());
}

/**
 * Pré-rendu:
 */

// Ajoute le premier cercle du chemin, orienté en direction du deuxième point:
void Path::addFirstRender() {
  // S'il n'y a pas assez de points:
  if (m_points.size() < 2) {
    cerr << "Rendu de la première portion du tube impossible : pas suffisament de points" << endl;
    return;
  }

  // Points du cercle autour du premier point ("vers" le deuxième point):
  vec3 pos = m_points[0];
  vec3 normale = m_points[1] - pos;
  vector<vec3> cercle = moveCercle(m_render_cercle, pos, normale);

  // Ajoute les points à l'ensemble de points:
  insertSommets(cercle);
}

// Ajoute le rendu pour créer le tube entre le point <idx> et <idx-1>:
void Path::addRender(int idx) {
  // S'il n'y a pas assez de points:
  if (m_points.size() < 2) {
    cerr << "Rendu de la portion du tube impossible : nombre insuffisant de points" << endl;
    return;
  }

  // Ajoute le premier cercle lors du traitement du 2ème point:
  if (idx == 1) {
    addFirstRender();
  }

  // Récupère les points du cercle formés en direction du point précédent:
  vec3 pos = m_points[idx];
  vec3 normale = pos - m_points[idx - 1];  // Normale en direction du point précédent
  vector<vec3> cercle = moveCercle(m_render_cercle, pos, normale);

  // Ajoute les points à l'ensemble de points:
  insertSommets(cercle);

  // Ajoute les indices des triangles pour former le tube entre le cercle ajouté et le précédent:
  addTubeIndices(idx);
}

// Ajoute les indices pour modéliser un tube entre le point i et i-1 du chemin:
void Path::addTubeIndices(int i) {
  // Indices du premier sommet de chaque cercle:
  int iCercle1 = (i - 1) * (m_render_res + 1) + 1;  // Indice du premier point du cercle 1 (précédent)
  int iCercle2 = iCercle1 + m_render_res + 1;       // Indice du premier point du cercle 2 (en cours)

  /** Adaption d'indices **/

  // Pour corriger une erreur d'orientation des deux cercles
  // (si les premiers indices des deux cercles ne sont pas juste en face)
  // il faut adapter, si nécessaire, les indices pour créer les triangles:

  // Calcul la distance entre les points supposés les plus proches,
  // et les points supposé les plus éloignés, entre le Cercle1 et Cercle2:
  float d1 = norm(m_render_sommets[iCercle1] - m_render_sommets[iCercle2]);  // Supposé les plus proches
  float d2 = norm(m_render_sommets[iCercle1 + m_render_res / 2] - m_render_sommets[iCercle2]);  // Supposé les plus éloignés

  // Pour décaler, si nécessaire les indices pour créer les triangles:
  int di = 0;
  // Si les points les plus éloignés sont en réalité les plus proches:
  if (d2 < d1) di = m_render_res / 2 - 1;  // Décalage des indices

  /** Ajout des indices **/

  // Ajoute les triangles, entre le triangle précédent et en cours, pour former le tube:
  for (int j = 0; j < m_render_res - 1; ++j) {
    m_render_indices.push_back(triangle_index(iCercle1 + j, iCercle2 + (j + di) % m_render_res, iCercle2 + (j + 1 + di) % m_render_res));
    m_render_indices.push_back(triangle_index(iCercle1 + j, iCercle1 + j + 1, iCercle2 + (j + 1 + di) % m_render_res));
  }
  m_render_indices.push_back(triangle_index(iCercle1, iCercle1 + m_render_res - 1, iCercle2 + di));
  m_render_indices.push_back(triangle_index(iCercle1 + m_render_res - 1, iCercle2 + di, iCercle2 + (m_render_res - 1 + di) % m_render_res));

  return;
}

// Recommence le pré-rendu depuis le début:
void Path::resetRender() {
  // S'il n'y a pas assez de points:
  if (m_points.size() < 2) {
    cerr << "Rendu du tube impossible : pas suffisament de points" << endl;
    return;
  }

  // Chargement d'un cercle de référence:
  m_render_cercle = generateCercle();

  // Réinitialisation des points et indices composant le tube:
  m_render_sommets.clear();
  m_render_indices.clear();

  // Pour chaque point du chemin:
  for (int i = 1; i < (int)m_points.size(); ++i) {
    // Ajoute le rendu du tube entre les points i et i-1;
    addRender(i);
  }

  return;
}

/**
 * Rendu
 */

// Affiche le tube à partir des points des triangles 
// et les indices qui forment les triangles:
void Path::render() {
  GLuint vbo, vboi = 0;

  // Attribution d'un buffer de donneés:
  glGenBuffers(1, &vbo);  PRINT_OPENGL_ERROR();
  // Affectation du buffer courant:
  glBindBuffer(GL_ARRAY_BUFFER, vbo);  PRINT_OPENGL_ERROR();
  // Copie des données des sommets sur la carte graphique:
  glBufferData(GL_ARRAY_BUFFER, m_render_sommets.size() * sizeof(vec3), m_render_sommets.data(), GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();

  // Active l'utilisation des données de positioncenter_views:
  glEnableClientState(GL_VERTEX_ARRAY);  PRINT_OPENGL_ERROR();
  // Indique que le buffer courant (désigné par la variable vbo) est utilisé pour les positions de sommets
  glVertexPointer(3, GL_FLOAT, 1 * sizeof(vec3), 0);  PRINT_OPENGL_ERROR();

  // Attribution d'un autre buffer de données:
  glGenBuffers(1, &vboi);  PRINT_OPENGL_ERROR();
  // Affectation du buffer courant (buffer d'indice):
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboi);  PRINT_OPENGL_ERROR();
  // Copie des indices sur la carte graphique:
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_render_indices.size() * sizeof(triangle_index), m_render_indices.data(), GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();

  // Paramètres du shader:
  glUniformMatrix4fv(get_uni_loc(m_render_program, "rotation"), 1, false, pointeur(mat4()));  PRINT_OPENGL_ERROR();
  glUniform4f(get_uni_loc(m_render_program, "rotation_center"), 0.0f, 0.0f, 0.0f, 0.0f);  PRINT_OPENGL_ERROR();
  glUniform4f(get_uni_loc(m_render_program, "translation"), 0.0f, 0.0f, 0.0f, 0.0f);  PRINT_OPENGL_ERROR();

  // Affichage du tube:
  //glDrawElements(GL_TRIANGLES, m_render_indices.size() * 3, GL_UNSIGNED_INT, 0);  PRINT_OPENGL_ERROR();
  glDrawElements(GL_LINES, m_render_indices.size() * 3, GL_UNSIGNED_INT, 0);  PRINT_OPENGL_ERROR();

  return;
}

/**
 * Getters:
 */

// Nombre de points composant le chemin:
int Path::getLength() { return m_points.size(); }

// Point du chemin:
vec3 Path::getPoint(int n) { 
  if(n >= 0 && n < (int)m_points.size()) 
    return m_points[n];

  // Sinon, renvoie un vecteur nul:
  cout << "Path::getPoint() : impossible de récupérer le point " << n << endl;
  return vec3(0,0,0);
}

// Rayon du tube:
float Path::getRadius() { return m_render_radius; }
