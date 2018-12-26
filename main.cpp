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

//using namespace std;


/*****************************************************************************\
 * Variables globales
\*****************************************************************************/

// Identifiant du shader:
GLuint shader_program_id;

// Caméra:
Camera camera;

// Eléments de jeu:
std::vector<Cube*> cubes;
Cube player;
Path path;

// Contrôles: (TODO: dans un struct)
bool left = false;
bool right = false;

// Matrice de projection:
mat4 projection;

// Chargement d'un cube sur la carte graphique:
void loadCube(float dim) {
  GLuint vbo, vboi;

  // Tableau entrelacant coordonnees-normales:
  vec3 geometrie[] = {dim * vec3(-0.2f, -0.2f, -0.2f), vec3(0.0f, 0.0f, 0.0f),
                      dim * vec3(0.2f, -0.2f, -0.2f),  vec3(1.0f, 0.0f, 0.0f),
                      dim * vec3(0.2f, 0.2f, -0.2f),   vec3(0.0f, 1.0f, 0.0f),
                      dim * vec3(-0.2f, 0.2f, -0.2f),  vec3(1.0f, 1.0f, 0.0f),
                      dim * vec3(-0.2f, -0.2f, 0.2f),  vec3(1.0f, 0.0f, 1.0f),
                      dim * vec3(0.2f, -0.2f, 0.2f),   vec3(1.0f, 0.0f, 0.0f),
                      dim * vec3(0.2f, 0.2f, 0.2f),    vec3(0.0f, 1.0f, 0.0f),
                      dim * vec3(-0.2f, 0.2f, 0.2f),   vec3(1.0f, 1.0f, 0.0f)};

  // Indice des triangles:
  triangle_index tri0 = triangle_index(0, 1, 2);
  triangle_index tri1 = triangle_index(0, 2, 3);
  triangle_index tri2 = triangle_index(1, 2, 5);
  triangle_index tri3 = triangle_index(5, 2, 6);
  triangle_index tri4 = triangle_index(0, 4, 3);
  triangle_index tri5 = triangle_index(4, 3, 7);
  triangle_index tri6 = triangle_index(4, 7, 5);
  triangle_index tri7 = triangle_index(5, 7, 6);
  triangle_index tri8 = triangle_index(2, 3, 6);
  triangle_index tri9 = triangle_index(3, 6, 7);
  triangle_index tri10 = triangle_index(0, 1, 5);
  triangle_index tri11 = triangle_index(0, 5, 4);

  triangle_index index[] = {tri0, tri1, tri2, tri3, tri4,  tri5,
                            tri6, tri7, tri8, tri9, tri10, tri11};

  // Attribution d'un buffer de donnees (1 indique la création d'un buffer):
  glGenBuffers(1, &vbo);  PRINT_OPENGL_ERROR();
  // Affectation du buffer courant:
  glBindBuffer(GL_ARRAY_BUFFER, vbo);  PRINT_OPENGL_ERROR();
  // Copie des donnees des sommets sur la carte graphique:
  glBufferData(GL_ARRAY_BUFFER, sizeof(geometrie), geometrie, GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();

  // Active l'utilisation des données de positioncenter_views:
  glEnableClientState(GL_VERTEX_ARRAY);  PRINT_OPENGL_ERROR();
  // Indique que le buffer courant (désigné par la variable vbo) est utilisé pour les positions de sommets:
  glVertexPointer(3, GL_FLOAT, 2 * sizeof(vec3), 0);  PRINT_OPENGL_ERROR();

  // Active l'utilisation des données de couleurs:
  glEnableClientState(GL_COLOR_ARRAY);  PRINT_OPENGL_ERROR();
  // Indique que le buffer courant (désigné par la variable vbo) est utilisé pour les couleurs:
  glColorPointer(3, GL_FLOAT, 2 * sizeof(vec3), buffer_offset(sizeof(vec3)));  PRINT_OPENGL_ERROR();

  // Attribution d'un autre buffer de données:
  glGenBuffers(1, &vboi);  PRINT_OPENGL_ERROR();
  // Affectation du buffer courant (buffer d'indice):
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboi);  PRINT_OPENGL_ERROR();
  // Copie des indices sur la carte graphique:
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();
}

