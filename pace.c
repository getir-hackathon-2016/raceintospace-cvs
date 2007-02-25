#include "Buzz_inc.h"
#include "externs.h"
#include "assert.h"
#include "pace.h"
#include "av.h"
#include "options.h"
#include "utils.h"

extern GXHEADER vhptr;

void seq_init (void);

char
DoModem(int sel)
{
	printf ("DoModem not implemented\n");
	return (0);
}

char
MPrefs(char mode) 
{
	return (0);
}

struct Prest_Upd MP[3];

int put_serial(unsigned char n) {return 0;}
void MesCenter(void){}

int
AquireDrive (void)
{
	return 'K' - 'A';
}

char *letter_dat;

void
OpenEmUp(void)
{
	randomize();

	seq_init ();

	GV(&vhptr,320,200);     // Allocate only Virtual Buffer

	XMAS=1; /* we do have a mouse */

	letter_dat = slurp_gamedat ("letter.dat");
}

int
PCX_D (void *src_raw,void *dest_raw,unsigned src_size)
{
  char *src = src_raw;
  char *dest = dest_raw;
  char num;
  char *orig_dest = dest;
  do {
    if ((*src&0xc0)==0xc0) {
      num=*(src++)&0x3f; src_size--;
      while ((num--)>0) {*(dest++)=*src;}
      src++;src_size--;
    } else {
      (*dest++)=*(src++);src_size--;
    };
  }  while(src_size);
  return (dest - orig_dest);
}

int
RLED (void *src_raw, void *dest_raw, unsigned int src_size)
{
	signed char *src = src_raw;
	signed char *dest = dest_raw;
	unsigned int used;
	int count, val;
	int i;

	used = 0;
	while (used < src_size) {
		count = src[used++];
		
		if (count < 0) {
			count = -count + 1;
			val = src[used++];
			for (i = 0; i < count; i++)
				*dest++ = val;
		} else {
			count++;
			for (i = 0; i < count; i++)
				*dest++ = src[used++];
		}
	}

	if (0)
		printf ("total bytes %d\n", (void *)dest - (void *)dest_raw);

	return ((void *)dest - (void *)dest_raw);
}

int
RLED_img (void *src_raw, void *dest_raw, unsigned int src_size, int w, int h)
{
	signed char *src = src_raw;
	signed char *dest;
	unsigned int used;
	int count, val;
	int total;
	signed char buf[128 * 1024];
	int row;

	dest = buf;

	used = 0;
	while (used < src_size) {
		count = src[used++];
		
		if (count < 0) {
			count = -count + 1;
			val = src[used++];
            memset(dest, val, count);
            dest += count;
		} else {
			count++;
            memcpy(dest, &src[used], count);
            used += count;
            dest += count;
		}
	}

	total = dest - buf;

	if (total < w * h + h) {
		memcpy (dest_raw, buf, w * h);
		return (w * h);
	}

	dest = dest_raw;
	for (row = 0; row < h; row++) {
        memcpy(dest, &buf[row * (w+1)], w);
        dest += w;
	}

	return (w * h);
}

/*
 * Original sources say:
 * @param wh = 0 - fade colors from 0 to val,
 *           = 1 - fade colors from val to 3*256,
 *           = 2 - fade all colors
 * @param palx pointer to palette
 * @param steps
 * @param val pivot index in palette array
 * @param mode if mode == 1 then preserve non-faded colors, else make black
 */
void
FadeIn (char wh,char *palx,int steps,int val,char mode)
{
    int from = 0;
    int to = 256;
    if (wh == 0)
        to = val;
    else if (wh == 1)
        from = val;
    else
        assert(wh == 2);
    av_set_fading(AV_FADE_IN, from, to, steps, !!mode);
}

void
FadeOut(char wh,char *palx,int steps,int val,char mode)
{
    int from = 0;
    int to = 256;

    if (wh == 0)
        to = val;
    else if (wh == 1)
        from = val;
    else
        assert(wh == 2);
    av_set_fading(AV_FADE_OUT, from, to, steps, !!mode);
}

void
delay (int millisecs)
{
	idle_loop_secs (millisecs / 1000.0);
}

void
bzdelay (int ticks)
{
	idle_loop_secs (ticks / 100.0);
}

int
CDAccess (int drive,int track,char op)
{
	return (0);
}

int
brandom (int limit)
{
	if (limit == 0)
		return (0);
	return (int) (limit * (rand() / (RAND_MAX + 1.0)));
}

