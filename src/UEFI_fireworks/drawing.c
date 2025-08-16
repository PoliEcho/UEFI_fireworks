#include "Base.h"
#include "Library/UefiLib.h"
#include "ProcessorBind.h"
#include "bmp.h"
#include "const.h"
#include "global.h"
#include "types.h"
#include <../MdeModulePkg/Include/Library/BmpSupportLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Protocol/GraphicsOutput.h>

void draw_pixel(const UINT32 x, const UINT32 y,
                const EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel) {
  if (x > GraphicsOutput->Mode->Info->HorizontalResolution ||
      y > GraphicsOutput->Mode->Info
              ->VerticalResolution) { // ignore when out of bounds
    return;
  }
  UINTN framebuffer_offset =
      (y * GraphicsOutput->Mode->Info->PixelsPerScanLine) + x;

  framebuffer[framebuffer_offset] = pixel;
}

// uses Mid-Point Circle Drawing Algorithm
// inspired by
// https://www.geeksforgeeks.org/dsa/mid-point-circle-drawing-algorithm/
void draw_circle(int xc, int yc, int r,
                 const EFI_GRAPHICS_OUTPUT_BLT_PIXEL color) {
  int x = r, y = 0;
  draw_pixel(x + xc, y + yc, color);

  // fix missng pixels
  draw_pixel(xc - r, yc, color);
  draw_pixel(xc, yc - r, color);

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

BOOLEAN step_firework(firework_instance *firework) {
  for (UINT8 i = 0; i < ARRAY_SIZE(firework->r); i++) {
    if (firework->r[i] < firework->max_r) {
      if (i == 0 || (firework->max_r / 3.5) * i <= firework->r[i - 1]) {
        draw_circle(firework->x, firework->y, firework->r[i],
                    firework->color[i]);
        firework->r[i]++;
      }
    }
  }
  if (firework->r[1] >= firework->max_r) {
    if (firework->cleanup_r < firework->max_r) {
      draw_circle(firework->x, firework->y, firework->cleanup_r, night_sky);
      firework->cleanup_r++;
    } else {
      return FALSE;
    }
  }
  return TRUE;
}
rocket_blt rocket_asset;

EFI_GRAPHICS_OUTPUT_BLT_PIXEL **mask_stack;
BOOLEAN **rocket_alfa_mask;

void init_rocket_blt() {
  EFI_STATUS Status = TranslateBmpToGopBlt(
      assets_rocket_bmp, assets_rocket_bmp_len, &rocket_asset.blt,
      &rocket_asset.blt_size, &rocket_asset.height, &rocket_asset.width);
  if (EFI_ERROR(Status)) {
    Print(L"Failed to load rocket asset: %r\n", Status);
    Exit(Status);
  }
}

BOOLEAN step_rocket(rocket_instance *rocket, UINT32 max_y) {
  GraphicsOutput->Blt(GraphicsOutput,
                      rocket_asset.blt,    // BltBuffer
                      EfiBltBufferToVideo, // BltOperation
                      0,                   // src X
                      0,                   // src Y
                      rocket->x,           // dst X
                      rocket->y,           // dst Y
                      rocket_asset.width, rocket_asset.height,
                      0 // unused Delta
  );

  if (rocket->y <= max_y) {
    GraphicsOutput->Blt(
        GraphicsOutput,
        &night_sky,              // BltBuffer
        EfiBltVideoFill,         // BltOperation
        0,                       // SourceX
        0,                       // SourceY
        rocket->x,               // DestinationX
        rocket->y,               // DestinationY
        rocket_asset.width,      // Width
        rocket_asset.height + 1, // Height // remove trail from previous frame
        0                        // Delta (not used for fill operations)
    );
    return FALSE;
  } else {
    for (UINT8 i = 0; i < rocket_asset.width; i++) {
      draw_pixel(rocket->x + i, rocket->y + rocket_asset.height, night_sky);
    }
    rocket->y--;
    return TRUE;
  }
}