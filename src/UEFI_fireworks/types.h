#pragma once
#include "ProcessorBind.h"
#include "Protocol/GraphicsOutput.h"
#include <Library/BaseLib.h>
struct firework_instance {
  UINT32 x;
  UINT32 y;
  UINT16 max_r;
  UINT16 r[3];
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL color[3];
  UINT16 cleanup_r;
};

struct firework_node {
  UINTN Signature;
  struct firework_instance Firework;
  LIST_ENTRY Link;
};
#define FIREWORK_NODE_SIGNATURE SIGNATURE_32('f', 'w', 'r', 'k')