#include "Library/UefiApplicationEntryPoint.h"
#include "ProcessorBind.h"
#include "drawing.h"
#include "global.h"
#include "macros.h"
#include "types.h"
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/RngLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Uefi.h>

EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput = NULL;
EFI_GRAPHICS_OUTPUT_BLT_PIXEL *framebuffer = NULL;

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE imgHandle,
                           IN EFI_SYSTEM_TABLE *sysTable) {
  gST = sysTable;
  gBS = sysTable->BootServices;
  gImageHandle = imgHandle;

  // UEFI apps automatically exit after 5 minutes. Stop that here
  gBS->SetWatchdogTimer(0, 0, 0, NULL);

  // get screen size
  UINTN Columns;
  UINTN Rows;

  gST->ConOut->QueryMode(gST->ConOut, gST->ConOut->Mode->Mode, &Columns, &Rows);

  gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL,
                      (VOID **)&GraphicsOutput);

  framebuffer =
      (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)GraphicsOutput->Mode->FrameBufferBase;

  clear_screen(GraphicsOutput);

  LIST_ENTRY firework_list;
  InitializeListHead(&firework_list);

  if (SerialPortInitialize() == RETURN_SUCCESS) {
    SERIAL_PRINT("Serial initialized");
  } else {
    Print(L"Failed to initialize Serial");
    Exit(RETURN_DEVICE_ERROR);
  }
  while (TRUE) {
    UINT32 random;
    GetRandomNumber32(&random);
    if (random % 6 == 0) {
      // spawn new firework
      firework_node *new_firework_node =
          AllocateZeroPool(sizeof(firework_node));
      if (new_firework_node == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      new_firework_node->Signature = FIREWORK_NODE_SIGNATURE;
      GetRandomNumber32(&random);
      new_firework_node->Firework.max_r =
          random % 501; // so max number can be 500
      for (UINT8 i = 0; i < ARRAY_SIZE(new_firework_node->Firework.color);
           i++) {
        GetRandomNumber32(
            (UINT32 *)&new_firework_node->Firework.color[i]); // belive
        new_firework_node->Firework.r[i] = 0;
      }
      new_firework_node->Firework.cleanup_r = 0;
      GetRandomNumber32(&random);
      new_firework_node->Firework.x =
          random % GraphicsOutput->Mode->Info->HorizontalResolution + 1;
      GetRandomNumber32(&random);
      new_firework_node->Firework.y =
          random % GraphicsOutput->Mode->Info->VerticalResolution + 1;

      InsertTailList(&firework_list, &new_firework_node->Link);
    }

    firework_node *current_node = NULL;
    for (LIST_ENTRY *node = GetFirstNode(&firework_list);
         !IsNodeAtEnd(&firework_list, node);
         node = GetNextNode(&firework_list, node)) {
      // Print(L"Processing firework\r\n");

      current_node = CR(node, firework_node, Link, FIREWORK_NODE_SIGNATURE);
      if (!step_firework(&current_node->Firework)) {
        RemoveEntryList(node); // remove if firework ended
      }
    }
    MicroSecondDelay(500000);
  }
  return EFI_SUCCESS;
}
