// Header:
#include "Player.hpp"

// Librairie:
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>

// Outils:
#include "../tools/glutils.hpp"
#include "../tools/mat4.hpp"

// Modèles:
#include "../modele/vertex_opengl.hpp"
#include "../modele/mesh.hpp"
#include "../modele/image.hpp"

// Constructeur:
Player::Player(Path* path) : PathAgent(path) {
  // Paramètre du joueur:
  m_life = 3;
  m_angle = 0.0f;
  m_desired_angle = 0.0f;
  setSpeed(1);
  setAngleSpeed(0);

  // Rendu:
  m_vbo = 0;
  m_vboi = 0;
  m_texture_id = 0;
  m_nbr_triangles = 0;

  m_model_angle_offset = vec3(1.5f * M_PI, 0, 0);
}

/**
 * Getters:
 */

// Shaders utilisés par render():
vec3 Player::getRotationCenter() { return m_rotation_center; }

// Matrice de rotation pour correctement orienter le joueur dans le tube:
mat4 Player::getRotationMatrix() {
  
  // Rotation en direction du centre du tube:
  vec3 position = getPosition();
  vec3 path_position = getPathPosition();
  vec3 orientation = position - path_position;
  mat3 rotation3 = mat3_rotation_from_vec3_z(orientation * -1);
  mat4 rotation4 = mat4_from_mat3(rotation3);

  // Offset de rotation du modèle:
  mat4 rotation_x = matrice_rotation(m_model_angle_offset.x, 1.0f, 0.0f, 0.0f);
  mat4 rotation_y = matrice_rotation(m_model_angle_offset.y, 0.0f, 1.0f, 0.0f);
  mat4 rotation_z = matrice_rotation(m_model_angle_offset.z, 0.0f, 0.0f, 1.0f);
  mat4 model_offset = rotation_x * rotation_y * rotation_z;

  // Animation de rotation du vaisseau:
  mat4 animation = matrice_rotation(m_angle, 0.0f, 1.0f, 0.0f);

  // Ajout des rotations:
  return rotation4 * model_offset * animation;
}

/**
 * Setters:
 */

// Angle désiré pour tourner le joueur:
void Player::setDesiredAngle(float angle) { m_desired_angle = angle; };

// Shaders utilisés par render():
void Player::setRotationCenter(vec3 p) { m_rotation_center = p; }

// Programme de rendu:
void Player::setRenderProgram(GLuint program) { m_render_program = program; }

// Modifie l'angle d'orientation du joueur (pour animation):
void Player::updateModelAngle(float dangle) { m_angle += dangle; }


/**
 * Mise à jour de la position du joueur:
 * @param int path_points_deleted  Nombre de points supprimé du chemin depuis la
 *                                 dernière mise à jour de l'agent
 * @return bool canDelete  Averti si le joueur peut être supprimé
 *                         (car en dehors du chemin)
 */
bool Player::update(int path_points_deleted) {

  // Mise à jour de la position du cube sur le chemin:
  bool canDelete = PathAgent::update(path_points_deleted);

  // Animation de la rotation du joueur sur lui-même:
  smoothRotation();

  return canDelete;
}


/**
 * Méthode:
 */

// Rotation du joueur sur lui-même: (todo: ajouter deltaTime)
void Player::smoothRotation() {
  float max_speed = 0.09;

  if (fabs(m_angle - m_desired_angle) > 0.001f) {
    if (m_angle < m_desired_angle)
      updateModelAngle(std::min(max_speed, m_desired_angle - m_angle));
    else if (m_angle > m_desired_angle)
      updateModelAngle(-std::min(max_speed, m_angle - m_desired_angle));
  }
}

// Le joueur a été touché: perd 1 point de vie
bool Player::hit() {
  m_life--;

  if(m_life < 1) return false;

  return true;
}

/**
 * Rendu:
 */

