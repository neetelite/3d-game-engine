struct Joint
{
	u32 id;
#define JOINT_NAME_MAX 20
	char name[JOINT_NAME_MAX];

	/* position and rotation, model transform, from original to current  */
	mat4 transform; /* loaded into the vertex shader as uniform array */
	mat4 transform_inverse; /* Inverse Bind Transform - inverse of original model transform */

	u32 child_count;
	struct Joint **children;
};

struct WeightVertex
{
	u32 joint_count;
	u32 joint_indices[WEIGHTS_MAX];
	u32 weight_indices[WEIGHTS_MAX];
	f32 weights[WEIGHTS_MAX];
};

struct Skeleton
{
	u32 joint_count;
	struct Joint *joints; /* hierarchy */

	u64 weight_count;
	f32 *weights;

	/* Count same as pos_coord_count */
	u32 weight_vertex_count;
	struct WeightVertex *weight_vertexes;

	mat4 *joint_matrices;
};

struct Joint *
joint_get_by_name(struct Skeleton *skel, char *name)
{
	struct Joint *result = NULL;

	for(i32 i = 0; i < skel->joint_count; ++i)
	{
		struct Joint *joint = skel->joints + i;
		if(cstr_eql(joint->name, name))
		{
			result = joint;
			break;
		}
	}

	return(result);
}

void
skeleton_joint_matrix_array(struct Skeleton *skel)
{
	for(i32 i = 0; i < skel->joint_count; ++i)
	{
		struct Joint *joint = skel->joints + i;
		mat4 *matrix = skel->joint_matrices + i;

		*matrix = mat4_transpose(joint->transform);
	}
}

void
joint_print(struct Joint *joint)
{
	printf("ID: %d\n", joint->id);
	printf("Name: %s\n\n", joint->name);
	printf("Transform:\n");
	mat4_print(joint->transform);
	printf("Transform Inverse:\n");
	mat4_print(joint->transform_inverse);
	printf("Child Count: %d\n", joint->child_count);
	for(i32 i = 0; i < joint->child_count; ++i)
	{
		printf("\t%s\n", joint->children[i]->name);
	}
	printf("\n\n\n");
}

void
joints_print(struct Skeleton *skel)
{
	for(i32 i = 0; i < skel->joint_count; ++i)
	{
		struct Joint *joint = skel->joints + i;
		joint_print(joint);
	}
}

void *
skeleton_alloc(u32 count)
{
	void *result = NULL;
	result = mem_alloc(count, sizeof(struct Skeleton), true);
	return(result);
}

void
skeleton_free(struct Skeleton *skeleton)
{
	if(!skeleton) return;

	mem_free(skeleton->joints);
	mem_free(skeleton->joint_matrices);
	for(i32 i = 0; i < skeleton->joint_count; ++i)
	{
		struct Joint *joint = skeleton->joints + i;
		mem_free(joint->children);
	}

	mem_free(skeleton->weights);
	mem_free(skeleton->weight_vertexes);
}
