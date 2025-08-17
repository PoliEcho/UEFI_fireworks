#pragma once
#include "ProcessorBind.h"
#include "Protocol/GraphicsOutput.h"
#include "const.h"
#include <Library/BaseLib.h>

typedef struct {
  UINT32 x;
  UINT32 y;
} location;

typedef struct {
  enum firework_status status; // IF INACTIVE can be overwriten
  UINT32 x;
  UINT32 y;
  UINT16 max_r;
  UINT16 r[3];
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL color[3];
  UINT16 cleanup_r;
  location rocket;
} firework_instance;

typedef struct {
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *blt;
  UINTN blt_size;
  UINTN height;
  UINTN width;
} rocket_blt;