long RLEC (char *src, char *dest, unsigned int src_size)
{
	unsigned int src_i;
	int dest_i,cpr;

	for (src_i = dest_i=0; src_i< src_size; ) {
		int k;  /* holds the number of characters to copy or repeat. */
		k = 0;
		cpr= ((src_size-src_i-1)<128) ? src_size-src_i-1 : 128;
		while (k<cpr && src[src_i] != src[src_i+1]) {
			k++;       /* increment the number of characters to copy */
			src_i++;   /* move pointer to the next character */
		}
		if (k) {
			dest[dest_i++] = (k-1);
			memcpy (&dest[dest_i], &src[src_i-k], k);
			dest_i += k;
		} else {
			k = 2;   /* there are at least two characters to be repeated */
			src_i++; /* advance pointer beyond the first match*/
			while ( k<cpr && src[src_i] == src[src_i+1]) {
				k++;       /* increment the number of characters to copy */
				src_i++;   /* move pointer to the next character */
			}  /* while */
			dest[dest_i++] = (-k+1);
			dest[dest_i++] = src[src_i++];
		}
	}
	return (dest_i);
}

void
StopAudio(char mode) 
{
    av_silence(AV_SOUND_CHANNEL);
}

void
CloseEmUp (unsigned char error,unsigned int value)
{
	/* DEBUG */ /* fprintf (stderr, "CloseEmUp()\n"); */
	exit(EXIT_SUCCESS);
}

struct tblinfo {
	int count;
	char **strings;
};

void
frm_read_tbl (char *keyname, struct tblinfo *tbl)
{
	FILE *fin;
	int lo, hi;
	int idx;
    char *p;
	char name[100];

	if ((fin = sOpen (keyname, "rb", 0)) == NULL)
		return;

	lo = getc (fin);
	hi = getc (fin);
	tbl->count = (hi << 8) | lo;
	
	tbl->strings = xcalloc(tbl->count, sizeof *tbl->strings);

	idx = 0;
	while (fread (name, 1, 8, fin) == 8) {
		name[8] = '\0';
        for (p = name; *p; p++) {
            *p = tolower (*p);
            if (*p == '#')
                *p = '_';
        }
		tbl->strings[idx++] = xstrdup (name);
	}

	/* now idx is number of read strings */
	if (tbl->count != idx) {
		tbl->count = idx;
		tbl->strings = xrealloc(tbl->strings, sizeof *tbl->strings);
	}
		
	fclose (fin);
}

struct tblinfo frm_tbl, frm_ftbl;

void
seq_init (void)
{
	frm_read_tbl ("SEQ.KEY", &frm_tbl);
	frm_read_tbl ("FSEQ.KEY", &frm_ftbl);
}

char *
seq_filename (int seq, int mode)
{
	struct tblinfo *tp;

	if (mode == 0)
		tp = &frm_tbl;
	else
		tp = &frm_ftbl;

	if (seq < 0 || seq >= tp->count)
		return (NULL);

	return (tp->strings[seq]);
}

void
SMove (void *p, int x, int y)
{
	GXHEADER local;

	GV (&local, 160, 100);
	memcpy (local.vptr, p, 160 * 100);
	gxPutImage (&local, gxSET, x, y, 0);
	DV (&local);
}

void
LMove (void *p)
{
	GXHEADER local;

	memset (screen, 0, 320 * 200);

	GV (&local, 160, 100);
	memcpy (local.vptr, p, 160 * 100);
	gxPutImage (&local, gxSET, 320 / 4, 200 / 4, 0);
	DV (&local);
}	

void
randomize (void)
{
	srand (get_time() * 1000);
}

void
idle_loop_secs (double secs)
{
	double start;

	gr_maybe_sync ();

	start = get_time ();

	while (1) {
		av_block ();

		if (get_time () - start >= secs)
			break;
	}
}

void
idle_loop (int ticks)
{
	idle_loop_secs (ticks / 2000.0);
}

long VoiceOff;

char *soundbuf;
size_t soundbuf_size = 0;
size_t soundbuf_used = 0;
struct audio_chunk news_chunk;
	
