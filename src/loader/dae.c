/* SPEED(lungu): Scanf takes a long time */
global u32 global_name_len_skip;

void
texture_gen(struct Texture *texture, char *path)
{
	img_load(&texture->image, path);

	glGenTextures(1, &texture->handle);
	glBindTexture(GL_TEXTURE_2D, texture->handle);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->image.width, texture->image.height,
			0, GL_BGRA, GL_UNSIGNED_BYTE, texture->image.data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	/* Unbind Texture */
	glBindTexture(GL_TEXTURE_2D, 0);
}

void
dae_library_images_read(struct Model *model, struct XML_Node *library)
{
	model->texture_count = xml_node_child_count_by_name(library, "image");
	model->textures = mem_alloc(model->texture_count, sizeof(struct Texture), true);

	struct XML_Node *child;
	bool child_set = false;
	u32 image_id = 0;
	for(i32 i = 0; i < library->child_count; ++i)
	{
		if(child_set) child = child->next;
		else {child = library->child_first; child_set = true;}

		if(cstr_eql(child->name, "image"));
		{
			struct Texture *texture = model->textures + image_id;
			texture->id = image_id;
			texture->name = CSTR(xml_attrib_value(child, "id"));

			struct XML_Node *node_path = child->child_first;
			char *texture_path = node_path->text;

			char path[PATH_MAX];
			cstr_path_parent(path, model->path);
			sprintf(path, "%s%s", path, texture_path);
			texture_gen(texture, path);

			++image_id;
		}
	}


	//char path[PATH_MAX];
	//sprintf(path_model, "%s%s/%s/%s%s", os_state.path_data, "model", model_name, model_name, ".dae");
}

void
dae_library_effects_read(struct Model *model, struct XML_Node *library)
{
	model->effect_count = xml_node_child_count_by_name(library, "effect");
	model->effects = mem_alloc(model->effect_count, sizeof(struct Effect), true);

	struct XML_Node *child_library;
	bool child_set = false;
	u32 effect_id = 0;
	for(i32 i = 0; i < library->child_count; ++i)
	{
		if(child_set) child_library = child_library->next;
		else {child_library = library->child_first; child_set = true;}

		if(!cstr_eql(child_library->name, "effect")) continue;

		struct Effect *effect = model->effects + effect_id;
		effect->id = effect_id;
		effect->name = CSTR(xml_attrib_value(child_library, "id"));

		struct XML_Node *profile_common = xml_node_find_by_name(child_library, "profile_COMMON");

		struct XML_Node *child_effect;
		bool child_set_2 = false;
		for(i32 j = 0; j < profile_common->child_count; ++j)
		{
			if(child_set_2) child_effect = child_effect->next;
			else {child_effect = profile_common->child_first; child_set_2 = true;}

			if(!cstr_eql(child_effect->name, "newparam")) continue;
			#if 0
			else if(cstr_eql(child_effect->name, "technique"))
			{
				/* INCOMPLETE(lungu): Complete this for when we need materials */
			}
			#endif

			/* New Param */
			struct XML_Node *new_param = child_effect->child_first;
			if(cstr_eql(new_param->name, "surface"))
			{
				char *texture_name = new_param->child_first->text;
				for(i32 texture_index = 0;
				    texture_index < model->texture_count;
				    ++texture_index)
				{
					struct Texture *texture = model->textures + texture_index;
					if(cstr_eql(texture_name, texture->name))
					{
						effect->surface_id = texture->id;
						break;
					}
				}
			}
			/* INCOMPLETE(lungu): Finish this sometime */
			#if 0
			if(cstr_eql(new_param->name, "sampler2D"))
			{
				char *texture_name = new_param->child_first->text;
				for(i32 texture_index = 0;
				    texture_index < model->texture_count;
				    ++texture_index)
				{
					struct Texture *texture = model->textures + texture_index;
					if(cstr_eql(texture_name, texture->name))
					{
						effect->surface_id = texture->id;
						break;
					}
				}
			}
			#endif
		}

		++effect_id;
	}
}

