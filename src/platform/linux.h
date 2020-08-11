#include <X11/X.h>
#include <X11/Xlib.h>

/* CLEANUP(lungu): Put these somewhere else */
#include <GL/glx.h>
#define gl_proc_address(name) glXGetProcAddress(name)

#include <linux/limits.h>

#define KEY_SPACE       32
#define KEY_ENTER       ?????
#define KEY_BACKSPACE   65288
#define KEY_ESCAPE      65307
#define KEY_ARROW_LEFT  65361
#define KEY_ARROW_UP    65362
#define KEY_ARROW_RIGHT 65363
#define KEY_ARROW_DOWN  65364
#define KEY_PAGE_UP     65365
#define KEY_PAGE_DOWN   65366
#define KEY_F4          65473
#define KEY_SHIFT_LEFT  65505
#define KEY_SHIFT_RIGHT 65506
#define KEY_CAPSLOCK    65509
#define KEY_ALT_LEFT    65513
#define KEY_ALT_RIGHT   65514

#define BUTTON_LEFT      1
#define BUTTON_MIDDLE    2
#define BUTTON_RIGHT     3
#define BUTTON_UP        4
#define BUTTON_DOWN      5
#define BUTTON_EXTRA_1   8
#define BUTTON_EXTRA_2   9

struct OsContext
{
	Display *display;
	Window window_main;
	Window window_root;

	u32 width;
	u32 height;

	u32 screen_width;
	u32 screen_height;

	bool fullscreen;

	GLXContext gl_context;
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
};

/* Input */
struct OsKeyboard
{
	/* CLEANUP(lungu) - Find something better, this wastes to much space */
#define KEY_MAX 0xffff
	bool keys[KEY_MAX];
	bool down[KEY_MAX];

	bool capslock_enabled;
};

struct OsMouse
{
	bool buttons[10];
	bool down[10];

	v2 p;
	v2 d;

	bool moved;
	bool locked;
	bool visible;
};

struct OsInput
{
	struct OsKeyboard keyboards[1];
	struct OsMouse mice[1];
};
