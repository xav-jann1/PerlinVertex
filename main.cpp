// TODO: trier et garder uniquement les includes utilisés

// Librairies:
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

// OpenGL:
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/glut.h>
#include "tools/glutils.hpp"

// Outils:
#include "tools/mat3.hpp"
#include "tools/mat4.hpp"
#include "tools/triangle_index.hpp"
#include "tools/vec2.hpp"
#include "tools/vec3.hpp"

// Classes:
#include "classes/Camera.hpp"
#include "classes/Cube.hpp"
#include "classes/Path.hpp"
#include "classes/Perlin.hpp"
#include "classes/Player.hpp"

/*****************************************************************************\
 * Variables globales
\*****************************************************************************/

// Identifiant du shader:
GLuint shader_program_id;

// Caméra:
Camera camera;

/** Eléments de jeu: **/

// Chemin:
Path path;
Perlin perlinX, perlinY, perlinZ;  // Générateur de valeurs aléatoires
vec3 p00;                          // Test Perlin

// Joueur:
Player player(&path);
float score;

// Ennemis:
// std::vector<Cube*> cubes;
Cube cube(&path);                  // Test PathAgent

// Contrôles: (TODO: dans un struct)
bool left = false;
bool right = false;

// Gestion des éléments du jeu:
int newCubeIn = 500;  // en ms
int prevCreation = 0;

// Matrice de projection:
mat4 projection;

// Test Perlin
int counter = 1;
vec3 pPrev = vec3();

/*****************************************************************************\
 * Fonctions
\*****************************************************************************/

/**
 * Initialisation des éléments du jeu:
 */
static void setup() {
   
  /** OpenGL **/

  // Chargement du shader:
  shader_program_id = read_shader("shaders/shader.vert", "shaders/shader.frag");

  // Matrice de projection:
  projection = matrice_projection(60.0f * M_PI / 180.0f, 1.0f, 0.01f, 1000.0f);
  glUniformMatrix4fv(get_uni_loc(shader_program_id, "projection"), 1, false, pointeur(projection));  PRINT_OPENGL_ERROR();

  // Activation de la gestion de la profondeur:
  glEnable(GL_DEPTH_TEST);  PRINT_OPENGL_ERROR();

  /** Eléments de jeu: **/

  // Caméra:
  camera.rotate_x(M_PI);

  // Chemin:
  path.setRenderProgram(shader_program_id);
  path.updateBetween(0, 10);

  /** Test PathAgent **/

  cube.setPointsAB(1);
  //cube.setSpeed(1);
  cube.setRenderProgram(shader_program_id);

  // Joueur:
  player.setRenderProgram(shader_program_id);
  player.init_model();
  player.setSpeed(1);
  player.setPointsAB(2);

}

/**
 * Mise à jour du jeu:
 */
void update() {

  /** Caméra: **/

  /** Chemin: **/
  float minA = player.getPathPosition().z - 10;
  float maxB = minA + 20;
  int path_points_deleted = path.updateBetween(minA, maxB);

  /** Joueur: **/

  // Déplacement du joueur:
  if (left == true) {
    // Déplace le joueur à gauche:
    float dL = 0.035f * ((player.getSpeed() - 1)/2 + 1);
    player.updateAngle(dL);

    // Modifie l'orientation du joueur:
    player.setDesiredAngle(-0.4f);
  } 
  else if (right == true) {
    // Déplace le joueur à droite:
    float dL = 0.035f * ((player.getSpeed() - 1)/2 + 1);
    player.updateAngle(-dL);

    // Modifie l'orientation du joueur:
    player.setDesiredAngle(0.4f);
  }
  else {
    player.setDesiredAngle(0);
  }

  // Mise à jour de la position du joueur:
  player.update(path_points_deleted);

  if (cube.update(path_points_deleted)) cube.setPointsAB(1);

  /** Ennemis: **/

  // maj de la position des cubes:
  /*for (unsigned int i = 0; i < cubes.size(); i++) {
    cubes[i]->update();
  }*/

  // Création d'un nouveau cube toutes les <newCubeIn> ms:
  if (glutGet(GLUT_ELAPSED_TIME) - prevCreation > newCubeIn) {
    prevCreation = glutGet(GLUT_ELAPSED_TIME);
    // cubes.push_back(generateCube());

  /** HitBox **/


  
  /** Accélération du jeu: **/
  // todo: ...

}

/**
 * Affichage:
 */
void draw() {
  // Effacement des couleurs du fond d'écran:
  glClearColor(0.5f, 0.6f, 0.9f, 1.0f);  PRINT_OPENGL_ERROR();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  PRINT_OPENGL_ERROR();

  // Envoie des paramètres caméra sur la carte graphique:
  glUniformMatrix4fv(get_uni_loc(shader_program_id, "cam_rotation"), 1, false, pointeur(camera.getMat4()));  PRINT_OPENGL_ERROR();
  glUniform4f(get_uni_loc(shader_program_id, "cam_rotation_center"), 0.0f, 0.0f, 0.0f, 0.0f);  PRINT_OPENGL_ERROR();
  glUniform4f(get_uni_loc(shader_program_id, "cam_translation"), camera.getX() + player.getPathPosition().x, camera.getY() + player.getPathPosition().y, camera.getZ() + player.getPathPosition().z, 0.0f);  PRINT_OPENGL_ERROR();
  //glUniform4f(get_uni_loc(shader_program_id, "cam_translation"), camera.getX(), camera.getY(), camera.getZ(), 0.0f);  PRINT_OPENGL_ERROR();

  // Affichage des cubes:
  /*Cube::loadCube(0.4);
  for (unsigned int i = 0; i < cubes.size(); i++) {
    cubes[i]->render();
  }*/
  Cube::loadCube(0.4);
  cube.render();

  // Affichage du tube:
  path.render();

  // Affichage du joueur:
  player.render();

  // Changement de buffer d'affichage pour éviter un effet de scintillement:
  glutSwapBuffers();
}

/*****************************************************************************\
 * Début du programme                                                        *
\*****************************************************************************/

int main(int argc, char** argv) {
  // Lancement des fonctions principales de GLUT:
  initGlut(argc, argv);

  // Fonction d'initialisation des données et chargement des shaders:
  setup();

  // Lancement de la boucle (infinie) d'affichage de la fenetre
  glutMainLoop();

  // Plus rien n'est executé après cela

  return 0;
}
