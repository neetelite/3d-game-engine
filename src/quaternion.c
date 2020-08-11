#define QUAT(a, b, c, d) V4(a, b, c, d)

Quat
quat_norm(Quat a)
{
	Quat result = {0};

	f32 magnitude = f32_sqrt(f32_sq(a.x) + f32_sq(a.y) + f32_sq(a.z) + f32_sq(a.w));

	result.x = a.x / magnitude;
	result.y = a.y / magnitude;
	result.z = a.z / magnitude;
	result.w = a.w / magnitude;

	return(result);
}

Quat
quat_lerp(Quat a, Quat b, f32 t)
{
	Quat result = QUAT(0, 0, 0, 1);

	/* TODO LEARN: Why do we do this check here? */
	if(v4_dot(a, b) < 0.0f) result = v4_lerp(a, v4_inv(b), t);
	else result = v4_lerp(a, b, t);

	result = quat_norm(result);
	return(result);
}
