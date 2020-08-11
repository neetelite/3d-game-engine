internal inline mat4
mat4_trans_v3(mat4 mat, v3 trans)
{
	mat4 result = mat;

	/* NOTE(lungu): matrix[row][col] */
	result.m[0][3] += trans.x;
	result.m[1][3] += trans.y;
	result.m[2][3] += trans.z;

	return(result);
}

inline v4
v4_m_m4x4(v4 a, m4x4 b)
{
	v4 result;

	result.x = a.x*b.m[0][0] + a.y*b.m[0][1] + a.z*b.m[0][2] + a.w*b.m[0][3];
	result.y = a.x*b.m[1][0] + a.y*b.m[1][1] + a.z*b.m[1][2] + a.w*b.m[1][3];
	result.z = a.x*b.m[2][0] + a.y*b.m[2][1] + a.z*b.m[2][2] + a.w*b.m[2][3];
	result.w = a.x*b.m[3][0] + a.y*b.m[3][1] + a.z*b.m[3][2] + a.w*b.m[3][3];

	return(result);
}

inline v3
v3_m_m4x4(v3 a, m4x4 b)
{
	v3 result;
	v4 vec = V4(a.x, a.y, a.z, 1);

	result = v4_m_m4x4(vec, b).xyz;

	return(result);
}

mat4
mat4_from_quat(Quat quat)
{
	mat4 result = {0};

	f32 xy = quat.x * quat.y;
	f32 xz = quat.x * quat.z;
	f32 xw = quat.x * quat.w;

	f32 yz = quat.y * quat.z;
	f32 yw = quat.y * quat.w;

	f32 zw = quat.z * quat.w;

	f32 xsq = f32_sq(quat.x);
	f32 ysq = f32_sq(quat.y);
	f32 zsq = f32_sq(quat.z);

	result.m00 = 1 - 2 * (ysq + zsq);
	result.m01 = 2 * (xy - zw);
	result.m02 = 2 * (xz + yw);
	result.m03 = 0;
	result.m10 = 2 * (xy + zw);
	result.m11 = 1 - 2 * (xsq + zsq);
	result.m12 = 2 * (yz - xw);
	result.m13 = 0;
	result.m20 = 2 * (xz - yw);
	result.m21 = 2 * (yz + xw);
	result.m22 = 1 - 2 * (xsq + ysq);
	result.m23 = 0;
	result.m30 = 0;
	result.m31 = 0;
	result.m32 = 0;
	result.m33 = 1;

	return(result);
}

Quat
quat_from_mat4(mat4 matrix)
{
	Quat result = {0};

	f32 w, x, y, z;
	f32 diagonal = matrix.m00 + matrix.m11 + matrix.m22;

	if(diagonal > 0)
	{
		f32 w4 = f32_sqrt(diagonal + 1.0f) * 2.0f;
		w = w4 / 4.0f;
		x = (matrix.m21 - matrix.m12) / w4;
		y = (matrix.m02 - matrix.m20) / w4;
		z = (matrix.m10 - matrix.m01) / w4;
	}
	else if((matrix.m00 > matrix.m11) && (matrix.m00 > matrix.m22))
	{
		f32 x4 = f32_sqrt(1.0f + matrix.m00 - matrix.m11 - matrix.m22) * 2.0f;
		w = (matrix.m21 - matrix.m12) / x4;
		x = x4 / 4.0f;
		y = (matrix.m01 + matrix.m10) / x4;
		z = (matrix.m02 + matrix.m20) / x4;
	}
	else if(matrix.m11 > matrix.m22)
	{
		f32 y4 = f32_sqrt(1.0f + matrix.m11 - matrix.m00 - matrix.m22) * 2.0f;
		w = (matrix.m02 - matrix.m20) / y4;
		x = (matrix.m01 + matrix.m10) / y4;
		y = y4 / 4.0f;
		z = (matrix.m12 + matrix.m21) / y4;
	}
	else
	{
		f32 z4 = f32_sqrt(1.0f + matrix.m22 - matrix.m00 - matrix.m11) * 2.0f;
		w = (matrix.m10 - matrix.m01) / z4;
		x = (matrix.m02 + matrix.m20) / z4;
		y = (matrix.m12 + matrix.m21) / z4;
		z = z4 / 4.0f;
	}

	result.x = x;
	result.y = y;
	result.z = z;
	result.w = w;

	return(result);
}

internal inline bool
char_is_num(char c)
{
	bool result = false;
	if('0' <= c && c <= '9') result = true;
	return(result);
}

inline bool
char_is_alph(char c)
{
	bool result = false;
	if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) result = true;
	return(result);
}

u32
u32_from_char(char c)
{
	u32 result = 0;
	result = (u32)(c - '0');
	return(result);
}

/* Conversion */
u32
u32_from_cstr(char *cstr)
{
	u32 result = 0;

	u32 length = 0;
	char *start = char_whitespace_skip(cstr);
	char *at = start;
	while(char_is_num(*at))
	{
		++length;
		++at;
	}

	at = start;
	u32 multiplier = length - 1;
	for(i32 i = 0; i < length; ++i)
	{
		char c = *at;
		u32 n = u32_from_char(c);
		//u32 s = n * u32_pow(10, multiplier); /* FIXME(lungu): */
		u32 s = n * f32_pow(10, multiplier);

		result += s;
		--multiplier;
		++at;
	}

	return(result);
}

f32
f32_from_cstr(char *cstring)
{
	/* IMPLEMENT(lungu): Exponentials */
	/* LEARN(lungu): float from string implementation */
	f32 result = 0.0f;
	char *start = char_whitespace_skip(cstring);
	#if 0
	u32 integer = u32_from_cstr(start);

	u32 len = char_find_whitespace(start) - start;
	char *at = start;
	u32 period = 0;
	for(i32 i = 0; i < len; ++i)
	{
		if(*at == '.') break;

		++period;
		++at;
	}

	at = start + (period + 1);
	len = char_find_whitespace(at) - at;
	u32 floating = u32_from_cstr(at);
	f64 p = f32_pow(10, -(len - 1));
	f32 flt = floating * p;

	result = integer + floating;
	#else
	sscanf(start, "%f", &result);
	#endif
	return(result);
}

v4
mat4_m_v4(mat4 mat, v4 vec)
{
	v4 result = V4(0, 0, 0, 0);

	for(i32 row = 0; row < 4; ++row)
	{
		for(i32 col = 0; col < 4; ++col)
		{
			result.i[row] += vec.i[col] * mat.m[row][col];
		}
	}

	return(result);
}

v3
mat4_m_v3(mat4 mat, v3 vec)
{
	v4 result = V4(vec.x, vec.y, vec.z, 0);
	result = mat4_m_v4(mat, result);
	return(result.xyz);
}

v3
v3_rot(v3 a, v3 b)
{
	v3 result = V3_ZERO;
	mat4 rotation_matrix = m4x4_euler_rot(b.x, b.y, b.z);

	result = mat4_m_v3(rotation_matrix, a);
	return(result);
}
