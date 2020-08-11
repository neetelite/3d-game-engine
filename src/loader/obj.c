#if 0
i32
obj_read(struct Model *model, FILE *file)
{
	/* SPEED LEARN(lungu): Multiple meshes, good or bad idea to have this many VAOs, VBOs */
	model->mesh_count = 1;
	model->meshes = mem_alloc(model->mesh_count, sizeof(struct Mesh), true);

	struct Mesh *mesh = model->meshes;

#define BUFFER_SIZE 256
	char buffer[BUFFER_SIZE];

	mesh->pos_coord_count = 0;
	mesh->pos_coords = mem_alloc(MESH_POS_MAX, sizeof(v3), false);

	mesh->tex_coord_count = 0;
	mesh->tex_coords = mem_alloc(MESH_TEX_MAX, sizeof(v2), false);

	mesh->face_list_count = 1;
	mesh->face_lists = mem_alloc(1, sizeof(struct Face), false);

	struct FaceList *face_list = &mesh->face_lists[0];
	face_list->face_count = 0;
	face_list->faces = mem_alloc(MESH_FACE_MAX, sizeof(struct Face), false);

	mesh->vertex_count = 0;

	char ignored[BUFFER_SIZE];
	while(fgets(buffer, BUFFER_SIZE, file) != NULL)
	{
		/* Position and Texture Vertices */
		if(buffer[0] == 'v')
		{
			if(0) {}
			else if(buffer[1] == ' ')
			{
				/* NOTE(lungu): Range 0 to object SCALE */
				v3 *pos_coords = mesh->pos_coords + mesh->pos_coord_count;;
				sscanf(buffer, "%s %f %f %f", ignored,
					&pos_coords->x, &pos_coords->y, &pos_coords->z);

				++mesh->pos_coord_count;
			}
			else if(buffer[1] == 't')
			{
				/* NOTE(lungu): 0 to 1 range */
				v2 *tex_coords = mesh->tex_coords + mesh->tex_coord_count;
				sscanf(buffer, "%s %f %f", ignored, &tex_coords->x, &tex_coords->y);

				++mesh->tex_coord_count;
			}
		}
		/* Faces */
		else if(buffer[0] == 'f')
		{
			struct Face *face = face_list->faces + face_list->face_count;

			/* NOTE(lungu): Where the coordinates actually start in the str */
			char *tri = buffer + 2;
			u8 vert_count = 0;

			for (char *str = strtok(tri," "); str != NULL; str = strtok(NULL, " "))
			{
				if(vert_count < 3)
				{
					/* Trilateral */
					sscanf(str, "%d/%d", &face->pos_indices[vert_count],
					       &face->tex_indices[vert_count]);

					--face->pos_indices[vert_count];
					--face->tex_indices[vert_count];
				}
				else
				{
					/* Quadrilateral */
					/* NOTE(lungu): Separate it into two triangles */
					/* NOTE(lungu): New triangle is has vertices: 0 2 3 from original face */

					struct Face *face_extra = face + 1;

					face_extra->pos_indices[0] = face->pos_indices[0];
					face_extra->tex_indices[0] = face->tex_indices[0];

					face_extra->pos_indices[1] = face->pos_indices[2];
					face_extra->tex_indices[1] = face->tex_indices[2];

					sscanf(str, "%d/%d", &face_extra->pos_indices[2],
					       &face_extra->tex_indices[2]);

					--face_extra->pos_indices[2];
					--face_extra->tex_indices[2];

					++face_list->face_count;
				}
				++vert_count;
			}
			++face_list->face_count;
		}
	}

	return(1);
}
#endif