// Initialisation du modèle du joueur:
void Player::init_model() {
  // Chargement d'un maillage a partir d'un fichier:
  mesh m = load_obj_file("data/test.obj");

  // Affecte une transformation sur les sommets du maillage:
  float s = 0.12f;
  mat4 transform = mat4(   s, 0.0f, 0.0f,  0.0f,
                        0.0f,    s, 0.0f, -0.9f,
                        0.0f, 0.0f,    s, -2.0f,
                        0.0f, 0.0f, 0.0f,  1.0f);
  apply_deformation(&m, transform);

  // Centre la rotation du modèle autour de son centre de gravité approximatif:
  setRotationCenter(vec3(0.0f, -0.5f, -2.0f));

  // Calcul automatique des normales du maillage:
  update_normals(&m);

  // Les sommets sont affectés à une couleur blanche:
  fill_color(&m, vec3(1.0f, 1.0f, 1.0f));

  // Attribution d'un buffer de données (1 indique la création d'un buffer):
  glGenBuffers(1, &m_vbo);  PRINT_OPENGL_ERROR();

  // Affectation du buffer courant
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);  PRINT_OPENGL_ERROR();
  // Copie des donnees des sommets sur la carte graphique:
  glBufferData(GL_ARRAY_BUFFER, m.vertex.size() * sizeof(vertex_opengl), &m.vertex[0], GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();

  // Attribution d'un autre buffer de données:
  glGenBuffers(1, &m_vboi);  PRINT_OPENGL_ERROR();
  // Affectation du buffer courant (buffer d'indice):
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboi);  PRINT_OPENGL_ERROR();
  // Copie des indices sur la carte graphique:
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.connectivity.size() * sizeof(triangle_index), &m.connectivity[0], GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();

  // Nombre de triangles de l'objet:
  m_nbr_triangles = m.connectivity.size();

  // Chargement de la texture:
  load_texture("data/metal.tga", &m_texture_id);
}

// Chargement d'une texture:
void Player::load_texture(const char* filename, GLuint* texture_id) {
  // Chargement d'une texture (seul les textures tga sont supportes)
  Image* image = image_load_tga(filename);

  // Vérification que l'image est bien chargée:
  if (image) {
    // Création d'un identifiant pour la texture:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  PRINT_OPENGL_ERROR();
    glGenTextures(1, texture_id);  PRINT_OPENGL_ERROR();

    // Sélection de la texture courante à partir de son identifiant:
    glBindTexture(GL_TEXTURE_2D, *texture_id);  PRINT_OPENGL_ERROR();

    // Paramètres de la texture:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  PRINT_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  PRINT_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  PRINT_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  PRINT_OPENGL_ERROR();

    // Envoie de l'image en memoire video:
    if (image->type == IMAGE_TYPE_RGB) {  // image RGB
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);  PRINT_OPENGL_ERROR();
    } else if (image->type == IMAGE_TYPE_RGBA) {  // image RGBA
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);  PRINT_OPENGL_ERROR();
    } else {
      std::cout << "Image type not handled" << std::endl;
    }

    delete image;
  } else {
    std::cerr
        << "Erreur chargement de l'image, etes-vous dans le bon repertoire?"
        << std::endl;
    abort();
  }

  glUniform1i(get_uni_loc(m_render_program, "texture"), 0);  PRINT_OPENGL_ERROR();
}

// Affichage du joueur:
void Player::render() {
  // Envoie des paramètres uniformes:
  {
    glUniformMatrix4fv(get_uni_loc(m_render_program, "rotation"), 1, false, pointeur(getRotationMatrix()));  PRINT_OPENGL_ERROR();

    vec3 c = getRotationCenter();
    glUniform4f(get_uni_loc(m_render_program, "rotation_center"), c.x, c.y, c.z, 0.0f);  PRINT_OPENGL_ERROR();

    vec3 t = getPosition() - getRotationCenter();
    glUniform4f(get_uni_loc(m_render_program, "translation"), t.x, t.y, t.z, 0.0f);  PRINT_OPENGL_ERROR();
  }

  // Placement des VBO:
  {
    // Sélection du VBO courant:
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);  PRINT_OPENGL_ERROR();

    // Mise en place des différents pointeurs:
    glEnableClientState(GL_VERTEX_ARRAY);  PRINT_OPENGL_ERROR();
    glVertexPointer(3, GL_FLOAT, sizeof(vertex_opengl), 0);  PRINT_OPENGL_ERROR();

    glEnableClientState(GL_NORMAL_ARRAY);  PRINT_OPENGL_ERROR();  PRINT_OPENGL_ERROR();
    glNormalPointer(GL_FLOAT, sizeof(vertex_opengl), buffer_offset(sizeof(vec3)));  PRINT_OPENGL_ERROR();

    glEnableClientState(GL_COLOR_ARRAY);  PRINT_OPENGL_ERROR();  PRINT_OPENGL_ERROR();
    glColorPointer(3, GL_FLOAT, sizeof(vertex_opengl), buffer_offset(2 * sizeof(vec3)));  PRINT_OPENGL_ERROR();

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);  PRINT_OPENGL_ERROR();  PRINT_OPENGL_ERROR();
    glTexCoordPointer(2, GL_FLOAT, sizeof(vertex_opengl), buffer_offset(3 * sizeof(vec3)));  PRINT_OPENGL_ERROR();
  }

  // Affichage:
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboi);  PRINT_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, m_texture_id);  PRINT_OPENGL_ERROR();
    glDrawElements(GL_TRIANGLES, 3 * m_nbr_triangles, GL_UNSIGNED_INT, 0);  PRINT_OPENGL_ERROR();
  }
}