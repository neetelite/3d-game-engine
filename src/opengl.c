/* OpenGL Defines */
#define GL_NUM_EXTENSIONS				 0x821D

#define GL_printf_SEVERITY_HIGH			0x9146
#define GL_printf_SEVERITY_MEDIUM		  0x9147
#define GL_printf_SEVERITY_LOW			 0x9148
#define GL_printf_TYPE_MARKER			  0x8268
#define GL_printf_TYPE_PUSH_GROUP		  0x8269
#define GL_printf_TYPE_POP_GROUP		   0x826A
#define GL_printf_SEVERITY_NOTIFICATION	0x826B

#define GL_MIRRORED_REPEAT                0x8370
#define GL_BGRA                           0x80E1
#define GL_RGB                            0x1907
#define GL_RGBA                           0x1908
#define GL_TEXTURE0                       0x84C0

#define GL_printf_OUTPUT_SYNCHRONOUS	   0x8242
#define GL_ARRAY_BUFFER				   0x8892
#define GL_STREAM_DRAW					0x88E0
#define GL_STREAM_READ					0x88E1
#define GL_STREAM_COPY					0x88E2
#define GL_STATIC_DRAW					0x88E4
#define GL_STATIC_READ					0x88E5
#define GL_STATIC_COPY					0x88E6
#define GL_DYNAMIC_DRAW				   0x88E8
#define GL_DYNAMIC_READ				   0x88E9
#define GL_DYNAMIC_COPY				   0x88EA

#define GL_CLAMP_TO_EDGE				  0x812F

#define GL_FRAMEBUFFER_SRGB			   0x8DB9
#define GL_SRGB8_ALPHA8				   0x8C43

#define GL_SHADING_LANGUAGE_VERSION	   0x8B8C
#define GL_FRAGMENT_SHADER				0x8B30
#define GL_VERTEX_SHADER				  0x8B31
#define GL_COMPILE_STATUS				 0x8B81
#define GL_LINK_STATUS					0x8B82
#define GL_VALIDATE_STATUS				0x8B83

#define GL_FRAMEBUFFER					0x8D40
#define GL_READ_FRAMEBUFFER			   0x8CA8
#define GL_DRAW_FRAMEBUFFER			   0x8CA9
#define GL_COLOR_ATTACHMENT0			  0x8CE0
#define GL_DEPTH_ATTACHMENT			   0x8D00
#define GL_FRAMEBUFFER_COMPLETE		   0x8CD5

#define GL_DEPTH_COMPONENT16			  0x81A5
#define GL_DEPTH_COMPONENT24			  0x81A6
#define GL_DEPTH_COMPONENT32			  0x81A7
#define GL_DEPTH_COMPONENT32F			 0x8CAC

#define GL_MULTISAMPLE					0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE	   0x809E
#define GL_SAMPLE_ALPHA_TO_ONE			0x809F
#define GL_SAMPLE_COVERAGE				0x80A0
#define GL_SAMPLE_BUFFERS				 0x80A8
#define GL_SAMPLES						0x80A9
#define GL_SAMPLE_COVERAGE_VALUE		  0x80AA
#define GL_SAMPLE_COVERAGE_INVERT		 0x80AB
#define GL_TEXTURE_2D_MULTISAMPLE		 0x9100
#define GL_MAX_SAMPLES					0x8D57
#define GL_MAX_COLOR_TEXTURE_SAMPLES	  0x910E
#define GL_MAX_DEPTH_TEXTURE_SAMPLES	  0x910F

/* OpenGL Types */
typedef char GLchar;
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;

#if OS_WINDOWS
#define API WINAPI
#else
/* FIXME LEARN(lungu): This turns the return type into pointers. */
//#define API APIENTRYP
#define API
#endif

/* OpenGL Functions Types */
typedef void API FN_glTexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat,
						   GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void API FN_glBindFramebuffer(GLenum target, GLuint framebuffer);
typedef void API FN_glGenFramebuffers(GLsizei n, GLuint *framebuffers);
typedef void API FN_glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget,
						  GLuint texture, GLint level);
