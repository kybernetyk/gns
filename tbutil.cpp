#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "tbutil.h"
#include "termbox/src/termbox.h"

void print_tb(const char *str, int x, int y, uint16_t fg, uint16_t bg) {
	while (*str) {
		uint32_t uni;
		str += tb_utf8_char_to_unicode(&uni, str);
		tb_change_cell(x, y, uni, fg, bg);
		x++;
	}
}

void printf_tb(int x, int y, uint16_t fg, uint16_t bg, const char *fmt, ...) {
	char buf[4096];
	va_list vl;
	va_start(vl, fmt);
	vsnprintf(buf, sizeof(buf), fmt, vl);
	va_end(vl);
	print_tb(buf, x, y, fg, bg);
}