void
dae_library_materials_read(struct Model *model, struct XML_Node *library)
{
	model->material_count = xml_node_child_count_by_name(library, "material");
	model->materials = mem_alloc(model->material_count, sizeof(struct Material), true);

	struct XML_Node *node_material;
	bool child_set = false;
	u32 material_index = 0;
	for(i32 i = 0; i < library->child_count; ++i)
	{
		if(child_set) node_material = node_material->next;
		else {node_material = library->child_first; child_set = true;}

		if(!cstr_eql(node_material->name, "material")) continue;
		struct Material *material = model->materials + material_index;

		material->id = material_index;
		material->name = CSTR(xml_attrib_value(node_material, "id"));

		struct XML_Node *node_instance_effect = node_material->child_first;
		char *effect_name = xml_attrib_value(node_instance_effect, "url");
		for(i32 effect_index = 0;
		    effect_index < model->effect_count;
		    ++effect_index)
		{
			struct Effect *effect = model->effects + effect_index;
			/* NOTE(lungu): +1 because effect_name starts with a '#' */
			if(cstr_eql(effect->name, effect_name + 1))
			{
				material->effect_id = effect->id;
				break;
			}
		}

		++material_index;
	}
}


struct TriangleInput
{
	char *semantic;
	char *source;
	u32 offset;
	u32 set;

	u64 skip;
	bool okay;
};

