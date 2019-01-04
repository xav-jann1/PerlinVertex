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

//using namespace std;

/*****************************************************************************\
 * Variables globales
\*****************************************************************************/

// Identifiant du shader:
GLuint shader_program_id;

// Caméra:
Camera camera;

// Eléments de jeu:
//std::vector<Cube*> cubes;
//Cube player;
Path path;
Perlin perlinX, perlinY, perlinZ;  // Générateur de valeurs aléatoires
vec3 p00;  // Test Perlin
Cube cube(&path); // Test PathAgent
int path_points_deleted = 0;

// Contrôles: (TODO: dans un struct)
bool left = false;
bool right = false;

// Matrice de projection:
mat4 projection;


// Génération d'un objet Cube: (TODO: à intégrer dans le constructeur de Cube)
Cube* generateCube() {
  Cube* c = new Cube(&path);
  float angle = ((float)rand() / (RAND_MAX)) * 2 * M_PI;
  c->translateZ(-20.0f);
  c->rotateZ(angle);
  c->translateX(cos(angle));
  c->translateY(sin(angle));
  return c;
}

// Test Perlin
int counter = 1;
vec3 pPrev = vec3();

/*****************************************************************************\
 * init                                                                      *
\*****************************************************************************/
static void init() {
  // Initialisation de la caméra:
  camera.translation_y(1.0f);
  camera.translation_z(-1.0f);
  camera.rotate_x(M_PI);

  // Initialisation du joueur:
  //player.translateZ(-2);
  //player.translateY(-1.0f);

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

    //std::cout << i << " " << (p - p0) * 20 << std::endl;
  }

  p00 = p0;
  
  path.setRenderProgram(shader_program_id);



  /** Test PathAgent **/

  cube.setPointsAB(8);
  cube.setSpeed(1);

}

// Fonction d'affichage:
static void display_callback() {
  // Effacement des couleurs du fond d'écran:
  glClearColor(0.5f, 0.6f, 0.9f, 1.0f);  PRINT_OPENGL_ERROR();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  PRINT_OPENGL_ERROR();

  //camera.setX(cube.getCoord().x);
  //camera.setY(cube.getCoord().y);
  //camera.setZ(cube.getCoord().z - 0.5);

  //camera.print();

  /*mat3 rotation3 = mat3_rotation_from_vec3(cube.getDirection());
  mat4 rotation4 = mat4_from_mat3(rotation3);*/

  // Envoie des paramètres caméra sur la carte graphique:
  //glUniformMatrix4fv(get_uni_loc(shader_program_id, "cam_rotation"), 1, false, pointeur(rotation4));  PRINT_OPENGL_ERROR();
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
  glUniformMatrix4fv(get_uni_loc(shader_program_id, "cam_rotation"), 1, false, pointeur(mat4()));  PRINT_OPENGL_ERROR();
  player.render(shader_program_id);
*/
  // Changement de buffer d'affichage pour eviter un effet de scintillement
  glutSwapBuffers();
}

// Gestion des éléments du jeu:
int newCubeIn = 500; // en ms
int prevCreation = 0;



