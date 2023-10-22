#include "boot.h"

void EfiMain(VOID* imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
    Print(L"Hello, world");
    while (1) {}
}
