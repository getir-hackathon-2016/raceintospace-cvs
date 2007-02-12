#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include "gamedata.h"

int vflag;

int
RLED(void *src_raw, void *dest_raw, unsigned int src_size)
{
	signed char *src = src_raw;
	signed char *dest = dest_raw;
	unsigned short used;
	short count, val;
	short i;

	used = 0;
	while (used < src_size)
	{
		count = src[used++];

		if (count < 0)
		{
			count = -(count - 1);
			val = src[used++];

			if (vflag)
				printf("%6d: repeat %02x\n", count, val & 0xff);

			for (i = 0; i < count; i++)
				*dest++ = val;
		}
		else
		{
			count++;

			if (vflag)
				printf("%6d:", count);

			for (i = 0; i < count; i++)
			{
				if (vflag)
					printf(" %02x", src[used] & 0xff);
				*dest++ = src[used++];
			}

			if (vflag)
				printf("\n");
		}
	}

	return ((void *) dest - (void *) dest_raw);
}

void
usage(void)
{
	fprintf(stderr,
		"usage: mknews [-d output_dir] [-b output_basename] totnews.cdr\n");
	exit(EXIT_FAILURE);
}

void
pexit(const char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

#pragma pack(1)
typedef struct aCHART
{
	uint8_t frames;
	uint8_t x_off;
	uint8_t y_off;
	uint8_t width;
	uint8_t height;
} seqInfo;
#pragma pack()

struct news
{
	FILE *fp;
	int anim_idx;
	int frame_idx;
	SimpleHdr *frames;
	seqInfo animations[12];
	int anim_offsets[12];
	unsigned char palette[256 * 3];
};

struct news *
news_open(char *fname)
{
	int i = 0;
	SimpleHdrW headers[12];
	struct news *n = malloc(sizeof *n);

	if (!n)
		pexit("malloc");
	n->fp = fopen(fname, "rb");
	if (!n->fp)
		pexit("fopen");
	if (fread_SimpleHdrW(headers, 12, n->fp) != 12)
		pexit("fread main headers");
	for (i = 0; i < 12; ++i)
	{
		if (0 != fseek(n->fp, headers[i].offset, SEEK_SET))
			pexit("fseek");
		if (1 != fread(&n->animations[i], sizeof(seqInfo), 1, n->fp))
			pexit("fread animations");

		n->anim_offsets[i] = headers[i].offset;
	}
	n->anim_idx = -1;
	n->frame_idx = -1;
	n->frames = NULL;
	return n;
}

int
news_next_animation(struct news *n, int *x, int *y, int *w, int *h)
{
	int idx, nframes, to_read;

	assert(n);
	idx = n->anim_idx + 1;
	if (idx >= 12)
		return 0;
	nframes = n->animations[idx].frames;
	n->frames = realloc(n->frames, nframes * sizeof(*n->frames));
	if (!n->frames)
		pexit("realloc");
	if (0 != fseek(n->fp, n->anim_offsets[idx] + sizeof(seqInfo), SEEK_SET))
		pexit("fseek anim_offset");
	if (nframes != fread_SimpleHdr(n->frames, nframes, n->fp))
		pexit("fread frame info");
	memset(n->palette, 0, sizeof(n->palette));
	to_read = (256 - 32) * 3;
	if (to_read != fread(&n->palette[32 * 3], 1, to_read, n->fp))
		pexit("fread palette");
	n->anim_idx = idx;
	n->frame_idx = -1;
	if (x)
		*x = n->animations[idx].x_off;
	if (y)
		*y = n->animations[idx].y_off;
	if (w)
		*w = n->animations[idx].width;
	if (h)
		*h = n->animations[idx].height;
	return nframes;
}

int
news_next_frame(struct news *n, char *data)
{
	int aidx, fidx, nframes, fsize, fusize;
	char tmp[64000];

	assert(n);
	assert(data);
	aidx = n->anim_idx;
	if (aidx >= 12)
		return -1;
	nframes = n->animations[aidx].frames;
	fidx = n->frame_idx + 1;
	if (fidx >= nframes)
		return 0;
	n->frame_idx = fidx;
	fsize = n->frames[fidx].size;
	if (fsize > 64000)
		pexit("something wrong with size");
	if (0 != fseek(n->fp,
			n->anim_offsets[aidx] + n->frames[fidx].offset, SEEK_SET))
		pexit("fseek frame");
	if (fsize != fread(tmp, 1, fsize, n->fp))
		pexit("fread frame");
	fusize = RLED(tmp, data, fsize);
	return fusize;
}

void
news_close(struct news *n)
{
	assert(n);
}

char *dirname = ".";
char *basename = "";

void
write_image(char *data, int width, int height, unsigned char *palette,
	int anim_no, int frame_no)
{
	FILE *fp;
	char fname[1000];
	unsigned char r, g, b, pix;
	int i;
	unsigned char *pp = NULL;

	struct type
	{
		int is_usa;
		int is_bw;
		char *shot;
	} sequence[] =
	{
		{ 1, 1, "opening"},
		{ 1, 0, "opening"},
		{ 1, 1, "closing"},
		{ 1, 0, "closing"},
		{ 0, 0, "opening"},
		{ 0, 1, "opening"},
		{ 0, 0, "closing"},
		{ 0, 1, "closing"},
		{ 1, 1, "angle"},
		{ 1, 0, "angle"},
		{ 0, 0, "angle"},
		{ 0, 1, "angle"}
	};

	/* who the hell designed these indices?? */
	sprintf(fname, "%s/%s%s%s_%s_%s_%02d.ppm", dirname, basename,
		(basename[0] ? "_" : ""),
		(sequence[anim_no].is_usa ? "usa" : "sov"),
		(sequence[anim_no].is_bw  ? "bw"  : "col"),
		sequence[anim_no].shot, frame_no);

	fp = fopen(fname, "wt");

	if (!fp)
		pexit("fopen image (w)");

	fprintf(fp, "P6\n%d %d\n255\n", width, height);

	for (i = 0; i < width * height; ++i)
	{
		pix = data[i];
		pp = &palette[pix * 3];

		r = pp[0] * 4;
		g = pp[1] * 4;
		b = pp[2] * 4;

		putc(r, fp);
		putc(g, fp);
		putc(b, fp);
	}

	fclose(fp);
}

int
main(int argc, char **argv)
{
	char *filename;
	struct news *news;
	int num_frames, frame_size;
	int anim_no;
	int c;
	int w, h, x, y;

	while ((c = getopt(argc, argv, "d:b:")) != EOF)
	{
		switch (c)
		{
			case 'd':
				dirname = optarg;
				break;
			case 'b':
				basename = optarg;
				break;
			default:
				usage();
		}
	}

	if (optind >= argc)
		usage();

	filename = argv[optind++];

	if (optind != argc)
		usage();

	if ((news = news_open(filename)) == NULL)
	{
		fprintf(stderr, "can't open %s\n", filename);
		exit(EXIT_FAILURE);
	}

	anim_no = 0;
	while (0 < (num_frames = news_next_animation(news, &x, &y, &w, &h)))
	{
		/* some magic numbers from the code */
		char img_base[40000];
		char img_add[15000];
		int first_width = 312, first_height = 106;
		int x_off = 4, y_off = 4;
		int xx, yy;
		int frame_no = 0;

		printf("A %2d: (%3d, %2d) %3dx%3d, %2d frames\n",
			anim_no, x, y, w, h, num_frames);

		memset(img_base, 0, sizeof(img_base));

		/* first frame is bigger and presents whole img */
		frame_size = news_next_frame(news, img_base);

		/* further frames are just "xor diffs" */
		for (frame_no = 1; news_next_frame(news, img_add) > 0; ++frame_no)
		{
			for (yy = 0; yy < h; ++yy)
				for (xx = 0; xx < w; ++xx)
				{
					int toff = (y + yy - y_off) * first_width
						+ x + xx - x_off;
					img_base[toff] ^= img_add[yy * w + xx];
				}

			write_image(img_base, first_width, first_height,
				news->palette, anim_no, frame_no);

		}

		anim_no++;
	}

	news_close(news);

	return (EXIT_SUCCESS);
}
