#include "../inc/standard.c"
#include "../inc/graphic.c"

#include "type.c"
#include "memory.c"
#include "char.c"
#include "cstring.c"
#include "utility.c"
#include "debug.c"

#include "math_scalar.c"
#include "vector.c"
#include "matrix.c"
#include "quaternion.c"
#include "geometry.c"
#include "math_complex.c"

#if OS_WINDOWS
#include "platform/windows.h"
#include "platform/windows.c"
#else
#include "platform/linux.h"
#include "platform/linux.c"
#endif

#include "opengl.c"
#include "shader/shader.c"

#if OS_WINDOWS
#include "platform/windows_gl.c"
#else
#include "platform/linux_gl.c"
#endif

struct OsContext os_context;
struct OsState os_state;
struct OsInput os_input;

#include "xml.c"
#include "sort.c"
#include "image.c"

#include "mesh.c"
#include "skeleton.c"
#include "animation.c"

#include "model.c"
#include "camera.c"
#include "character.c"

#include "network.c"

#include "app.h"
#include "input.c"

struct AppMemory app_memory;
struct AppState *app_state;

#include "app.c"

i32
main(void)
{
	/* Path */
	os_path_build(&os_state);

	/* Window */
	os_create_window(&os_context);

	/* Memory */
	os_allocate_memory(&os_state);
	app_state = os_state.app_memory;

	/* Graphics */
	os_opengl_init(&os_context);

	/* Timer */
	os_timer_init(&os_state);

	/* Input */

	os_state.running = 1;
	while(os_state.running)
	{
		os_process_events(&os_context, &os_state, &os_input);
		app_main();
		os_render(&os_context);
		os_timer(&os_state, &counter_last);
	}

	#if BUILD_DEBUG
	printf("\n");
	printf("ALLOC: %lld\t", global_alloc_count);
	printf("FREE: %lld\t", global_free_count);
	printf("LEAKED: %lld\t", global_alloc_count - global_free_count);
	printf("\n");
	#endif
}
