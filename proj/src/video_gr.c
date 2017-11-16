#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "video_gr.h"
#include "read_xpm.h"
#include "keyboard.h"
#include "timer.h"


/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */

/* Private global variables */

typedef struct {
	char *buf;
	size_t size;
} p_Buffer;

static unsigned bufSize = 0;
static unsigned bufCapacity = 0;
static p_Buffer *secBufs = NULL;
static p_Buffer *primBufs = NULL;

// Following pointers are elements of secBufs and primBufs.
static char *sec_buffer = NULL;
static char *video_mem = NULL; /* Process address to which VRAM is mapped */

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned char background_color = 0; // Black by default.


static unsigned mrSize = 0;
static unsigned mrCapacity = 0;
static struct mem_range *memRanges;


void initBufs() {
	bufCapacity = 2;
	primBufs = malloc(bufCapacity * sizeof(p_Buffer));
	secBufs = malloc(bufCapacity * sizeof(p_Buffer));
}

p_Buffer* getSecBufferFromArray(unsigned int vram_size) {
	for (size_t ind = 0; ind < bufSize; ++ind) {
		if (vram_size == secBufs[ind].size) {
			return &secBufs[ind];
		}
	}

	return NULL;
}

p_Buffer* getPrimaryBufferFromArray(unsigned int vram_size) {
	for (size_t ind = 0; ind < bufSize; ++ind) {
		if (vram_size == primBufs[ind].size) {
			return &primBufs[ind];
		}
	}

	return NULL;
}

int newBuffer(struct mem_range *mr, unsigned int vram_size) {
	++bufSize;
	if (bufCapacity < bufSize) {
		p_Buffer *temp;
		temp = realloc(primBufs, bufSize * sizeof(p_Buffer));
		if (temp == NULL) {
			printf("newBuffer(): realloc() failure\n");
			return -1;
		}
		primBufs = temp;
		temp = realloc(secBufs, bufSize * sizeof(p_Buffer));
		if (temp == NULL) {
			printf("newBuffer(): realloc() failure\n");
			return -1;
		}
		secBufs = temp;
		bufCapacity = bufSize;
	}
	primBufs[bufSize - 1].buf = vm_map_phys(SELF, (void *) mr->mr_base, vram_size);
	if (primBufs[bufSize - 1].buf == MAP_FAILED)
		panic("newBuffer(): Couldn't map video memory");
	primBufs[bufSize - 1].size = vram_size;
	secBufs[bufSize - 1].buf = malloc(vram_size);
	secBufs[bufSize - 1].size = vram_size;

//	printf("video_mem = 0x%x\n", video_mem);

	return OK;
}


void initMemRange() {
	mrCapacity = 2;
	memRanges = malloc(mrCapacity * sizeof(struct mem_range));
}

//	mr.mr_base = (phys_bytes) vram_base;
//	mr.mr_limit = mr.mr_base + vram_size;
struct mem_range* getMemRange(unsigned int vram_base, unsigned int vram_size) {
	for (size_t ind = 0; ind < mrSize; ++ind) {
		if (memRanges[ind].mr_base == (phys_bytes) vram_base
				&& memRanges[ind].mr_limit == vram_base + vram_size) {
			return &memRanges[ind];
		}
	}

	return NULL;
}

struct mem_range* newMemRange(unsigned int vram_base, unsigned int vram_size) {
	printf("In newMemRange()\n");
	struct mem_range *mr = malloc(sizeof(struct mem_range));
	mr->mr_base = (phys_bytes) vram_base;
	mr->mr_limit = mr->mr_base + vram_size;
//	printf("mr_base = %x | mr_limit = %x\n", mr->mr_base, mr->mr_limit);

	++mrSize;
	if (mrSize > mrCapacity) {
		memRanges = realloc(memRanges, (mrCapacity + 1) * sizeof(struct mem_range));
		++mrCapacity;
	}
	memRanges[mrSize - 1] = *mr;
	int response;
	if (OK != (response = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &(memRanges[mrSize - 1]))))
			panic("sys_privctl (ADD_MEM) failed: %d\n", response);
	free(mr);

	return &(memRanges[mrSize - 1]);
}


