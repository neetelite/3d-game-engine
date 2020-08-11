void
scene_cam_next(struct Scene *scene)
{
	scene->current_camera_id = (scene->current_camera_id + 1) % scene->camera_count;
}

struct Camera *
scene_current_cam(struct Scene *scene)
{
	struct Camera *result;
	result = &scene->cameras[scene->current_camera_id];
	return(result);
}

void
opengl_program_anim_create(GLuint *program)
{
	char path[PATH_MAX];

	cstr_cat(path, os_state.path_src, "shader/vertex.glsl");
	u32 vertex_shader = shader_load(path, GL_VERTEX_SHADER);

	cstr_cat(path, os_state.path_src, "shader/fragment.glsl");
	u32 fragment_shader = shader_load(path, GL_FRAGMENT_SHADER);

	*program = glCreateProgram();

	glAttachShader(*program, vertex_shader);
	glAttachShader(*program, fragment_shader);
	glLinkProgram(*program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void
opengl_program_static_create(GLuint *program)
{
	char path[PATH_MAX];

	cstr_cat(path, os_state.path_src, "shader/vertex_old.glsl");
	u32 vertex_shader = shader_load(path, GL_VERTEX_SHADER);

	cstr_cat(path, os_state.path_src, "shader/fragment.glsl");
	u32 fragment_shader = shader_load(path, GL_FRAGMENT_SHADER);

	*program = glCreateProgram();

	glAttachShader(*program, vertex_shader);
	glAttachShader(*program, fragment_shader);
	glLinkProgram(*program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

struct Model *
model_init(struct Model *model, char *model_name)
{
	struct Mesh *mesh = model->meshes;
	struct Skeleton *skel = model->skeletons;

	char path_model[PATH_MAX];
	sprintf(path_model, "%s%s/%s/%s%s", os_state.path_data, "model", model_name, model_name, ".dae");
	model_load(model, path_model);

	if(model->flags & MODEL_FLAG_ANIM)
	{
		model->animator.model = model;
		model->animator.animation = &model->animations[0];
		//model->animator.start = model->animator.animations[0].keyframes[0].time;
		model->animator.speed = 0.7;
		model->animator.loop = true;
		//model->animator.time = model->animator.start;
	}

	for(i32 mesh_index = 0; mesh_index < model->mesh_count; ++mesh_index)
	{
		struct Mesh *mesh = model->meshes + mesh_index;

		for(i32 vertex_list_index = 0; vertex_list_index < mesh->vertex_list_count; ++vertex_list_index)
		{
			struct VertexList *vertex_list = mesh->vertex_lists + vertex_list_index;

			/* Gen a VAO and bind it */
			glGenVertexArrays(1, &vertex_list->VAO);
			glBindVertexArray(vertex_list->VAO);

			/* Gen a VBO and push data into it */
			glGenBuffers(1, &vertex_list->VBO);
			glBindBuffer(GL_ARRAY_BUFFER, vertex_list->VBO);
			glBufferData(GL_ARRAY_BUFFER, vertex_list->vertex_count * sizeof(struct Vertex),
				     vertex_list->vertexes, GL_STATIC_DRAW);

			/* Postion Coordiantes */
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)0);
			glEnableVertexAttribArray(0);

			/* Texture Coordiantes */
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex),
					    (void *)OFFSET_OF(struct Vertex, tex));
			glEnableVertexAttribArray(1);

			/* Joint Indices */
			glVertexAttribIPointer(2, 4, GL_UNSIGNED_INT, sizeof(struct Vertex),
					    (void *)OFFSET_OF(struct Vertex, joint_indices));
			glEnableVertexAttribArray(2);

			/* Joint Weights */
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(struct Vertex),
					    (void *)OFFSET_OF(struct Vertex, weights));
			glEnableVertexAttribArray(3);

			/* Unbind VBO and VAO */
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
	}
	return(model);
}

