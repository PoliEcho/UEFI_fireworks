#pragma once
#include <Protocol/GraphicsOutput.h>
void draw_circle(int xc, int yc, int r,
                 const EFI_GRAPHICS_OUTPUT_BLT_PIXEL color);
void clear_screen(EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput);