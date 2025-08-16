#pragma once
#include "ProcessorBind.h"
#include "Protocol/GraphicsOutput.h"
#include "const.h"
#include <Library/BaseLib.h>

typedef struct {
  UINT32 x;
  UINT32 y;
} rocket_instance;

typedef struct {
  enum firework_status status; // IF INACTIVE can be overwriten
  UINT32 x;
  UINT32 y;
  UINT16 max_r;
  UINT16 r[3];
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL color[3];
  UINT16 cleanup_r;
  rocket_instance rocket;
} firework_instance;
