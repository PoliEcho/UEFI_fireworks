#pragma once
#include "types.h"
#include <Protocol/GraphicsOutput.h>
void draw_circle(int xc, int yc, int r,
                 const EFI_GRAPHICS_OUTPUT_BLT_PIXEL color);
void clear_screen();
BOOLEAN step_firework(firework_instance *firework);
void init_rocket_blt();
BOOLEAN step_rocket(location *rocket, UINT32 max_y);