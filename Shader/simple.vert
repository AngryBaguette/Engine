#version 150 core
#extension GL_ARB_explicit_attrib_location : require
//#extension GL_ARB_separate_shader_objects

layout(location = 0) in vec4 Position;
layout(location = 1) in vec3 Color;

out vec3 vColor;

void main()
{
//gl_Position = ProjectionMatrix * test * Position;
	//vColor = Color;
	gl_Position = Position;
}
