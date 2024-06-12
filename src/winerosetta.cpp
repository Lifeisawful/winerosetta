#include <windows.h>
#include <cstdint>
#include <cstdio>

LONG WINAPI
VectoredHandler1(
    struct _EXCEPTION_POINTERS *ExceptionInfo
    )
{
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ILLEGAL_INSTRUCTION) {
        auto context = ExceptionInfo->ContextRecord;

        if (*reinterpret_cast<uint16_t*>(context->Eip) == 0xD063) {
            // emulate arpl    ax, dx
            /*
IF RPL bits(0,1) of DEST < RPL bits(0,1) of SRC
THEN
   ZF := 1;
   RPL bits(0,1) of DEST := RPL bits(0,1) of SRC;
ELSE
   ZF := 0;
FI;            
            */
            auto dest = reinterpret_cast<uint16_t*>(&context->Eax);
            auto src = reinterpret_cast<uint16_t*>(&context->Edx);
            if ((*dest & 3) < (*src & 3)) {
                context->EFlags |= 0x40; // set ZF
                *dest = (*dest & ~3) | (*src & 3);
            } else {
                context->EFlags &= ~0x40; // clear ZF
            }

            context->Eip += 2;

            return EXCEPTION_CONTINUE_EXECUTION;
        }

        // .text:006FA876 DC D8                                               fcomp   st
        if (*reinterpret_cast<uint16_t*>(context->Eip) == 0xD8DC) {
            DWORD oldProtect;
            VirtualProtect(reinterpret_cast<void*>(context->Eip), 2, PAGE_EXECUTE_READWRITE, &oldProtect);
            *reinterpret_cast<uint16_t*>(context->Eip) = 0xD8D8; // fcomp st0
            VirtualProtect(reinterpret_cast<void*>(context->Eip), 2, PAGE_EXECUTE_READ, &oldProtect);
            return EXCEPTION_CONTINUE_EXECUTION;
        }

        char buffer[256];
        sprintf(buffer, "Exception: %08X at %08X", ExceptionInfo->ExceptionRecord->ExceptionCode, context->Eip);
        MessageBoxA(NULL, buffer, "Win32", MB_OK);
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

BOOL WINAPI DllMain(HMODULE mod, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        AddVectoredExceptionHandler(1, VectoredHandler1);
    }
    return TRUE;
}