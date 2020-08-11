#include <Windows.h>
#include <windowsx.h> /* GET_X_LPARAM */

/* Defines */
#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif

#define KEY_SPACE       VK_SPACE
#define KEY_ENTER       VK_RETURN
#define KEY_ESCAPE      VK_ESCAPE
#define KEY_ARROW_LEFT  VK_LEFT
#define KEY_ARROW_UP    VK_UP
#define KEY_ARROW_RIGHT VK_RIGHT
#define KEY_ARROW_DOWN  VK_DOWN
#define KEY_PAGE_UP     VK_PRIOR
#define KEY_PAGE_DOWN   VK_NEXT

//#define KEY_SHIFT_LEFT   VK_NEXT

//#define BUTTON_LEFT      VK_LBUTTON
//#define BUTTON_MIDDLE    VK_MBUTTON
//#define BUTTON_RIGHT     VK_RBUTTON
//#define BUTTON_UP        4
//#define BUTTON_DOWN      5
//#define BUTTON_EXTRA_1   VK_XBUTTON1
//#define BUTTON_EXTRA_2   VK_XBUTTON2

#define BUTTON_LEFT      1
#define BUTTON_MIDDLE    2
#define BUTTON_RIGHT     3
#define BUTTON_UP        4
#define BUTTON_DOWN      5
#define BUTTON_EXTRA_1   6
#define BUTTON_EXTRA_2   7

struct OsContext
{
	HWND window;

	u32 width;
	u32 height;

	WINDOWPLACEMENT window_position;

	u32 screen_width;
	u32 screen_height;

	bool fullscreen;
};

struct OsState
{
	u64 app_memory_size;
	void *app_memory;

	char path[PATH_MAX];
	char path_exe[PATH_MAX];
	char path_run[PATH_MAX];
	char path_src[PATH_MAX];
	char path_data[PATH_MAX];

	bool running;

	f32 fps;
	f32 fps_max;
	u32 frame;

	f32 dt;

	HDC device_context;
};