void
cam_dir_from_rot(struct Camera *cam)
{
	/* REDUNDANT(lungu): Similar to v3_rot() */
	#if 0
	f32 heading = f32_rad_from_deg(cam->rot.x); //heading
	f32 elevation = f32_rad_from_deg(cam->rot.y); //elevation
	#else
	f32 heading = f32_rad_from_deg(cam->rot.y); //heading
	f32 elevation = f32_rad_from_deg(cam->rot.x); //elevation
	#endif

	//SPEED: swap sinf and cosf with fast_sin and fast_cos
	f32 sin_ele = f32_sin(elevation);
	f32 cos_ele = f32_cos(elevation);
	f32 sin_hea = f32_sin(heading);
	f32 cos_hea = f32_cos(heading);

	cam->n.x = cos_ele * sin_hea;
	cam->n.y = sin_ele;
	cam->n.z = cos_ele * cos_hea;

	cam->dir = v3_inv(cam->n);
}

v3
dir_from_rot(v3 rot)
{
	v3 result = V3_ZERO;

	f32 heading = f32_rad_from_deg(rot.y); //heading
	f32 elevation = f32_rad_from_deg(rot.x); //elevation

	//SPEED: swap sinf and cosf with fast_sin and fast_cos
	f32 sin_ele = f32_sin(elevation);
	f32 cos_ele = f32_cos(elevation);
	f32 sin_hea = f32_sin(heading);
	f32 cos_hea = f32_cos(heading);

	result.x = cos_ele * sin_hea;
	result.y = sin_ele;
	result.z = cos_ele * cos_hea;

	result = v3_inv(result);
	return(result);
}

v3
rot_from_dir(v3 dir)
{
	v3 result = V3_ZERO;

	for(i32 i = 0; i < 3; ++i)
	{
		result.i[i] = dir.i[i] * 180;
	}

	return(result);
}

void
cam_matrix_target(struct Camera *cam)
{
	/* "Look At" Matrix */
	/* pos, tar, up */
	cam->n = v3_norm(v3_s(cam->pos, cam->tar));

	cam->u = v3_norm(v3_cross(cam->v, cam->n));
	cam->v = v3_cross(cam->n, cam->u);

	m4x4 look_at = MAT4
		(
			cam->u.x, cam->u.y, cam->u.z, 0,
			cam->v.x,    cam->v.y,    cam->v.z,    0,
			cam->n.x,   cam->n.y,   cam->n.z,   0,
			0,            0,            0,            1
		);

	m4x4 pos_neg = MAT4
		(
			1, 0, 0, -cam->pos.x,
			0, 1, 0, -cam->pos.y,
			0, 0, 1, -cam->pos.z,
			0, 0, 0,           1
		);

	cam->matrix = m4x4_m(look_at, pos_neg);
}

void
cam_matrix_free(struct Camera *cam)
{
	//STEP 1: n = <target position - view reference point>
	if(cam->target_locked)
	{
		cam->n = v3_s(cam->tar, cam->pos);
	}
	else
	{
		cam->n = v3_inv(cam->dir);
	}

	//STEP 2: let v = <0, 1, 0>
	cam->v = V3(0, 1, 0);

	//STEP 3: u = (v x n)
	v3p_cross(&cam->u, &cam->v, &cam->n);

	//STEP 4: v = (n x u)
	v3p_cross(&cam->v, &cam->n, &cam->u);

	//STEP 5: normalize all vectors
	v3p_norm(&cam->u, &cam->u);
	v3p_norm(&cam->v, &cam->v);
	v3p_norm(&cam->n, &cam->n);

	m4x4 look_at = MAT4
		(
			cam->u.x, cam->u.y, cam->u.z, 0,
			cam->v.x, cam->v.y, cam->v.z, 0,
			cam->n.x, cam->n.y, cam->n.z, 0,
			0,        0,        0,        1
		);

	m4x4 pos_neg = MAT4
		(
			1, 0, 0, -cam->pos.x,
			0, 1, 0, -cam->pos.y,
			0, 0, 1, -cam->pos.z,
			0, 0, 0,           1
		);

	cam->matrix = m4x4_m(look_at, pos_neg);
}

