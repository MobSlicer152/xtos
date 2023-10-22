#include "boot.h"

int _fltused;

EFI_STATUS EfiMain(IN VOID* imageHandle, IN EFI_SYSTEM_TABLE* systemTable)
{
    EFI_STATUS status;

    ST = systemTable;
    BS = ST->BootServices;
    RT = ST->RuntimeServices;

    ST->ConOut->ClearScreen(ST->ConOut);
    Print(L"Image handle: 0x%016X\nSystem table: 0x%016X\n", imageHandle, ST);
    Print(L"%ls firmware v%d.%d, UEFI v%d.%d\n", ST->FirmwareVendor,
        ST->FirmwareRevision >> 16, ST->FirmwareRevision & 0xFFFF,
        ST->Hdr.Revision >> 16, ST->Hdr.Revision & 0xFFFF);

    // Disable the watchdog
    BS->SetWatchdogTimer(0, 0, 0, NULL);

    Print(L"Getting loaded image information\n");
    EFI_LOADED_IMAGE_PROTOCOL* loadedImage = NULL;
    status = GetProtocol(&LoadedImageProtocol, imageHandle, FALSE, &loadedImage);
    if (EFI_ERROR(status))
    {
        return status;
    }

    //Print(L"Getting boot volume handle\n");

    return EFI_SUCCESS;
}
