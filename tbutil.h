#pragma once
#include <stdint.h>
#include <math.h>

void print_tb(const char *str, int x, int y, uint16_t fg, uint16_t bg);
void printf_tb(int x, int y, uint16_t fg, uint16_t bg, const char *fmt, ...);
	
namespace math {
	float rad2deg(float rad);
	float deg2rad(float deg);
	float sq(float n);
}


