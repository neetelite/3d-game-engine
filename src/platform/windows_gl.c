#define WGL_DRAW_TO_WINDOW_ARB                  0x2001
#define WGL_ACCELERATION_ARB                    0x2003
#define WGL_SUPPORT_OPENGL_ARB                  0x2010
#define WGL_DOUBLE_BUFFER_ARB                   0x2011
#define WGL_PIXEL_TYPE_ARB                      0x2013

#define WGL_TYPE_RGBA_ARB                       0x202B
#define WGL_FULL_ACCELERATION_ARB               0x2027

#define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB        0x20A9

#define WGL_RED_BITS_ARB                        0x2015
#define WGL_GREEN_BITS_ARB                      0x2017
#define WGL_BLUE_BITS_ARB                       0x2019
#define WGL_ALPHA_BITS_ARB                      0x201B
#define WGL_DEPTH_BITS_ARB                      0x2022

/* Windows OpenGL Functions */
typedef BOOL WINAPI FN_wglChoosePixelFormatARB(HDC hdc,
	const int *piAttribIList,
	const FLOAT *pfAttribFList,
	UINT nMaxFormats,
	int *piFormats,
	UINT *nNumFormats);
global FN_wglChoosePixelFormatARB *wglChoosePixelFormatARB;

typedef HGLRC WINAPI FN_wglCreateContextAttribsARB(HDC hDC, HGLRC hShareContext,
	const int *attribList);
global FN_wglCreateContextAttribsARB *wglCreateContextAttribsARB;

typedef BOOL WINAPI FN_wglSwapIntervalEXT(int interval); /* VSync */
global FN_wglSwapIntervalEXT *wglSwapIntervalEXT;

typedef const char * WINAPI FN_wglGetExtensionsStringEXT(void);
global FN_wglGetExtensionsStringEXT *wglGetExtensionsStringEXT;

/* LEARN(lungu): What is this for? */
global struct OpenGL opengl;

internal void
os_pixel_format_set(HDC opengl_dc)
{
	int suggested_pixel_format_index = 0;
	GLuint extended_pick = 0;

	if(wglChoosePixelFormatARB)
	{
		int attrib_list[] =
		{
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE, // 0
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB, // 1
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE, // 2
//#if HANDMADE_STREAMING
#if 0
			WGL_DOUBLE_BUFFER_ARB, GL_FALSE, // 3
#else
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE, // 3
#endif
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB, // 4
			WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE, // 5
			0,
		};

		if(!opengl.supports_srgb_framebuffer)
		{
			attrib_list[10] = 0;
		}

		wglChoosePixelFormatARB(opengl_dc, attrib_list, 0, 1,
			&suggested_pixel_format_index, &extended_pick);
	}

	if(!extended_pick)
	{
		// TODO(casey): Hey Raymond Chen - what's the deal here?
		// Is cColorBits ACTUALLY supposed to exclude the alpha bits, like MSDN says, or not?
		PIXELFORMATDESCRIPTOR DesiredPixelFormat = {0};
		DesiredPixelFormat.nSize = sizeof(DesiredPixelFormat);
		DesiredPixelFormat.nVersion = 1;
		DesiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
#if HANDMADE_STREAMING
		// NOTE(casey): PFD_DOUBLEBUFFER appears to prevent OBS from reliably streaming the window
		DesiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW;
#else
		DesiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER;
#endif
		DesiredPixelFormat.cColorBits = 32;
		DesiredPixelFormat.cAlphaBits = 8;
		DesiredPixelFormat.cDepthBits = 24;
		DesiredPixelFormat.iLayerType = PFD_MAIN_PLANE;

		suggested_pixel_format_index = ChoosePixelFormat(opengl_dc, &DesiredPixelFormat);
	}

	PIXELFORMATDESCRIPTOR SuggestedPixelFormat;
	// NOTE(casey): Technically you do not need to call DescribePixelFormat here,
	// as SetPixelFormat doesn't actually need it to be filled out properly.
	DescribePixelFormat(opengl_dc, suggested_pixel_format_index,
		sizeof(SuggestedPixelFormat), &SuggestedPixelFormat);
	SetPixelFormat(opengl_dc, suggested_pixel_format_index, &SuggestedPixelFormat);
}

