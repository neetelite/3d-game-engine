//definition
typedef struct Matrice3x3
{
	union
	{
		f32 m[3][3];
		f32 i[9];
		struct
		{
			f32 m00, m01, m02;
			f32 m10, m11, m12;
			f32 m20, m21, m22;
		};
	};
} m3x3, mat3;

typedef struct Matrice4x4
{
	union
	{
		f32 m[4][4];
		f32 i[16];
		struct
		{
			f32 m00, m01, m02, m03;
			f32 m10, m11, m12, m13;
			f32 m20, m21, m22, m23;
			f32 m30, m31, m32, m33;
		};
	};
} m4x4, mat4;

internal inline mat3
MAT3(f32 m00,  f32 m01,  f32 m02,
     f32 m10,  f32 m11,  f32 m12,
     f32 m20,  f32 m21,  f32 m22)
{
	//NOTE(kaede): When initializing you need to use brakets for every
	//struct and union that's wrapping the array. Tested in G++ too.
	mat3 result = (mat3){m00, m01, m02,
			     m10, m11, m12,
			     m20, m21, m22};
	return(result);
}

internal inline mat4
MAT4(f32 m00,  f32 m01,  f32 m02,  f32 m03,
     f32 m10,  f32 m11,  f32 m12,  f32 m13,
     f32 m20,  f32 m21,  f32 m22,  f32 m23,
     f32 m30,  f32 m31,  f32 m32,  f32 m33)
{
	//NOTE(kaede): When initializing you need to use brakets for every
	//struct and union that's wrapping the array. Tested in G++ too.
	mat4 result = (mat4){m00, m01, m02, m03,
			     m10, m11, m12, m13,
			     m20, m21, m22, m23,
			     m30, m31, m32, m33};
	return(result);
}

#define MAT3_ID MAT3(1,0,0, 0,1,0, 0,0,1)
#define MAT3_ZERO MAT3(0,0,0, 0,0,0, 0,0,0)

#define MAT4_ID MAT4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1)
#define MAT4_ZERO MAT4(0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0)

internal inline m4x4
m4x4_m(m4x4 a, m4x4 b)
{
	m4x4 result;
	for(i32 row = 0; row < 4; row++)
	{
		for(int col = 0; col < 4; col++)
		{
			f32 sum = 0;
			for(i32 i = 0; i < 4; i++)
			{
				sum += (a.m[row][i] * b.m[i][col]);
			}
			result.m[row][col] = sum;
		}
	}

	return(result);
}

internal inline void
m4x4p_m(m4x4 *r, m4x4 *a, m4x4 *b)
{
	for(i32 row = 0; row < 4; row++)
	{
		for(int col = 0; col < 4; col++)
		{
			f32 sum = 0;
			for(i32 i = 0; i < 4; i++)
			{
				sum += (a->m[row][i] * b->m[i][col]);
			}
			r->m[row][col] = sum;
		}
	}
}

internal inline f32
mat3_det(mat3 a)
{
	f32 result = a.m00 * (a.m11 * a.m22 - a.m12 * a.m21) +
		a.m01 * (a.m12 * a.m20 - a.m10 * a.m22) +
		a.m02 * (a.m10 * a.m21 - a.m11 * a.m20);

	return(result);
}

internal inline f32
mat4_det(mat4 a)
{
	float f = a.m00 * ((a.m11 * a.m22 * a.m33 + a.m12 * a.m23 * a.m31 + a.m13 * a.m21 * a.m32)
		   - a.m13 * a.m22 * a.m31 - a.m11 * a.m23 * a.m32 - a.m12 * a.m21 * a.m33);

	f -= a.m01 * ((a.m10 * a.m22 * a.m33 + a.m12 * a.m23 * a.m30 + a.m13 * a.m20 * a.m32)
		   - a.m13 * a.m22 * a.m30 - a.m10 * a.m23 * a.m32 - a.m12 * a.m20 * a.m33);

	f += a.m02 * ((a.m10 * a.m21 * a.m33 + a.m11 * a.m23 * a.m30 + a.m13 * a.m20 * a.m31)
		   - a.m13 * a.m21 * a.m30 - a.m10 * a.m23 * a.m31 - a.m11 * a.m20 * a.m33);

	f -= a.m03 * ((a.m10 * a.m21 * a.m32 + a.m11 * a.m22 * a.m30 + a.m12 * a.m20 * a.m31)
		   - a.m12 * a.m21 * a.m30 - a.m10 * a.m22 * a.m31 - a.m11 * a.m20 * a.m32);
	return f;
}

