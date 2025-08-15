#include "rng.h"
#include "Library/UefiBootServicesTableLib.h"
#include "Library/UefiLib.h"
#include "ProcessorBind.h"
#include "time.h"
#include <Library/BaseLib.h>
#include <Library/RngLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Uefi.h>
void (*fill_random_bytes)(void *dst, UINTN n);

UINT16 shift_LFSR(UINT16 LFSR) {
  UINT16 lsb = LFSR & 1;
  LFSR >>= 1;
  if (lsb) {
    LFSR ^= 0xB400;
  }
  return LFSR;
}

void fill_random_bytes_LFSR(void *dst, UINTN n) {
  static UINT16 LFSR = 0xf5d7;

  if (n % 2 != 0) {
    LFSR = shift_LFSR(LFSR);
    *(UINT8 *)dst = (UINT8)LFSR;
    dst++;
    n--;
  }
  while (n != 0) {
    LFSR = shift_LFSR(LFSR);
    *(UINT16 *)dst = LFSR;
    dst += 2;
    n -= 2;
  }
}
void fill_random_bytes_TRUE(void *dst, UINTN n) {
  UINT16 rand;
  if (n % 2 != 0) {
    GetRandomNumber16(&rand);
    *(UINT8 *)dst = (UINT8)rand;
    dst++;
    n--;
  }
  while (n != 0) {
    GetRandomNumber16(&rand);
    *(UINT16 *)dst = rand;
    dst += 2;
    n -= 2;
  }
}

void init_rng() {
  UINT16 test_var;
  if (GetRandomNumber16(&test_var)) {
    fill_random_bytes = fill_random_bytes_TRUE;
  } else {
    gST->ConOut->SetAttribute(gST->ConOut, EFI_YELLOW);
    Print(L"[WARNING]");
    gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
    Print(L" RNG device not available falling back to LFSR\r\n");

    Print(L"Continuing in  ");
    for (UINT8 i = 5; i > 0; i--) {
      Print(L"\b%d", i);
      milisleep(1000);
    }

    fill_random_bytes = fill_random_bytes_LFSR;
  }
}