#define TAB_CHAR ' '
#define TAB_SIZE 2

struct XML_Attrib
{
	char *name;
	char *value;
};

struct XML_Node
{
	u32 id;
	u32 level;
	char *name;

	u32 attrib_count;
	struct XML_Attrib *attribs;

	char *text;

	struct XML_Node *parent;

	struct XML_Node *prev;
	struct XML_Node *next;

	u32 child_count;
	struct XML_Node *child_first;
	struct XML_Node *child_last;
};

struct XML_Tree
{
	struct XML_Node *info;
	struct XML_Node *root;
};


struct XML_Attrib
xml_attrib(char *data)
{
	struct XML_Attrib result = {0};

	/* Name */
	char *c = data;

	u32 size = 0;
	while(*c++ != '=')
	{
		++size;
	}

	result.name = mem_alloc(1, size + 1, false);

	c = data;
	char *n = result.name;

	int i = 0;
	while(*c != '=') *n++ = *c++;
	result.name[size] = '\0';

	/* Value */
	char * d = c + 2;

	size = 0;
	while(*d++ != '"')
	{
		++size;
	}

	result.value = mem_alloc(1, size + 1, false);

	d = c + 2;
	char *v = result.value;

	i = 0;
	while(*d != '"') *v++ = *d++;
	result.value[size] = '\0';

	/* Return */
	return(result);
}

u32
xml_attrib_count(char *data)
{
	u32 count = 0;
	while(*data != '>') {if(*data == ' ') ++count; ++data;};

	return(count);
}

struct XML_Attrib *
xml_attrib_alloc(char *data, u32 count)
{
	struct XML_Attrib *result = mem_alloc(count, sizeof(struct XML_Attrib), false);

	data = char_whitespace_skip(data);

	char *last = data;
	char *c = data;
	for(int i = 0; i < count; ++i)
	{
		u32 size = 0;
		while(*c++ != '=') ++size;

		result[i].name = mem_alloc(1, size + 1, false);

		cstr_cpy_n(result[i].name, last, size);
		result[i].name[size] = '\0';
		++c;

		/* Value */
		last = c;
		size = 0;
		while(*c++ != '"') ++size;

		result[i].value = mem_alloc(1, size + 1, false);
		cstr_cpy_n(result[i].value, last, size);
		result[i].value[size] = '\0';
		++c;
		last = c;
	}

	/* Return */
	return(result);
}

void
xml_attrib_free(struct XML_Attrib *attrib)
{
	mem_free(attrib->name);
	mem_free(attrib->value);
}

void
xml_node_print(struct XML_Node *node)
{
	if(!node) {printf("ERROR: NULL Node!\n"); return;};

	printf("Name : %s\n", node->name);
	printf("ID : %d\n", node->id);
	printf("Level : %d\n", node->level);

	printf("Attrib Count : %d\n", node->attrib_count);
	if(node->attrib_count)
	{
		printf("Attribs:\n");
		for(int i = 0; i < node->attrib_count; ++i)
		{
			printf("\t%s = %s\n", node->attribs[i].name, node->attribs[i].value);
		}
	}
	else
	{
		printf("Attribs: NULL\n");

	}
	if(node->parent) printf("Parent: %s\n", node->parent->name); else printf("Parent: NULL\n");
	if(node->text) printf("Text: %s\n", node->text); else printf("Text: NULL\n");
	if(node->prev) printf("Prev: %s\n", node->prev->name); else printf("Prev: NULL\n");
	if(node->next) printf("Next: %s\n", node->next->name); else printf("Next: NULL\n");

	printf("Child Count: %d\n", node->child_count);
	if(node->child_count)
	{
		printf("Children:\n");
		struct XML_Node *child = node->child_first;
		printf("\t%s\n", child->name);

		for(int i = 1; i < node->child_count; ++i)
		{
			child = child->next;
			printf("\t%s\n", child->name);
		}
	}
	else
	{
		printf("Children: NULL\n");
	}
	printf("\n");
}

