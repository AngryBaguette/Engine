#version 150 core

in vec3 vColor;


void main()
{
  //gl_FragData[0] = vec4(vColor*2,1);
  gl_FragData[0] = vec4(1,0,0,1);
}