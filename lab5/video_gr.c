#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "video_gr.h"
#include "read_xpm.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */

/* Private global variables */

static char *video_mem; /* Process address to which VRAM is mapped */

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned char background_color = 0; // Black by default.

void* getVirtualAddress() {
	return (void *) video_mem;
}

unsigned char vg_getBackground() {
	return background_color;
}

void vg_setBackground(unsigned char back_color) {
	background_color = back_color;
	char *vPtr = video_mem;
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
	lm_init();

	struct mem_range mr;
	vbe_mode_info_t infoStruct;
	vbe_get_mode_info(mode, &infoStruct);
	h_res = infoStruct.XResolution;
	v_res = infoStruct.YResolution;
	bits_per_pixel = infoStruct.BitsPerPixel;
	unsigned int vram_base = infoStruct.PhysBasePtr;
	unsigned int vram_size = h_res * v_res * bits_per_pixel;

	struct reg86u r;
	int response;

	r.u.b.intno = INT_VIDEO_CARD;
	r.u.b.ah = VBE_FUNC;
	r.u.b.al = SET_VBE_MODE;
	r.u.w.bx = LINEAR_MODEL_BIT | mode;

	if (sys_int86(&r) != OK) {
		printf("vg_init(): sys_int86() failed \n");
		return NULL;
	}

	/* Allow memory mapping */
	mr.mr_base = (phys_bytes) vram_base;
	mr.mr_limit = mr.mr_base + vram_size;

	if (OK != (response = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", response);

	/* Map memory */
	video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

	if (video_mem == MAP_FAILED)
		panic("couldn’t map video memory");

	return (void *) video_mem;
}

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
	char *vPtr = video_mem + h_res * y + x_original;
	for (size_t numLines = 0; numLines < size + y_original; ++numLines) {
		for (size_t numCols = 0; numCols < size + x_original; ++numCols) {
			*vPtr = color;
			++vPtr;
		}
		vPtr = video_mem + h_res * y + x_original;
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
	char *vPtr = video_mem + h_res * yi + xi;
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
		vPtr = video_mem + h_res * round(y) + round(x);
		*vPtr = color;
	}

	return OK;
}

int vg_draw_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	int width, height;
	char *pixmap = read_xpm(xpm, &width, &height);
	char *vPtr = video_mem + h_res * yi + xi;
	for (size_t line = 0; line < height; ++line) {
		for (size_t column = 0; column < width; ++column) {
			*vPtr = *pixmap;
			++vPtr;
			++pixmap;
		}
		vPtr = video_mem + h_res * (yi + line) + xi;
	}

	return OK;
}

int vg_clear_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	int width, height;
	char *pixmap = read_xpm(xpm, &width, &height);
	char *vPtr = video_mem + h_res * yi + xi;
	for (size_t line = 0; line < height; ++line) {
		for (size_t column = 0; column < width; ++column) {
			*vPtr = background_color;
			++vPtr;
			++pixmap;
		}
		vPtr = video_mem + h_res * (yi + line) + xi;
	}

	return OK;
}

int vg_move_xpm(unsigned short xi, unsigned short yi, char *xpm[],
		unsigned short hor, short delta, unsigned short time) {
	bool isHorizontal = (hor != 0);
	if (time == 0) {
		unsigned short xf = xi, yf = yi;
		if (isHorizontal) {
			xf = xi + delta;
		} else {
			yf = yi + delta;
		}
		vg_draw_xpm(xf, yf, xpm);
		return OK;
	}
	long numFrames = FRAMES_PER_SEC * time;
	double timePerFrame = (double) time / numFrames;
	double pixelsPerFrame = (double) abs(delta) / numFrames;
	unsigned short x = xi, y = yi;
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
		vg_draw_xpm(x, y, xpm);
		if (kbd_pressed_esc()) {
			break;
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

