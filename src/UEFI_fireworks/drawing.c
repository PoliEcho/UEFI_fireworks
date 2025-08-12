#include "Library/UefiLib.h"
#include "ProcessorBind.h"
#include "const.h"
#include "global.h"
#include "macros.h"
#include <Library/UefiApplicationEntryPoint.h>
#include <Protocol/GraphicsOutput.h>

void draw_pixel(const UINT32 x, const UINT32 y,
                const EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel) {
  UINTN framebuffer_offset =
      (y * GraphicsOutput->Mode->Info->PixelsPerScanLine) + x;
  if (framebuffer_offset >= GraphicsOutput->Mode->FrameBufferSize) {
    Print(L"Atepted out of bounds write to framebuffer\r\n");
    Exit(RETURN_WARN_BUFFER_TOO_SMALL);
  }

  framebuffer[framebuffer_offset] = pixel;
}

// uses Mid-Point Circle Drawing Algorithm
// inspired by
// https://www.geeksforgeeks.org/dsa/mid-point-circle-drawing-algorithm/
void draw_circle(int xc, int yc, int r,
                 const EFI_GRAPHICS_OUTPUT_BLT_PIXEL color) {
  int x = r, y = 0;
  draw_pixel(x + xc, y + yc, color);

  if (r > 0) {
    draw_pixel(x + xc, -y + yc, color);
    draw_pixel(y + xc, x + yc, color);
    draw_pixel(-y + xc, x + yc, color);
  }

  int P = 1 - r;
  while (x > y) {
    y++;

    if (P <= 0)
      P = P + 2 * y + 1;

    else {
      x--;
      P = P + 2 * y - 2 * x + 1;
    }

    if (x < y)
      break;

    draw_pixel(x + xc, y + yc, color);
    draw_pixel(-x + xc, y + yc, color);
    draw_pixel(x + xc, -y + yc, color);
    draw_pixel(-x + xc, -y + yc, color);

    if (x != y) {
      draw_pixel(y + xc, x + yc, color);
      draw_pixel(-y + xc, x + yc, color);
      draw_pixel(y + xc, -x + yc, color);
      draw_pixel(-y + xc, -x + yc, color);
    }
  }
}

void clear_screen() {
  // fill screen
  if (GraphicsOutput != NULL) {
    GraphicsOutput->Blt(
        GraphicsOutput,
        &night_sky,                                       // BltBuffer
        EfiBltVideoFill,                                  // BltOperation
        0,                                                // SourceX
        0,                                                // SourceY
        0,                                                // DestinationX
        0,                                                // DestinationY
        GraphicsOutput->Mode->Info->HorizontalResolution, // Width
        GraphicsOutput->Mode->Info->VerticalResolution,   // Height
        0 // Delta (not used for fill operations)
    );
  }
}