ssize_t
load_audio_file(const char *name, char **data, size_t *size)
{
    mm_file mf;
    unsigned channels, rate;
    const size_t def_size = 16 * 1024;
    size_t offset = 0;
    ssize_t read = 0;
    double start = get_time();

	/* make compiler happy */
	start *= 1.0;

    assert(name);
    assert(data);
    assert(size);

    if (mm_open_fp(&mf, sOpen(name, "rb", FT_AUDIO)) < 0)
        return -1;

    if (mm_audio_info(&mf, &channels, &rate) < 0)
    {
		/* WARN */ fprintf(stderr, "no audio data in file '%s'\n", name);
        mm_close(&mf);
        return -1;
    }

	if (channels != 1 || rate != 11025)
	{
		/* WARN */ fprintf(stderr,
				"file '%s' is not mono, 11025Hz\n", name);
		mm_close(&mf);
		return -1;
	}

    if (!*data)
        *data = xmalloc(*size = def_size);

	while (0 < (read = mm_decode_audio(&mf,
						*data+offset, *size-offset)))
	{
		offset += read;
		if (*size <= offset)
			*data = xrealloc(*data, *size *= 2);
	}

    mm_close(&mf);

#if 0
    /* DEBUG */ fprintf(stderr, "load_audio_file(%s) took %5.4f sec.\n",
            name, get_time() - start);
#endif

    return offset;
}

void
NGetVoice(char plr,char val)
{
    char fname[100];
    ssize_t bytes = 0;

    sprintf(fname, "%s_%03d.ogg", (plr ? "sov" : "usa"), val);
    bytes = load_audio_file(fname, &soundbuf, &soundbuf_size);
    soundbuf_used = (bytes > 0) ? bytes : 0;
}

void
PlayVoice (void)
{
    if (!soundbuf_used)
        return;
	news_chunk.data = soundbuf;
	news_chunk.size = soundbuf_used;
	news_chunk.next = NULL;
	play(&news_chunk, AV_SOUND_CHANNEL);
}

void
KillVoice (void)
{
	av_silence(AV_SOUND_CHANNEL);
}

void
StopVoice (void)
{
	av_silence(AV_SOUND_CHANNEL);
}

void
PlayAudio(char *name, char mode)
{
    ssize_t bytes = 0;
    bytes = load_audio_file(name, &soundbuf, &soundbuf_size);
    soundbuf_used = (bytes > 0) ? bytes : 0;
	PlayVoice();
}

int
getch (void)
{
	int c;

	while (1) {
		av_block ();
		if ((c = bioskey (0)) != 0)
			return (c);
	}
}

void
play_audio (int sidx, int mode)
{
	char filename[1000];
	ssize_t size;

    sprintf(filename, "%s.ogg", seq_filename(sidx,mode));
    size = load_audio_file(filename, &soundbuf, &soundbuf_size);
	soundbuf_used = (size > 0) ? size : 0;
    PlayVoice ();
}

#define debug_file stdout
#if 0
void
vdbg (char const *fmt, va_list args)
{
	char buf[10000];
	char *p;
	struct tm tm;
	struct timeval tv;
	time_t t;
	int binchars;
	static unsigned long last_millitime;
	unsigned long this_millitime;
	int delta;

	gettimeofday (&tv, NULL);
	t = tv.tv_sec;
	tm = *localtime (&t);

	p = buf;

	sprintf (p, "%02d:%02d:%02d.%03d ", tm.tm_hour, tm.tm_min, tm.tm_sec,
		 (int)(tv.tv_usec / 1000));
	p += strlen (p);

	this_millitime = (tm.tm_hour * 3600 + tm.tm_min * 60 + tm.tm_sec) 
		* 1000 + tv.tv_usec / 1000;

	if (last_millitime == 0)
		last_millitime = this_millitime;

	delta = this_millitime - last_millitime;
	last_millitime = this_millitime;

	if (delta < 0)
		delta = 0;

	sprintf (p, "%5d ", delta);
	p += strlen (p);

	vsprintf (p, fmt, args);

	p += strlen (p);
	while (p != buf && (p[-1] == '\n' || p[-1] == '\r'))
		*--p = 0;

	binchars = 0;
	for (p = buf; *p && binchars < 20; p++) {
		int c = *p;
		if ((c >= ' ' && c < 0177) || c == '\t' || c == '\n') {
			putc (c, debug_file);
		} else {
			binchars++;
			putc ('.', debug_file);
		}
	}
	putc ('\n', debug_file);
	fflush (debug_file);
}

void
dbg (char const *fmt, ...)
{
	va_list args;

	va_start (args, fmt);
	vdbg (fmt, args);
	va_end (args);
}
#endif
