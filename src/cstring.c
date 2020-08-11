internal inline char *
char_whitespace_find(char *data)
{
	char *result = NULL;

	char *at = data;
	while(*at != ' ' && *at != '\t' && *at != '\n' && *at != '\0')
	{
		++at;
	}

	result = at;
	return(result);
}

internal inline char *
char_whitespace_skip(char *data)
{
	while(*data == ' ' || *data == '\t' || *data == '\n')
	{
		++data;
	}
	return(data);
}

char *
char_find_next(char *data, char c)
{
	char *result = data;

	while(*result != c) ++result;
	return(result);
}

char *
next_line(char *data)
{
	data = char_find_next(data, '\n');
	++data;
	return(data);
}

u32
char_count(char *data, char c)
{
	u32 result = 0;
	while(*data == c) {++result; ++data;};
	return(result);
}

/* C STRING */
internal u32
cstr_len(char *cstr)
{
	u32 length = 0;

	char *c = cstr;
	while(*c++ != '\0')
	{
		++length;
	}

	return(length);
}

internal i32
cstr_cpy(char *dst, char *src)
{
	/* INCOMPLETE(neet): Error checking */
	#if 0
	if(sizeof(dst) < sizeof(src))
	{
		return(-1);
	}
	#endif

	char *d = dst;
	char *s = src;

	while(*s != '\0' || *d != '\0')
	{
		*d++ = *s++;
	}

	return(1);
}

internal i32
cstr_cpy_n(char *dst, char *src, u32 n)
{
	char *d = dst;
	char *s = src;

	for(int i = 0; i < n; ++i)
	{
		*d++ = *s++;
	}

	return(1);
}

internal i32
cstr_cat(char *dst, char *a, char *b)
{
	u64 len_a = cstr_len(a);
	u64 len_b = cstr_len(b);

	/* INCOMPLETE(neet): Error checking */
	#if 0
	printf("D: %d, A: %d, B: %d\n", sizeof(dst), sizeof(a), sizeof(b));
	if(sizeof(dst) < (sizeof(a) + sizeof(b)))
	{
		return(-1);
	}
	#endif

	cstr_cpy(dst, a);
	cstr_cpy(dst + len_a, b);
	dst[len_a + len_b] = '\0';

	return(1);
}

/* Path */
internal char *
cstr_path_extension(char *path)
{
	char *c = path + cstr_len(path);
	while(*--c != '.')
	{

	}
	++c;
	return(c);
}

internal void
cstr_path_parent(char *dst, char *src)
{
	cstr_cpy(dst, src);

	char *c = dst + cstr_len(dst) - 1;
	while(*--c != '/');
	*++c = '\0';
}

internal bool
cstr_eql(char *a, char *b)
{
	bool result = true;

	u32 a_len = cstr_len(a);
	u32 b_len = cstr_len(b);

	u32 len = a_len > b_len ? a_len : b_len;

	for(int i = 0; i < len; ++i)
	{
		if(*a++ != *b++)
		{
			result = false;
			break;
		}
	}

	return(result);
}

internal bool
cstr_eql_n(char *a, char *b, u64 len)
{
    bool result = false;

    if(b)
    {
        char *at = b;
        for(u64 i = 0; i < len; ++i, ++at)
        {
            if((*at == 0) ||
               (a[i] != *at))
            {
               return(false);
            }
        }

        result = (*at == 0);
    }
    else
    {
        result = (len == 0);
    }

    return(result);
}

char *
CSTR(char *cstr)
{
	char *result = NULL;
	u32 len = cstr_len(cstr);
	result = mem_alloc(1, len + 1, false);
	cstr_cpy_n(result, cstr, len);
	result[len] = '\0';
	return(result);
}

/* Windows */
internal void
cstr_fs_from_bs(char *dst, char *src)
{
	/* SPEED(neet): If you remove str_cpy you may iterate only once */
	cstr_cpy(dst, src);

	char *c = dst;
	while(*c != '\0')
	{
		if(*c == '\\')
		{
			*c = '/';
		}
		++c;
	}
}