void
dae_library_geometries_read(struct Model *model, struct XML_Node *library)
{
	/* Count number of meshes */
	model->mesh_count = 0;
	struct XML_Node *child;
	bool child_set = false;
	for(i32 i = 0; i < library->child_count; ++i)
	{
		if(child_set) child = child->next;
		else {child = library->child_first; child_set = true;}

		if(cstr_eql(child->name, "geometry")) ++model->mesh_count;
	}

	/* Allocate space for meshes */
	model->meshes = mem_alloc(model->mesh_count, sizeof(struct Mesh), true);

	/* Get the names and position, texture, and normal coordinates */
	u32 mesh_index = 0;
	child_set = false;
	for(i32 i = 0; i < library->child_count; ++i)
	{
		if(!child_set) {child = library->child_first; child_set = true;}
		else child = child->next;

		if(!cstr_eql(child->name, "geometry")) continue;

		struct XML_Node *geometry = child;
		struct XML_Node *node_mesh = xml_node_child_find_by_name(geometry, "mesh");

		struct Mesh *mesh = model->meshes + mesh_index;

		/* ID and Name */
		mesh->id = mesh_index;
		mesh->name = CSTR(xml_attrib_value(geometry, "name"));

		/* Preparing for finding the right arrays */
		char *geometry_id = xml_attrib_value(geometry, "id");
		char source_array_id[256];

		/* Variables */
		char *at = NULL;
		u32 stride = 0;

		/* Position Coordinates */
		cstr_cat(source_array_id, geometry_id, "-positions-array");
		struct XML_Node *positions = xml_node_find_by_attrib(geometry, "id", source_array_id);

		/* TEMPORARY INCOMPLETE(lungu): Read <accessor> and and replace the constant stride */
		stride = 3;
		char *count_string = xml_attrib_value(positions, "count");
		mesh->pos_coord_count = u32_from_cstr(count_string) / stride;
		mesh->pos_coords = mem_alloc(mesh->pos_coord_count, sizeof(v3), false);

		at = positions->text;
		for(int i = 0; i < mesh->pos_coord_count; ++i)
		{
			v3 *pos_coords = mesh->pos_coords + i;

			for(i32 j = 0; j < stride; ++j)
			{
				pos_coords->i[j] = f32_from_cstr(at);
				at = char_whitespace_find(at) + 1;
			}
		}

		/* TODO INCOMPLETE(lungu): Normals */

		/* Texture Coordinates */
		cstr_cat(source_array_id, geometry_id, "-map-0-array");
		struct XML_Node *textures = xml_node_find_by_attrib(geometry, "id", source_array_id);

		/* TODO TEMPORARY(lungu): Read <accessor> and and replace the constant stride */
		stride = 2;
		mesh->tex_coord_count = u32_from_cstr(xml_attrib_value(textures, "count")) / stride;
		mesh->tex_coords = mem_alloc(mesh->tex_coord_count, sizeof(v2), false);

		at = textures->text;
		for(int i = 0; i < mesh->tex_coord_count; ++i)
		{
			v2 *tex_coords = mesh->tex_coords + i;
			for(i32 j = 0; j < stride; ++j)
			{
				tex_coords->i[j] = f32_from_cstr(at);
				at = char_whitespace_find(at) + 1;
			}
		}

		/* FacesList */
		/* SPEED SPACE(lungu): Maybe skip the face steps and go straight to vectors? */
		struct XML_Node *face_list_nodes[10] = {0};
		struct XML_Node *faces;
		bool node_mesh_child_set = false;
		for(i32 i = 0; i < node_mesh->child_count; ++i)
		{
			if(!node_mesh_child_set)
			{
				faces = node_mesh->child_first;
				node_mesh_child_set = true;
			}
			else faces = faces->next;

			if(!(cstr_eql(faces->name, "polylist") ||
				cstr_eql(faces->name, "triangles")))
			{
				continue;
			}

			face_list_nodes[mesh->face_list_count] = faces;
			++mesh->face_list_count;
		}
		mesh->face_lists = mem_alloc(mesh->face_list_count, sizeof(struct FaceList), true);

		for(i32 i = 0; i < mesh->face_list_count; ++i)
		{
			struct XML_Node *faces = face_list_nodes[i];
			struct FaceList *face_list = mesh->face_lists + i;

			char *material_name = xml_attrib_value(faces, "material");
			for(i32 material_index = 0; material_index < model->material_count; ++material_index)
			{
				struct Material *material = model->materials + material_index;
				if(cstr_eql(material->name, material_name))
				{
					face_list->material_id = material->id;
					break;
				}
			}

			/* TODO(lungu): This will probably be changed in the future when
			    we start using draw elements instead of triangles */
			/* TEMPORARY(lungu): We assume all polygons have 3 faces */
			face_list->face_count = u32_from_cstr(xml_attrib_value(faces, "count"));
			face_list->faces = mem_alloc(face_list->face_count, sizeof(struct Face), true);

			u32 input_count = xml_node_child_count_by_name(faces, "input");
			struct TriangleInput triangle_inputs[10] = {0};
			for(i32 i = 0; i < input_count; ++i)
			{
				struct XML_Node *node_child = xml_node_child_find_by_id(faces, i);
				if(cstr_eql(node_child->name, "input"))
				{
					struct TriangleInput *input = triangle_inputs + i;
					input->semantic = xml_attrib_value(node_child, "semantic");
					input->source = xml_attrib_value(node_child, "source");
					input->offset = u32_from_cstr(xml_attrib_value(node_child, "offset"));
					input->okay = true;

					if(cstr_eql(input->semantic, "VERTEX"))
						input->skip = OFFSET_OF(struct Face, pos_indices);
					else if(cstr_eql(input->semantic, "TEXCOORD"))
						input->skip = OFFSET_OF(struct Face, tex_indices);
					else
					{
						input->okay = false;
					}

					if(xml_attrib_value(node_child, "set"))
						input->set = u32_from_cstr(xml_attrib_value(node_child, "set"));
					else input->set = 0;
				}
			}

			struct XML_Node *p = xml_node_child_find_by_name(faces, "p");

			i32 ignored;
			at = p->text;
			for(int i = 0; i < face_list->face_count; ++i)
			{
				struct Face *face = face_list->faces + i;

				for(int j = 0; j < 3; ++j)
				{
					for(i32 l = 0; l < input_count; ++l)
					{
						struct TriangleInput *input = triangle_inputs + l;
						void *pointer = (u8 *)face + input->skip + (j * sizeof(u32));

						if(input->okay) sscanf(at, "%d", (u32 *)pointer);

						if(i == face_list->face_count - 1 && j == 2 && l == input_count-1) break;
						if(input->set == 0)
						{
							/* FIXME(lungu): it always fails at the last iteration */
							at = char_whitespace_find(at) + 1;
						}
					}
				}
			}
		}
		++mesh_index;
	}
}