typedef GLenum API FN_glCheckFramebufferStatus(GLenum target);
typedef void API FN_glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
					 GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1,
					 GLbitfield mask, GLenum filter);
typedef void API FN_glAttachShader(GLuint program, GLuint shader);
typedef void API FN_glCompileShader(GLuint shader);
typedef GLuint API FN_glCreateProgram(void);
typedef GLuint API FN_glCreateShader(GLenum type);
typedef void API FN_glLinkProgram(GLuint program);
typedef void API FN_glShaderSource(GLuint shader, GLsizei count, GLchar **string, GLint *length);
typedef void API FN_glUseProgram(GLuint program);
typedef void API FN_glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void API FN_glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void API FN_glValidateProgram(GLuint program);
typedef void API FN_glGetProgramiv(GLuint program, GLenum pname, GLint *params);
typedef GLint API FN_glGetUniformLocation (GLuint program, const GLchar *name);
typedef void API FN_glUniform4fv(GLint location, GLsizei count, const GLfloat *value);
typedef void API FN_glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void API FN_glUniform1i(GLint location, GLint v0);
typedef const GLubyte * API FN_glGetStringi(GLenum name, GLuint index);
typedef void API FN_glUniform1f(GLint location, GLfloat v0);
typedef void API FN_glUniform2fv(GLint location, GLsizei count, const GLfloat *value);
typedef void API FN_glUniform3fv(GLint location, GLsizei count, const GLfloat *value);
typedef void API FN_glEnableVertexAttribArray(GLuint index);
typedef void API FN_glDisableVertexAttribArray(GLuint index);
typedef GLint API FN_glGetAttribLocation(GLuint program, const GLchar *name);
typedef void API FN_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void API FN_glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
typedef void API FN_glBindVertexArray(GLuint array);
typedef void API FN_glGenVertexArrays(GLsizei n, GLuint *arrays);
typedef void API FN_glBindBuffer(GLenum target, GLuint buffer);
typedef void API FN_glGenBuffers(GLsizei n, GLuint *buffers);
typedef void API FN_glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void API FN_glGetShaderiv(GLuint shader, GLenum pname, GLint *params);
typedef void API FN_glDeleteShader(GLuint shader);
typedef void API FN_glGenerateMipmap(GLenum target);
typedef void API FN_glActiveTexture (GLenum texture);

/* OpenGL Global Functions */
#define OPENGL_GLOBAL_FUNCTION(name) FN_##name *name
OPENGL_GLOBAL_FUNCTION(glTexImage2DMultisample);
OPENGL_GLOBAL_FUNCTION(glBindFramebuffer);
OPENGL_GLOBAL_FUNCTION(glGenFramebuffers);
OPENGL_GLOBAL_FUNCTION(glFramebufferTexture2D);
OPENGL_GLOBAL_FUNCTION(glCheckFramebufferStatus);
OPENGL_GLOBAL_FUNCTION(glBlitFramebuffer);
OPENGL_GLOBAL_FUNCTION(glAttachShader);
OPENGL_GLOBAL_FUNCTION(glCompileShader);
OPENGL_GLOBAL_FUNCTION(glCreateProgram);
OPENGL_GLOBAL_FUNCTION(glCreateShader);
OPENGL_GLOBAL_FUNCTION(glLinkProgram);
OPENGL_GLOBAL_FUNCTION(glShaderSource);
OPENGL_GLOBAL_FUNCTION(glUseProgram);
OPENGL_GLOBAL_FUNCTION(glGetProgramInfoLog);
OPENGL_GLOBAL_FUNCTION(glGetShaderInfoLog);
OPENGL_GLOBAL_FUNCTION(glValidateProgram);
OPENGL_GLOBAL_FUNCTION(glGetProgramiv);
OPENGL_GLOBAL_FUNCTION(glGetUniformLocation);
OPENGL_GLOBAL_FUNCTION(glUniform4fv);
OPENGL_GLOBAL_FUNCTION(glUniformMatrix4fv);
OPENGL_GLOBAL_FUNCTION(glUniform1i);
OPENGL_GLOBAL_FUNCTION(glGetStringi);
OPENGL_GLOBAL_FUNCTION(glUniform1f);
OPENGL_GLOBAL_FUNCTION(glUniform2fv);
OPENGL_GLOBAL_FUNCTION(glUniform3fv);
OPENGL_GLOBAL_FUNCTION(glEnableVertexAttribArray);
OPENGL_GLOBAL_FUNCTION(glDisableVertexAttribArray);
OPENGL_GLOBAL_FUNCTION(glGetAttribLocation);
OPENGL_GLOBAL_FUNCTION(glVertexAttribPointer);
OPENGL_GLOBAL_FUNCTION(glVertexAttribIPointer);
OPENGL_GLOBAL_FUNCTION(glBindVertexArray);
OPENGL_GLOBAL_FUNCTION(glGenVertexArrays);
OPENGL_GLOBAL_FUNCTION(glBindBuffer);
OPENGL_GLOBAL_FUNCTION(glGenBuffers);
OPENGL_GLOBAL_FUNCTION(glBufferData);
//OPENGL_GLOBAL_FUNCTION(glprintfMessageCallbackARB);
OPENGL_GLOBAL_FUNCTION(glGetShaderiv);
OPENGL_GLOBAL_FUNCTION(glDeleteShader);
OPENGL_GLOBAL_FUNCTION(glGenerateMipmap);
//OPENGL_GLOBAL_FUNCTION(glActiveTexture); /* FIXME(lungu): Is this already defined somewhere? */

