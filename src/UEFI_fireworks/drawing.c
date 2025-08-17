#include "Base.h"
#include "Library/BaseMemoryLib.h"
#include "Library/MemoryAllocationLib.h"
#include "Library/UefiLib.h"
#include "ProcessorBind.h"
#include "bmp.h"
#include "const.h"
#include "global.h"
#include "time.h"
#include "types.h"
#include <../MdeModulePkg/Include/Library/BmpSupportLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Protocol/GraphicsOutput.h>
#include <alloca.h>

void draw_pixel(const UINT32 x, const UINT32 y,
                const EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel) {
  if (x >= GraphicsOutput->Mode->Info->HorizontalResolution ||
      y >= GraphicsOutput->Mode->Info
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

BOOLEAN **rocket_alfa_mask;

UINT32 *rocket_clean_up_mask;

void init_rocket_blt() {
  EFI_STATUS Status = TranslateBmpToGopBlt(
      assets_rocket_bmp, assets_rocket_bmp_len, &rocket_asset.blt,
      &rocket_asset.blt_size, &rocket_asset.height, &rocket_asset.width);
  if (EFI_ERROR(Status)) {
    Print(L"Failed to load rocket asset: %r\n", Status);
    Exit(Status);
  }
  rocket_clean_up_mask = AllocatePool(rocket_asset.width * sizeof(location));
  rocket_alfa_mask = AllocateZeroPool(rocket_asset.height * sizeof(BOOLEAN *));
  for (UINT32 i = 0; i < rocket_asset.height; i++) {
    rocket_alfa_mask[i] = AllocatePool(rocket_asset.width * sizeof(BOOLEAN));
    for (UINT32 j = 0; j < rocket_asset.width; j++) {
      if (rocket_asset.blt[i * rocket_asset.width + j].Blue == 0 &&
          rocket_asset.blt[i * rocket_asset.width + j].Green == 0 &&
          rocket_asset.blt[i * rocket_asset.width + j].Red == 0) {
        rocket_alfa_mask[i][j] = FALSE;
      } else {
        rocket_alfa_mask[i][j] = TRUE;
        rocket_clean_up_mask[j] = i + 1; // build the cleanup mask
      }
    }
  }
}
void draw_rocket(location *rocket) {
  for (UINT32 i = 0; i < rocket_asset.width; i++) {
    for (UINT32 j = 0; j < rocket_asset.height; j++) {
      if (rocket_alfa_mask[j][i]) {
        draw_pixel(i + rocket->x, j + rocket->y,
                   rocket_asset.blt[j * rocket_asset.width + i]);
      }
    }
  }
}

BOOLEAN step_rocket(location *rocket, UINT32 max_y) {
  draw_rocket(rocket);
  for (UINT8 i = 0; i < rocket_asset.width; i++) {
    draw_pixel(rocket->x + i, rocket->y + rocket_clean_up_mask[i], night_sky);
  }
  if (rocket->y <= max_y) {
    // remove rocket
    for (UINT32 i = 0; i < rocket_asset.width; i++) {
      for (UINT32 j = 0; j < rocket_asset.height; j++) {
        if (rocket_alfa_mask[j][i]) {
          draw_pixel(i + rocket->x, j + rocket->y, night_sky);
        }
      }
    }
    return FALSE;
  } else {
    rocket->y--;
    return TRUE;
  }
}