#version 120

uniform vec4 cam_translation;
uniform mat4 cam_rotation;
uniform vec4 cam_rotation_center;

uniform vec4 translation;
uniform mat4 rotation;
uniform vec4 rotation_center;

uniform mat4 projection;

varying vec4 colors;


//Un Vertex Shader minimaliste
void main (void)
{
    //Coordonnees du sommet
    vec4 p = rotation * (gl_Vertex - rotation_center) + rotation_center + translation;
    p = cam_rotation * (p + cam_rotation_center) - cam_rotation_center + cam_translation;

    //Projection du sommet
    p = projection * p;

    //Gestion des couleurs
    colors = gl_Color;

    //position dans l'espace ecran
    gl_Position = p;
}