void gameUpdate() {
  // maj de la position des cubes:
  /*for (unsigned int i = 0; i < cubes.size(); i++) {
    cubes[i]->update();
  }*/

  if(cube.update(path_points_deleted)) cube.setPointsAB(1);
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

/*****************************************************************************\
 * keyboard_callback                                                         *
\*****************************************************************************/
static void keyboard_callback(unsigned char key, int, int) {
  //float d_angle = 0.001f;
  float d_angle = 0.01f;
  float dz = 0.1f;

  // Quitte le programme si on appuie sur les touches 'q', 'Q', ou 'echap'
  switch (key) {
    case 'q':
    case 'Q':
    case 27:
      exit(0);
      break;

    // Caméra:
    case 'i':
      camera.rotate_x(d_angle);
      break;
    case 'k':
      camera.rotate_x(-d_angle);
      break;

    case 'j':
      camera.rotate_y(d_angle);
      break;
    case 'l':
      camera.rotate_y(-d_angle);
      break;

    case 'p':
      camera.translation_z(dz);
      break;
    case 'm':
      camera.translation_z(-dz);
      break;

    // Cubes:
    case 'z':
      camera.rotate_x(d_angle*10);
      /*for (unsigned int i = 0; i < cubes.size(); i++) {
        cubes[i]->rotateX(d_angle * (i + 1));
      }*/
      break;
    case 's':
      /*for (unsigned int i = 0; i < cubes.size(); i++) {
        cubes[i]->rotateY(-d_angle * (i + 1));
      }*/
      if(cube.update()) std::cout << "true" << std::endl;
      break;

    case 'a':
      cube.updateSpeed(0.1);
      break;
    case 'w':
      cube.updateSpeed(-0.1);
      break;

    // Path:
    case 'b':
      path.updateRadius(-0.01);
      break;
    case 'n':
      path.updateRadius(0.01);
      break;

    case 'x':
      path.removeFirst();
      path_points_deleted++;
      break;


    // Perlin:
    case 'r':
      /*** Test Perlin ***/
      vec3 p = vec3(perlinX.getNext(), perlinY.getNext(), counter / 10.0f);
      std::cout << "p " << p << std::endl;
      std::cout << "dp " << p - pPrev << std::endl;
      path.pushPoint((p - p00) * 20);

      std::cout << std::endl;

      counter++;
      pPrev = p;
      break;
  }
}

static void keyboardUp_callback(int key, int, int) {
  switch (key) {
    case GLUT_KEY_LEFT:
      left = false;
      break;

    case GLUT_KEY_RIGHT:
      right = false;
      break;
  }
}

/*****************************************************************************\
 * special_callback                                                          *
\*****************************************************************************/
static void special_callback(int key, int, int) {
  float dL = 0.03f;
  switch (key) {
    case GLUT_KEY_UP:
      camera.translation_y(dL);  // rotation avec la touche du haut
      break;
    case GLUT_KEY_DOWN:
      camera.translation_y(-dL);  // rotation avec la touche du bas
      break;
    case GLUT_KEY_LEFT:
      left = true;
      // camera.rotate_z(dL); //rotation avec la touche de gauche
      break;
    case GLUT_KEY_RIGHT:
      right = true;
      // camera.rotate_z(-dL); //rotation avec la touche de droite
      break;
  }
}

/*****************************************************************************\
 * timer_callback                                                            *
\*****************************************************************************/
static void timer_callback(int) {
  // Demande de rappel de cette fonction dans 25ms:
  glutTimerFunc(25, timer_callback, 0);

  // Gestion du jeu:
  gameUpdate();

  // Réactualisation de l'affichage:
  glutPostRedisplay();
}


/*****************************************************************************\
 * Début du programme                                                        *
\*****************************************************************************/

int main(int argc, char** argv) {
  //**********************************************//
  // Lancement des fonctions principales de GLUT
  //**********************************************//

  // initialisation
  glutInit(&argc, argv);

  // TODO: intégrer l'initialisation de GLUT dans une fonction ou un objet

  // Mode d'affichage (couleur, gestion de profondeur, ...)
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  // Taille de la fenêtre à l'ouverture:
  glutInitWindowSize(800, 600);

  // Titre de la fenetre:
  glutCreateWindow("PerlinVertex");

  // Fonction de la boucle d'affichage:
  glutDisplayFunc(display_callback);

  // Fonction de gestion du clavier:
  glutKeyboardFunc(keyboard_callback);

  // Fonction de gestion du clavier:
  glutSpecialUpFunc(keyboardUp_callback);

  // Fonction des touches speciales du clavier (fleches directionnelles)
  glutSpecialFunc(special_callback);

  // Fonction d'appel d'affichage en chaine:
  glutTimerFunc(25, timer_callback, 0);

  // Initialisation des fonctions OpenGL:
  glewInit();

  // Notre fonction d'initialisation des données et chargement des shaders:
  init();

  // Lancement de la boucle (infinie) d'affichage de la fenetre
  glutMainLoop();

  // Plus rien n'est executé après cela

  return 0;
}
