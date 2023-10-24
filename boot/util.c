#include "boot.h"

EFI_STATUS GetProtocol(IN EFI_GUID* protocolGuid, IN EFI_HANDLE handle,
    IN BOOLEAN locate, OUT PVOID protocol)
{
    EFI_STATUS status = EFI_SUCCESS;
    
    Print(L"Getting protocol %g using handle 0x%016X\n", protocolGuid, handle);

    if (locate)
    {
        status = BS->LocateProtocol(handle, protocolGuid, protocol);
    }
    else
    {
        status = BS->HandleProtocol(handle, protocolGuid, protocol);
    }
    if (EFI_ERROR(status))
    {
        Print(L"Failed to get protocol: %r\n", status);
        goto Done;
    }

    Print(L"Found protocol at 0x%016X\n", *(EFI_HANDLE*)protocol);

Done:
    return status;
}