void* initLowArea() {
	return lm_init();
}

unsigned getHorRes() {
	return h_res;
}

unsigned getVertRes() {
	return v_res;
}

void* getVirtualAddress() {
	return (void *) video_mem;
}

char* getSecBuffer() {
	return sec_buffer;
}

unsigned char vg_getBackground() {
	return background_color;
}

void vg_updateMainBuffer() {
	unsigned vram_size = h_res * v_res * bits_per_pixel / 8;
	memcpy(video_mem, sec_buffer, vram_size);
}

void vg_setBackground(unsigned char back_color) {
	background_color = back_color;
	char *vPtr = sec_buffer;
	for (size_t pixel = 0; pixel < h_res * v_res; ++pixel) {
		*vPtr = back_color;
		++vPtr;
	}
}

void vg_init_background(unsigned short mode, unsigned char back_color) {
	vg_init(mode);
	vg_setBackground(back_color);
}


void *vg_init(unsigned short mode) {
	if (mrCapacity == 0) {
		initMemRange();
	}
	mmap_t map;
	vbe_mode_info_t infoStruct;
	vbe_get_mode_info(mode, &map);
	infoStruct = *((vbe_mode_info_t *) map.virtual);
	h_res = infoStruct.XResolution;
	v_res = infoStruct.YResolution;
	bits_per_pixel = infoStruct.BitsPerPixel;
	unsigned int vram_base = infoStruct.PhysBasePtr;
	unsigned int vram_size = h_res * v_res * bits_per_pixel / 8;

	struct reg86u r;

	r.u.b.intno = INT_VIDEO_CARD;
	r.u.b.ah = VBE_FUNC;
	r.u.b.al = SET_VBE_MODE;
	r.u.w.bx = LINEAR_MODEL_BIT | mode;

	if (sys_int86(&r) != OK) {
		printf("vg_init(): sys_int86() failed \n");
		return NULL;
	}

	/* Allow memory mapping */
//	struct mem_range mr;
//	printf("Before getMemRange()\n");
	struct mem_range *mr = getMemRange(vram_base, vram_size);
//	printf("After getMemRange()\n");
	if (mr == NULL) {
		mr = newMemRange(vram_base, vram_size);
//		printf("After newMemRange()\n");
	}
//	printf("mr_base = %x | mr_limit = %x\n", mr->mr_base, mr->mr_limit);
//	printf("mr addr = %x\n", mr);
//	mr.mr_base = (phys_bytes) vram_base;
//	mr.mr_limit = mr.mr_base + vram_size;

	/* Map memory */
	p_Buffer *primBuf, *secBuf;

	primBuf = getPrimaryBufferFromArray(vram_size);
	if (primBuf == NULL) {
		if (-1 == newBuffer(mr, vram_size)) {
			printf("vg_init(): newBuffer() failure\n");
			return NULL;
		}
		primBuf = getPrimaryBufferFromArray(vram_size);
		if (primBuf == NULL) {
			printf("vg_init(): primBuf init failure\n");
			return NULL;
		}
	}
//	printf("After primBuf init\n");

	secBuf = getSecBufferFromArray(vram_size);
	if (secBuf == NULL) {
		printf("vg_init(): getSecBufferFromArray() failure\n");
		return NULL;
	}
//	printf("After secBuf init\n");

//	printf("primBuf = 0x%x | secBuf = 0x%x | video_mem = 0x%x | sec_buffer = 0x%x\n", primBuf, secBuf, video_mem, sec_buffer);
//	printf("primBuf->buf = 0x%x | secBuf->buf = 0x%x\n", primBuf->buf, secBuf->buf);

	video_mem = primBuf->buf;
	sec_buffer = secBuf->buf;
	vg_clear_screen();
//	printf("primBuf = 0x%x | secBuf = 0x%x | video_mem = 0x%x | sec_buffer = 0x%x\n", primBuf, secBuf, video_mem, sec_buffer);

//	video_mem = vm_map_phys(SELF, (void *) mr->mr_base, vram_size);
//	printf("video_mem = 0x%x\n", video_mem);
//	if (video_mem == MAP_FAILED)
//			panic("couldn't map video memory");
//	printf("sec_buffer = 0x%x | VRAM Size = %d\n", sec_buffer, vram_size);
//	char *temp = realloc(sec_buffer, vram_size);
//	if (temp != NULL) {
//		sec_buffer = temp;
//	} else {
//		printf("Realloc failure\n");
//	}
//	printf("After sec_buffer\n");

	lm_free(&map);
	return (void *) video_mem;
}

