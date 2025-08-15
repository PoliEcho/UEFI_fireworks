#include "Library/UefiApplicationEntryPoint.h"
#include "ProcessorBind.h"
#include "drawing.h"
#include "global.h"
#include "macros.h"
#include "rng.h"
#include "time.h"
#include "types.h"
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/RngLib.h>
#include <Library/SerialPortLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Uefi.h>
#include <stdint.h>

firework_instance create_firework();
EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput = NULL;
EFI_GRAPHICS_OUTPUT_BLT_PIXEL *framebuffer = NULL;

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE imgHandle,
                           IN EFI_SYSTEM_TABLE *sysTable) {
  Print(L"Starting program\n");
  gST = sysTable;
  gBS = sysTable->BootServices;
  gImageHandle = imgHandle;
  gRT = sysTable->RuntimeServices;

  // UEFI apps automatically exit after 5 minutes. Stop that here
  gBS->SetWatchdogTimer(0, 0, 0, NULL);

  // get screen size
  UINTN Columns;
  UINTN Rows;

  gST->ConOut->QueryMode(gST->ConOut, gST->ConOut->Mode->Mode, &Columns, &Rows);

  gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL,
                      (VOID **)&GraphicsOutput);

  if (GraphicsOutput == NULL) {
    Print(L"GraphicsOutput protocol not found!\n");
    return EFI_NOT_FOUND;
  }

  framebuffer =
      (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)GraphicsOutput->Mode->FrameBufferBase;

  init_rng();

  if (SerialPortInitialize() == RETURN_SUCCESS) {
    SERIAL_PRINT("Serial initialized\n");
  } else {
    Print(L"Failed to initialize Serial!\n");
  }
  SerialPortSetControl(EFI_SERIAL_CLEAR_TO_SEND |
                       EFI_SERIAL_DATA_TERMINAL_READY |
                       EFI_SERIAL_REQUEST_TO_SEND);

  firework_instance *firework_array[UINT8_MAX];
  gBS->SetMem(firework_array, sizeof(firework_array),
              0); // make all pointers null

  SERIAL_PRINT("DOES it work?\n");
  Print(L"If you see this message timer does not work\n");
  milisleep(100);
  clear_screen();

  while (TRUE) {
    UINT8 random;
    fill_random_bytes(&random, sizeof(random));
    if (random % 6 == 0) {
      // spawn new firework
      firework_instance *new_firework_instence =
          AllocateZeroPool(sizeof(firework_instance));
      if (new_firework_instence == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }

      *new_firework_instence = create_firework();

      for (UINT8 i = 0; i < ARRAY_SIZE(firework_array); i++) {
        if (firework_array[i] == NULL || firework_array[i]->active != TRUE) {
          if (firework_array[i] != NULL) {
            FreePool(firework_array[i]); // free firework
            firework_array[i] = NULL;
          }
          firework_array[i] = new_firework_instence;
          goto assgned;
        } // firework will not be created if if all slots are full
      }
      SERIAL_PRINT("NO free slots\n");
    }
  assgned:
    for (UINT8 i = 0; i < ARRAY_SIZE(firework_array); i++) {
      if (firework_array[i] != NULL) {
        if (firework_array[i]->active == TRUE) {
          if (!step_firework(firework_array[i])) {
            firework_array[i]->active = FALSE;
          }
        } else {
          FreePool(firework_array[i]); // free firework
          firework_array[i] = NULL;
        }
      }
    }

    milisleep(10);
  }

  return EFI_SUCCESS;
}

firework_instance create_firework() {
  firework_instance firework;
  UINT32 random;
  fill_random_bytes(&random, sizeof(random));
  firework.max_r = (random % 200) + 1; // 1 to 200, avoiding 0

  for (UINT8 i = 0; i < ARRAY_SIZE(firework.color); i++) {
    fill_random_bytes(&firework.color[i],
                      sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL) - 1);
    firework.r[i] = 0;
  }
  firework.cleanup_r = 0;

  fill_random_bytes(&random, sizeof(random));
  firework.x = random % GraphicsOutput->Mode->Info->HorizontalResolution;
  fill_random_bytes(&random, sizeof(random));
  firework.y = random % GraphicsOutput->Mode->Info->VerticalResolution;

  firework.active = TRUE;
  return firework;
}