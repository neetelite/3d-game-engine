struct Camera
{
	u32 id;
	char *name;

	v3 pos;
	v3 tar;

	f32 offset_back;
	f32 offset_side;
	f32 offset_vertical;

	v3 offset;

	f32 sensitivity;
	v3 dir;
	v3 rot;

	v3 u; /* right */
	v3 v; /* up */
	v3 n; /* backward */

	f32 fov;
	f32 near_clip;
	f32 far_clip;

	bool free;
	bool target_locked;
	bool perspective;

	m4x4 matrix;
};
