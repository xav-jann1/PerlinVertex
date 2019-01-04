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

/*****************************************************************************\
 * Variables globales
\*****************************************************************************/

// Identifiant du shader:
GLuint shader_program_id;

// Caméra:
Camera camera;

// Eléments de jeu:
// std::vector<Cube*> cubes;
// Cube player;
Path path;
Perlin perlinX, perlinY, perlinZ;  // Générateur de valeurs aléatoires
vec3 p00;                          // Test Perlin
Cube cube(&path);                  // Test PathAgent
int path_points_deleted = 0;

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
  // Initialisation de la caméra:
  camera.rotate_x(M_PI);

  // Initialisation du joueur:
  // player.translateZ(-2);
  // player.translateY(-1.0f);

  // Chargement du shader:
  shader_program_id = read_shader("shaders/shader.vert", "shaders/shader.frag");

  // Matrice de projection:
  projection = matrice_projection(60.0f * M_PI / 180.0f, 1.0f, 0.01f, 1000.0f);
  glUniformMatrix4fv(get_uni_loc(shader_program_id, "projection"), 1, false, pointeur(projection));  PRINT_OPENGL_ERROR();

  // Activation de la gestion de la profondeur:
  glEnable(GL_DEPTH_TEST);  PRINT_OPENGL_ERROR();

  /**** Test Perlin ****/
  int nCount = 10;

  // Ajoute les points au chemin :
  vec3 p0 = vec3(perlinX.getNext(), perlinY.getNext(), 0);
  path.pushPoint((p0 - p0) * 20);
  for (int i = 1; i < nCount; i++) {
    vec3 p = vec3(perlinX.getNext(), perlinY.getNext(), counter / 10.0f);
    path.pushPoint((p - p0) * 20);
    counter++;
  }

  p00 = p0;

  path.setRenderProgram(shader_program_id);

  /** Test PathAgent **/

  cube.setPointsAB(8);
  cube.setSpeed(1);
}

/**
 * Mise à jour du jeu:
 */
void gameUpdate() {
  // maj de la position des cubes:
  /*for (unsigned int i = 0; i < cubes.size(); i++) {
    cubes[i]->update();
  }*/

  // Mise à jour de la position de la caméra:
  camera.setX(cube.getPathCoord().x);
  camera.setY(cube.getPathCoord().y);
  camera.setZ(cube.getPathCoord().z);

  if (cube.update(path_points_deleted)) cube.setPointsAB(1);
  path_points_deleted = 0;

  // Création d'un nouveau cube toutes les <newCubeIn> ms:
  if (glutGet(GLUT_ELAPSED_TIME) - prevCreation > newCubeIn) {
    prevCreation = glutGet(GLUT_ELAPSED_TIME);
    // cubes.push_back(generateCube());

    /*** Test Perlin ***/
    /*vec3 p = vec3(perlinX.getNext(), perlinY.getNext(), counter / 10.0f);
    std::cout << "dp " << p - pPrev << std::endl;
    path.pushPoint((p - p00) * 20);

    std::cout << std::endl;

    counter++;
    pPrev = p;*/
  }

  // Déplacement du joueur <=> déplacement de la caméra:
  if (left == true) {
    float dL = 0.01f;
    camera.rotate_z(dL);
  }
  if (right == true) {
    float dL = 0.01f;
    camera.rotate_z(-dL);
  }
}

/**
 * Affichage:
 */
void draw() {
  // Effacement des couleurs du fond d'écran:
  glClearColor(0.5f, 0.6f, 0.9f, 1.0f);  PRINT_OPENGL_ERROR();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  PRINT_OPENGL_ERROR();

  camera.print();

  // mat3 rotation3 = mat3_rotation_from_vec3(cube.getDirection() * -1.0f);
  // mat4 rotation4 = mat4_from_mat3(rotation3);

  // Envoie des paramètres caméra sur la carte graphique:
  // glUniformMatrix4fv(get_uni_loc(shader_program_id, "cam_rotation"), 1,
  // false, pointeur(rotation4));  PRINT_OPENGL_ERROR();
  glUniformMatrix4fv(get_uni_loc(shader_program_id, "cam_rotation"), 1, false, pointeur(camera.getMat4()));  PRINT_OPENGL_ERROR();
  glUniform4f(get_uni_loc(shader_program_id, "cam_rotation_center"), 0.0f, 0.0f, 0.0f, 0.0f);  PRINT_OPENGL_ERROR();
  glUniform4f(get_uni_loc(shader_program_id, "cam_translation"), camera.getX(), camera.getY(), camera.getZ(), 0.0f);  PRINT_OPENGL_ERROR();

  // Affichage des cubes:
  /*Cube::loadCube(0.4);
  for (unsigned int i = 0; i < cubes.size(); i++) {
    cubes[i]->render(shader_program_id);
  }*/

  Cube::loadCube(0.4);
  cube.render(shader_program_id);

  // Affichage du tube:
  path.render();

  // Affichage du joueur:
  /*Cube::loadCube(0.4);
  glUniformMatrix4fv(get_uni_loc(shader_program_id, "cam_rotation"), 1, false,
  pointeur(mat4()));  PRINT_OPENGL_ERROR(); player.render(shader_program_id);
*/
  // Changement de buffer d'affichage pour eviter un effet de scintillement
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
