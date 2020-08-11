/* LEARN(lungu): Maybe let shader be a pointer so when it returns null we know it failed */
GLuint
shader_load(char *shader_path, GLenum shader_type)
{
	GLuint result = 0;

	FILE *file = fopen(shader_path, "rb");
	if(!file)
	{
		ERROR("Cannot open file for reading! %s\n", shader_path);
		return(-1);
	}

	u32 size = 0;
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);


	char *shader_source = mem_alloc(1, size+1, false);
	fread(shader_source, size, 1, file);
	shader_source[size] = '\0';
	fclose(file);

	result = glCreateShader(shader_type);
	glShaderSource(result, 1, (char **)&shader_source, NULL);
	glCompileShader(result);

	i32 success;
	char info_log[512];
	glGetShaderiv(result, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(result, 512, NULL, info_log);
		ERROR("Failed to compile shader (%s): %s\n", shader_path, info_log);
		return(-1);
	}

	mem_free(shader_source);

	return(result);
}
