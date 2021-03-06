// TODO: trier et garder uniquement les includes utilisés

// Librairies:
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>

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

// Identifiant des shaders:
GLuint shader_program_id;
GLuint shader_HUD;

// Caméra:
Camera camera;

/** Eléments de jeu: **/

// Chemin:
Path path;

// Joueur:
Player player(&path);
float score;

// Ennemis:
std::vector<Cube*> cubes;

// Contrôles:
struct Inputs {
  bool left, right, up, down;
};
struct Inputs inputs;

// Gestion des éléments du jeu:
int newCubeIn = 500;  // en ms
int prevCreation = 0;

// Matrice de projection:
mat4 projection;

/*****************************************************************************\
 * Fonctions Cubes
\*****************************************************************************/

// Ajoute un cube:
void addCube() {
  Cube* c = new Cube(&path);
  c->setSpeed(-2);
  c->setAngleSpeed(0);
  c->setRenderProgram(shader_program_id);
  cubes.push_back(c);
}

// Supprime un cube:
void removeCube(int i) { cubes.erase(cubes.begin() + i); }

// Mise à jour des cubes:
void updateCubes(int path_points_deleted) {
  for (int i = (int)cubes.size() - 1; i >= 0; i--) {
    bool canDelete = cubes[i]->update(path_points_deleted);
    if (canDelete) removeCube(i);
  }
}

// Affichage des cubes:
void renderCubes() {
  // Charge le modèle du cube sur le GPU:
  Cube::loadCube(0.4);

  // Affiche chaque cube:
  for (unsigned int i = 0; i < cubes.size(); i++) {
    cubes[i]->render();
  }
}

/*****************************************************************************\
 * HUD
\*****************************************************************************/

// Affichage du texte:
void vBitmapOutput(float x, float y, const char *string, void *font) {
  int len, i;  // len donne la longueur de la chaîne de caractères

  glRasterPos2f(x, y);        // Positionne le premier caractère de la chaîne
  len = (int)strlen(string);  // Calcule la longueur de la chaîne

  // Affiche chaque caractère de la chaîne
  for (i = 0; i < len; i++) glutBitmapCharacter(font, string[i]);  
}

void vStrokeOutput(GLfloat x, GLfloat y, char *string, void *font) {
  char *p;

  glPushMatrix();  // glPushMatrix et glPopMatrix sont utilisées pour
                   // sauvegarder et restaurer les systèmes de coordonnées non
                   // translatés
  glTranslatef(x, y, 0);  // Positionne le premier caractère de la chaîne

  // Affiche chaque caractère de la chaîne:
  for (p = string; *p; p++) glutStrokeCharacter(font, *p);  
  glPopMatrix();
}

