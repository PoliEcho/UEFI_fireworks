Arch uefi_run fix: sudo ln -s /usr/share/edk2/x64/OVMF_CODE.4m.fd /usr/share/qemu/OVMF.fd

cd  edk2
source edksetup.sh
make -C BaseTools # if first run