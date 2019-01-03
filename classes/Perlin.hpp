#ifndef PERLIN_H
#define PERLIN_H

/**
 * Génère des valeurs aléatoires suivant le bruit de Perlin
 *   Algorithme inspiré par : 'Procedural Generation Part 1 - 1D Perlin Noise'
 *    (https://codepen.io/Tobsta/post/procedural-generation-part-1-1d-perlin-noise)
 */

class Perlin {
 public:
  // Constructeur:
  Perlin();

  // Paramètre du générateur de valeurs aléatoires:
  void setSeed(unsigned long long seed);
  void setAmplitude(float);

  // Pour récupérer une ou plusieurs valeurs aléatoires:
  float getNext();
  float* getNext(int);

 private:
  // Générateur de valeurs aléatoires:
  unsigned long long m_M, m_A, m_C, m_Z;

  // Paramètres du bruit:
  float m_amplitude;
  int m_wavelength;
  float m_frequency;

  // Valeurs aléatoires:
  double m_a, m_b;

  // Itérateur:
  int m_x;

  // Méthode pour générer des valeurs aléatoires:
  float interpolate(float, float, float);
  double custom_rand();
};

#endif