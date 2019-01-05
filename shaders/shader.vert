#version 120

uniform vec4 cam_translation;
uniform mat4 cam_rotation;
uniform vec4 cam_rotation_center;

uniform vec4 translation;
uniform mat4 rotation;
uniform vec4 rotation_center;

uniform mat4 projection;

varying vec3 coordonnee_3d;
varying vec3 coordonnee_3d_locale;
varying vec3 normale;
varying vec4 colors;

// Un Vertex Shader minimaliste:
void main (void) {

  // Coordonnées 3D du sommet:
  coordonnee_3d = gl_Vertex.xyz;

  // Coordonn"es du sommet:
  vec4 p = rotation * (gl_Vertex - rotation_center) + rotation_center + translation;
  coordonnee_3d_locale = p.xyz;
  p = cam_rotation * (p + cam_rotation_center) - cam_rotation_center + cam_translation;

  // Projection du sommet:
  p = projection * p;

  // Gestion des normales:
  vec4 n=rotation*vec4(gl_Normal,1.0);
  normale=n.xyz;

  // Gestion des couleurs:
  colors = gl_Color;

  // Position dans l'espace écran:
  gl_Position = p;
}
