#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

int vbe_get_mode_info(unsigned short mode, mmap_t *map) {
	struct reg86u r;
	lm_alloc(sizeof(vbe_mode_info_t), map);

	r.u.b.intno = INT_VIDEO_CARD;
	r.u.b.ah = VBE_FUNC;
	r.u.b.al = RET_VBE_INFO;
	r.u.w.cx = mode;
	r.u.w.es = PB2BASE(map->phys);
	r.u.w.di = PB2OFF(map->phys);

	if (sys_int86(&r) != OK) {
		printf("vbe_get_mode_info(): sys_int86() failed \n");
		return -1;
	}

	return OK;
}
