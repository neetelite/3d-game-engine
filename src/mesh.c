/* TODO(lungu): mem_alloc() instead - NOTE(lungu): Only used in .obj files */
#define MESH_POS_MAX 140000 //140,000
#define MESH_TEX_MAX 140000
#define MESH_FACE_MAX 140000


struct Face
{
	u32 pos_indices[3];
	u32 tex_indices[3];
};

struct FaceList
{
	u32 material_id;

	u32 face_count;
	struct Face *faces;
};

struct Vertex
{
	v3 pos;
	v2 tex;
	//v3 norm;

#define WEIGHTS_MAX 4
	u32 joint_indices[WEIGHTS_MAX];
	f32 weights[WEIGHTS_MAX];
};

struct VertexList
{
	u32 material_id;

	u32 vertex_count;
	struct Vertex *vertexes;

	/* OPENGL STUFF */
	GLuint VAO;
	GLuint VBO;
};

/* Structures */
struct Mesh
{
	u32 id;
	char *name;

	u64 pos_coord_count;
	v3 *pos_coords;

	u64 tex_coord_count;
	v2 *tex_coords;

	u64 norm_coord_count;
	v2 *norm_coords;

	/* TODO(lungu): This step is to be removed */
	u64 face_list_count;
	struct FaceList *face_lists;

	/* TODO(lungu): vertex list */
	u32 vertex_list_count;
	struct VertexList *vertex_lists;

};

void
mesh_free(struct Mesh *mesh)
{
	if(!mesh) return;

	mem_free(mesh->name);
	mem_free(mesh->pos_coords);
	mem_free(mesh->tex_coords);
	mem_free(mesh->norm_coords);

	for(i32 i = 0; i < mesh->face_list_count; ++i)
	{
		struct FaceList *face_list = mesh->face_lists + i;
		struct VertexList *vertex_list = mesh->vertex_lists + i;

		mem_free(face_list->faces);
		mem_free(vertex_list->vertexes);
	}
	mem_free(mesh->face_lists);
	mem_free(mesh->vertex_lists);
}