void
dae_library_controllers_read(struct Model *model, struct XML_Node *library)
{
	model->flags |= MODEL_FLAG_SKEL;

	/* LEARN(lungu): Can you have more than skeleton per model? */
	model->skeleton_count = 1;
	model->skeletons = skeleton_alloc(model->skeleton_count);

	/* INCOMPLETE TEMPORARY(lungu): Change if more needed */
	struct Skeleton *skel = model->skeletons;

	/* LEARN(lungu): Can you have more than one controller node? */
	struct XML_Node *controller = xml_node_find_by_name(library, "controller");
	char *id = xml_attrib_value(controller, "id");
	char *name = xml_attrib_value(controller, "name");
	global_name_len_skip = cstr_len(name) + 1;

	/* Joints */
	char array[256];
	cstr_cat(array, id, "-joints-array");
	struct XML_Node *joints = xml_node_find_by_attrib(controller, "id", array);
	skel->joint_count = u32_from_cstr(xml_attrib_value(joints, "count"));
	skel->joints = mem_alloc(skel->joint_count, sizeof(struct Joint), true);
	skel->joint_matrices = mem_alloc(skel->joint_count, sizeof(mat4), true);

	char *at = joints->text;
	for(int i = 0; i < skel->joint_count; ++i)
	{
		struct Joint *joint = skel->joints + i;
		joint->id = i;

		sscanf(at, "%s", &joint->name);

		at = char_whitespace_find(at) + 1;
	}

	/* Transform and Inverse Bind Matrix */
	cstr_cat(array, id, "-bind_poses-array");
	struct XML_Node *bind_matrixes = xml_node_find_by_attrib(controller, "id", array);

	at = bind_matrixes->text;;
	for(int i = 0; i < skel->joint_count; ++i)
	{
		struct Joint *joint = skel->joints + i;
		joint->transform = MAT4_ID;
		mat4 *matrix = &joint->transform_inverse;

		for(int j = 0; j < 4; ++j)
		{
			for(i32 l = 0; l < 4; ++l)
			{
				matrix->m[j][l] = f32_from_cstr(at);
				at = char_whitespace_find(at) + 1;
			}
		}
	}

	/* Weight Values */
	cstr_cat(array, id, "-weights-array");
	struct XML_Node *weights = xml_node_find_by_attrib(controller, "id", array);
	skel->weight_count = u32_from_cstr(xml_attrib_value(weights, "count"));
	skel->weights = mem_alloc(skel->weight_count, sizeof(f32), true);

	char *c = weights->text;
	for(int i = 0; i < skel->weight_count; ++i)
	{
		f32 *weight = skel->weights + i;
		sscanf(c, "%f", weight);

		if(i == skel->weight_count - 1) break;
		while(*c++ != ' ');
	}

	/* Weight Vertexes */
	struct XML_Node *vertex_weights = xml_node_find_by_name(controller, "vertex_weights");
	skel->weight_vertex_count = u32_from_cstr(xml_attrib_value(vertex_weights, "count"));
	skel->weight_vertexes = mem_alloc(skel->weight_vertex_count, sizeof(struct WeightVertex), true);

	struct XML_Node *vcount = xml_node_find_by_name(controller, "vcount");
	struct XML_Node *v_node = xml_node_find_by_name(controller, "v");
	c =  vcount->text;
	char *v =  v_node->text;
	for(int i = 0; i < skel->weight_vertex_count; ++i)
	{
		struct WeightVertex *weight_vertex = skel->weight_vertexes + i;

		/* joint_count */
		sscanf(c, "%d", &weight_vertex->joint_count);
		while(*c++ != ' ');

		/* LEARN(lungu): Should this be dynamic or 10 is a good max? */
		u32 tmp_joint_indices[10] = {0};
		u32 tmp_weight_indices[10] = {0};
		f32 tmp_weights[10] = {0};

		/* joint_ids, joint_weights */
		//printf("VERTEX: %d\n", i);
		for(int j = 0; j < weight_vertex->joint_count; ++j)
		{
			sscanf(v, "%d", &tmp_joint_indices[j]);
			while(*v++ != ' ');

			sscanf(v, "%d", &tmp_weight_indices[j]);
			while(*v++ != ' ');

			f32 weight = skel->weights[tmp_weight_indices[j]];
			tmp_weights[j] = weight;
		}

		i32 n = weight_vertex->joint_count;
		i32 j, l;
		for (j = 0; j < n-1; ++j)
		{
			for (l = 0; l < n-j-1; ++l)
			{
				if(tmp_weights[l] < tmp_weights[l+1])
				{
					i32_swap(&tmp_joint_indices[l], &tmp_joint_indices[l+1]);
					i32_swap(&tmp_weight_indices[l], &tmp_weight_indices[l+1]);
					f32_swap(&tmp_weights[l], &tmp_weights[l+1]);
				}
			}
		}

		f32 sum = 0;
		for(int j = 0; j < WEIGHTS_MAX; ++j)
		{
			sum += tmp_weights[j];
		}

		u32 devisor = 0;
		if(n > WEIGHTS_MAX) devisor = WEIGHTS_MAX;
		else devisor = n;

		f32 rest = 1.0f - sum;
		f32 normalizer = rest / devisor;

		for(int j = 0; j < devisor; ++j)
		{
			weight_vertex->joint_indices[j] = tmp_joint_indices[j];
			weight_vertex->weight_indices[j] = tmp_weight_indices[j];
			weight_vertex->weights[j] = tmp_weights[j] + normalizer;
		}
	}
}

