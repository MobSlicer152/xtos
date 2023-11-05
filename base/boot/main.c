#include "bootp.h"

int _fltused;

CONST WCHAR g_kernelPath[] = L"\\xtos.exe";

EFI_STATUS EfiMain(IN VOID* imageHandle, IN EFI_SYSTEM_TABLE* systemTable)
{
    EFI_STATUS status;

    ST = systemTable;
    BS = ST->BootServices;
    RT = ST->RuntimeServices;

    THISCALL(ST->ConOut, ClearScreen);
    Print(L"Image handle: 0x%lX\nSystem table: 0x%lX\n", imageHandle, ST);
    Print(L"%ls firmware v%d.%d, UEFI v%d.%d\n", ST->FirmwareVendor,
          ST->FirmwareRevision >> 16, ST->FirmwareRevision & 0xFFFF,
          ST->Hdr.Revision >> 16, ST->Hdr.Revision & 0xFFFF);

    // Disable the watchdog
    BS->SetWatchdogTimer(0, 0, 0, NULL);

    EFI_LOADED_IMAGE_PROTOCOL* loadedImage = NULL;
    status =
        GetProtocol(&LoadedImageProtocol, imageHandle, FALSE, &loadedImage);
    if (EFI_ERROR(status))
    {
        goto Done;
    }

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* bootVolume = NULL;
    status = GetProtocol(&FileSystemProtocol, loadedImage->DeviceHandle, FALSE,
                         &bootVolume);
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

    Print(L"Loading kernel %ls\n", g_kernelPath);
    EFI_FILE* kernel = NULL;
    status = THISCALL(root, Open, &kernel, (PWCHAR)g_kernelPath,
                      EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    if (EFI_ERROR(status))
    {
        Print(L"Failed to open %ls: %r\n", g_kernelPath, status);
        goto Done;
    }

    Print(L"Reading DOS header\n");
    SIZE_T size = sizeof(IMAGE_DOS_HEADER);
    IMAGE_DOS_HEADER dosHeader = {};
    THISCALL(kernel, Read, &size, &dosHeader);
    if (dosHeader.e_magic != IMAGE_DOS_SIGNATURE)
    {
        Print(L"DOS signature is 0x%04X, should be 0x%04X\n", dosHeader.e_magic,
              IMAGE_DOS_SIGNATURE);
        status = EFI_LOAD_ERROR;
        goto Done;
    }

    Print(L"Reading NT headers at 0x%08X\n", dosHeader.e_lfanew);
    THISCALL(kernel, SetPosition, dosHeader.e_lfanew);
    IMAGE_NT_HEADERS ntHeaders = {};
    size = sizeof(IMAGE_NT_HEADERS) -
           IMAGE_NUMBEROF_DIRECTORY_ENTRIES * sizeof(IMAGE_DATA_DIRECTORY);
    THISCALL(kernel, Read, &size, &ntHeaders);
    if (ntHeaders.Signature != IMAGE_NT_SIGNATURE ||
        ntHeaders.FileHeader.Machine != IMAGE_FILE_MACHINE_HOST ||
        ntHeaders.OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC)
    {
        Print(L"NT signature is 0x%04X, should be 0x%04X. Architecture is "
              L"0x%04X, should be 0x%04X. Optional header magic should be "
              L"0x%04X, should be 0x%04X\n",
              ntHeaders.Signature, IMAGE_NT_SIGNATURE,
              ntHeaders.FileHeader.Machine, IMAGE_FILE_MACHINE_HOST,
              ntHeaders.OptionalHeader.Magic, IMAGE_NT_OPTIONAL_HDR64_MAGIC);
        status = EFI_LOAD_ERROR;
        goto Done;
    }

    Print(L"Loading sections relative to base address 0x%16X\n",
          ntHeaders.OptionalHeader.ImageBase);
    THISCALL(kernel, GetPosition, &size);
    THISCALL(kernel, SetPosition,
             size + ntHeaders.OptionalHeader.NumberOfRvaAndSizes *
                        sizeof(IMAGE_DATA_DIRECTORY));
    size = ntHeaders.FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER);
    IMAGE_SECTION_HEADER* sections = NULL;
    BS->AllocatePool(EfiLoaderData, size, (PVOID*)&sections);
    THISCALL(kernel, Read, &size, sections);

    THISCALL(kernel, SetPosition, ntHeaders.OptionalHeader.SizeOfHeaders);
    for (SIZE_T i = 0; i < ntHeaders.FileHeader.NumberOfSections; i++)
    {
        size = sections[i].Misc.VirtualSize;
        PBYTE sectionData = (PBYTE)(ntHeaders.OptionalHeader.ImageBase +
                                    sections[i].VirtualAddress);

        Print(L"Loading %u-byte section %a from offset 0x%08X to %u-page "
              L"region 0x%lX-0x%lX\n",
              sections[i].SizeOfRawData, sections[i].Name,
              sections[i].PointerToRawData, EFI_SIZE_TO_PAGES(size),
              sectionData,
              sectionData + EFI_SIZE_TO_PAGES(size) * EFI_PAGE_SIZE);

        status = BS->AllocatePages(
            AllocateAddress,
            sections[i].Characteristics & IMAGE_SCN_CNT_CODE ? EfiLoaderCode
                                                             : EfiLoaderData,
            EFI_SIZE_TO_PAGES(size), (PUINT_PTR)&sectionData);
        if (EFI_ERROR(status))
        {
            Print(L"Failed to allocate %u pages(s) at 0x%lX: %r\n",
                  EFI_SIZE_TO_PAGES(size), sectionData, status);
            if (status == EFI_NOT_FOUND)
            {
                Print(L"This could mean you don't have enough memory\n");
            }
            goto Done;
        }

        size = sections[i].SizeOfRawData;
        status = THISCALL(kernel, Read, &size, sectionData);
        if (EFI_ERROR(status))
        {
            Print(L"Failed to read %u bytes to 0x%lX: %r\n", size, sectionData,
                  status);
            goto Done;
        }
    }

    PVOID stack = NULL;

    size = KERNEL_BOOT_STACK_SIZE;
    Print(L"Allocating 0x%lX-page stack for kernel\n", EFI_SIZE_TO_PAGES(size));
    status = BS->AllocatePages(AllocateAnyPages, EfiLoaderData,
                               EFI_SIZE_TO_PAGES(size), (PUINT_PTR)&stack);
    if (EFI_ERROR(status))
    {
        Print(L"Failed to allocate 0x%lX-page boot stack: %r\n",
              EFI_SIZE_TO_PAGES(size), status);
        goto Done;
    }

    Print(L"Getting framebuffer\n");
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop = NULL;
    status = GetProtocol(&GraphicsOutputProtocol, NULL, true, &gop);
    if (EFI_ERROR(status))
    {
        goto Done;
    }

    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* gopInfo = NULL;
    SIZE_T gopInfoSize = 0;
    status = THISCALL(gop, QueryMode, gop->Mode ? gop->Mode->Mode : 0,
                      &gopInfoSize, &gopInfo);
    if (status == EFI_NOT_STARTED)
    {
        status = THISCALL(gop, SetMode, 0);
    }
    if (EFI_ERROR(status))
    {
        Print(L"Failed to get video mode: %r\n", status);
        goto Done;
    }

    SIZE_T nativeVideoMode = gop->Mode->Mode;
    Print(L"Setting video to mode %d\n", nativeVideoMode);
    status = THISCALL(gop, SetMode, nativeVideoMode);
    if (EFI_ERROR(status))
    {
        Print(L"Failed to set video to mode %d: %r\n", nativeVideoMode, status);
        goto Done;
    }

    Print(L"Got %ld-byte %ux%u framebuffer at 0x%lX with %ld-pixel scanlines\n",
          gop->Mode->FrameBufferSize, gop->Mode->Info->HorizontalResolution,
          gop->Mode->Info->VerticalResolution, gop->Mode->FrameBufferBase,
          gop->Mode->Info->PixelsPerScanLine);

    PKERNEL_BOOT_DATA bootData =
        (PKERNEL_BOOT_DATA)(stack + EFI_SIZE_TO_PAGES(size) * EFI_PAGE_SIZE -
                            sizeof(KERNEL_BOOT_DATA));
    SetMem(bootData, sizeof(KERNEL_BOOT_DATA), 0);

    bootData->efiRt = RT;

    Print(L"Getting memory map\n");
    UINT_PTR mapKey = 0;
    UINT32 descriptorVersion = 0;
    status =
        BS->GetMemoryMap(&bootData->memoryMapSize, bootData->memoryMap, &mapKey,
                         &bootData->memoryMapEntrySize, &descriptorVersion);
    bootData->memoryMapSize +=
        2 * bootData->memoryMapEntrySize; // 1 for the additional allocation, 1
                                          // to terminate it or something
    status = BS->AllocatePool(EfiLoaderData, bootData->memoryMapSize,
                              (PVOID*)&bootData->memoryMap);
    if (EFI_ERROR(status))
    {
        Print(L"Failed to allocate memory for memory map: %r\n");
        goto Done;
    }
    status =
        BS->GetMemoryMap(&bootData->memoryMapSize, bootData->memoryMap, &mapKey,
                         &bootData->memoryMapEntrySize, &descriptorVersion);
    if (EFI_ERROR(status))
    {
        Print(L"Failed to get memory map: %r\n");
        goto Done;
    }
    Print(L"Got %lu %lu-byte version %u memory descriptors\n",
          bootData->memoryMapSize / bootData->memoryMapEntrySize,
          bootData->memoryMapEntrySize, descriptorVersion);

    bootData->framebuffer = (PBOOT_PIXEL)gop->Mode->FrameBufferBase;
    bootData->framebufferSize = gop->Mode->FrameBufferSize;
    bootData->framebufferWidth = gop->Mode->Info->HorizontalResolution;
    bootData->framebufferHeight = gop->Mode->Info->VerticalResolution;
    bootData->framebufferScanlineSize = gop->Mode->Info->PixelsPerScanLine;

    PFN_KERNEL_ENTRY kernelEntry =
        (PFN_KERNEL_ENTRY)(ntHeaders.OptionalHeader.ImageBase +
                   ntHeaders.OptionalHeader.AddressOfEntryPoint);
    Print(L"Exiting boot services and jumping to 0x%lX with stack 0x%lX in 3 seconds\n",
          kernelEntry, stack);
    BS->Stall(3000000);
    BS->ExitBootServices(imageHandle, mapKey);

    EnterKernel(kernelEntry, stack, bootData);

Done:
    return status;
}
