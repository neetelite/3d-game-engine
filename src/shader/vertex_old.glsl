#version 330 core

layout (location = 0) in vec3 pos_coords;
layout (location = 1) in vec2 tex_coords;

uniform mat4x4 matrix_model;
uniform mat4x4 matrix_cam;

out vec2 pass_tex;

void main()
{
	gl_Position = matrix_cam * matrix_model * vec4(pos_coords, 1.0);
	//gl_Position = vec4(pos_coords, 1.0);
	pass_tex = tex_coords;
};
