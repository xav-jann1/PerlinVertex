// Header:
#include "Perlin.hpp"

// Librairies:
#include <math.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

// Constructeur:
Perlin::Perlin() {
  // Paramètres du générateur de valeurs aléatoires:
  m_M = 4294967296;
  m_A = 1664525;  // m_A - 1 should be divisible by m_M's prime factors
  m_C = 1;        // m_C and m_M should be co-prime

  m_Z = floor(((double)rand() / RAND_MAX) * (double)m_M);

  // Initialisation des valeurs aléatoires:
  m_a = custom_rand();
  m_b = custom_rand();

  // Itérateur:
  m_x = 0;

  // Paramètres du bruit:
  m_amplitude = 1;
  m_wavelength = 10;
  m_frequency = 1.0f / m_wavelength;
}

// Modification des paramètres du générateur de valeurs aléatoires:
void Perlin::setSeed(unsigned long long seed) { m_Z = seed; }
void Perlin::setAmplitude(float amplitude) { m_amplitude = amplitude; }

// Pour récupérer une valeur du générateur:
float Perlin::getNext() {
  float y;
  if (m_x % m_wavelength == 0) {
    m_a = m_b;
    m_b += custom_rand() * 0.2;
    y = m_a;
  } else {
    y = interpolate(m_a, m_b,
                    (double)(m_x % m_wavelength) / (double)m_wavelength);
  }

  m_x++;

  return y * m_amplitude;
}

// Pour récupérer plusieurs valeurs aléatoires:
float* Perlin::getNext(int n) {
  float* values = new float[n];

  // Récupère n valeurs aléatoires:
  for (int i = 0; i < n; i++) {
    values[i] = getNext();
  }

  return values;
}

/**
 * Méthodes:
 */

// Interpolation cosinus entre les valeurs pa et pb, à la position px:
float Perlin::interpolate(float pa, float pb, float px) {
  double ft = px * M_PI;
  double f = (1.0f - cos(ft)) * 0.5f;
  return pa * (1.0f - f) + pb * f;
}

// Génération de valeurs aléatoires:
double Perlin::custom_rand() {
  m_Z = (m_A * m_Z + m_C) % m_M;
  return (double)m_Z / (double)m_M - 0.5f;
}
