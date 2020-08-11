#version 330 core

const int JOINT_MAX = 16;
const int WEIGHT_MAX = 4;

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_tex;

layout (location = 2) in int in_joint_indices[WEIGHT_MAX];
layout (location = 3) in float in_joint_weights[WEIGHT_MAX];

uniform mat4 matrix_joints[JOINT_MAX];
uniform mat4 matrix_model;
uniform mat4 matrix_cam;

out vec2 pass_tex;

void main()
{
	vec4 pos = vec4(in_pos, 1.0f);
	vec4 position = vec4(0.0f);

	for(int i = 0; i < WEIGHT_MAX; ++i)
	{
		mat4 transform = matrix_joints[in_joint_indices[i]];
		vec4 pose_position = transform * pos;
		position += pose_position * in_joint_weights[i];
	}

	gl_Position = matrix_cam * matrix_model * position;

	pass_tex = in_tex;
}