char *
xml_node_alloc(struct XML_Node *parent, char *data)
{
	u32 id = 0;
	u32 level = char_count(data, TAB_CHAR) / TAB_SIZE;

	if(parent->level == level)
	{
		//char *x = char_find_next(data, '\n');
		//*x = '\0';
		//printf("DATA: %s\n", data);

		//xml_node_alloc(parent, data);
		//data = next_line(data);
		return(data);
	}
	else id = parent->child_count;

	struct XML_Node *node = mem_alloc(1, sizeof(struct XML_Node), true);

	node->id = parent->child_count;
	node->level = level;
	node->parent = parent;

	if(!parent->child_last)
	{
		parent->child_first = node;
		parent->child_last = node;
	}
	else
	{
		node->prev = parent->child_last;
		parent->child_last->next = node;
		parent->child_last = node;
	}

	++parent->child_count;

	char *c = char_find_next(data, '<') + 1;
	char *l = c;

	u32 node_name_size = 0;
	while(*c != ' ' && *c != '>') {++node_name_size; ++c;}

	node->name = mem_alloc(1, node_name_size + 1, false);
	cstr_cpy_n(node->name, l, node_name_size);
	node->name[node_name_size] = '\0';

	//printf("Name: %s\n", node->name);

	node->attrib_count = xml_attrib_count(c);
	if(node->attrib_count) node->attribs = xml_attrib_alloc(c, node->attrib_count);
	else node->attribs = NULL;

	c = char_find_next(c, '>');
	++c;

	if(*(c-2) == '/');
	else if(*c == '\n')
	{
		node->text = NULL;
		c = next_line(c);
		c = xml_node_alloc(node, c);
	}
	else
	{
		/* NOTE(lungu): These always close inline I believe */
		char *last = c;
		u32 size = 0;
		while(*c != '<') {++c; ++size;};

		node->text = mem_alloc(1, size + 1, false);
		cstr_cpy_n(node->text, last, size);
		node->text[size] = '\0';
	}

	c = next_line(c);
	c = xml_node_alloc(parent, c);

	//xml_node_print(node);
	//printf("Name: %s\n", node->name);

	return(c);
}

void
xml_node_free(struct XML_Node *node)
{
	if(node == NULL) return;

	mem_free(node->name);

	if(node->attrib_count)
	{
		for(int i = 0; i < node->attrib_count; ++i)
		{
			xml_attrib_free(&node->attribs[i]);
		}
	}
	mem_free(node->attribs);
	if(node->text) mem_free(node->text);

	if(node->child_count)
	{
		struct XML_Node *child = node->child_first;
		xml_node_free(child);

		for(int i = 1; i < node->child_count; ++i)
		{
			child = child->next;
			xml_node_free(child);
		}
	}

	mem_free(node);
}

void
xml_tree_info(struct XML_Tree *tree, char *data)
{
	struct XML_Node *info = mem_alloc(1, sizeof(struct XML_Node), false);
	tree->info = info;

	char *c = char_whitespace_skip(data);
	if(*c == '<' && *(c+1) == '?' && cstr_eql(c+2, "xml"))
	{
		c = char_find_next(c, ' ');
		++c;
	}

	info->id = 0;
	info->name = CSTR("xml");
	info->level = 0;

	info->attrib_count = 2;
	info->attribs = mem_alloc(info->attrib_count, sizeof(struct XML_Attrib), false);

	info->attribs[0] = xml_attrib(c);
	c = char_find_next(c, ' ');
	c = char_whitespace_skip(c);
	info->attribs[1] = xml_attrib(c);

	info->text = NULL;

	info->child_count = 0;
	info->child_first = NULL;
}

void
xml_tree_root(struct XML_Tree *tree, char *data)
{
	if(*data != '<') return;

	struct XML_Node *root = mem_alloc(1, sizeof(struct XML_Node), true);
	root->id = 0;
	root->level = 0;

	char *c = data + 1;
	u32 root_name_size = 0;
	//printf("CHAR: %c\n", *data);

	while(*c != ' ' && *c != '>') {++root_name_size; ++c;}

	root->name = mem_alloc(1, root_name_size + 1, false);
	cstr_cpy_n(root->name, data + 1, root_name_size);
	root->name[root_name_size] = '\0';

	root->attrib_count = xml_attrib_count(c);
	root->attribs = xml_attrib_alloc(c, root->attrib_count);

	c = char_find_next(c, '>');
	++c;

	if(*c == '\n')
	{
		root->text = NULL;
		c = next_line(c);
		xml_node_alloc(root, c);
	}
	tree->root = root;
	//xml_node_print(root);

}

struct XML_Tree *
//xml_tree_alloc(char *file_path)
xml_tree_alloc(char *file_path)
{
	/* Open file */
	FILE *file = fopen(file_path, "rb");
	if(!file)
	{
		ERROR("[ERROR] Couln't open file: %s\n", file_path);
		return(NULL);
	}

	/* Load into memory */
	u64 size = 0;
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *file_data = mem_alloc(1, size, false);
	fread(file_data, size, 1, file);

	/* Close the file */
	fclose(file);

	/* Parse the file contents into a tree */
	struct XML_Tree *tree = mem_alloc(1, sizeof(struct XML_Tree), false);
	tree->info = NULL;
	tree->root = NULL;

	char *c = file_data;

	xml_tree_info(tree, c);
	c = next_line(c);
	xml_tree_root(tree, c);

	/* Free the memory */
	mem_free(file_data);

	return(tree);
}

