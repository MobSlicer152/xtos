#include "Base.h"
#include "Library/PcdLib.h"

static UINT8 PcdGet8(PcdSpeculationBarrierType) = 0x1;
static UINT32 PcdGet32(PcdMaximumLinkedListLength) = 1024;
static UINT32 PcdGet32(PcdMaximumAsciiStringLength) = 16384;
static UINT32 PcdGet32(PcdMaximumUnicodeStringLength) = 16384;
static UINT32 PcdGet32(PcdUefiLibMaxPrintBufferSize) = 16384;
static BOOLEAN PcdGetBool(PcdVerifyNodeInList) = FALSE;
static BOOLEAN PcdGetBool(PcdUgaConsumeSupport) = TRUE;
static BOOLEAN PcdGetBool(PcdDriverDiagnosticsDisable) = TRUE;
static BOOLEAN PcdGetBool(PcdDriverDiagnostics2Disable) = TRUE;
static BOOLEAN PcdGetBool(PcdComponentNameDisable) = TRUE;
static BOOLEAN PcdGetBool(PcdComponentName2Disable) = TRUE;

