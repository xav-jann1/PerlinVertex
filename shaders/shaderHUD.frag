#version 120

//Un Fragment Shader minimaliste
void main (void)
{
    float r=abs(gl_FragCoord.x/400.0);

    //couleur du fragment
    gl_FragColor = vec4(1-r,r,0.0,1.0);
}
