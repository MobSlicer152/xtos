#pragma once

#include "efi.h"
#include "efilib.h"

extern EFI_STATUS GetProtocol(IN EFI_GUID* protocolGuid, IN EFI_HANDLE handle,
    IN BOOLEAN locate, OUT EFI_HANDLE* protocol);

