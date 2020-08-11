/* SOURCE(lungu): Animation by ThinMatirx
https://www.youtube.com/playlist?list=PLRIWtICgwaX2tKWCxdeB7Wv_rTET9JtWW */

/* IMPLEMENT(lungu): Time independent joint animation */

struct JointTransform
{
	/* Relative to parent joint, not model */
	v3 pos;
	Quat rot;
};

struct Keyframe
{
	f32 time;

	struct JointTransform *joint_transforms;
	/* NOTE(lungu): joint_count is in model */
};

enum AnimationStatus
{
	ANIMATION_STATUS_NULL,
	ANIMATION_STATUS_PAUSED,
	ANIMATION_STATUS_PLAYING,
};

struct Animation
{
	f32 length; /* In seconds */

	enum AnimationStatus status;

	u32 keyframe_count;
	struct Keyframe *keyframes;
	struct Keyframe *keyframe_current;
};

struct Animator
{
	f32 start;
	f32 time;
	f32 speed;

	struct Model *model;
	struct Animation *animation;

	bool loop;
};

/* TEMPORARY */
#define TODO_JOINT_COUNT 16

mat4
mat4_from_joint_transform(struct JointTransform *joint_transform)
{
	mat4 result = {0};
	result = mat4_trans_v3(MAT4_ID, joint_transform->pos);

	mat4 mat_rot = mat4_from_quat(joint_transform->rot);
	result = mat4_m(mat_rot, result);

	return(result);
}

struct JointTransform
joint_transform_lerp(struct JointTransform *prev, struct JointTransform *next, f32 t)
{
	struct JointTransform result = {0};

	result.pos = v3_lerp(prev->pos, next->pos, t);
	result.rot = quat_lerp(prev->rot, next->rot, t);

	return(result);
}

struct Keyframe *
keyframe_lerp(struct Keyframe *prev, struct Keyframe *next, f32 time, struct Keyframe *keyframe)
{
	struct Keyframe *result = keyframe;
	result->time = time;
	result->joint_transforms = mem_alloc(TODO_JOINT_COUNT, sizeof(struct JointTransform), false);

	f32 duration = next->time - prev->time;
	f32 time_passed = time - prev->time;
	f32 t = time_passed / duration;

	for(i32 i = 0; i < TODO_JOINT_COUNT; ++i)
	{
		struct JointTransform *joint_prev = prev->joint_transforms + i;
		struct JointTransform *joint_next = next->joint_transforms + i;
		result->joint_transforms[i] = joint_transform_lerp(joint_prev, joint_next, t);
	}

	return(result);
}

void
joint_apply_transform(struct Keyframe *keyframe, struct Joint *joint, mat4 parent_model_transform)
{
	struct JointTransform *joint_transform = keyframe->joint_transforms + joint->id;
	mat4 transform_local = mat4_from_joint_transform(joint_transform);

	mat4 transform = mat4_m(transform_local, parent_model_transform);

	for(i32 i = 0; i < joint->child_count; ++i)
	{
		struct Joint *child = *(joint->children + i);
		joint_apply_transform(keyframe, child, transform);
	}

	joint->transform = mat4_m(joint->transform_inverse, transform);
}
