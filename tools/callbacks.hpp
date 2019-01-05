#ifndef CALLBACKS_H
#define CALLBACKS_H

// Classes:
#include "../classes/Camera.hpp"
#include "../classes/Cube.hpp"
#include "../classes/Path.hpp"
#include "../classes/Perlin.hpp"
#include "../classes/Player.hpp"

// Objets globaux:
extern Cube cube;
extern Player player;
extern Camera camera;
extern Path path;
extern Perlin perlinX, perlinY, perlinZ;

// Variables globales:
extern int path_points_deleted;

// Fonction globale:
extern void update();

// A supprimer:
extern int counter;
extern vec3 pPrev;
extern vec3 p00;
extern bool left, right;

// Clavier:
void keyboard_callback(unsigned char key, int, int);
void keyboardUp_callback(int key, int, int);
void special_callback(int key, int, int);

// Timer:
void timer_callback(int);

#endif