GLuint vbo_HUD = 0;
void HUD_render() {

  /** Vie **/
  
  // Sommets du quadrilatère représentant la vie du joueur:
  float sommets[] = {-0.9f, 0.9f, 0.0f,
                     -0.9f + player.getLife() * 0.1f, 0.9f, 0.0f,
                     -0.9f + player.getLife() * 0.1f, 0.8f, 0.0f,
                     -0.9f, 0.8f, 0.0f};

  // Chargement du shader pour le HUD:
  glUseProgram(shader_HUD);

  // Attribution d'un buffer de données (1 indique la création d'un buffer):
  glGenBuffers(1, &vbo_HUD);  PRINT_OPENGL_ERROR();
  // Affectation du buffer courant:
  glBindBuffer(GL_ARRAY_BUFFER, vbo_HUD);  PRINT_OPENGL_ERROR();
  // Copie des données des sommets sur la carte graphique:
  glBufferData(GL_ARRAY_BUFFER, sizeof(sommets), sommets, GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();

  // Active l'utilisation des données de positions:
  glEnableClientState(GL_VERTEX_ARRAY);  PRINT_OPENGL_ERROR();
  // Indique que le buffer courant (désigné par la variable vbo) est utilisé pour les positions de sommets
  glVertexPointer(3, GL_FLOAT, 0, 0);  PRINT_OPENGL_ERROR();

  // Envoie des paramètres du HUD sur la carte graphique:
  float translation_x = 0.0f, translation_y = 0.0f, translation_z = 0.0f;
  glUniform4f(get_uni_loc(shader_HUD, "translation"), translation_x, translation_y, translation_z, 0.0f);  PRINT_OPENGL_ERROR();

  // Affiche la barre de vie:
  glDrawArrays(GL_QUADS, 0, 4);  PRINT_OPENGL_ERROR();

  /** Score **/
  std::ostringstream ss;
  ss << std::ceil(score);
  std::string str = ss.str();
  std::string texte = "Score : " + str;
  vBitmapOutput(0.62f, 0.82f, texte.c_str(), GLUT_BITMAP_HELVETICA_18);
  glFlush();
}

/*****************************************************************************\
 * Fonctions
\*****************************************************************************/

/**
 * Initialisation des éléments du jeu:
 */
static void setup() {
   
  /** OpenGL **/

  // Chargement du shader:
  shader_HUD = read_shader("shaders/shaderHUD.vert", "shaders/shaderHUD.frag");
  shader_program_id = read_shader("shaders/shader.vert", "shaders/shader.frag");
  
  // Matrice de projection:
  projection = matrice_projection(60.0f * M_PI / 180.0f, 1.0f, 0.01f, 1000.0f);
  glUniformMatrix4fv(get_uni_loc(shader_program_id, "projection"), 1, false, pointeur(projection));  PRINT_OPENGL_ERROR();

  // Activation de la gestion de la profondeur:
  glEnable(GL_DEPTH_TEST);  PRINT_OPENGL_ERROR();

  /** Eléments de jeu: **/

  // Caméra:
  camera.rotate_x(M_PI);
  camera.setPlayer(&player);
  camera.setRenderProgram(shader_program_id);
  camera.translation_z(-1.5f);

  // Chemin:
  path.setRenderProgram(shader_program_id);
  path.updateBetween(0, 10);  // Créer tube entre z=0 et z=10;

  // Joueur:
  player.setRenderProgram(shader_program_id);
  player.init_model();
  player.setSpeed(1);
  player.setPointsAB(3);

  // Ennemi:
  addCube();
}

/**
 * Mise à jour du jeu:
 */
void update() {

  /** Caméra: **/

  /** Chemin: **/
  float minA = player.getPathPosition().z - 5;
  float maxB = player.getPathPosition().z + 30;
  int path_points_deleted = path.updateBetween(minA, maxB);

  /** Joueur: **/

  // Déplacement du joueur:
  if (inputs.left == true) {
    // Déplace le joueur à gauche:
    float dL = 0.035f * ((player.getSpeed() - 1) / 3.0f + 1);
    player.updateAngle(dL);

    // Modifie l'orientation du joueur:
    player.setDesiredAngle(-0.4f);
  } 
  else if (inputs.right == true) {
    // Déplace le joueur à droite:
    float dL = 0.035f * ((player.getSpeed() - 1) / 3.0f + 1);
    player.updateAngle(-dL);

    // Modifie l'orientation du joueur:
    player.setDesiredAngle(0.4f);
  }
  else player.setDesiredAngle(0);
  
  if (inputs.up== true) player.updateSpeed(0.05f);
  if (inputs.down== true) player.updateSpeed(-0.05f);

  // Mise à jour de la position du joueur:
  player.update(path_points_deleted);

  /** Ennemis: **/

  // Création d'un nouveau cube toutes les <newCubeIn> ms:
  if (glutGet(GLUT_ELAPSED_TIME) - prevCreation > newCubeIn / (player.getSpeed() * 0.5)) {
    prevCreation = glutGet(GLUT_ELAPSED_TIME);
    addCube();
  }

  // Mise à jour de la position des cubes:
  updateCubes(path_points_deleted);

  /** HitBox **/

  // Position et angle du joueur:
  float player_position = player.getPathPosition().z + 0.5;
  float player_angle = player.getAngle();

  // Vérifie pour tous les cubes:
  for (unsigned int i = 0; i < cubes.size(); i++) {
    
    // Ecart de position avec le joueur:
    float cube_position = cubes[i]->getPathPosition().z;
    float diff_position = cube_position - player_position;

    // Si joueur et cube sont proches:
    if(fabs(diff_position) < 0.4) {
      // Vérifie l'angle:
      float cube_angle = cubes[i]->getAngle();
      float diff_angle = fmod(fabs(player_angle - cube_angle) + M_PI, 2 * M_PI) - M_PI;

      // Si joueur et cube se touche:
      if(fabs(diff_angle) < 0.25) {
        std::cout << "Touché" << std::endl;
        bool stillAlive = player.hit();
        if (!stillAlive){
           std::cout << "Game Over !" << std::endl;
           std::exit(0);
        }
        removeCube(i);
      }
    }

  }

  /** Accélération du jeu: **/
  if (player.getSpeed() <= 5.0f){
    player.setSpeed(player.getSpeed() + 0.0004f);
  }

  // Incrémentation du score:
  score += player.getSpeed();

}

/**
 * Affichage:
 */
void draw() {
  // Effacement des couleurs du fond d'écran:
  glClearColor(0.5f, 0.6f, 0.9f, 1.0f);  PRINT_OPENGL_ERROR();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  PRINT_OPENGL_ERROR();

  // Utilise le shader de rendu du jeu:
  glUseProgram(shader_program_id);

  // Envoie des paramètres caméra sur la carte graphique:
  camera.sendDataToGPU();

  // Affichage des cubes:
  renderCubes();

  // Affichage du tube:
  path.render();

  // Affichage du joueur:
  player.render();

  // HUD:
  HUD_render();

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
