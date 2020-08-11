enum EntityType
{
	entity_type_null,
	entity_type_character,
};

struct Entity
{
	/* Have a physical place in the world */
	u32 id;
	char *name;
	enum EntityType type;

	v3 pos;
	v3 vel;
	v3 acl;
	v3 dir;
	v3 rot;
	v3 scl;

	struct Model *model;
};

enum CharacterState
{
	character_state_idle,
	character_state_walking,
	character_state_running,
};

struct Character
{
	u32 id;
	char *name;

	enum CharacterState state;
	bool control;

	v3 pos;
	v3 vel;
	v3 acl;
	v3 dir;
	v3 rot;
	v3 scl;

	f32 rot_y_speed;
	f32 rot_y_target;
	f32 rot_y_t;
	f32 rot_y_t_target;

	v3 walking_dir; /* You can't walk up or down - Use only x and z */
	v3 looking_dir;

	u32 camera_id;
	struct Model *model;
};
