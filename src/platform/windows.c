#include "includes.c"

#include "main.h"

#if 0
#define WIDTH 1600
#define HEIGHT 900
#else
#define WIDTH 1920
#define HEIGHT 1080
#endif

#define FPS_MAX 300

global f32 fps_array[FPS_MAX] = {0};
global i32 fps_array_index = 0;

/* Global Variables Definition - Os */
global struct OsState os_state = {0};
global struct OsContext os_context = {0};

#include "app.h"

/* Global Variables Definition - App */
global struct AppInput app_input;
global struct AppMemory app_memory;
global struct AppState *app_state = NULL;

#include "wgl.c"

#include "input.c"
#include "app.c"

/* File Variables Definition */
internal i64 performance_counter_frequency = 0;

/* Init functions */
internal void
os_path_build(void)
{
	/* cstr_path_run */
	char path_exe[PATH_MAX];
	GetModuleFileNameA(NULL, path_exe, PATH_MAX);

	/* path_exe */
	cstr_fs_from_bs(os_state.path_exe, path_exe);

	/* path_run */
	cstr_path_parent(os_state.path_run, os_state.path_exe);

	/* path_data */
	cstr_cat(os_state.path_data, os_state.path_run, "data/");

	/* path */
	cstr_path_parent(os_state.path, os_state.path_run);

	/* path_src */
	cstr_cat(os_state.path_src, os_state.path, "src/");
}

//RENDER FUNCTIONS
void
os_render(HDC device_context)
{
	SwapBuffers(device_context);
}

//CALLBACK FUNCTIONS
//WINDOW PROCEDURE
LRESULT CALLBACK
os_window_procedure(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	LRESULT result = 0;

	switch(message)
	{
		#if 0
		/* IMPLEMENT LEARN(lungu): Top borders like Discord */
		case WM_PAINT:
		{
			PAINTSTRUCT paint;
			HDC device_context = BeginPaint(window, &paint);
			i32 x = paint.rcPaint.left;
			i32 y = paint.rcPaint.top;
			i32 height = paint.rcPaint.bottom - paint.rcPaint.top;
			i32 width = paint.rcPaint.right - paint.rcPaint.left;

			os_render(device_context);
			EndPaint(window, &paint);
		} break;
		#endif
		case WM_QUIT:
		case WM_CLOSE:
		case WM_DESTROY:
		{
			os_state.running = 0;
		} break;
		default:
		{
			result = DefWindowProc(window, message, w_param, l_param);
		} break;
	}

	return(result);
}

/* Clock Functions */
internal LARGE_INTEGER
time_wall_get(void)
{
	LARGE_INTEGER result;
	QueryPerformanceCounter(&result);
	return(result);
}

f32
time_elapsed_get(LARGE_INTEGER start, LARGE_INTEGER end)
{
	f32 result = ((f32)(end.QuadPart - start.QuadPart) / (f32)performance_counter_frequency);
	return(result);
}

void
os_timer(LARGE_INTEGER *counter_last, f32 seconds_per_frame, bool granular_sleep)
{
	LARGE_INTEGER counter_work = time_wall_get();
	f32 work_seconds_elapsed = time_elapsed_get(*counter_last, counter_work);
	f32 seconds_elapsed_for_frame = work_seconds_elapsed;

	if(seconds_elapsed_for_frame < seconds_per_frame)
	{
		if(granular_sleep)
		{
			DWORD sleep_ms = (DWORD)(1000.0f * (seconds_per_frame - seconds_elapsed_for_frame));
			if(sleep_ms > 0)
			{
				Sleep(sleep_ms);
			}
		}
		f32 test_seconds_elapsed_for_frame = time_elapsed_get(*counter_last, time_wall_get());

		if(test_seconds_elapsed_for_frame < seconds_per_frame)
		{
			/* INCOMPLETE */
		}

		while(seconds_elapsed_for_frame < seconds_per_frame)
		{
			seconds_elapsed_for_frame = time_elapsed_get(*counter_last, time_wall_get());
		}

	}
	else
	{
		/* INCOMPLETE */
	}

	LARGE_INTEGER end_counter = time_wall_get();
	os_state.dt = time_elapsed_get(*counter_last, end_counter);
	*counter_last = end_counter;

	#if 1
	f32 ms_per_frame = (1000.0f * os_state.dt);
	if(fps_array_index == os_state.fps_max)
	{
		f32 total_fps = 0;
		for(i32 i = 0; i < os_state.fps_max; ++i)
		{
			total_fps += fps_array[i];
		}
		fps_array_index = 0;
		os_state.fps = total_fps / os_state.fps_max;
		//printf("FPS: %f\n", os_state.fps);
	}
	fps_array[fps_array_index] = 1000 / ms_per_frame;
	++fps_array_index;
	#endif
}