// Changed to free secondary buffer.
int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = INT_VIDEO_CARD; /* BIOS video services */

	reg86.u.b.ah = 0x00; /* Set Video Mode function */
	reg86.u.b.al = 0x03; /* 80x25 text mode*/

	if (sys_int86(&reg86) != OK) {
		printf("vg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return OK;
}

int vg_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {

	unsigned short x_original = x, y_original = y;
	char *vPtr = sec_buffer + h_res * y + x_original;
	for (size_t numLines = 0; numLines < size + y_original; ++numLines) {
		for (size_t numCols = 0; numCols < size + x_original; ++numCols) {
			*vPtr = color;
			++vPtr;
		}
		vPtr = sec_buffer + h_res * y + x_original;
		++y;
	}

	return OK;
}

short round(double num) {
	double decimalPart = num - floor(num);
	if (decimalPart >= 0.5)
		return num + 1;
	else
		return num;
}

int vg_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, unsigned long color) {
	char *vPtr = sec_buffer + h_res * yi + xi;
	short dx, dy, steps;
	double xIncrement, yIncrement;
	double x, y, temp;

	x = xi;
	y = yi;
	dx = xf - xi;
	dy = yf - yi;

	if (abs(dx) > abs(dy))
		steps = abs(dx);
	else
		steps = abs(dy);

	xIncrement = dx / (float) steps;
	yIncrement = dy / (float) steps;

	for (int v = 0; v < steps; v++) {
		x = x + xIncrement;
		y = y + yIncrement;
		vPtr = sec_buffer + h_res * round(y) + round(x);
		*vPtr = color;
	}

	return OK;
}

int vg_draw_xpm(short xi, short yi, char *xpm[]) {
	int width, height;
	char *pixmap = read_xpm(xpm, &width, &height);
	char *vPtr = sec_buffer + h_res * yi + xi;
	for (size_t line = 0; line < height; ++line) {
		for (size_t column = 0; column < width; ++column) {
			*vPtr = *pixmap;
			++vPtr;
			++pixmap;
		}
		vPtr = sec_buffer + h_res * (yi + line) + xi;
	}

	return OK;
}
int vg_draw_xpm_2b(short xi, short yi, char *xpm[]) {
	int width, height;
	char *pixmap = read_xpm(xpm, &width, &height);
	unsigned long colour;
	char *vPtr = sec_buffer + (h_res * yi + xi) * 2;
	for (size_t line = 0; line < height; ++line) {
		for (size_t column = 0; column < width; ++column) {
			if (*pixmap == 0) {
				*vPtr = 0x00;
			} else {
				*vPtr = 0xFF;
			}

			vPtr++;
			++pixmap;
		}
		vPtr = sec_buffer + (h_res * (yi + line) + xi) * 2;
	}

	return OK;
}

int vg_clear_screen() {
	memset(sec_buffer, background_color, h_res * v_res);
	return OK;
}

int vg_clear_xpm(short xi, short yi, char *xpm[]) {
	int width, height;
	char *pixmap = read_xpm(xpm, &width, &height);
	char *vPtr = sec_buffer + h_res * yi + xi;
	for (size_t line = 0; line < height; ++line) {
		for (size_t column = 0; column < width; ++column) {
			*vPtr = background_color;
			++vPtr;
			++pixmap;
		}
		vPtr = sec_buffer + h_res * (yi + line) + xi;
	}

	return OK;
}

