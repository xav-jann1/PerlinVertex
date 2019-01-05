// Librairie:
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>

// Header:
#include "callbacks.hpp"

// Outil:
#include "glutils.hpp"

/*****************************************************************************\
 * timer_callback                                                            *
\*****************************************************************************/
void timer_callback(int) {
  // Demande de rappel de cette fonction dans 25ms:
  glutTimerFunc(25, timer_callback, 0);

  // Mise à jour du jeu:
  update();

  // Réactualisation de l'affichage:
  glutPostRedisplay();
}

/*****************************************************************************\
 * keyboardUp_callback                                                         *
\*****************************************************************************/
void keyboardUp_callback(int key, int, int) {
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
void special_callback(int key, int, int) {
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
 * keyboard_callback                                                         *
\*****************************************************************************/
void keyboard_callback(unsigned char key, int, int) {
  // float d_angle = 0.001f;
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

    // Path:
    case 'b':
      path.updateRadius(-0.01);
      break;
    case 'n':
      path.updateRadius(0.01);
      break;

  }
}