struct OpenGLInfo
{
	bool modern_context;

	char *vendor;
	char *renderer;
	char *version;
	char *shading_language_version;
	char *extensions;

	//bool gl_ext_texture_srgb;
	bool gl_ext_texture_srgb;
	bool gl_ext_framebuffer_srgb;
	bool gl_arb_framebuffer_object;
};

struct OpenGL
{
	//b32 sRGBSupport;
	bool supports_srgb_framebuffer;

	#if 0
	GLint MaxMultiSampleCount;
	GLuint DefaultSpriteTextureFormat;
	GLuint DefaultFramebufferTextureFormat;

	GLuint VertexBuffer;

	GLuint ReservedBlitTexture;
	GLuint BasicZBiasProg;

	GLuint TransformID;
	GLuint TextureSamplerID;
	GLuint CameraP;
	GLuint FogDirection;
	GLuint FogColor;
	GLuint FogStartDistance;
	GLuint FogEndDistance;

	GLuint VertPID;
	GLuint VertUVID;
	GLuint VertColorID;
	#endif

	//loaded_bitmap WhiteBitmap;
	//u32 White[4][4];
};

/* OpenGL Structs */
struct OpenGLInfo
os_opengl_info(bool modern_context)
{
	struct OpenGLInfo result = {0};

	result.modern_context = modern_context;
	result.vendor = (char *)glGetString(GL_VENDOR);
	result.renderer = (char *)glGetString(GL_RENDERER);
	result.version = (char *)glGetString(GL_VERSION);

	printf("%s %s %s\n", result.vendor, result.renderer, result.version);

	if(result.modern_context)
	{
		result.shading_language_version = (char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
	}
	else
	{
		result.shading_language_version = "(none)";
	}

	if(glGetStringi)
	{
		GLint extension_count = 0;
		glGetIntegerv(GL_NUM_EXTENSIONS, &extension_count);

		for(GLint i = 0; i < extension_count; ++i)
		{
			char *extension_name = (char *)glGetStringi(GL_EXTENSIONS, i);

			if(0) {}
			else if(cstr_eql(extension_name, "gl_ext_texture_srgb"))
				result.gl_ext_texture_srgb = true;
			else if(cstr_eql(extension_name, "gl_ext_framebuffer_srgb"))
				result.gl_ext_framebuffer_srgb = true;
			else if(cstr_eql(extension_name, "GL_ARB_framebuffer_sRGB"))
				result.gl_ext_framebuffer_srgb = true;
			else if(cstr_eql(extension_name, "gl_arb_framebuffer_object"))
				result.gl_arb_framebuffer_object = true;
		}
	}

	char *MajorAt = result.version;
	char *MinorAt = 0;

	for(char *At = result.version; *At; ++At)
	{
		if(At[0] == '.')
		{
			MinorAt = At + 1;
			break;
		}
	}

	i32 Major = 1;
	i32 Minor = 0;
	if(MinorAt)
	{
		Major = atoi(MajorAt);
		Minor = atoi(MinorAt);
	}

	if((Major > 2) || ((Major == 2) && (Minor >= 1)))
	{
		// NOTE(casey): We _believe_ we have sRGB textures in 2.1 and above automatically.
		result.gl_ext_texture_srgb = true;
	}

	return(result);
}

#define OPENGL_FUNCTION_LOAD(name) name = (FN_##name *)gl_proc_address(#name)

void
opengl_fn_load(void)
{
	OPENGL_FUNCTION_LOAD(glTexImage2DMultisample);
	OPENGL_FUNCTION_LOAD(glBindFramebuffer);
	OPENGL_FUNCTION_LOAD(glGenFramebuffers);
	OPENGL_FUNCTION_LOAD(glFramebufferTexture2D);
	OPENGL_FUNCTION_LOAD(glCheckFramebufferStatus);
	OPENGL_FUNCTION_LOAD(glBlitFramebuffer);
	OPENGL_FUNCTION_LOAD(glAttachShader);
	OPENGL_FUNCTION_LOAD(glCompileShader);
	OPENGL_FUNCTION_LOAD(glCreateProgram);
	OPENGL_FUNCTION_LOAD(glCreateShader);
	OPENGL_FUNCTION_LOAD(glLinkProgram);
	OPENGL_FUNCTION_LOAD(glShaderSource);
	OPENGL_FUNCTION_LOAD(glUseProgram);
	OPENGL_FUNCTION_LOAD(glGetProgramInfoLog);
	OPENGL_FUNCTION_LOAD(glGetShaderInfoLog);
	OPENGL_FUNCTION_LOAD(glValidateProgram);
	OPENGL_FUNCTION_LOAD(glGetProgramiv);
	OPENGL_FUNCTION_LOAD(glGetUniformLocation);
	OPENGL_FUNCTION_LOAD(glUniform4fv);
	OPENGL_FUNCTION_LOAD(glUniformMatrix4fv);
	OPENGL_FUNCTION_LOAD(glUniform1i);
	OPENGL_FUNCTION_LOAD(glGetStringi);
	OPENGL_FUNCTION_LOAD(glUniform1f);
	OPENGL_FUNCTION_LOAD(glUniform2fv);
	OPENGL_FUNCTION_LOAD(glUniform3fv);
	OPENGL_FUNCTION_LOAD(glEnableVertexAttribArray);
	OPENGL_FUNCTION_LOAD(glDisableVertexAttribArray);
	OPENGL_FUNCTION_LOAD(glGetAttribLocation);
	OPENGL_FUNCTION_LOAD(glVertexAttribPointer);
	OPENGL_FUNCTION_LOAD(glVertexAttribIPointer);
	OPENGL_FUNCTION_LOAD(glBindVertexArray);
	OPENGL_FUNCTION_LOAD(glGenVertexArrays);
	OPENGL_FUNCTION_LOAD(glBindBuffer);
	OPENGL_FUNCTION_LOAD(glGenBuffers);
	OPENGL_FUNCTION_LOAD(glBufferData);
	OPENGL_FUNCTION_LOAD(glGetShaderiv);
	OPENGL_FUNCTION_LOAD(glDeleteShader);
	OPENGL_FUNCTION_LOAD(glDeleteShader);
	OPENGL_FUNCTION_LOAD(glGenerateMipmap);
	//OPENGL_FUNCTION_LOAD(glActiveTexture);
}

void
opengl_init(bool modern_context)
{
	//struct OpenGLInfo opengl_info = os_opengl_info(modern_context);
	os_opengl_info(modern_context);

	#if 0
	if(opengl_info.gl_arb_framebuffer_object)
	{
		/* INCOMPLETE: Get Opengl Framebuffer functions */
	}
	#endif

	opengl_fn_load();
}
