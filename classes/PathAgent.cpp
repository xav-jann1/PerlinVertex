// Header:
#include "PathAgent.hpp"

// Librairies:
#include <GL/glut.h>
#include <cmath>

// Outils:
#include "../tools/mat3.hpp"
#include "../tools/vec3.hpp"

// Constructeur:
PathAgent::PathAgent(Path* path) {
  // Chemin:
  m_path = path;

  // Paramètres de l'agent:
  setAngle(0);
  setSpeed(1);
  setAngleSpeed(0);

  // Variables pour le déplacement de l'agent:
  m_iteratorA = 0;
  m_pointA = vec3(0, 0, 0);
  m_pointB = vec3(0, 0, 1);
  updateDirection();
};

/**
 * Setters:
 */

// Angle:
void PathAgent::setAngle(float angle) {
  // TODO: ajouter vérification <0??
  m_path_angle = angle;
}

// Vitesse de rotation:
void PathAgent::setAngleSpeed(float speed) { m_angle_speed = speed; }

// Vitesse de translation:
void PathAgent::setSpeed(float speed) { m_speed = speed; }

// Modification relative de la vitesse de translation:
void PathAgent::updateSpeed(float dspeed) { m_speed += dspeed; }

// Génère le vecteur normalisé dirigé du point A vers le point B:
void PathAgent::updateDirection() {
  m_direction = normalize(m_pointB - m_pointA);
}

// Défini les points entre lesquels l'agent se déplace
// parmi les points du chemin:
void PathAgent::setPointsAB(int i) {
  m_iteratorA = i;
  m_pointA = m_path->getPoint(m_iteratorA);
  m_pointB = m_path->getPoint(m_iteratorA + 1);
  updateDirection();

  m_path_position = m_pointA;

  m_updated_time = glutGet(GLUT_ELAPSED_TIME);  // TODO: à enlever
}

/**
 * Getter:
 */

// Position de l'agent sur le tube:
vec3 PathAgent::getPos() { return m_tube_position; }

// Direction:
vec3 PathAgent::getDirection() { return m_direction;}

/**
 * Méthodes:
 */

// Ecart de temps (en ms) entre deux appels de cette fonction:
float PathAgent::getDeltaTime() {
  int now = glutGet(GLUT_ELAPSED_TIME);
  int deltaTime = now - m_updated_time;
  m_updated_time = now;

  return deltaTime / 1000.0f;
}

/**
 * Mise à jour de la position de l'Agent sur le chemin et sur le tube:
 * @param int path_points_deleted  Nombre de points supprimé du chemin depuis la
 *                                 dernière mise à jour de l'agent
 * @return bool canDelete  Averti si l'agent peut être supprimé
 *                         (car en dehors du chemin)
 */
bool PathAgent::update(int path_points_deleted) {
  float deltaTime = getDeltaTime();

  /** Vérification **/

  // Mise à jour de l'itérateur sur les points du chemin:
  m_iteratorA -= path_points_deleted;

  // Si le point A n'existe plus sur le chemin:
  if (m_iteratorA < 0)
    // L'agent peut être supprimé:
    return true;

  /** Mise à jour de la position de l'agent sur le chemin **/

  // Mise à jour de l'angle:
  m_path_angle += m_angle_speed * deltaTime;

  // Mise à jour de la position:
  m_path_position += m_speed * m_direction * deltaTime;

  /** Vérification de la position **/

  // Si l'agent ne se trouve plus entre les points A et B:

  // Si l'agent a dépassé le point A:
  if (m_path_position.z < m_pointA.z) {
    // S'il n'y a plus de point A avant:
    if (m_iteratorA - 1 < 0)
      // L'agent peut être supprimé:
      return true;

    // Recule l'itérateur sur les points du chemin:
    m_iteratorA--;

    // Calcul la distance dépassée:
    float distance = norm(m_pointA - m_path_position);  // todo: ajouter sqrt
    // Mise à jour des points A et B:
    m_pointB = m_pointA;
    m_pointA = m_path->getPoint(m_iteratorA);

    // Nouveau vecteur de direction pour aller du point A au point B:
    updateDirection();

    // Nouvelle position de l'agent:
    m_path_position = m_pointB - m_direction * distance;
  }

  // Si l'agent a dépassé le point B:
  else if (m_path_position.z > m_pointB.z) {
    // S'il n'y a plus de point B après:
    if ((m_iteratorA + 1) + 1 >= m_path->getLength())
      // L'agent peut être supprimé:
      return true;

    // Avance l'itérateur sur les points du chemin:
    m_iteratorA++;

    // Calcul la distance dépassée:
    float distance = norm(m_pointB - m_path_position);  // todo: ajouter sqrt

    // Mise à jour des points A et B:
    m_pointA = m_pointB;
    m_pointB = m_path->getPoint(m_iteratorA + 1);

    // Nouveau vecteur de direction pour aller du point A au point B:
    updateDirection();

    // Nouvelle position de l'agent:
    m_path_position = m_pointA + m_direction * distance;
  }

  /** Position de l'agent dans le tube **/
  // TODO...
  /* m_tube_position = 0;

  // Position au bord du tube, sans rotation:
  vec3 position = vec3(m_path->getRadius(), 0, 0); // TODO: ajouter offset

  // Ajoute la rotation:
  mat3 rotation_z = mat3_rotation(m_path_angle, 0.0f, 0.0f, 1.0f);
  position = rotation_z * position;*/

  // Ajoute la translation:
  m_tube_position = m_path_position;

  return false;
}