// Génération d'un objet Cube:
Cube* generateCube() {
  Cube* c = new Cube();
  float angle = ((float)rand() / (RAND_MAX)) * 2 * M_PI;
  c->translateZ(-20.0f);
  c->rotateZ(angle);
  c->translateX(cos(angle));
  c->translateY(sin(angle));
  return c;
}

/*****************************************************************************\
 * init                                                                      *
\*****************************************************************************/
static void init() {

  // Initialisation de la caméra:
  camera.translation_y(1.0f);
  camera.translation_z(-1.0f);
  camera.rotate_x(M_PI / 10.0f);

  // Initialisation du joueur:
  player.translateZ(-2);
  player.translateY(-1.0f);

  // Chargement du shader:
  shader_program_id = read_shader("shaders/shader.vert", "shaders/shader.frag");

  // Matrice de projection:
  projection = matrice_projection(60.0f * M_PI / 180.0f, 1.0f, 0.01f, 50.0f);
  glUniformMatrix4fv(get_uni_loc(shader_program_id, "projection"), 1, false, pointeur(projection));  PRINT_OPENGL_ERROR();

  // Activation de la gestion de la profondeur:
  glEnable(GL_DEPTH_TEST);  PRINT_OPENGL_ERROR();


  /**** Test Tube ****/

  vec3 p1 = vec3(0,0,0);
  vec3 p2 = vec3(1,0,1);
  vec3 p3 = vec3(3,0,2);
  path.pushPoint(p1);
  path.pushPoint(p2);
  path.pushPoint(p3);
  path.setRenderProgram(shader_program_id);
  path.setRenderRadius(1);
  path.setRenderRes(10);

}

// Fonction d'affichage:
static void display_callback() {
  // Effacement des couleurs du fond d'écran:
  glClearColor(0.5f, 0.6f, 0.9f, 1.0f);  PRINT_OPENGL_ERROR();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  PRINT_OPENGL_ERROR();

  // Envoie des paramètres caméra sur la carte graphique:
  glUniformMatrix4fv(get_uni_loc(shader_program_id, "cam_rotation"), 1, false, pointeur(camera.getMat4()));  PRINT_OPENGL_ERROR();
  glUniform4f(get_uni_loc(shader_program_id, "cam_rotation_center"), 0.0f, 0.0f, 0.0f, 0.0f);  PRINT_OPENGL_ERROR();
  glUniform4f(get_uni_loc(shader_program_id, "cam_translation"), camera.getX(), camera.getY(), camera.getZ(), 0.0f);  PRINT_OPENGL_ERROR();

  // Affichage des cubes:
  loadCube(0.4);
  for (unsigned int i = 0; i < cubes.size(); i++) {
    cubes[i]->render(shader_program_id);
  }

  // Affichage du joueur:
  ///glUniformMatrix4fv(get_uni_loc(shader_program_id, "cam_rotation"), 1, false, pointeur(mat4()));  PRINT_OPENGL_ERROR();
  //player.render(shader_program_id);
  

  path.render();

  // Changement de buffer d'affichage pour eviter un effet de scintillement
  glutSwapBuffers();
}

// Gestion des éléments du jeu:
int newCubeIn = 100; // en ms
int prevCreation = 0;
void gameUpdate() {
  // maj de la position des cubes:
  for (unsigned int i = 0; i < cubes.size(); i++) {
    cubes[i]->update();
  }

  // Création nouveaux cubes:
  if (glutGet(GLUT_ELAPSED_TIME) - prevCreation > newCubeIn) {
    prevCreation = glutGet(GLUT_ELAPSED_TIME);
    cubes.push_back(generateCube());
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
  float d_angle = 0.1f;
  float dz = 0.5f;

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
      for (unsigned int i = 0; i < cubes.size(); i++) {
        cubes[i]->rotateX(d_angle * (i + 1));
      }
      break;
    case 's':
      for (unsigned int i = 0; i < cubes.size(); i++) {
        cubes[i]->rotateY(-d_angle * (i + 1));
      }
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

  // Plus rien n'est execute après cela

  return 0;
}
