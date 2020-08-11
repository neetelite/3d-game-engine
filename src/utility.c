/* Decimal */
#define K(value) ((value) * 1000)
#define M(value) (K(value) * 1000)

/* Bytes */
#define KB(value) ((value) * 1024LL)
#define MB(value) (KB(value) * 1024LL)
#define GB(value) (MB(value) * 1024LL)
#define TB(value) (GB(value) * 1024LL)

/* Arrays */
#define COUNT(array) (sizeof(array) / sizeof(array[0]))

#define OFFSET_OF(type, member) (u64)&(((type *)0)->member)

#define ZERO(name,size) zero_size(&(name), (size))
#define ZERO_STRUCT(struct_name) zero_size(&(struct_name), sizeof(struct_name))
//#define ZERO_STRUCT(struct_name) zero_size(&(struct_name), sizeof(struct_name))
//#define ZERO_ARRAY(Count, Pointer) zero_size(Count * sizeof((Pointer)[0]), Pointer)
//#define ZERO_ARRAY(array_name) zero_size(array_name, sizeof(array_name))

void
zero_size(void *addr, size_type size)
{
    u8 *byte = (u8 *)addr;
    while(size--)
    {
        *byte++ = 0;
    }
}

/* Color */
#define COLOR_WHITE   0xffffffff
#define COLOR_RED     0xffff0000
#define COLOR_YELLOW  0xffffff00
#define COLOR_GREEN   0xff00ff00
#define COLOR_CYAN    0xff00ffff
#define COLOR_BLUE    0xff0000ff
#define COLOR_MAGENTA 0xffff00ff
#define COLOR_BLACK   0xff000000

/* FIXME(lungu): Assertion */
#define ASSERT(name)

#define BIT_SET(word, bit_flag) ((word) = ((word) | (bit_flag)))
#define BIT_RESET(word, bit_flag) ((word) = ((word) & (~bit_flag)))
