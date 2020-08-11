global u64 global_alloc_count;
global u64 global_free_count;

void *
mem_alloc(u32 count, u64 size, bool clear)
{
	void *result = NULL;

	if(clear) result = calloc(count, size);
	else result = malloc(count * size);

	#if BUILD_DEBUG
	++global_alloc_count;
	#endif/* BULD_DEBUG */

	return(result);
}

void
mem_free(void *pointer)
{
	if(!pointer) return;

	#if BUILD_DEBUG
	++global_free_count;
	#endif/* BULD_DEBUG */
	free(pointer);
}
