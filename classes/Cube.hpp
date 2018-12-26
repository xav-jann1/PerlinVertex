#ifndef DEF_CUBE
#define DEF_CUBE

#include <iostream>
#include <string>

#include "../tools/vec3.hpp"
#include "../tools/mat4.hpp"
#include "../tools/triangle_index.hpp"

class Cube
{
    public:

    //Constructeur
    //Cube(vec3 coord, mat4 rot, float dim);
    Cube();
    

    //Translation
    void translate(vec3 t);
    void translateX(float x);
    void translateY(float y);
    void translateZ(float z);

    //Rotation
    void rotateX(float theta);
    void rotateY(float theta);
    void rotateZ(float theta);

    //Getters
    mat4 getMat4();
    vec3 getCoord();
    float getX();
    float getY();
    float getZ();
    vec3* getGeometrie();
    triangle_index* getIndex();

    // Mise à jour de la position du cube:
    void update();

    // Chargement d'un cube sur la carte graphique:
    static void loadCube(float);

    // Affichage du cube:
    void render(GLuint shader_program_id);


    private:
    //Methodes privees
    void initPoints();
    void initColors();

    //Dimension
    float m_dim;
    float m_speed;

    //Transformation
    vec3 m_coord;
    vec3 m_rot_center;
    float m_angleX, m_angleY, m_angleZ;

    //Donnees GPU
    vec3 m_points[8];
    //vec3[8] normales;
    vec3 m_colors[8];
    triangle_index m_index[8];
};

#endif
