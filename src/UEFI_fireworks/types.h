#pragma once
#include "ProcessorBind.h"
#include "Protocol/GraphicsOutput.h"
#include <Library/BaseLib.h>
typedef struct {
  BOOLEAN active; // IF FALSE can be overwriten
  UINT32 x;
  UINT32 y;
  UINT16 max_r;
  UINT16 r[3];
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL color[3];
  UINT16 cleanup_r;
} firework_instance;
