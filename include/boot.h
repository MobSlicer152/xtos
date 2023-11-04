#pragma once

#include "efi.h"

#include "basetsd.h"

#define KERNEL_BOOT_STACK_SIZE 0x2000

typedef enum BOOT_FRAMEBUFFER_FORMAT
{
    BootFramebufferRgb,
    BootFramebufferBgr,
} BOOT_FRAMEBUFFER_FORMAT, *PBOOT_FRAMEBUFFER_FORMAT;

typedef union BOOT_PIXEL
{
    struct
    {
        BYTE red;
	BYTE green;
	BYTE blue;
	BYTE unused;
    } be;
    struct
    {
        BYTE unused;
	BYTE blue;
	BYTE green;
	BYTE red;
    } le;
    UINT32 all;
} BOOT_PIXEL, *PBOOT_PIXEL;

typedef struct KERNEL_BOOT_DATA
{
    EFI_RUNTIME_SERVICES* efiRt;
    EFI_MEMORY_DESCRIPTOR* memoryMap;
    SIZE_T memoryMapSize;
    SIZE_T memoryMapEntrySize;
    PVOID rsdp;
    PBOOT_PIXEL framebuffer;
    SIZE_T framebufferSize;
    SIZE_T framebufferWidth;
    SIZE_T framebufferHeight;
    SIZE_T framebufferScanlineSize;
    BOOT_FRAMEBUFFER_FORMAT framebufferFormat;
} KERNEL_BOOT_DATA, *PKERNEL_BOOT_DATA;

