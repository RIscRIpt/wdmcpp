#include <fltKernel.h>

#include "cpp.hpp"

extern "C" DRIVER_INITIALIZE DriverEntry;
DRIVER_UNLOAD DriverUnload;

void DriverUnload(PDRIVER_OBJECT)
{
    InvokeDtors();
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING)
{
    InvokeCtors();
    DriverObject->DriverUnload = DriverUnload;
    return STATUS_SUCCESS;
}