void
cam_matrix(struct Camera *cam)
{
	/* View */
	if(cam->target_locked) cam_matrix_target(cam);
	else cam_matrix_free(cam);

	/* Projection */
	m4x4 matrix_projection;

	f32 n = cam->near_clip; /* NOTE(lungu): Can't be negative or zero */
	f32 f = cam->far_clip;
	if(cam->perspective)
	{
		f64 fov = 1.0 / tan(f32_rad_from_deg(cam->fov) / 2);

		f32 A = fov * 1.0f;
		f32 B = fov * ((f32)os_context.width / os_context.height);
		f32 C = (n+f) / (n-f);
		f32 D = (2*n*f) / (n-f);

		matrix_projection = MAT4
			(
				A, 0,  0, 0,
				0, B,  0, 0,
				0, 0,  C, D,
				0, 0, -1, 0
			);
	}
	else
	{
		/* LEARN(lungu): Is this the correct matrix? */
		f32 A = 1.0f;
		f32 B = ((f32)os_context.width / os_context.height);
		f32 C = 2.0f / (n-f);
		f32 D = (n+f) / (n-f);

		matrix_projection = MAT4
			(
				A, 0, 0, 0,
				0, B, 0, 0,
				0, 0, C, D,
				0, 0, 0, 1
			);
	}

	cam->matrix = m4x4_m(matrix_projection, cam->matrix);
	cam->matrix = mat4_transpose(cam->matrix);
}

void
cam_control(struct Camera *cam)
{
	//struct OsKeyboard *keyboard = &os_input.keyboards[0];
	struct OsMouse *mouse = &os_input.mice[0];
	f32 dt = os_state.dt;

	f32 mov_speed = 1.4 * dt; /* Average Walking Speed - 1.4 meters per second */

	f32 rot_speed = 90 * dt; /* 90 degress per second */
	rot_speed = f32_rad_from_deg(rot_speed);

	/* FIXME(lungu): Goes crazy when mouse is not locked */
	/* Rotation */
	v3 new_rot = cam->rot;
	new_rot.x -= (mouse->d.y * 0.01) * cam->sensitivity;
	new_rot.y -= (mouse->d.x * 0.01) * cam->sensitivity;
	new_rot = v3_angle_norm(new_rot);

	f32 cap_up = -37; // -70
	f32 cap_down = 70;
	if(new_rot.x < cap_up) new_rot.x = cap_up;
	if(new_rot.x > cap_down) new_rot.x = cap_down;

	//v3_print(new_rot, "R: ");

	cam->rot = new_rot;
	cam_dir_from_rot(cam);

	/* Translation */
	if(!cam->free) return;
	if(key_down(KEY_SPACE))
	{
		cam->pos = v3_a(cam->pos, v3_mf(cam->v, mov_speed));
	}

	if(key_down(KEY_ESCAPE))
	{
		cam->pos = v3_s(cam->pos, v3_mf(cam->v, mov_speed));
	}
	if(key_down('w'))
	{
		//cam->pos = v3_a(cam->pos, v3_mf(cam->n, mov_speed));
		cam->pos = v3_a(cam->pos, v3_mf(cam->dir, mov_speed));
	}
	if(key_down('a'))
	{
		cam->pos = v3_s(cam->pos, v3_mf(cam->u, mov_speed));
	}
	if(key_down('s'))
	{
		//cam->pos = v3_s(cam->pos, v3_mf(cam->n, mov_speed));
		cam->pos = v3_s(cam->pos, v3_mf(cam->dir, mov_speed));
	}
	if(key_down('d'))
	{
		cam->pos = v3_a(cam->pos, v3_mf(cam->u, mov_speed));
	}
}


