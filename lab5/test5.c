#include "test5.h"

void *test_init(unsigned short mode, unsigned short delay) {
	vbe_mode_info_t infoStruct;
	vbe_get_mode_info(mode, &infoStruct);
	void *physAddr = infoStruct.PhysBasePtr;
	void *virtAddr = vg_init(mode);
	waitByTimer0(delay);
	if (vg_exit() != OK)
		return NULL;
	if (virtAddr == NULL) {
		return NULL;
	}
	printf("Physical VRAM: %x\n", physAddr);
	return virtAddr;
}

int test_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {
	void *virtAddr = vg_init(MODE_768_256);
	if (virtAddr == NULL) {
		vg_exit();
		return -1;
	}

	vg_square(x, y, size, color);
	waitByKey(ESC_BREAK);

	if (vg_exit() != OK) {
		return -1;
	}

	return OK;
}

int test_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, unsigned long color) {
	void *virtAddr = vg_init(MODE_768_256);
	if (virtAddr == NULL) {
		vg_exit();
		return -1;
	}
	vg_line(xi, yi, xf, yf, color);
	waitByKey(ESC_BREAK);
	if (vg_exit() != OK) {
		return -1;
	}

	return OK;
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	void *virtAddr = vg_init(MODE_768_256);
	if (virtAddr == NULL) {
		vg_exit();
		return -1;
	}
	vg_draw_xpm(xi, yi, xpm);
	waitByKey(ESC_BREAK);
	if (vg_exit() != OK) {
		return -1;
	}

	return OK;
}

int test_move(unsigned short xi, unsigned short yi, char *xpm[],
		unsigned short hor, short delta, unsigned short time) {
	void *virtAddr = vg_init(MODE_768_256);
	if (virtAddr == NULL) {
		vg_exit();
		return -1;
	}
	vg_move_xpm(xi, yi, xpm, hor, delta, time);
	waitByKey(ESC_BREAK);
	if (vg_exit() != OK) {
		return -1;
	}

	return OK;

}

int test_controller() {
	short *virt_firstMegAddr = lm_init();
	VbeInfoBlock *infoStruct = vg_control_info();

	long capabilities = infoStruct->Capabilities;
	printf("Capabilities:\n");

	printf("- ");
	if (capabilities & BIT(0))
		printf("DAC width is switchable to 8 bits per primary color.\n");
	else
		printf("DAC width is fixed, with 6 bits per primary color.\n");

	printf("- ");
	if (capabilities & BIT(1))
		printf("Controller is not VGA compatible.\n");
	else
		printf("Controller is VGA compatible.\n");

	printf("- ");
	if (capabilities & BIT(2)) {
		printf("When programming large blocks of information to the RAMDAC, ");
        printf("use the blank bit in Function 09h.\n");
	} else
		printf("Normal RAMDAC operation.\n");

	unsigned long vram_size = infoStruct->TotalMemory * 64;
	printf("VRAM size: %d\n", vram_size);

	short *videoModePtr = (short *) infoStruct->VideoModePtr;
	videoModePtr = REAL_TO_PROTECT(videoModePtr);
	videoModePtr = (short *) ((long) videoModePtr + (long) virt_firstMegAddr);

	printf("Supported modes:\n");

	unsigned iterNum = 1;
	if (*videoModePtr != -1) {
		printf("-");
	}
	while (*videoModePtr != -1) {
		printf(" 0x%x", *videoModePtr);
		++videoModePtr;
		if (iterNum % 5 == 0) {
			printf("\n");
			if (*videoModePtr != -1) {
				printf("-");
			}
		} else if (*videoModePtr != -1) {
			printf(", ");
		}
		++iterNum;
	}
	printf("\n");

	return OK;
}

