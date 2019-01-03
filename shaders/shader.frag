#version 120

varying vec4 colors;
varying vec3 coordonnee_3d;


void main (void)
{
    float x = abs(coordonnee_3d.x);
    float y = abs(coordonnee_3d.y);
    float z = abs(coordonnee_3d.z);

    float r = mod(z, 10) / 10.0;

    float g = 0.5;
    //if (abs(cos(0.001 * z * z)) > 0.95) g=1.0;

    gl_FragColor = vec4(r, g, 0.0, 0.0);
}



/*** Archives ***/

//Un Fragment Shader minimaliste
/*void main (void)
{
    vec4 color = colors;

    gl_FragColor = color;

    //gl_FragColor = vec4(1,1,1,1);

}*/
