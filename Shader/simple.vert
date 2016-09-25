#version 330 core
layout(location = 0) in vec4 Position;
layout(std140) uniform Transform
{
mat4 ModelViewMatrix;
mat4 ProjectionMatrix;
};
void main()
{
gl_Position = Position;
}
