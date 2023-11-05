#include "bootp.h"

EFI_STATUS GetProtocol(IN EFI_GUID* protocolGuid, IN EFI_HANDLE handle,
                       IN BOOLEAN locate, OUT PVOID protocol)
{
    EFI_STATUS status = EFI_SUCCESS;

    Print(L"Getting protocol %g using handle 0x%lX\n", protocolGuid, handle);

    if (locate)
    {
        status = BS->LocateProtocol(protocolGuid, NULL, protocol);
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

    Print(L"Found protocol at 0x%lX\n", *(PVOID*)protocol);

Done:
    return status;
}
