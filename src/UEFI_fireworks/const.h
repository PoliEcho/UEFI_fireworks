#pragma once
#include "macros.h"
#include <Protocol/GraphicsOutput.h>

EFI_GRAPHICS_OUTPUT_BLT_PIXEL night_sky =
    COLOR_FROM_HEX(0x11095e); // this cannot be const becose EDK2 said so
