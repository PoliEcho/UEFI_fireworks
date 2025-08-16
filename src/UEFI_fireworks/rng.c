#include "rng.h"
#include "Library/UefiBootServicesTableLib.h"
#include "Library/UefiLib.h"
#include "ProcessorBind.h"
#include "Uefi/UefiBaseType.h"
#include "time.h"
#include <Library/BaseLib.h>
#include <Library/RngLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Uefi.h>
#include <stdint.h>
void (*fill_random_bytes)(void *dst, UINTN n);

UINT16 shift_LFSR(UINT16 LFSR) {
  UINT16 lsb = LFSR & 1;
  LFSR >>= 1;
  if (lsb) {
    LFSR ^= 0xB400;
  }
  return LFSR;
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
UINT16 LFSR;
void fill_random_bytes_LFSR(void *dst, UINTN n) {
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

void init_rng() {
  UINT16 test_var;
  if (GetRandomNumber16(&test_var)) {
    fill_random_bytes = fill_random_bytes_TRUE;
  } else {
    gST->ConOut->SetAttribute(gST->ConOut, EFI_YELLOW);
    Print(L"[WARNING]");
    gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
    Print(L" RNG device not available falling back to LFSR!\r\n");

    EFI_TIME Time;
    EFI_STATUS Status;
    UINT32 starting;
    Status = gRT->GetTime(&Time, NULL);
    if (EFI_ERROR(Status)) {
      Print(L"failed to get time\r\n");
      milisleep(2000);
      starting = 1;
    } else {
      starting = Time.Second;
    }

    Print(L"Press any key to seed the LFSR\r\n");
    EFI_INPUT_KEY Key;
    UINTN Index;
    Status = gBS->WaitForEvent(1, &(gST->ConIn->WaitForKey), &Index);
    if (EFI_ERROR(Status)) {
      Print(L"failed to wait key\r\n");
      Key.ScanCode = 1;
      Key.UnicodeChar = 1;
    } else {

      Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);
      if (EFI_ERROR(Status)) {
        Print(L"failed to read key\r\n");
        Key.ScanCode = 1;
        Key.UnicodeChar = 1;
      }
    }
    if (Key.ScanCode == 0) {
      Key.ScanCode = 1;
    }

    Status = gRT->GetTime(&Time, NULL);
    if (EFI_ERROR(Status)) {
      Print(L"failed to get time\r\n");
      Time.Second = 1;
    }

    UINT8 time_diff = (Time.Second - starting);
    if (time_diff == 0) {
      time_diff = 1;
    }

    LFSR = (typeof(LFSR))(((UINTN)Key.ScanCode * (UINTN)Key.UnicodeChar *
                           (UINTN)(time_diff)) %
                          UINT16_MAX);

    if (LFSR <= 1) {
      LFSR = 0xf5d7;
    }

    fill_random_bytes = fill_random_bytes_LFSR;
  }
}