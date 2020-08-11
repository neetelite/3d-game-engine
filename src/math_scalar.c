#define PI 3.14159265358979323846264338327950288

internal inline u32
u32_pow(u32 a, u32 p)
{
	u32 result = 0;
	if(p == 0) return(1);
	//if(p < 0) ERROR("Not implemented yet");

	result = a;
	for(i32 i = 1; i < p; ++i)
	{
		result *= a;
	}
	return(result);
}

internal inline f32
f32_pow(i32 a, i32 p)
{
	f32 result = 0;

	result = pow(a, p);

	return(result);
}

internal inline f32
f32_d(f32 a, f32 b)
{
	if(b == 0.0f) return(0);

	f32 result = a / b;
	return(result);
}

internal inline f32
f32_cos(f32 a)
{
	f32 result = cosf(a);
	return(result);
}

internal inline f32
f32_sin(f32 a)
{
	f32 result = sinf(a);
	return(result);
}

internal inline f32
f32_rad_from_deg(f32 a)
{
	f32 half_circle = PI / 180.0;
	f32 result = a * half_circle;
	return(result);
}

internal inline f32
f32_mod(f32 a, f32 b)
{
	f32 result = fmod(a, b);
	return(result);
}

internal inline f32
f32_sq(f32 a)
{
	f32 result = a * a;
	return(result);
}

internal inline f32
f32_sqrt(f32 a)
{
	f32 result = sqrt(a);
	return(result);
}

internal inline f32
f32_lerp(f32 a, f32 b, f32 t)
{
	f32 result = 0;
	result = (1-t)*a + t*b;
	return(result);
}

internal inline f32
f32_abs(f32 a)
{
	f32 result = a;
	if(a < 0) result = -a;
	return(result);
}
