struct Default
{
	struct Texture texture;
};

struct Scene
{
	//u32 camera_count;
	struct Character character;
	struct Entity floor;
	struct Entity cube;
	u32 current_entity_id;

	u32 camera_count;
	struct Camera *cameras;
	u32 current_camera_id;
};

/* Important structures */
struct AppState
{
	struct Default dflt;

	//u32 scene_count;
	struct Scene scene;

	GLuint program_anim;
	GLuint program_static;
	GLuint current_program_id;
};

struct AppMemory
{
	u64 size;
	void *storage;

	bool init;
};