void
xml_tree_free(struct XML_Tree *tree)
{
	/* LEAK(lungu): Leaking */
	xml_node_free(tree->info);
	xml_node_free(tree->root);
}

struct XML_Node *
xml_node_child_find_by_id(struct XML_Node *node, u32 id)
{
	struct XML_Node *result = NULL;

	if(!node->child_count) return(result);

	struct XML_Node *child = node->child_first;
	if(child->id == id) result = child;
	else
	{
		for(i32 i = 1; i < node->child_count; ++i)
		{
			child = child->next;
			if(child->id == id)
			{
				result = child;
				break;
			}
		}
	}

	return(result);
}

struct XML_Node *
xml_node_child_find_by_name(struct XML_Node *node, char *name)
{
	struct XML_Node *result = NULL;

	if(!node->child_count) return(result);

	struct XML_Node *child = node->child_first;
	if(cstr_eql(child->name, name)) result = child;
	else
	{
		for(i32 i = 1; i < node->child_count; ++i)
		{
			child = child->next;
			if(cstr_eql(child->name, name))
			{
				result = child;
				break;
			}
		}
	}

	return(result);
}

u32
xml_node_child_count_by_name(struct XML_Node *node, char *name)
{
	u32 result = 0;

	if(!node->child_count) return(result);

	struct XML_Node *child = node->child_first;
	if(cstr_eql(child->name, name)) ++result;

	for(i32 i = 1; i < node->child_count; ++i)
	{
		child = child->next;
		if(cstr_eql(child->name, name)) ++result;
	}

	return(result);
}

struct XML_Node *
xml_node_child_find_by_attrib(struct XML_Node *node, char *name, char *value)
{
	struct XML_Node *result = NULL;
	if(!node->child_count) return(result);

	/* First child */
	struct XML_Node *child = node->child_first;
	if(!child->attrib_count) return(result);

	for(i32 i = 0; i < child->attrib_count; ++i)
	{
		struct XML_Attrib *attrib = child->attribs + i;
		if(cstr_eql(attrib->name, name) && cstr_eql(attrib->value, value))
		{
			result = child;
			break;
		}
	}

	/* Other children */
	if(!result)
	{
		for(i32 i = 1; i < node->child_count; ++i)
		{
			child = child->next;
			if(!child->attrib_count) continue;

			for(i32 i = 0; i < child->attrib_count; ++i)
			{
				struct XML_Attrib *attrib = child->attribs + i;
				if(cstr_eql(attrib->name, name) && cstr_eql(attrib->value, value))
				{
					result = child;
					break;
				}
			}
		}
	}

	return(result);
}

struct XML_Node *
xml_node_find_by_name(struct XML_Node *node, char *name)
{
	struct XML_Node *result = NULL;

	if(!node) return(result);
	if(!node->child_count) return(result);

	result = xml_node_child_find_by_name(node, name);

	if(!result)
	{
		struct XML_Node *child = node->child_first;
		result = xml_node_find_by_name(child, name);

		if(!result)
		{
			for(i32 i = 1; i < node->child_count; ++i)
			{
				child = child->next;
				result = xml_node_find_by_name(child, name);
				if(result) break;
			}
		}
	}

	return(result);
}

struct XML_Node *
xml_node_find_by_attrib(struct XML_Node *node, char *name, char *value)
{
	struct XML_Node *result = NULL;

	if(!node) return(result);
	if(!node->child_count) return(result);

	result = xml_node_child_find_by_attrib(node, name, value);

	if(!result)
	{
		struct XML_Node *child = node->child_first;
		result = xml_node_find_by_attrib(child, name, value);

		if(!result)
		{
			for(i32 i = 1; i < node->child_count; ++i)
			{
				child = child->next;
				result = xml_node_find_by_attrib(child, name, value);
				if(result) break;
			}
		}
	}

	return(result);
}

char *
xml_attrib_value(struct XML_Node *node, char *name)
{
	char *result = NULL;
	if(!node->attrib_count) return(result);

	for(i32 i = 0; i < node->attrib_count; ++i)
	{
		struct XML_Attrib *attrib = node->attribs + i;
		if(cstr_eql(attrib->name, name))
		{
			result = attrib->value;
		}
	}

	return(result);
}
