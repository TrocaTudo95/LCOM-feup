.intel_syntax

#include "i8042.h"


.global _asm_kbd_int_handler
.global _scanCode

.bss
_scanCode:
	.byte

.text
_asm_kbd_int_handler:
	in AL, OUT_BUF
	mov _scanCode, AL
	ret
