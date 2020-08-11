#version 330 core

in vec2 texture_coords;

out vec4 fragment_color;

uniform sampler2D texture;

void main()
{
	fragment_color = texture2D(texture, texture_coords);
};
