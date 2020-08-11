/* SPEED LEARN(lungu): Make everything static inline or not? */

typedef union v2
{
	struct
	{
		f32 x, y;
	};
	struct
	{
		f32 u, v;
	};
	struct
	{
		f32 width, height;
	};
	f32 i[2];
} v2;

typedef union v2i
{
	struct
	{
		i32 x, y;
	};
	struct
	{
		i32 u, v;
	};
	struct
	{
		i32 width, height;
	};
	i32 i[2];
} v2i;

typedef union v2u
{
	struct
	{
		u32 x, y;
	};
	struct
	{
		u32 u, v;
	};
	struct
	{
		u32 width, height;
	};
	u32 i[2];
} v2u;

typedef union v3
{
	struct
	{
		f32 x, y, z;
	};
	struct
	{
		f32 u, v, n;
	};
	struct
	{
		f32 r, g, b;
	};
	struct
	{
		f32 pitch, yaw, roll;
	};
	struct
	{
		v2 xy;
		f32 ignored_0;
	};
	struct
	{
		f32 ignored_1;
		v2 yz;
	};
	struct
	{
		v2 uv;
		f32 ignored_2;
	};
	struct
	{
		f32 ignored_3;
		v2 vn;
	};
	f32 i[3];
} v3;

typedef union v3i
{
    struct
    {
        i32 x;
        i32 y;
        i32 z;
    };
    i32 E[3];
} v3i;

/* NOTE(kaede): It's written this style because you can't have multiple W's */
typedef union v4
{
    struct
    {
        union
        {
            v3 xyz;
            struct
            {
                f32 x, y, z;
            };
        };

        f32 w;
    };
    struct
    {
        union
        {
            v3 rgb;
            struct
            {
                f32 r, g, b;
            };
        };

        f32 a;
    };
    struct
    {
        v2 xy;
        f32 Ignored0_;
        f32 Ignored1_;
    };
    struct
    {
        f32 Ignored2_;
        v2 yz;
        f32 Ignored3_;
    };
    struct
    {
        f32 Ignored4_;
        f32 Ignored5_;
        v2 zw;
    };
    f32 i[4];
} v4, rgba, Quat;

v2
V2(f32 a, f32 b)
{
	v2 x;
	x.x = a;
	x.y = b;
	return(x);
}

v3
V3(f32 a, f32 b, f32 c)
{
	v3 x;
	x.x = a;
	x.y = b;
	x.z = c;
	return(x);
}

v4
V4(f32 a, f32 b, f32 c, f32 d)
{
	v4 x;
	x.x = a;
	x.y = b;
	x.z = c;
	x.w = d;
	return(x);
}

/* Defines */
/* V2 */
#define V2_ZERO V2(0, 0)
#define V2_ONE V2(1, 1)

#define V2_RIGHT V2(1, 0)
#define V2_LEFT V2(-1, 0)
#define V2_UP V2(0, 1)
#define V2_DOWN V2(0, -1)

/* V3 */
#define V3_ZERO V3(0, 0, 0)
#define V3_ONE V3(1, 1, 1)

#define V3_RIGHT V3(1, 0, 0)
#define V3_LEFT V3(-1, 0, 0)

#define V3_UP V3(0, 1, 0)
#define V3_DOWN V3(0, -1, 0)

#define V3_FRONT V3(0, 0,-1)
#define V3_BACK V3(0, 0, 1)

/* Arithmetics */
internal inline v2
v2_a(v2 a, v2 b)
{
	v2 x;
	x.x = a.x + b.x;
	x.y = a.y + b.y;
	return(x);
}

internal inline void
v2_ap(v2 *x, v2 *a, v2 *b)
{
	x->x = a->x + b->x;
	x->y = a->y + b->y;
}

internal inline v2
v2_af(v2 a, f32 b)
{
	v2 x;
	x.x = a.x + b;
	x.y = a.y + b;
	return(x);
}

/* Arithmetics */
internal inline v2
v2_s(v2 a, v2 b)
{
	v2 x;
	x.x = a.x - b.x;
	x.y = a.y - b.y;
	return(x);
}

internal inline v2
v2_mf(v2 a, f32 b)
{
	v2 x;
	x.x = a.x * b;
	x.y = a.y * b;

	return(x);
}

internal inline void
v2_print(v2 a, char *s)
{
	printf("%s x: %12f, y: %12f\n", s, a.x, a.y);
}

internal inline bool
v2_eql(v2 a, v2 b)
{
	bool result = true;
	if(a.x != b.x || a.y != b.y)
	{
		result = false;
	}
	return(result) ;
}


/* Arithmetics */
internal inline v3
v3_a(v3 a, v3 b)
{
	v3 x;

	x.x = a.x + b.x;
	x.y = a.y + b.y;
	x.z = a.z + b.z;

	return(x);
}