/* Window Functions */
void
os_fullscreen_toggle()
{
	DWORD style = GetWindowLong(os_context.window, GWL_STYLE);
	if(style & WS_OVERLAPPEDWINDOW)
	{
		MONITORINFO monitor_info = {sizeof(monitor_info)};
		if(GetWindowPlacement(os_context.window, &os_context.window_position) &&
		   GetMonitorInfo(MonitorFromWindow(os_context.window, MONITOR_DEFAULTTOPRIMARY), &monitor_info))
		{
			SetWindowLong(os_context.window, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(os_context.window, HWND_TOP,
						 monitor_info.rcMonitor.left, monitor_info.rcMonitor.top,
						 monitor_info.rcMonitor.right - monitor_info.rcMonitor.left,
						 monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top,
						 SWP_NOOWNERZORDER | SWP_FRAMECHANGED
				);
		}
	}
	else
	{
		SetWindowLong(os_context.window, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(os_context.window, &os_context.window_position);
		SetWindowPos(os_context.window, 0, 0, 0, 0, 0,
					 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
					 SWP_NOOWNERZORDER | SWP_FRAMECHANGED
			) ;
	}

	os_context.fullscreen = !os_context.fullscreen;
}

void
os_mouse_set_position(struct AppMouse *mouse, v2 pos)
{
	SetCursorPos(pos.x, pos.y);
}

/* Input Functions */
void
os_process_events()
{

	struct AppKeyboard *keyboard = &app_input.keyboards[0];
	struct AppMouse *mouse = &app_input.mice[0];

	for(i32 i = 0; i < COUNT(keyboard->keys); ++i)
	{
		if(keyboard->keys[i]) keyboard->down[i] = 1;
		else keyboard->down[i] = 0;
	}

	for(i32 i = 0; i < COUNT(mouse->buttons); ++i)
	{
		if(mouse->buttons[i]) mouse->down[i] = 1;
		else mouse->down[i] = 0;
	}

	f32 w = 1920;
	f32 h = 1080;

	POINT pos_point;
	GetCursorPos(&pos_point);
	v2 pos_new = V2(pos_point.x, h - pos_point.y);
	v2 pos_center = V2(w/2, h/2); /* TODO LEARN(lungu): Should we use center of screen or center of window?  */
	if(mouse->locked && GetFocus())
	{
		/* SPEED */
		if(!v2_eql(pos_center, pos_new))
		{
			mouse->d = v2_s(pos_new, mouse->p);
			//v2_print(mouse->d, "D: ");
			mouse->moved = 1;

			os_mouse_set_position(mouse, pos_center);
			mouse->p = pos_center;
		}
		else
		{
			mouse->moved = 0;
		}
	}
	else
	{
		/* INCOMPLETE(lungu): */
	}
	if(!mouse->moved) mouse->d = V2_ZERO;

	MSG message;
	while(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		switch(message.message)
		{
		/* keyboard input */
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			u32 k = (u32)message.wParam;
			bool alt_down = ((message.lParam & (1 << 29)) != 0);
			bool was_down = ((message.lParam & (1 << 30)) != 0);
			bool key_down = ((message.lParam & (1 << 31)) == 0);

			//if(key_down != was_down)
			{
				if(k >= 65 && k <= 90)
				{
					k += 32;
				}

				keyboard->down[k] = was_down;
				keyboard->keys[k] = key_down;

				if(keyboard->keys[KEY_ENTER] && alt_down)
				{
					os_fullscreen_toggle();
				}

				/* FIXME(lungu): Change this to f4 */
				if(keyboard->keys['q'])
				{
					os_state.running = 0;
				}
			}

			if(k <= 10)
			{
				mouse->buttons[k] = 1;
			}
		} break;

		/* mosue input */
		case WM_LBUTTONDOWN:
		{
			mouse->buttons[BUTTON_LEFT] = 1;
		} break;
		case WM_LBUTTONUP:
		{
			mouse->buttons[BUTTON_LEFT] = 0;
		} break;
		case WM_MBUTTONDOWN:
		{
			mouse->buttons[BUTTON_MIDDLE] = 1;
		} break;
		case WM_MBUTTONUP:
		{
			mouse->buttons[BUTTON_MIDDLE] = 0;
		} break;
		case WM_RBUTTONDOWN:
		{
			mouse->buttons[BUTTON_RIGHT] = 1;
		} break;
		case WM_RBUTTONUP:
		{
			mouse->buttons[BUTTON_RIGHT] = 0;
		} break;
		/* mosue movement */
		#if 0
		case WM_MOUSEMOVE:
		{
			mouse->moved = 1;

			v2 position_new = V2(GET_X_LPARAM(message.lParam),
						 os_context.height - GET_Y_LPARAM(message.lParam) - 1);

			mouse->d = v2_s(position_new, mouse->p);

			mouse->p = position_new;
		} break;
		#endif

		default:
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		}
	}
}

/* NOTE(neet): Program start */
int main(void)
{
	#if BUILD_DEBUG
	//mem_alloc_array_init(&global_alloc_array, 50);
	#endif

	HINSTANCE instance = GetModuleHandle(NULL);

	/* Path */
	os_path_build();

	/* Set Granularity - Performance Counter Frequency */
	LARGE_INTEGER performance_counter_frequency_result;
	QueryPerformanceFrequency(&performance_counter_frequency_result);
	performance_counter_frequency = performance_counter_frequency_result.QuadPart;

	u32 scheduler_ms = 1;
	bool granular_sleep_bool = timeBeginPeriod(scheduler_ms) == TIMERR_NOERROR;

	/* Window Class */
	WNDCLASS window_class = {0};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = os_window_procedure;
	window_class.hInstance = instance;
	window_class.hCursor = LoadCursor(0, IDC_ARROW);
	//window_class.hIcon = 0;
	window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	window_class.lpszClassName = "window_class";

	if(RegisterClassA(&window_class) == 0)
	{
		MessageBox(0, "Failed to register a window class.", "ERROR", MB_OK | MB_ICONERROR);
		return(-1);
	}

	/* Window | Context */
	os_context.width = WIDTH;
	os_context.height = HEIGHT;

	char window_name[] = "LUNGU // 3D ENGINE";
	os_context.window = CreateWindowEx(0, window_class.lpszClassName, window_name,
					   WS_OVERLAPPEDWINDOW | WS_VISIBLE,
					   CW_USEDEFAULT, CW_USEDEFAULT,
					   os_context.width, os_context.height,
					   0, 0, instance, 0);

	if(os_context.window == NULL)
	{
		MessageBox(0, "Failed to create a window .", "ERROR", MB_OK | MB_ICONERROR);
		return(-1);
	}

	os_context.screen_width = GetSystemMetrics(SM_CXSCREEN);
	os_context.screen_width = GetSystemMetrics(SM_CYSCREEN);

	/* App Memory */
	#if 1
	u64 size = sizeof(struct AppState);
	#else
	//u64 size = MB(500);
	u64 size = GB(4);
	#endif

	app_memory.size = size;
	app_memory.init = false;
	os_state.app_memory_size = size;
	os_state.app_memory = mem_alloc(1, size, true);

	if(os_state.app_memory == NULL)
	{
		MessageBox(0, "ERROR: Failed to allocate memory for the game.", "ERROR", MB_OK | MB_ICONERROR);
		return(-1);
	}

	app_memory.storage = os_state.app_memory;
	app_state = (struct AppState *)app_memory.storage;

	/* OpenGL */
	HDC device_context = GetDC(os_context.window);
	os_opengl_init(device_context);

	os_state.device_context = device_context;

	/* Clock */
	os_state.fps_max = FPS_MAX;
	f32 app_target_seconds_per_frame = 1.0f / os_state.fps_max;

	/* LEARN: What do these do? */
	/* FIXME: Function names */
	LARGE_INTEGER counter_last = time_wall_get();

	if(WIDTH == 1920 && HEIGHT == 1080) os_fullscreen_toggle();

	ShowCursor(0);

	/* Main Loop */
	os_state.running = 1;
	while(os_state.running)
	{
		os_process_events();
		app_main();
		os_render(device_context);

		os_timer(&counter_last, app_target_seconds_per_frame, granular_sleep_bool);
	}

	/* Terminate */
	/* LEAK(lungu): Make sure you all other windows specific stuff */
	mem_free(os_state.app_memory);
	ReleaseDC(os_context.window, device_context);

	#if BUILD_DEBUG
	printf("\n");
	printf("ALLOC: %lld\t", global_alloc_count);
	printf("FREE: %lld\t", global_free_count);
	printf("LEAKED: %lld\t", global_alloc_count - global_free_count);
	printf("\n");
	#endif

	return(0);
}
