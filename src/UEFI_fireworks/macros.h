#pragma once
#define COLOR_FROM_HEX(hex)                                                    \
  {.Blue = (hex) & 0xFF,                                                       \
   .Green = ((hex) >> 8) & 0xFF,                                               \
   .Red = ((hex) >> 16) & 0xFF,                                                \
   .Reserved = 0}
