#include "Library/UefiLib.h"
#include <Library/UefiBootServicesTableLib.h>
EFI_STATUS
microsleep(UINTN n) {
  EFI_STATUS Status;
  EFI_EVENT TimerEvent;

  Status =
      gBS->CreateEvent(EVT_TIMER, TPL_APPLICATION, NULL, NULL, &TimerEvent);

  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = gBS->SetTimer(TimerEvent, TimerRelative,
                         EFI_TIMER_PERIOD_MICROSECONDS(n));

  if (!EFI_ERROR(Status)) {
    gBS->WaitForEvent(1, &TimerEvent, NULL);
  }

  gBS->CloseEvent(TimerEvent);
  return Status;
}