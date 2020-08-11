#version 330 core

in vec2 pass_tex;
in vec3 pass_rgb;

out vec4 fragment_color;

uniform sampler2D texture;

void main()
{
	fragment_color = texture2D(texture, pass_tex);
};
