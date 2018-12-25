#version 120

varying vec4 colors;

//Un Fragment Shader minimaliste
void main (void)
{
    vec4 color = colors;

    gl_FragColor = color;

}
