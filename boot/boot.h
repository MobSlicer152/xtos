#pragma once

#include "efi.h"
#include "efilib.h"

#include "basetsd.h"
#include "pe.h"

#define THISCALL(obj, function, ...) obj->function(obj __VA_OPT__(,) __VA_ARGS__);

extern EFI_STATUS GetProtocol(IN EFI_GUID* protocolGuid, IN EFI_HANDLE handle,
    IN BOOLEAN locate, OUT EFI_HANDLE* protocol);