void
dae_joint_read(struct XML_Node *node, struct Skeleton *skel)
{
	char *node_id = xml_attrib_value(node, "id");
	node_id += global_name_len_skip;

	struct Joint *joint = joint_get_by_name(skel, node_id);
	if(!joint) return;

	/* Transform Matrix */
	struct XML_Node *matrix = xml_node_child_find_by_name(node, "matrix");

	mat4 transform_local;
	char *c = matrix->text;
	for(int j = 0; j < 4; ++j)
	{
		sscanf(c, "%f %f %f %f",
			&transform_local.m[j][0], &transform_local.m[j][1],
			&transform_local.m[j][2], &transform_local.m[j][3]);

		u32 count = 0;
		while(count < 4) if(*c++ == ' ') ++count;
	}

	joint->transform = MAT4_ID;

	/* Children */
	u32 joint_child_count = 0;
	for(i32 i = 0; i < node->child_count; ++i)
	{
		struct XML_Node *node_child = xml_node_child_find_by_id(node, i);
		if(cstr_eql(node_child->name, "node") && cstr_eql("JOINT", xml_attrib_value(node_child, "type")))
		{
			char *child_id = xml_attrib_value(node_child, "id");
			child_id += global_name_len_skip;
			struct Joint *joint_child = joint_get_by_name(skel, child_id);

			++joint_child_count;
		}
	}

	joint->child_count = joint_child_count;
	if(joint->child_count) joint->children = mem_alloc(joint->child_count, sizeof(void *), true);
	else {joint->children = NULL; return;}

	u32 joint_child_index = 0;
	for(i32 i = 0; i < node->child_count; ++i)
	{
		struct XML_Node *node_child = xml_node_child_find_by_id(node, i);
		if(cstr_eql(node_child->name, "node") && cstr_eql("JOINT", xml_attrib_value(node_child, "type")))
		{
			char *node_child_id = xml_attrib_value(node_child, "id");
			node_child_id += global_name_len_skip;
			struct Joint *joint_child = joint_get_by_name(skel, node_child_id);

			ASSERT(joint_child);

			joint->children[joint_child_index] = joint_child;

			dae_joint_read(node_child, skel);
			++joint_child_index;
		}
	}
}

void
dae_library_visual_scenes_read(struct Model *model, struct XML_Node *library)
{
	struct XML_Node *root_joint = xml_node_find_by_attrib(library, "type", "JOINT");
	if(!root_joint) return;

	struct Skeleton *skel = model->skeletons;
	dae_joint_read(root_joint, skel);
}

