struct Texture
{
	u32 id;
	char *name;
	GLuint handle; /* OpenGL */

	Image image;
};

enum ModelFlag
{
	MODEL_FLAG_SKEL = 0x1,
	MODEL_FLAG_ANIM = 0x2,
};

struct Effect
{
	u32 id;
	char *name;

	u32 surface_id; /* From texture */
	u32 sampler_id; /* From surface */
	u32 difuse_id; /* From sampler */

	v3 color_emision;
	v3 color_ambient;
	v3 color_specular;
	f32 shine;
};

struct Material
{
	u32 id;
	char *name;

	u32 effect_id;
};

struct Model
{
	u32 id;
	char *name;

	mat4 transform;

	char *path;
	u32 flags;

	u32 effect_count;
	struct Effect *effects;

	u32 material_count;
	struct Material *materials;

	u32 texture_count;
	struct Texture *textures;

	u32 mesh_count;
	struct Mesh *meshes;

	u32 skeleton_count;
	struct Skeleton *skeletons;

	u32 animation_count;
	struct Animation *animations;

	struct Animator animator;

};

/* Loaders */
#include "loader/dae.c"
#include "loader/obj.c"


/* FUNCTIONS */
#define BUFFER_SIZE 256

void
model_vertex_from_face(struct Model *model)
{
	/* SPEED SPACE(lungu): Read coordinates directly into vertex buffers?  */
	/* LEARN(lungu): More than one skeleton? */
	struct Skeleton *skel = model->skeletons;

	for(i32 mesh_index = 0; mesh_index < model->mesh_count; ++mesh_index)
	{
		struct Mesh *mesh = model->meshes + mesh_index;
		mesh->vertex_list_count = mesh->face_list_count;
		mesh->vertex_lists = mem_alloc(mesh->vertex_list_count, sizeof(struct VertexList), true);

		for(i32 face_list_index = 0; face_list_index < mesh->face_list_count; ++face_list_index)
		{
			struct FaceList *face_list = mesh->face_lists + face_list_index;
			struct VertexList *vertex_list = mesh->vertex_lists + face_list_index;

			vertex_list->material_id = face_list->material_id;

			vertex_list->vertex_count = face_list->face_count * 3;
			vertex_list->vertexes = mem_alloc(vertex_list->vertex_count, sizeof(struct Vertex), true);

			for(u32 face_index = 0; face_index < face_list->face_count; ++face_index)
			{
				struct Face *face = face_list->faces + face_index;

				for(u32 j = 0; j < 3; ++j)
				{
					struct Vertex *vertex = vertex_list->vertexes + (face_index * 3) + j;

					u32 pos_index = face->pos_indices[j];
					u32 tex_index = face->tex_indices[j];

					vertex->pos = mesh->pos_coords[pos_index];
					vertex->tex = mesh->tex_coords[tex_index];

					if(!(model->flags & MODEL_FLAG_SKEL)) continue;
					struct WeightVertex *weight_vertex =
						skel->weight_vertexes + pos_index;

					for(i32 l = 0; l < WEIGHTS_MAX; ++l)
					{
						vertex->joint_indices[l] = weight_vertex->joint_indices[l];
						vertex->weights[l] = weight_vertex->weights[l];
					}
				}
			}
		}
	}
}

i32
model_dae_load(struct Model *model, char *path)
{
	model->path = CSTR(path);

	struct XML_Tree *tree = xml_tree_alloc(path);
	struct XML_Node *root = tree->root;

	ASSERT(root->child_first);

	dae_read(model, root);
	model_vertex_from_face(model);

	/* FIXME LEAK(lungu): Sometimes it leaks, on non-standard files */
	xml_tree_free(tree);
	return(1);
}

#if 0
internal i32
model_obj_load(struct Model *model, FILE *file)
{
	obj_read(model, file);

	model_vertex_from_face(model);

	return(1);
}
#endif

i32
model_load(struct Model *model, char *model_path)
{
	//struct Mesh *mesh = &model->mesh;

	FILE *fp;
	fp = fopen(model_path, "rb");

	if(!fp)
	{
		ERROR("Couln't open file: %s\n", model_path);
		return(0);
	}

	char *ext = cstr_path_extension(model_path);
	#if 0
	if(cstr_eql(ext, "obj")) /* Wavefront OBJ */
	{
		model_obj_load(model, fp);
	}
	#endif
	if(cstr_eql(ext, "dae")) /* Collada */
	{
		model_dae_load(model, model_path);
	}
	else
	{
		ERROR("%s", "Unsuported model extension!\n");
		return(0);
	}

	fclose(fp);
	return(1);
}

void
model_anim_update(struct Model *model, f32 dt)
{
	if(!model->flags & MODEL_FLAG_ANIM) return;

	/* LEARN(lungu): Multiple skeletons? */
	struct Skeleton *skel = &model->skeletons[0];

	struct Animator *animator = &model->animator;
	if(!animator->animation) return;

	struct Animation *animation = animator->animation;

	/* Increase frame */
	//static count = 0;
	//printf("C: %d, DT: %f\n", count, dt);
	//++count;

	if(animator->loop && animator->time >= animation->length)
	{
		/* Loop */
		animator->time = f32_mod(animator->time, animation->length);
		//animator->time = animator->start;
	}
	//printf("LENGTH: %f, TIMER: %f\n", animation->length, animator->time);
	//if(animator->time > 0.833333) shit();

	/* Calculate current pose */
	/* Get previous and next frame */
	bool interpolate = false;
	struct Keyframe *prev = NULL;
	struct Keyframe *next = NULL;

	/* NOTE(lungu): -1 because we compare to the next keyframe */
	for(i32 i = 0; i < animation->keyframe_count - 1; ++i)
	{
		struct Keyframe *keyframe = animation->keyframes + i;
		if(animator->time == keyframe->time)
		{
			prev = keyframe;
			next = keyframe;
			interpolate = false;
			break;
		}
		else if(0 < animator->time  && animator->time < keyframe->time)
		{
			prev = keyframe;
			next = keyframe;
			interpolate = true;
			break;
		}
		else if(animator->time > keyframe->time && animator->time < (keyframe+1)->time)
		{
			prev = keyframe;
			next = keyframe + 1;

			interpolate = true;
			break;
		}
		else if(animator->time > animation->keyframes[animation->keyframe_count - 1].time)
		{
			return;
		}
	}

	struct Keyframe *keyframe = NULL;
	if(interpolate)
	{
		keyframe = keyframe_lerp(prev, next, animator->time, animation->keyframe_current);
	}
	else
	{
		keyframe = next;
	}

	/* Apply pose to joints */
	struct Joint *root_joint = skel->joints;
	joint_apply_transform(keyframe, root_joint, MAT4_ID);

	animator->time += dt * animator->speed;
}

void
model_free(struct Model *model)
{
	for(i32 i = 0; i < model->mesh_count; ++i) mesh_free(model->meshes + i);
	mem_free(model->meshes);

	for(i32 i = 0; i < model->skeleton_count; ++i) skeleton_free(model->skeletons + i);
	mem_free(model->skeletons);

	/* INCOMPLETE(lungu): Animations */

	/* INCOMPLETE LEARN(lungu): How do we want to allocate and free these? */
	//img_unload(&model->texture.image);
}