void
model_draw(struct Model *model)
{
	struct Scene *scene = &app_state->scene;
	struct Skeleton *skeleton = &model->skeletons[0];

	/* NOTE(lungu): You MUST issue glUseProgram before loading ANY uniform values */
	u32 program = 0;
	if(model->flags & MODEL_FLAG_ANIM)
	{
		program = app_state->program_anim;
		glUseProgram(program);

		skeleton_joint_matrix_array(skeleton);
		GLuint matrix_joints_location = glGetUniformLocation(program, "matrix_joints");
		glUniformMatrix4fv(matrix_joints_location, 16, GL_FALSE, (f32 *)skeleton->joint_matrices);

	}
	else
	{
		program = app_state->program_static;
		glUseProgram(program);
	}

	struct Camera *cam = scene_current_cam(scene);
	GLuint matrix_cam_location = glGetUniformLocation(program, "matrix_cam");
	glUniformMatrix4fv(matrix_cam_location, 1, GL_FALSE, (f32 *)&cam->matrix);

	GLuint matrix_model_location = glGetUniformLocation(program, "matrix_model");
	glUniformMatrix4fv(matrix_model_location, 1, GL_FALSE, (f32 *)&model->transform);


	/* LEARN(lungu): More than one skeleton? */
	for(i32 mesh_index = 0; mesh_index < model->mesh_count; ++mesh_index)
	{
		struct Mesh *mesh = model->meshes + mesh_index;

		for(i32 vertex_list_index = 0; vertex_list_index < mesh->vertex_list_count; ++vertex_list_index)
		{
			struct VertexList *vertex_list = mesh->vertex_lists + vertex_list_index;

			glBindVertexArray(vertex_list->VAO);
			if(model->texture_count)
			{
				u32 material_id = vertex_list->material_id;
				u32 effect_id = model->materials[material_id].effect_id;
				u32 texture_id = model->effects[effect_id].surface_id;
				glBindTexture(GL_TEXTURE_2D, model->textures[texture_id].handle);
			}
			else glBindTexture(GL_TEXTURE_2D, app_state->dflt.texture.handle);

			glDrawArrays(GL_TRIANGLES, 0, vertex_list->vertex_count);
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void
character_init(struct Character *character, char *path)
{
	/* TEMPORARY(lungu): Hard coded */
	character->id = 0;
	character->name = CSTR(path);

	character->pos = V3(0, 0, 0);
	character->vel = V3_ZERO;
	character->acl = V3_ZERO;
	character->dir = V3_FRONT;
	character->rot = V3(0, 0, 0);
	//character->rot = V3(90, 180, 0);
	character->scl = V3_ONE;
	//character->scl = v3_mf(V3_ONE, 0.18);

	character->rot_y_target = character->rot.y;
	character->rot_y_speed = 540;

	character->looking_dir = V3_BACK;
	character->walking_dir = V3_BACK;

	character->camera_id = 1;

	character->model = mem_alloc(1, sizeof(struct Model), true);

	struct Model *model = character->model;
	model->id = character->id;
	model->name = character->name;
	model_init(model, path);
}

void
entity_matrix(struct Entity *entity)
{
	m4x4 matrix_transform = MAT4_ID;
	m4x4 matrix_scale = m4x4_scale(entity->scl.x, entity->scl.y, entity->scl.z);
	m4x4 matrix_rotation = m4x4_euler_rot(entity->rot.x, entity->rot.y, entity->rot.z);
	m4x4 matrix_translation = m4x4_trans(entity->pos.x, entity->pos.y, entity->pos.z);

	/* FIXME(lungu): Left to right, instead of right to left multiplication, use the new mat4 functions */
	matrix_transform = mat4_m(matrix_scale, matrix_rotation);
	entity->model->transform = mat4_m(matrix_translation, matrix_transform);
}

void
character_matrix(struct Character *character)
{
	m4x4 matrix_scale = m4x4_scale(character->scl.x, character->scl.y, character->scl.z);
	m4x4 matrix_rotation = m4x4_euler_rot(character->rot.x, character->rot.y, character->rot.z);
	m4x4 matrix_translation = m4x4_trans(character->pos.x, character->pos.y, character->pos.z);

	/* FIXME(lungu): Left to right, instead of right to left multiplication, use the new mat4 functions */
	m4x4 matrix_transform = MAT4_ID;
	matrix_transform = mat4_m(matrix_scale, matrix_transform);
	matrix_transform = mat4_m(matrix_rotation, matrix_transform);
	matrix_transform = mat4_m(matrix_translation, matrix_transform);
	character->model->transform = matrix_transform;
}

void
character_control(struct Character *character)
{
	struct Scene *scene = &app_state->scene;
	struct Camera *cam = &scene->cameras[character->camera_id];

	struct OsMouse *mouse = &os_input.mice[0];
	f32 dt = os_state.dt;

	/* INITIALIZE */
	/* Constants */
	bool move = true;
	f32 mov_speed = 1.4 * dt; /* Average Walking Speed - 1.4 meters per second */
	f32 jump_speed = 200 * dt;
	f32 gravity = 50.0f * dt;

	/* Variables */
	bool jump;
	v3 new_pos = V3_ZERO;

	v3 new_walking_dir = V3_ZERO;
	v3 new_input_dir = V3_ZERO;

	f32 new_rot_forward = 360 - cam->rot.y;
	f32 new_rot_y = 0;

	if(character->rot.y != character->rot_y_target) character->rot_y_t += 0.1 * dt;
	else character->rot_y_t = 0;

	/* INPUT */
	if(character->control)
	{
		character->looking_dir = cam->dir;

		/* Keyboard */
		if(key_down(KEY_SPACE)) jump = true; else jump = false;

		if(key_down('w'))
		{
			new_input_dir.z += 1;
		}
		if(key_down('a'))
		{
			new_input_dir.x -= 1;
		}
		if(key_down('s'))
		{
			new_input_dir.z -= 1;
		}
		if(key_down('d'))
		{
			new_input_dir.x += 1;
		}

		/* INCOMPLETE(lungu): Interpolate between rotations */
		/* LEARN(lungu): Change the rotation to quaternion */
		if(new_input_dir.x && !new_input_dir.z)
		{
			new_rot_y = new_rot_forward + 90 * new_input_dir.x;
		}
		else if(new_input_dir.x && new_input_dir.z == 1)
		{
			new_rot_y = new_rot_forward + (45 * new_input_dir.x);
		}
		else if(!new_input_dir.x && new_input_dir.z == 1)
		{
			new_rot_y = new_rot_forward + 0;
		}
		else if(new_input_dir.x && new_input_dir.z == -1)
		{
			new_rot_y = new_rot_forward + (135 * new_input_dir.x);
		}
		else if(!new_input_dir.x && new_input_dir.z == -1)
		{
			new_rot_y = new_rot_forward + 180;
		}
		else
		{
			new_rot_y = character->rot.y;
			move = false;
		}
		new_rot_y = f32_mod(new_rot_y, 360);

		if(move)
		{
			//if(key_down(KEY_SHIFT_LEFT) || key_down(KEY_SHIFT_RIGHT))
			if(key_down(' '))
			{
				mov_speed = 3.7 * dt;
				character->state = character_state_running;
			}
			else
			{
				character->state = character_state_walking;
			}

			new_walking_dir = dir_from_rot(V3(0, -new_rot_y, 0));
			character->rot_y_target = new_rot_y;
			character->walking_dir = new_walking_dir;
			new_pos = v3_mf(new_walking_dir, mov_speed);
		}
		else
		{
			character->state = character_state_idle;
		}
	}

	#if 0
	/* LEARN(lungu): Change this to quaternion interpolation */
	bool skipper = false;
	i8 rot_dir = 1;
	f32 dif = character->rot_y_target - character->rot.y;
	if(dif > 0) rot_dir = 1;
	else rot_dir = -1;

	if(f32_abs(dif) > 180)
	{
		skipper = true;
		if(rot_dir == 1) rot_dir = -1;
		else rot_dir = 1;
	}

	if(character->rot.y != character->rot_y_target)
	{
		f32 rot_y_skip = character->rot.y + (rot_dir * character->rot_y_speed * dt);
		if(!skipper && rot_dir == 1 && rot_y_skip > character->rot_y_target)
		{
			character->rot.y = character->rot_y_target;
		}
		else if(skipper && rot_dir == 1 && rot_y_skip - 180 > character->rot_y_target + 180)
		{
			character->rot.y = character->rot_y_target;
		}
		else if(skipper && rot_dir == -1 && rot_y_skip + 180 < character->rot_y_target - 180)
		{
			character->rot.y = character->rot_y_target;
		}
		else character->rot.y = rot_y_skip;

		//character->rot.y += rot_dir * character->rot_y_speed * dt;
	}
	#else
	character->rot.y = character->rot_y_target;
	#endif

	/* Physics */
	new_pos = v3_a(character->pos, new_pos);
	new_pos.y -= gravity;

	//f32 floor_height = -8;
	f32 floor_height = 0.92;
	if(new_pos.y < floor_height) new_pos.y = floor_height;

	/* Terminate */
	character->pos = new_pos;

	/* Camera */
	/* IMPLEMENT LEARN(lungu): Reycast behind and pull the camera closer as needed */
	cam->pos = v3_a(character->pos, v3_a(v3_mf(cam->dir, cam->offset.z), V3(0, cam->offset.y, 0)));

	/* Animation */
	//struct Animator *animator = &character->model->animator;
	//animator->speed = (mov_speed / dt) * 0.2;
}

void
character_animate(struct Character *character)
{
	struct Model *model = character->model;
	struct Animator *animator = &model->animator;

	/* Animation Pause/Play */
	static bool toggle_animation = true;
	if(key_press('o')) toggle_animation = !toggle_animation;

	if(!toggle_animation) return;

	switch(character->state)
	{
	case character_state_idle:
	{
		animator->animation = NULL;
	} break;
	case character_state_walking:
	{
		animator->animation = NULL;
	} break;
	case character_state_running:
	{
		animator->animation = &model->animations[0];
	} break;
	default:
	{
		ERROR("%s", "Invalid character state!");
	} break;
	}

	model_anim_update(model, os_state.dt);
}

void
character_update(struct Character *character)
{
	character_control(character);
	character_animate(character);
}

void
scene_update(struct Scene *scene)
{
	struct Character *character = &scene->character;
	struct Camera *cam = scene_current_cam(scene);

	if(key_press('n')) scene_cam_next(scene);

	if(scene->current_camera_id == 0)
	{
		cam->free = true;
		character->control = false;
	}
	else
	{
		cam->free = false;
		character->control = true;
	}

	cam_control(scene_current_cam(scene));
	character_update(character);

	cam_matrix(cam);
	character_matrix(character);
	entity_matrix(&scene->floor);
}

void
scene_draw(struct Scene *scene)
{
	struct Character *character = &scene->character;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Rendering Type */
	static bool toggle_rendering = true;
	if(key_press('u') && toggle_rendering)
	{
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		toggle_rendering = !toggle_rendering;
	}
	else if(key_press('u') && !toggle_rendering)
	{
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		toggle_rendering = !toggle_rendering;
	}

	model_draw(character->model);
	model_draw(scene->floor.model);
}

v3
rgb_from_u32(void)
{
	v3 result = V3_ZERO;

	/* IMPLEMENT */

	return(result);
}

void
app_main(void)
{
	struct Scene *scene = &app_state->scene;
	struct Character *character = &scene->character;
	struct Entity *floor = &scene->floor;
	struct Entity *cube = &scene->cube;

	/* Initialize */
	if(!app_memory.init)
	{
		/* OpenGL */
		/* FIXME(lungu): Depth test doesn't work sometimes, it has to do with the graphics card it's using */
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, os_context.width, os_context.height);

		v3 clear_color = V3(0, 0, 0);
		glClearColor(clear_color.r, clear_color.g, clear_color.b, 1);

		opengl_program_anim_create(&app_state->program_anim);
		opengl_program_static_create(&app_state->program_static);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Texture NULL */
		Image *image = &app_state->dflt.texture.image;
		char texture_path[PATH_MAX] = "";
		cstr_cat(texture_path, os_state.path_data, "/img/null.bmp");
		img_load(image, texture_path);

		glGenTextures(1, &app_state->dflt.texture.handle);
		glBindTexture(GL_TEXTURE_2D, app_state->dflt.texture.handle);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height,
				0, GL_BGRA, GL_UNSIGNED_BYTE, image->data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		#if 0
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		#else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		#endif

		glBindTexture(GL_TEXTURE_2D, 0);

		/* Model */
		/* FIXME(lungu): OBJ Loader doesn't work anymore becaues of FaceList */
		//character_init(character, "lila");
		character_init(character, "cowboy");
		//character_init(character, "pain");

		floor->model = mem_alloc(1, sizeof(struct Model), true);
		model_init(floor->model, "floor");
		floor->scl = v3_mf(V3_ONE, 8);
		floor->rot = V3(0, 0, 0);

		cube->model = mem_alloc(1, sizeof(struct Model), true);
		model_init(cube->model, "cube");
		cube->scl = v3_mf(V3_ONE, 1);
		cube->rot = V3(0, 0, 0);

		/* Cameras */
		scene->camera_count = 2;
		scene->cameras = mem_alloc(scene->camera_count, sizeof(struct Camera), true);

		/* Camera 0 */
		scene->cameras[0].id = 0;
		scene->cameras[0].name = CSTR("Free Camera");
		scene->cameras[0].pos = V3(0, 1, 1);
		scene->cameras[0].tar = V3(0, 0, -1);
		scene->cameras[0].sensitivity = 4;
		scene->cameras[0].dir = V3(0, 0, -1);
		scene->cameras[0].rot = V3(0, 0, 0);

		scene->cameras[0].fov = 90; /* NOTE(lungu): Less than 180, greater than ?? */
		scene->cameras[0].near_clip = 0.01;
		scene->cameras[0].far_clip = 1000;
		scene->cameras[0].target_locked = false;
		scene->cameras[0].perspective = true;
		scene->cameras[0].free = true;

		/* Camera 1 */
		scene->cameras[1].pos = V3(0, 50, 10);
		scene->cameras[1].tar = character->pos;
		scene->cameras[1].sensitivity = 4;
		scene->cameras[1].dir = character->dir;
		scene->cameras[1].rot = V3(0, 0, 0);
		scene->cameras[1].offset = V3(0, 0.7, -2.5);

		scene->cameras[1].id = 1;
		scene->cameras[1].name = CSTR("Following Camera");
		scene->cameras[1].fov = 90; /* NOTE(lungu): Less than 180, greater than ?? */
		scene->cameras[1].near_clip = 0.01;
		scene->cameras[1].far_clip = 1000;
		scene->cameras[1].target_locked = false;
		scene->cameras[1].perspective = true;
		scene->cameras[1].free = false;

		scene->current_camera_id = 1;

		os_input.mice[0].locked = true;

		struct Net_Connection *connection = net_alloc();
		net_init(connection, NET_UDP, NET_ADDRESS_ANY, 5000);

		char *message = "GET / HTTP/1.1\r\n\r\n";
		net_send_to(connection, message, &connection->address);

		app_memory.init = true;
	}

	/* Update */
	scene_update(scene);

	/* Rendering */
	scene_draw(scene);

	/* Terminate */
	if(!os_state.running)
	{
		printf("App terminating...\n");

		/* Free Stuff */
		//model_free(model);
		img_unload(&app_state->dflt.texture.image);

		printf("App terminated with success.\n");
	}
}
