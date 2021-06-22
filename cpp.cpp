extern "C" {

#pragma section(".CRT$XIA", long, read)
#pragma section(".CRT$XIZ", long, read)
#pragma section(".CRT$XCA", long, read)
#pragma section(".CRT$XCZ", long, read)
#pragma section(".CRT$XPA", long, read)
#pragma section(".CRT$XPZ", long, read)
#pragma section(".CRT$XTA", long, read)
#pragma section(".CRT$XTZ", long, read)
#pragma comment(linker, "/merge:.CRT=.rdata")

using _PVFV = void(_cdecl *)(void);
using _PIFV = int(_cdecl *)(void);

// C initializers
__declspec(allocate(".CRT$XIA")) _PIFV __xi_a[] = {0};
__declspec(allocate(".CRT$XIZ")) _PIFV __xi_z[] = {0};

// C++ initializers
__declspec(allocate(".CRT$XCA")) _PVFV __xc_a[] = {0};
__declspec(allocate(".CRT$XCZ")) _PVFV __xc_z[] = {0};

// C pre-terminators
__declspec(allocate(".CRT$XPA")) _PVFV __xp_a[] = {0};
__declspec(allocate(".CRT$XPZ")) _PVFV __xp_z[] = {0};

// C terminators
__declspec(allocate(".CRT$XTA")) _PVFV __xt_a[] = {0};
__declspec(allocate(".CRT$XTZ")) _PVFV __xt_z[] = {0};

}

#include <fltKernel.h>

#include "cpp.hpp"

#include <exception>
#include <typeinfo>

void *__cdecl operator new(size_t count)
{
    void *result = ExAllocatePoolWithTag(NonPagedPoolNx, count, 'WDMX');
    if (!result) {
        ExRaiseStatus(STATUS_FATAL_MEMORY_EXHAUSTION);
    }
    return result;
}

void *__cdecl operator new(size_t, void *address)
{
    return address;
}

void __cdecl operator delete(void *address)
{
    ExFreePoolWithTag(address, 'WDMX');
}

void __cdecl operator delete[](void *address)
{
    ExFreePoolWithTag(address, 'WDMX');
}

void __cdecl operator delete(void *, void *) {}

void __cdecl operator delete(void *address, size_t)
{
    ::operator delete(address);
}

type_info::~type_info() noexcept {}

namespace std {

    _Prhand _Raise_handler;

    [[noreturn]] void __cdecl _Xbad_alloc()
    {
        ExRaiseStatus(STATUS_INTERNAL_ERROR);
    }
    [[noreturn]] void __cdecl _Xinvalid_argument(_In_z_ const char *)
    {
        ExRaiseStatus(STATUS_INTERNAL_ERROR);
    }
    [[noreturn]] void __cdecl _Xlength_error(_In_z_ const char *)
    {
        ExRaiseStatus(STATUS_INTERNAL_ERROR);
    }
    [[noreturn]] void __cdecl _Xout_of_range(_In_z_ const char *)
    {
        ExRaiseStatus(STATUS_INTERNAL_ERROR);
    }
    [[noreturn]] void __cdecl _Xoverflow_error(_In_z_ const char *)
    {
        ExRaiseStatus(STATUS_INTERNAL_ERROR);
    }
    [[noreturn]] void __cdecl _Xruntime_error(_In_z_ const char *)
    {
        ExRaiseStatus(STATUS_INTERNAL_ERROR);
    }
    [[noreturn]] void __cdecl _Xbad_function_call()
    {
        ExRaiseStatus(STATUS_INTERNAL_ERROR);
    }

    const char *__cdecl _Syserror_map(int) { return "unknown error"; }
    int __cdecl _Winerror_map(int) { return 0; }

    unsigned int __cdecl _Random_device()
    {
        ULONG value = 0;
        return RtlRandomEx(&value);
    }

}  // namespace std

extern "C" [[nodiscard]] size_t __stdcall __std_system_error_allocate_message(
    unsigned long _Message_id, char **_Ptr_str) noexcept
{
    return 0;
}

extern "C" void __stdcall __std_system_error_deallocate_message(
    char *_Str) noexcept
{
}

[[noreturn]] void __cdecl _invalid_parameter_noinfo_noreturn()
{
    ExRaiseStatus(STATUS_INVALID_PARAMETER);
}

[[noreturn]] void __cdecl _invoke_watson(_In_opt_z_ wchar_t const *Expression,
                                         _In_opt_z_ wchar_t const *FunctionName,
                                         _In_opt_z_ wchar_t const *FileName,
                                         _In_ unsigned int LineNo,
                                         _In_ uintptr_t Reserved)
{
    ExRaiseStatus(STATUS_INVALID_PARAMETER);
}

using AtExitCallback = void(__cdecl *)();
static AtExitCallback AtExitCallbacks[32];
static AtExitCallback *LastAtExitCallback = AtExitCallbacks - 1;

template<typename F>
static void InvokeFunctions(F *begin, F *end)
{
    for (auto pf = begin; pf < end; pf++) {
        if (*pf == nullptr) {
            continue;
        }
        using return_type = std::invoke_result_t<F>;
        static_assert(std::is_same_v<void, return_type> ||
                      std::is_same_v<int, return_type>);
        if constexpr (std::is_same_v<void, return_type>) {
            (**pf)();
        }
        else if constexpr (std::is_same_v<int, return_type>) {
            if ((**pf)()) {
                ExRaiseStatus(STATUS_INTERNAL_ERROR);
            }
        }
    }
}

void InvokeCtors()
{
    InvokeFunctions(__xi_a, __xi_z);
    InvokeFunctions(__xc_a, __xc_z);
}

void InvokeDtors()
{
    InvokeFunctions(AtExitCallbacks, LastAtExitCallback + 1);
    InvokeFunctions(__xp_a, __xp_z);
    InvokeFunctions(__xt_a, __xt_z);
}

extern "C" int __cdecl atexit(AtExitCallback callback)
{
    if (!callback) {
        return 0;
    }
    *reinterpret_cast<AtExitCallback *>(InterlockedAdd64(
        reinterpret_cast<LONG64 *>(&LastAtExitCallback), sizeof(void *))) =
        callback;
    return 1;
}