internal void
os_opengl_ext_load(void)
{
	WNDCLASSA window_class = {0};

	window_class.lpfnWndProc = DefWindowProcA;
	window_class.hInstance = GetModuleHandle(0);
	window_class.lpszClassName = "game_wgl_loader";

	if(!RegisterClassA(&window_class))
	{
		return;
	}

	HWND window = CreateWindowExA(
		0,
		window_class.lpszClassName,
		"Game",
		0,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		window_class.hInstance,
		0);

	HDC window_dc = GetDC(window);
	os_pixel_format_set(window_dc);
	HGLRC opengl_rc = wglCreateContext(window_dc);
	if(wglMakeCurrent(window_dc, opengl_rc))
	{
		wglChoosePixelFormatARB =
			(FN_wglChoosePixelFormatARB *)wglGetProcAddress("wglChoosePixelFormatARB");

		wglCreateContextAttribsARB =
			(FN_wglCreateContextAttribsARB *)wglGetProcAddress("wglCreateContextAttribsARB");

		wglSwapIntervalEXT = (FN_wglSwapIntervalEXT *)wglGetProcAddress("wglSwapIntervalEXT");
		wglGetExtensionsStringEXT = (FN_wglGetExtensionsStringEXT *)wglGetProcAddress("wglGetExtensionsStringEXT");

		if(wglGetExtensionsStringEXT)
		{
			char *extensions = (char *)wglGetExtensionsStringEXT();
			char *at = extensions;
			while(*at)
			{
				at = ascii_whitespace_skip(at);
				char *end = at;
				while(*end && !ascii_whitespace_is(*end)) {++end;}

				u64 len = end - at;

				if(0) {}
				else if(cstr_eql_n(at, "WGL_EXT_framebuffer_sRGB", len)) opengl.supports_srgb_framebuffer = true;
				else if(cstr_eql_n(at, "WGL_ARB_framebuffer_sRGB", len)) opengl.supports_srgb_framebuffer = true;

				at = end;
			}
		}

		wglMakeCurrent(0, 0);
	}

	wglDeleteContext(opengl_rc);
	ReleaseDC(window, window_dc);
	DestroyWindow(window);
}

// NOTE(casey): Windows-specific
#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB             0x2093
#define WGL_CONTEXT_FLAGS_ARB                   0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126

#define WGL_CONTEXT_printf_BIT_ARB               0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB  0x0002

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB        0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

int os_opengl_attribs[] =
{
	WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
	WGL_CONTEXT_MINOR_VERSION_ARB, 0,
	WGL_CONTEXT_FLAGS_ARB, 0 // NOTE(casey): Enable for testing WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
//#if HANDMADE_INTERNAL
#if 0
	|WGL_CONTEXT_printf_BIT_ARB
#endif
	,
	//WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
	WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	0,
};

internal HGLRC
os_opengl_init(HDC opengl_dc)
{
	os_opengl_ext_load();
	os_pixel_format_set(opengl_dc);

	bool modern_context = true;
	HGLRC opengl_rc = 0;

	if(wglCreateContextAttribsARB)
	{
		opengl_rc = wglCreateContextAttribsARB(opengl_dc, 0, os_opengl_attribs);
	}

	if(!opengl_rc)
	{
		modern_context = false;
		opengl_rc = wglCreateContext(opengl_dc);
	}

	if(wglMakeCurrent(opengl_dc, opengl_rc))
	{
		opengl_init(modern_context);
		if(wglSwapIntervalEXT) wglSwapIntervalEXT(1);
	}

	return(opengl_rc);
}

#define gl_proc_address(name) wglGetProcAddress(name)
