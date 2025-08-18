#include "ProcessorBind.h"
#include "Protocol/SimpleTextIn.h"
#include "Uefi/UefiBaseType.h"
#include "const.h"
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
EFI_GRAPHICS_OUTPUT_BLT_PIXEL night_sky =
    COLOR_FROM_HEX(0x090531); // this cannot be const becose EDK2 said so
#define DEFAULT_SLEEP_TIME 10000
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
  init_rocket_blt();

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
  Print(L"If you see this message for long time, timer does not work\n");
  microsleep(1);
  clear_screen();

  UINTN sleep_time = DEFAULT_SLEEP_TIME;
  while (TRUE) {
    UINT8 random;
    fill_random_bytes(&random, sizeof(random));
    if (random % 30 == 0) {
      // spawn new firework
      firework_instance *new_firework_instence =
          AllocateZeroPool(sizeof(firework_instance));
      if (new_firework_instence == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }

      *new_firework_instence = create_firework();

      for (UINT8 i = 0; i < ARRAY_SIZE(firework_array); i++) {
        if (firework_array[i] == NULL ||
            firework_array[i]->status == INACTIVE) {
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
        if (firework_array[i]->status == ACTIVE) {
          if (!step_firework(firework_array[i])) {
            firework_array[i]->status = INACTIVE;
          }
        } else if (firework_array[i]->status == LAUNCHING) {
          if (!step_rocket(&firework_array[i]->rocket, firework_array[i]->y)) {
            firework_array[i]->status = ACTIVE;
          }
        } else {
          FreePool(firework_array[i]); // free firework
          firework_array[i] = NULL;
        }
      }
    }
    EFI_INPUT_KEY key;
    EFI_STATUS status = gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
    gST->ConIn->Reset(gST->ConIn, FALSE);
    if (status == EFI_SUCCESS) {
      switch (key.ScanCode) {
      case SCAN_UP: // increase simulation speed

        if (sleep_time > 100) {
          sleep_time = sleep_time - 100;
        } else if (sleep_time > 10) {
          sleep_time = sleep_time - 10;
        } else if (sleep_time > 1) {
          sleep_time = sleep_time - 1;
        }
        break;
      case SCAN_DOWN:
        if (sleep_time < UINT32_MAX - 100) {
          sleep_time = sleep_time + 100;
        } else if (sleep_time < UINT32_MAX - 10) {
          sleep_time = sleep_time + 10;
        } else if (sleep_time < UINT32_MAX - 1) {
          sleep_time = sleep_time + 1;
        }
        break;
      case SCAN_PAGE_UP:
        if (sleep_time > 1000) {
          sleep_time = sleep_time - 1000;
        } else if (sleep_time > 100) {
          sleep_time = sleep_time - 100;
        } else if (sleep_time > 10) {
          sleep_time = sleep_time - 10;
        } else if (sleep_time > 1) {
          sleep_time = sleep_time - 1;
        }
        break;
      case SCAN_PAGE_DOWN:
        if (sleep_time < UINT32_MAX - 1000) {
          sleep_time = sleep_time + 1000;
        } else if (sleep_time < UINT32_MAX - 100) {
          sleep_time = sleep_time + 100;
        } else if (sleep_time < UINT32_MAX - 10) {
          sleep_time = sleep_time + 10;
        } else if (sleep_time < UINT32_MAX - 1) {
          sleep_time = sleep_time + 1;
        }
        break;
      case SCAN_HOME:
        sleep_time = DEFAULT_SLEEP_TIME;
        break;
      case SCAN_DELETE:
        for (UINT8 i = 0; i < ARRAY_SIZE(firework_array); i++) {
          if (firework_array[i] != NULL) {
            FreePool(firework_array[i]);
            firework_array[i] = NULL;
          }
        }
        // clear_screen(); does not work on real hardware here for some unknown
        // reason
        for (UINTN i = 0; i < GraphicsOutput->Mode->FrameBufferSize; i++) {
          framebuffer[i] = night_sky;
        }
        break;
      }
    }
    microsleep(sleep_time);
  }

  return EFI_SUCCESS;
}

firework_instance create_firework() {
  firework_instance firework;
  UINT32 random;
  fill_random_bytes(&random, sizeof(random));
  firework.max_r = (random % 190) + 10; // 10 to 200,

  for (UINT8 i = 0; i < ARRAY_SIZE(firework.color); i++) {
    fill_random_bytes(&firework.color[i],
                      sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL) - 1);
    firework.r[i] = 0;
  }
  firework.cleanup_r = 0;

  fill_random_bytes(&random, sizeof(random));
  firework.x = random % GraphicsOutput->Mode->Info->HorizontalResolution;
  fill_random_bytes(&random, sizeof(random));
  firework.y = random % GraphicsOutput->Mode->Info->VerticalResolution /
               2; // spawn only on upper half of the screen

  firework.rocket.x = firework.x + rocket_asset.width / 2;
  firework.rocket.y = GraphicsOutput->Mode->Info->VerticalResolution;
  firework.status = LAUNCHING;
  return firework;
}