internal inline m4x4
mat4_inv(m4x4 a)
{
	mat4 result = MAT4_ZERO;

	f32 det = mat4_det(a);
	if(!det) return(result);

	f32 det_inv = 1.0f / det;

	//f32 t00 = mat3_det(MAT3(a.m11));

	// first row
	f32 t00 =  mat3_det(MAT3(a.m11, a.m12, a.m13, a.m21, a.m22, a.m23, a.m31, a.m32, a.m33));
	f32 t01 = -mat3_det(MAT3(a.m10, a.m12, a.m13, a.m20, a.m22, a.m23, a.m30, a.m32, a.m33));
	f32 t02 =  mat3_det(MAT3(a.m10, a.m11, a.m13, a.m20, a.m21, a.m23, a.m30, a.m31, a.m33));
	f32 t03 = -mat3_det(MAT3(a.m10, a.m11, a.m12, a.m20, a.m21, a.m22, a.m30, a.m31, a.m32));

	// second row
	f32 t10 = -mat3_det(MAT3(a.m01, a.m02, a.m03, a.m21, a.m22, a.m23, a.m31, a.m32, a.m33));
	f32 t11 =  mat3_det(MAT3(a.m00, a.m02, a.m03, a.m20, a.m22, a.m23, a.m30, a.m32, a.m33));
	f32 t12 = -mat3_det(MAT3(a.m00, a.m01, a.m03, a.m20, a.m21, a.m23, a.m30, a.m31, a.m33));
	f32 t13 =  mat3_det(MAT3(a.m00, a.m01, a.m02, a.m20, a.m21, a.m22, a.m30, a.m31, a.m32));

	// third row
	f32 t20 =  mat3_det(MAT3(a.m01, a.m02, a.m03, a.m11, a.m12, a.m13, a.m31, a.m32, a.m33));
	f32 t21 = -mat3_det(MAT3(a.m00, a.m02, a.m03, a.m10, a.m12, a.m13, a.m30, a.m32, a.m33));
	f32 t22 =  mat3_det(MAT3(a.m00, a.m01, a.m03, a.m10, a.m11, a.m13, a.m30, a.m31, a.m33));
	f32 t23 = -mat3_det(MAT3(a.m00, a.m01, a.m02, a.m10, a.m11, a.m12, a.m30, a.m31, a.m32));

	// fourth row
	f32 t30 = -mat3_det(MAT3(a.m01, a.m02, a.m03, a.m11, a.m12, a.m13, a.m21, a.m22, a.m23));
	f32 t31 =  mat3_det(MAT3(a.m00, a.m02, a.m03, a.m10, a.m12, a.m13, a.m20, a.m22, a.m23));
	f32 t32 = -mat3_det(MAT3(a.m00, a.m01, a.m03, a.m10, a.m11, a.m13, a.m20, a.m21, a.m23));
	f32 t33 =  mat3_det(MAT3(a.m00, a.m01, a.m02, a.m10, a.m11, a.m12, a.m20, a.m21, a.m22));

	result.m00 = t00*det_inv;
	result.m11 = t11*det_inv;
	result.m22 = t22*det_inv;
	result.m33 = t33*det_inv;
	result.m01 = t10*det_inv;
	result.m10 = t01*det_inv;
	result.m20 = t02*det_inv;
	result.m02 = t20*det_inv;
	result.m12 = t21*det_inv;
	result.m21 = t12*det_inv;
	result.m03 = t30*det_inv;
	result.m30 = t03*det_inv;
	result.m13 = t31*det_inv;
	result.m31 = t13*det_inv;
	result.m32 = t23*det_inv;
	result.m23 = t32*det_inv;

	return(result);
}


