void
i32_swap(i32 *a, i32 *b)
{
    i32 tmp = *a;

    *a = *b;
    *b = tmp;
}

void
f32_swap(f32 *a, f32 *b)
{
    f32 tmp = *a;

    *a = *b;
    *b = tmp;
}

enum SortOrder
{
	SORT_ORDER_ASC,
	SORT_ORDER_DES,
};

// A function to implement bubble sort
void
i32_sort_bubble(i32 *array, i32 n, enum SortOrder order)
{
	i32 i, j;
	for(i = 0; i < n-1; ++i)
	{
		for (j = 0; j < n-i-1; ++j)
		{
			if(order == SORT_ORDER_ASC && array[j] > array[j+1]) i32_swap(&array[j], &array[j+1]);
			else if(order == SORT_ORDER_DES && array[j] < array[j+1]) i32_swap(&array[j], &array[j+1]);
		}
	}
}

void
f32_sort_bubble(f32 *array, i32 n, enum SortOrder order)
{
	i32 i, j;
	for (i = 0; i < n-1; ++i)
	{
		for (j = 0; j < n-i-1; ++j)
		{
			if(order == SORT_ORDER_ASC && array[j] > array[j+1]) f32_swap(&array[j], &array[j+1]);
			else if(order == SORT_ORDER_DES && array[j] < array[j+1]) f32_swap(&array[j], &array[j+1]);
		}
	}
}