int vg_move_xpm(short xi, short yi, char *xpm[],
		unsigned short hor, short delta, double time) {
	bool isHorizontal = (hor != 0);
	if (time == 0) {
		short xf = xi, yf = yi;
		if (isHorizontal) {
			xf = xi + delta;
		} else {
			yf = yi + delta;
		}
		vg_clear_xpm(xi, yi, xpm);
		vg_draw_xpm(xf, yf, xpm);
		return OK;
	}
	long numFrames = FRAMES_PER_SEC * time;
	double timePerFrame = time / numFrames;
	double pixelsPerFrame = (double) abs(delta) / numFrames;
	short x = xi, y = yi;
	double exact_x = xi, exact_y = yi;
	vg_draw_xpm(xi, yi, xpm);
	for (size_t frame = 0; frame < numFrames; ++frame) {
		vg_clear_xpm(x, y, xpm);
		if (isHorizontal) {
			if (delta > 0)
				exact_x += pixelsPerFrame;
			else
				exact_x -= pixelsPerFrame;
		} else {
			if (delta > 0)
				exact_y += pixelsPerFrame;
			else
				exact_y -= pixelsPerFrame;
		}
		x = round(exact_x);
		y = round(exact_y);
		//TODO: Screen not using all pixels.
		if (x < 0 || x >= getHorRes() - 60 || y < 0 || y >= getVertRes() - 25) {
			return OUT_BOUNDS;
		}
		vg_draw_xpm(x, y, xpm);
		if (kbd_pressed_esc()) {
			return ESC_RELEASED;
		}
		waitByTimer0(timePerFrame);
	}

	return OK;
}

VbeInfoBlock* vg_control_info() {
	struct reg86u r;
	mmap_t mode_map;
	VbeInfoBlock *controllerStruct = lm_alloc(sizeof(VbeInfoBlock), &mode_map);

	r.u.b.intno = INT_VIDEO_CARD;
	r.u.b.ah = VBE_FUNC;
	r.u.b.al = RET_VBE_CONTROLLER;
	r.u.w.es = PB2BASE(mode_map.phys);
	r.u.w.di = PB2OFF(mode_map.phys);

	if (sys_int86(&r) != OK) {
		printf("vg_control_info(): sys_int86() failed \n");
		return (VbeInfoBlock *) NULL;
	}

	*controllerStruct = *((VbeInfoBlock *) mode_map.virtual);
	return controllerStruct;
}

long* vg_getPalette() {
	mmap_t map;
	long *colors = malloc(256 * 4);
	long *realColors = lm_alloc(256 * 4, &map);

	// Switch DAC to 8 bits per primary color.

	struct reg86u regDAC;
	regDAC.u.b.intno = INT_VIDEO_CARD;
	regDAC.u.b.ah = VBE_FUNC;
	regDAC.u.b.al = PALETTE_FORMAT;
	regDAC.u.b.bl = SET_PALETTE_FORMAT;
	regDAC.u.b.bh = 8;

	if (sys_int86(&regDAC) != OK) {
		printf("vg_getPalette(): sys_int86() failed \n");
		return (long *) NULL;
	}

	// Retrieve palette.

	struct reg86u regPal;
	regPal.u.b.intno = INT_VIDEO_CARD;
	regPal.u.b.ah = VBE_FUNC;
	regPal.u.b.al = PALETTE_DATA;
	regPal.u.b.bl = GET_PALETTE_DATA;
	regPal.u.w.cx = 0;
	regPal.u.w.dx = 0;
	regPal.u.w.es = PB2BASE(map.phys);
	regPal.u.w.di = PB2OFF(map.phys);

	if (sys_int86(&regPal) != OK) {
		printf("vg_getPalette(): sys_int86() failed \n");
		return (long *) NULL;
	}

	for (size_t ind = 0; ind < 256; ++ind) {
		colors[ind] = realColors[ind];
	}

	// Switch DAC to original 6 bits per primary color.
	regDAC.u.b.bh = 6;
	if (sys_int86(&regDAC) != OK) {
		printf("vg_getPalette(): sys_int86() failed \n");
		return (long *) NULL;
	}

	lm_free(&map);
	return colors;
}

void vg_printPalette(long *colors, int numColors) {
	printf("Alignment | Red | Green | Blue\n");
	for (size_t colInd = 0; colInd < numColors; ++colInd) {
		printf("%d = 0x%x", colInd, colors[colInd]);
		if (colInd % 7 == 0)
			printf("\n");
		else
			printf(" | ");
	}
}
