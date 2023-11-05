#include "basetsd.h"
#include "boot.h"

PKERNEL_BOOT_DATA g_bootData;

VOID KernelMain(IN PKERNEL_BOOT_DATA data)
{
    g_bootData = data;

    FillScreen((BOOT_PIXEL){.rgb = {.red = 135, .green = 0, .blue = 255}},
               BootFramebufferRgb);

    while (TRUE)
    {
    }
}
