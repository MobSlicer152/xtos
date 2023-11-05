#pragma once

#include "efi.h"
#include "efidef.h"
#include "efilib.h"
#include "efisetjmp.h"

#include "basetsd.h"
#include "boot.h"
#include "intellisense.h"
#include "pe.h"

#define THISCALL(obj, function, ...)                                           \
    obj->function(obj __VA_OPT__(, ) __VA_ARGS__);

extern EFI_STATUS GetProtocol(IN EFI_GUID* protocolGuid, IN EFI_HANDLE handle,
                              IN BOOLEAN locate, OUT PVOID protocol);

extern VOID EnterKernel(IN PFN_KERNEL_ENTRY kernelEntry, IN PVOID kernelStack,
                        IN PKERNEL_BOOT_DATA data);
