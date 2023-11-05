#include "basetsd.h"
#include "boot.h"

VOID SetPixel(IN UINT32 x, IN UINT32 y, IN BOOT_PIXEL pixel,
              IN BOOT_FRAMEBUFFER_FORMAT format)
{
    if (x > g_bootData->framebufferWidth || y > g_bootData->framebufferHeight)
    {
        return;
    }
    g_bootData->framebuffer[y * g_bootData->framebufferScanlineSize + x] =
        format == g_bootData->framebufferFormat ? pixel : CONVERT_PIXEL(pixel);
}

VOID FillArea(IN UINT32 xStart, IN UINT32 yStart, IN UINT32 xEnd,
              IN UINT32 yEnd, IN BOOT_PIXEL pixel,
              IN BOOT_FRAMEBUFFER_FORMAT format)
{
    for (UINT32 y = yStart; y < yEnd; y++)
    {
        for (UINT32 x = xStart; x < xEnd; x++)
        {
            SetPixel(x, y, pixel, format);
        }
    }
}

VOID FillScreen(IN BOOT_PIXEL pixel, IN BOOT_FRAMEBUFFER_FORMAT format)
{
    FillArea(0, 0, g_bootData->framebufferWidth, g_bootData->framebufferHeight,
             pixel, format);
}
