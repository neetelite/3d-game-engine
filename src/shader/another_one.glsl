#version 330 core

const int JOINT_MAX = 16;
const int WEIGHT_MAX = 4;

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_tex;

layout (location = 2) in int in_joint_indices[WEIGHT_MAX];
layout (location = 3) in float in_joint_weights[WEIGHT_MAX];

uniform mat4x4 matrix_joints[JOINT_MAX];
uniform mat4x4 matrix_model;
uniform mat4x4 matrix_cam;

out vec2 pass_tex;

void main()
{

	mat4 transform = matrix_joints[in_joint_indices[0]] * in_joint_weights[0];
	transform += matrix_joints[in_joint_indices[1]] * in_joint_weights[1];
	transform += matrix_joints[in_joint_indices[2]] * in_joint_weights[2];
	transform += matrix_joints[in_joint_indices[3]] * in_joint_weights[3];

	vec4 position = transform * vec4(in_pos, 1.0f);
	gl_Position = matrix_cam * matrix_model * position;

	pass_tex = in_tex;
}
