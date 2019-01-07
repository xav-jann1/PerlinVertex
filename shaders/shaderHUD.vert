#version 120

uniform vec4 translation;

//Un Vertex Shader minimaliste
void main (void)
{
    //Coordonnees du sommet
    vec4 p=gl_Vertex+translation;
    gl_Position = p;
}
