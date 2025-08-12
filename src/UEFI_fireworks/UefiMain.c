#include "ProcessorBind.h"
#include "drawing.h"
#include "global.h"
#include "macros.h"
#include "types.h"
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
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

  struct firework_instance firework = {500,
                                       500,
                                       100,
                                       {0, 0, 0},
                                       {COLOR_FROM_HEX(0xff0000),
                                        COLOR_FROM_HEX(0x00ff00),
                                        COLOR_FROM_HEX(0x0000ff)},
                                       0};
  while (step_firework(&firework)) {
    MicroSecondDelay(50000);
  }

  LIST_ENTRY firework_list;
  InitializeListHead(&firework_list);

  return EFI_SUCCESS;
}