internal inline v3
v3_af(v3 a, f32 b)
{
	v3 x;

	x.x = a.x + b;
	x.y = a.y + b;
	x.z = a.z + b;

	return(x);
}

/* NOTE(lungu): v3 Functions */
internal inline v3
v3_s(v3 a, v3 b)
{
	v3 result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;

	return(result);
}

internal inline void
v3p_s(v3 *result, v3 *a, v3 *b)
{
	result->x = a->x - b->x;
	result->y = a->y - b->y;
	result->z = a->z - b->z;
}

internal inline v3
v3_mf(v3 a, f32 b)
{
	v3 x;

	x.x = a.x * b;
	x.y = a.y * b;
	x.z = a.z * b;

	return(x);
}

internal inline v3
v3_df(v3 a, f32 b)
{
	v3 x;

	x.x = a.x / b;
	x.y = a.y / b;
	x.z = a.z / b;

	return(x);
}

internal inline v3
v3_inv(v3 a)
{
	v3 x;

	x.x = -a.x;
	x.y = -a.y;
	x.z = -a.z;

	return(x);
}

internal inline bool
v3_eql(v3 a, v3 b)
{
	bool result = true;
	if(a.x != b.x || a.y != b.y || a.z != b.z)
	{
		result = false;
	}
	return(result) ;
}

internal inline f32
v3_len(v3 a)
{
	f32 result = result = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
	return(result);
}

internal inline v3
v3_norm(v3 a)
{
	v3 result;
	f32 len = v3_len(a);

	//if(len < EPSILON_E5) return;

	f32 len_inv = f32_d(1, len);
	result.x = a.x * len_inv;
	result.y = a.y * len_inv;
	result.z = a.z * len_inv;

	return(result);
}

internal inline void
v3p_norm(v3 *result, v3 *a)
{
	f32 len = v3_len(*a);

	//if(len < EPSILON_E5) return;

	f32 len_inv = 1 / len;
	result->x = a->x * len_inv;
	result->y = a->y * len_inv;
	result->z = a->z * len_inv;
}

internal inline v3
v3_cross(v3 a, v3 b)
{
	v3 result;

	result.x = ((a.y * b.z) - (a.z * b.y));
	result.y = -((a.x * b.z) - (a.z * b.x));
	result.z = ((a.x * b.y) - (a.y * b.x));

	return(result);
}

internal inline void
v3p_cross(v3 *result, v3 *a, v3 *b)
{
	result->x = ((a->y * b->z) - (a->z * b->y));
	result->y = -((a->x * b->z) - (a->z * b->x));
	result->z = ((a->x * b->y) - (a->y * b->x));
}

internal inline v3
v3_lerp(v3 a, v3 b, f32 t)
{
	/* (1 - t)*a + t*b */
	v3 result = v3_a(v3_mf(a, (1.0f - t)), v3_mf(b, t));
	return(result);
}

internal inline void
v3_print(v3 a, char *s)
{
	printf("%s x: %12f, y: %12f, z: %12f\n", s, a.x, a.y, a.z);
}

internal inline v3
v3_angle_norm(v3 a)
{
	v3 result = a;
	for(i32 i = 0; i < 3; ++i)
	{
		//if(result.i[i] < 0) result.i[i] += 360;
		result.i[i] = f32_mod(result.i[i], 360);
	}

	return(result);
}

internal inline v4
v4_a(v4 a, v4 b)
{
	v4 x;
	x.x = a.x + b.x;
	x.y = a.y + b.y;
	x.z = a.z + b.z;
	x.w = a.w;

	return(x);
}

//F32
internal inline v4
v4_mf(v4 a, f32 b)
{
	v4 x;
	x.x = a.x * b;
	x.y = a.y * b;
	x.z = a.z * b;
	x.w = a.w;

	return(x);
}

internal inline v4
v4_inv(v4 a)
{
	v4 result;

	result.x = -a.x;
	result.y = -a.y;
	result.z = -a.z;
	result.w = -a.w;

	return(result);
}

internal inline f32
v4_dot(v4 a, v4 b)
{
	f32 result = a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
	return(result);
}

internal inline v4
v4_lerp(v4 a, v4 b, f32 t)
{
	/* (1 - t)*a + t*b */
	v4 result = v4_a(v4_mf(a, (1.0f - t)), v4_mf(b, t));
	return(result);
}

/* Conversion Functions */
internal inline v4
v4_from_v3(v3 a, f32 b)
{
	v4 result;

	result.xyz = a;
	result.w = b;

	return(result);
}

internal inline void
v4_print(v4 a, char *s)
{
	printf("%s x: %12f, y: %12f, z: %12f, w: %12f\n", s, a.x, a.y, a.z, a.w);
}
