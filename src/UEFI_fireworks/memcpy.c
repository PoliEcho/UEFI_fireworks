#include <Library/BaseLib.h>
#include <Uefi.h>

VOID *memcpy(VOID *CONST Destination, CONST VOID *Source, UINTN Length) {
  UINT8 *Dest = (UINT8 *)Destination;
  CONST UINT8 *Src = (CONST UINT8 *)Source;

  if (Length < 16) {
    switch (Length) {
    case 15:
      *Dest++ = *Src++;
    case 14:
      *Dest++ = *Src++;
    case 13:
      *Dest++ = *Src++;
    case 12:
      *Dest++ = *Src++;
    case 11:
      *Dest++ = *Src++;
    case 10:
      *Dest++ = *Src++;
    case 9:
      *Dest++ = *Src++;
    case 8:
      *Dest++ = *Src++;
    case 7:
      *Dest++ = *Src++;
    case 6:
      *Dest++ = *Src++;
    case 5:
      *Dest++ = *Src++;
    case 4:
      *Dest++ = *Src++;
    case 3:
      *Dest++ = *Src++;
    case 2:
      *Dest++ = *Src++;
    case 1:
      *Dest++ = *Src++;
    case 0:
      break;
    }
    return Destination;
  }

#ifdef MDE_CPU_X64
  typedef UINT64 WORD_T;
#else
  typedef UINT32 WORD_T;
#endif

  while ((UINTN)Dest & (sizeof(WORD_T) - 1)) {
    *Dest++ = *Src++;
    Length--;
  }

  WORD_T *WordDest = (WORD_T *)Dest;
  CONST WORD_T *WordSrc = (CONST WORD_T *)Src;

  while (Length >= 8 * sizeof(WORD_T)) {
    WordDest[0] = WordSrc[0];
    WordDest[1] = WordSrc[1];
    WordDest[2] = WordSrc[2];
    WordDest[3] = WordSrc[3];
    WordDest[4] = WordSrc[4];
    WordDest[5] = WordSrc[5];
    WordDest[6] = WordSrc[6];
    WordDest[7] = WordSrc[7];
    WordDest += 8;
    WordSrc += 8;
    Length -= 8 * sizeof(WORD_T);
  }

  while (Length >= 4 * sizeof(WORD_T)) {
    WordDest[0] = WordSrc[0];
    WordDest[1] = WordSrc[1];
    WordDest[2] = WordSrc[2];
    WordDest[3] = WordSrc[3];
    WordDest += 4;
    WordSrc += 4;
    Length -= 4 * sizeof(WORD_T);
  }

  while (Length >= sizeof(WORD_T)) {
    *WordDest++ = *WordSrc++;
    Length -= sizeof(WORD_T);
  }

  Dest = (UINT8 *)WordDest;
  Src = (CONST UINT8 *)WordSrc;
  while (Length--) {
    *Dest++ = *Src++;
  }

  return Destination;
}