internal inline void
mat4_print(mat4 a)
{
	for(i32 i = 0; i < 4; ++i)
	{
		for(i32 j = 0; j < 4; ++j)
		{
			printf("%-12f ", a.m[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

internal inline m4x4
m4x4_euler_rot_x(f32 a)
{
	a = f32_rad_from_deg(a);

	f32 c = f32_cos(a);
	f32 s = f32_sin(a);

	m4x4 result = MAT4
		(
			1, 0, 0,  0,
			0, c, -s, 0,
			0, s, c,  0,
			0, 0, 0,  1
		);

	return(result);
}

internal inline m4x4
m4x4_euler_rot_y(f32 a)
{
	a = f32_rad_from_deg(a);

	f32 c = f32_cos(a);
	f32 s = f32_sin(a);

	m4x4 result = MAT4
		(
			c,  0, s,  0,
			0,  1, 0,  0,
			-s, 0, c,  0,
			0,  0, 0,  1
		);

	return(result);
}

internal inline m4x4
m4x4_euler_rot_z(f32 a)
{
	a = f32_rad_from_deg(a);

	f32 c = f32_cos(a);
	f32 s = f32_sin(a);

	m4x4 result = MAT4
		(
			c, -s, 0,  0,
			s, c,  0,  0,
			0, 0,  1,  0,
			0, 0,  0,  1
		);

	return(result);
}

internal inline m4x4
m4x4_euler_rot(f32 pitch, f32 yaw, f32 roll)
{
	m4x4 result = MAT4_ID;

	result = m4x4_m(result, m4x4_euler_rot_x(pitch));
	result = m4x4_m(result, m4x4_euler_rot_y(yaw));
	result = m4x4_m(result, m4x4_euler_rot_z(roll));

	return(result);
}

internal inline m4x4
m4x4_scale(f32 x, f32 y, f32 z)
{
	m4x4 result = MAT4_ID;

	result.m[0][0] = x;
	result.m[1][1] = y;
	result.m[2][2] = z;

	return(result);
}

internal inline m4x4
m4x4_trans(f32 x, f32 y, f32 z)
{
	m4x4 result = MAT4_ID;

	result.m[3][0] = x;
	result.m[3][1] = y;
	result.m[3][2] = z;

	return(result);
}

/* NOTE(lungu): Flipped version, much better */
internal inline m4x4
mat4_m(m4x4 b, m4x4 a)
{
	m4x4 result;
	for(i32 row = 0; row < 4; row++)
	{
		for(int col = 0; col < 4; col++)
		{
			f32 sum = 0;
			for(i32 i = 0; i < 4; i++)
			{
				sum += (a.m[row][i] * b.m[i][col]);
			}
			result.m[row][col] = sum;
		}
	}

	return(result);
}

internal inline mat3
mat3_from_mat4(mat4 a)
{
	mat3 result = MAT3_ZERO;

	for(i32 row = 0; row < 3; ++row)
	{
		for(i32 col = 0; col < 3; ++col)
		{
			result.m[row][col] = a.m[row][col];
		}
	}

	return(result);
}

internal inline bool
mat4_eql(mat4 a, mat4 b)
{
	/* FIXME LEARN(lungu): Slight float variations make it not work */
	bool result = true;
	for(i32 row = 0; row < 3; ++row)
	{
		for(i32 col = 0; col < 3; ++col)
		{
			if(a.m[row][col] != b.m[row][col]) result = false;
		}
	}
	return(result);
}

internal inline mat4
mat4_transpose(mat4 a)
{
	mat4 result = MAT4_ZERO;
	for(i32 i = 0; i < 4; ++i)
	{
		for(i32 j = 0; j < 4; ++j)
		{
			result.m[i][j] = a.m[j][i];
		}
	}
	return(result);
}
