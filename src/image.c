#pragma pack(push, 1)
#pragma pack(1)
struct ImageBMPFileHeader
{
	u16 type;
	u32 size;
	u16 reserved_1;
	u16 reserved_2;
	u32 offset;
};

struct ImageBMPInfoHeader
{
	u32 size;
	u32 width;
	u32 height;
	u16 planes;
	u16 bpp;
	u32 compression;
	u32 image_size;

	u32 x_res;
	u32 y_res;

	u32 color_count;
	u32 color_important;

	u32 mask_red;
	u32 mask_green;
	u32 mask_blue;
};

struct ImageBMPHeader
{
	struct ImageBMPFileHeader file;
	struct ImageBMPInfoHeader info;
};
#pragma pack(pop)

typedef struct Image
{
	i32 width;
	i32 height;
	u32 pitch;
	u32 size;
	void *data;
} Image;

internal i32
img_bmp_load(Image *image, char *path)
{
	FILE *file = fopen(path, "rb");
	if(!file)
	{
		printf("[ERROR]: FAILED TO LOAD BMP FILE - %s\n", path);
		return(-1);
	}

	struct ImageBMPHeader header;
	fread(&header, sizeof(header), 1, file);

	image->width = header.info.width;
	image->height = header.info.height;
	image->pitch = image->width * 4;
	image->size = header.info.image_size;
	image->data = mem_alloc(1, image->size, false);

	fseek(file, header.file.offset, SEEK_SET);
	fread(image->data, image->size, 1, file);

	fclose(file);
	return(1);
}

internal void
img_load(Image *image, char *path)
{
	char *extension = cstr_path_extension(path);

	if(cstr_eql(extension, "bmp")) img_bmp_load(image, path);
	else ERROR("%s", "img_load: wrong enxtension!\n");
	//elif(str_eql(extension, STR("png"))) img_png_alloc(path);
}

internal i32
img_unload(Image *image)
{
	image->width = 0;
	image->height = 0;
	image->pitch = 0;
	image->size = 0;
	mem_free(image->data);
	image->data = NULL;

	return(1);
}
