#ifndef PATH_AGENT_H
#define PATH_AGENT_H

// Outils:
#include "../tools/vec3.hpp"

// Classe:
#include "Path.hpp"

class PathAgent {
 public:
  // Constructeurs:
  PathAgent();
  PathAgent(Path*);

  // Mise à jour de l'agent:
  bool update(int path_points_deleted = 0);

  // Modification des paramètres de l'agent:
  void setSpeed(float);
  void updateSpeed(float);
  void setAngle(float);
  void setAngleSpeed(float);
  void setOffset(float);
  void setPointsAB(int);

  // Détermine le vecteur normalisé reliant le point A au point B:
  void updateDirection();

  // Accès à la position et direction de l'agent:
  vec3 getPos();
  vec3 getDirection();

 private:
  // Chemin sur lequel se trouve l'agent:
  Path* m_path;

  /** Paramètres de l'agent **/
  float m_speed;        // Vitesse de déplacement
  float m_angle_speed;  // Vitesse de rotation
  float m_tube_offset;  // Correction de la position de l'agent sur le tube

  /** Déplacement de l'agent sur le chemin **/

  int m_iteratorA;     // Indice du point A dans la liste de points du chemin
  float m_path_angle;  // Orientation de l'agent autour du tube

  // Coordonnées des points entre lesquels l'agent s'y déplace:
  vec3 m_pointA, m_pointB;

  // Position de l'agent sur le tube:
  vec3 m_tube_position;

  vec3 m_direction;  // Vecteur normalisé dirigé du point A vers le point B
  vec3 m_path_position;  // Position de l'agent sur le chemin

  int m_updated_time;  // Temps de la dernière mise à jour de l'agent

  /** Méthode **/

  // Mesure l'écart de temps entre deux appels de cette méthode:
  float getDeltaTime();
};

#endif