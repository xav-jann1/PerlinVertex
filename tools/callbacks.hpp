#ifndef CALLBACKS_H
#define CALLBACKS_H

// Classes:
#include "../classes/Camera.hpp"
#include "../classes/Cube.hpp"
#include "../classes/Path.hpp"
#include "../classes/Perlin.hpp"
#include "../classes/Player.hpp"

// Objets globaux:
extern Player player;
extern Camera camera;
extern Path path;

// Variable globale:
struct Inputs {
  bool left, right, up, down;
};
extern struct Inputs inputs;

// Fonction globale:
extern void update();

// Clavier:
void keyboard_callback(unsigned char key, int, int);
void keyboardUp_callback(int key, int, int);
void special_callback(int key, int, int);

// Timer:
void timer_callback(int);

#endif