#include "boot.h"

int _fltused;

CONST WCHAR g_kernelPath[] = L"\\xtos.exe";

EFI_STATUS EfiMain(IN VOID* imageHandle, IN EFI_SYSTEM_TABLE* systemTable)
{
    EFI_STATUS status;

    ST = systemTable;
    BS = ST->BootServices;
    RT = ST->RuntimeServices;

    THISCALL(ST->ConOut, ClearScreen);
    Print(L"Image handle: 0x%016X\nSystem table: 0x%016X\n", imageHandle, ST);
    Print(L"%ls firmware v%d.%d, UEFI v%d.%d\n", ST->FirmwareVendor,
        ST->FirmwareRevision >> 16, ST->FirmwareRevision & 0xFFFF,
        ST->Hdr.Revision >> 16, ST->Hdr.Revision & 0xFFFF);

    // Disable the watchdog
    BS->SetWatchdogTimer(0, 0, 0, NULL);

    EFI_LOADED_IMAGE_PROTOCOL* loadedImage = NULL;
    status = GetProtocol(&LoadedImageProtocol, imageHandle, FALSE,
        &loadedImage);
    if (EFI_ERROR(status))
    {
        goto Done;
    }

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* bootVolume = NULL;
    status = GetProtocol(&FileSystemProtocol, loadedImage->DeviceHandle,
        FALSE, &bootVolume);
    if (EFI_ERROR(status))
    {
        goto Done;
    }

    Print(L"Opening boot volume\n");
    EFI_FILE* root = NULL;
    status = THISCALL(bootVolume, OpenVolume, &root);
    if (EFI_ERROR(status))
    {
        Print(L"Failed to open boot volume: %r\n", status);
        goto Done;
    }

    Print(L"Loading kernel from %ls\n", g_kernelPath);
    EFI_FILE* kernel = NULL;
    status = THISCALL(root, Open, &kernel, g_kernelPath,
        EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    if (EFI_ERROR(status))
    {
        Print(L"Failed to open %ls: %r\n", g_kernelPath, status);
        goto Done;
    }

    Print(L"Reading DOS header\n");
    size_t size = sizeof(IMAGE_DOS_HEADER);
    IMAGE_DOS_HEADER dosHeader = {};
    THISCALL(kernel, Read, &size, &dosHeader);
    if (dosHeader.e_magic != IMAGE_DOS_SIGNATURE)
    {
        Print(L"DOS signature is 0x%04X, should be 0x%04X\n",
            dosHeader.e_magic, IMAGE_DOS_SIGNATURE);
        status = EFI_LOAD_ERROR;
        goto Done;
    }

    Print(L"Reading NT headers at 0x%08X\n", dosHeader.e_lfanew);
    THISCALL(kernel, SetPosition, dosHeader.e_lfanew);
    IMAGE_NT_HEADERS ntHeaders = {};
    size = sizeof(IMAGE_NT_HEADERS) - IMAGE_NUMBEROF_DIRECTORY_ENTRIES *
        sizeof(IMAGE_DATA_DIRECTORY);
    THISCALL(kernel, Read, &size, &ntHeaders);
    if (ntHeaders.Signature != IMAGE_NT_SIGNATURE)
    {
        Print(L"NT signature is 0x%04X, should be 0x%04X\n",
                ntHeaders.Signature, IMAGE_NT_SIGNATURE);
        status = EFI_LOAD_ERROR;
        goto Done;
    }

    status = EFI_SUCCESS;
Done:
    while (1) {}
    return status;
}
