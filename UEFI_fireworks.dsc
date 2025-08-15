[Defines]
  PLATFORM_NAME                  = UEFI_fireworks
  PLATFORM_GUID                  = A05265BC-6F89-4969-9CC3-5F2D8C99CF39
  PLATFORM_VERSION               = 1.0
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/UEFI_fireworks
  SUPPORTED_ARCHITECTURES        = X64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT

[LibraryClasses]
UefiApplicationEntryPoint|edk2/MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
UefiLib|edk2/MdePkg/Library/UefiLib/UefiLib.inf
BaseLib|edk2/MdePkg/Library/BaseLib/BaseLib.inf
BaseMemoryLib|edk2/MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
MemoryAllocationLib|edk2/MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
DevicePathLib|edk2/MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
UefiBootServicesTableLib|edk2/MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
UefiRuntimeServicesTableLib|edk2/MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
DebugLib|edk2/MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
PrintLib|edk2/MdePkg/Library/BasePrintLib/BasePrintLib.inf
PcdLib|edk2/MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
RegisterFilterLib|edk2/MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf
StackCheckLib|edk2/MdePkg/Library/StackCheckLibNull/StackCheckLibNull.inf
IoLib|edk2/MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
RngLib|edk2/MdePkg/Library/BaseRngLib/BaseRngLib.inf
SerialPortLib|edk2/UefiPayloadPkg/Library/BaseSerialPortLibHob/BaseSerialPortLibHob.inf
TimerLib|edk2/OvmfPkg/Library/AcpiTimerLib/DxeAcpiTimerLib.inf
PciLib|edk2/MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
HobLib|edk2/MdePkg/Library/DxeHobLib/DxeHobLib.inf
PciCf8Lib|edk2/MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
DebugPrintErrorLevelLib|edk2/MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf

[Components]
  src/UEFI_fireworks/UEFI_fireworks.inf