/* TEMPORARY IMPLEMENT(lungu): Multiple time independent animations */
/* INCOMPLETE TEMPORARY TODO(lungu): Make animations general!!! */
void
dae_library_animations_read(struct Model *model, struct XML_Node *library)
{
	model->flags |= MODEL_FLAG_ANIM;

	/* LEARN(lungu): More than one skeleton? */
	struct Skeleton *skel = &model->skeletons[0];

	/* LEARN(lungu): More than one animation? */
	model->animation_count = 1;
	model->animations = mem_alloc(model->animation_count, sizeof(struct Animation), true);
	model->animations->keyframe_current = mem_alloc(1, sizeof(struct Keyframe), true);

	bool keyframe_time_set = false;

	struct XML_Node *node_action = library = library->child_first;
	for(i32 i = 0; i < node_action->child_count; ++i)
	{
		/* Assumes there are only animation noted inside */
		struct XML_Node *animation = xml_node_child_find_by_id(node_action, i);
		if(!animation || !cstr_eql(animation->name, "animation")) continue;

		char *animation_name = xml_attrib_value(animation, "name");
		char *animation_id = xml_attrib_value(animation, "id");

		struct XML_Node *channel = xml_node_child_find_by_name(animation, "channel");

		char *joint_name = xml_attrib_value(channel, "target") + global_name_len_skip;
		char *slash = joint_name;
		u32 size = 0;
		while(*slash++ != '/') ++size;

		/* NOTE(lungu): Here we're replacing the actual stored string since i'm lazy and don't want to make a copy of it */
		joint_name[size] = '\0';
		struct Joint *joint = joint_get_by_name(skel, joint_name);
		//printf("ID: %d\n", skel->joint_count);

		if(!keyframe_time_set)
		{
			/* The first float array is always the input time */
			struct XML_Node *times = xml_node_find_by_name(animation, "float_array");
			model->animations->keyframe_count = u32_from_cstr(xml_attrib_value(times, "count"));
			model->animations->keyframes = mem_alloc(model->animations->keyframe_count,
								 sizeof(struct Keyframe), true);
			char *at = times->text;
			for(i32 j = 0; j < model->animations->keyframe_count; ++j)
			{
				struct Keyframe *keyframe = model->animations->keyframes + j;
				keyframe->joint_transforms = mem_alloc(skel->joint_count,
								       sizeof(struct JointTransform), true);
				sscanf(at, "%f", &keyframe->time);
				at = char_whitespace_find(at) + 1;

				if(j == model->animations->keyframe_count - 1)
				{
					model->animations->length = keyframe->time;
				}
			}
			keyframe_time_set = true;
		}

#define BUFFER_SIZE 256
		char array_name[BUFFER_SIZE];
		cstr_cat(array_name, animation_id, "-output-array");
		struct XML_Node *output_array = xml_node_find_by_attrib(animation, "id", array_name);

		char *c = output_array->text;
		for(i32 j = 0; j < model->animations->keyframe_count; ++j)
		{
			struct Keyframe *keyframe = model->animations->keyframes + j;
			struct JointTransform *joint_transform = keyframe->joint_transforms + joint->id;
			mat4 matrix = {0};
			for(int j = 0; j < 4; ++j)
			{
				sscanf(c, "%f %f %f %f", &matrix.m[j][0], &matrix.m[j][1],
					&matrix.m[j][2], &matrix.m[j][3]);

				u32 count = 0;
				while(count < 4) if(*c++ == ' ') ++count;
			}

			v3 pos = V3(matrix.m[0][3], matrix.m[1][3], matrix.m[2][3]);
			Quat rot = quat_from_mat4(matrix);

			joint_transform->pos = pos;
			joint_transform->rot = rot;
		}

		f32 end = 0;
	}
}

void
dae_read(struct Model *model, struct XML_Node *root)
{
	/* Animations */
	for(i32 child_index = 0; child_index < root->child_count; ++child_index )
	{
		struct XML_Node *node = xml_node_child_find_by_id(root, child_index);
		if(cstr_eql(node->name, "library_images"))
		{
			dae_library_images_read(model, node);
			break;
		}
	}

	for(i32 child_index = 0; child_index < root->child_count; ++child_index )
	{
		struct XML_Node *node = xml_node_child_find_by_id(root, child_index);
		if(cstr_eql(node->name, "library_effects"))
		{
			dae_library_effects_read(model, node);
			continue;
		}
		if(cstr_eql(node->name, "library_materials"))
		{
			dae_library_materials_read(model, node);
			continue;
		}
		if(cstr_eql(node->name, "library_geometries"))
		{
			dae_library_geometries_read(model, node);
			continue;
		}
		if(cstr_eql(node->name, "library_controllers"))
		{
			dae_library_controllers_read(model, node);
			continue;
		}
		if(cstr_eql(node->name, "library_visual_scenes"))
		{
			dae_library_visual_scenes_read(model, node);
			continue;
		}
	}

	/* Animations */
	for(i32 child_index = 0; child_index < root->child_count; ++child_index )
	{
		struct XML_Node *node = xml_node_child_find_by_id(root, child_index);
		if(cstr_eql(node->name, "library_animations"))
		{
			dae_library_animations_read(model, node);
			break;
		}
	}
}
