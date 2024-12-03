//
// Created by ronan on 11/19/24.
//
#include <common.h>

// Give access to global instance for custom WinAPI
extern GATE Gate;

SIZE_T StringLengthA(_In_ LPCSTR String) {
    LPCSTR String2;
    for (String2 = String; *String2; ++String2);
    return (String2 - String);
}

SIZE_T StringLengthW(_In_ LPCWSTR String)
{
	LPCWSTR String2;

	for (String2 = String; *String2; ++String2);

	return (String2 - String);
}

INT StringCompareA(_In_ LPCSTR String1, _In_ LPCSTR String2)
{
    for (; *String1 == *String2; String1++, String2++)
    {
        if (*String1 == '\0')
            return 0;
    }

    return ((*(LPCSTR)String1 < *(LPCSTR)String2) ? -1 : +1);
}

INT StringCompareW(_In_ LPCWSTR String1, _In_ LPCWSTR String2)
{
    for (; *String1 == *String2; String1++, String2++)
    {
        if (*String1 == '\0')
            return 0;
    }

    return ((*(LPCWSTR)String1 < *(LPCWSTR)String2) ? -1 : +1);
}

INT StringNCompareA(_In_ LPCSTR String1, _In_ LPCSTR String2, _In_ INT num)
{
    for (INT i = 0; *String1 == *String2; String1++, String2++, i++)
    {
        if (*String1 == '\0' || i > num)
            return 0;
    }

    return ((*(LPCSTR)String1 < *(LPCSTR)String2) ? -1 : +1);
}

INT VxMemCompare(_In_ LPVOID First, _In_ LPVOID Second, size_t num) {
    LPCSTR *cFirst = (LPCSTR*)First;
    LPCSTR *cSecond = (LPCSTR*) Second;

    for (INT i = 0; *cFirst == *cSecond; cFirst++, cSecond++, i++)
    {
        if (i > num) return 0;
    }

    return ((*(LPCSTR)cFirst < *(LPCSTR)cSecond) ? -1 : +1);
}

__attribute__((optimize("O0")))
VOID VxZeroMemoryEx(_Inout_ PVOID Destination, _In_ SIZE_T Size)
{
	PULONG Dest = (PULONG)Destination;
	SIZE_T Count = Size / sizeof(ULONG);

	while (Count > 0)
	{
		*Dest = 0;
		Dest++;
		Count--;
	}

	return;
}

#ifdef DEBUG
void VxPrintf(const char* pszFormat, ...) {
    char buf[1024];
    va_list argList;
    va_start(argList, pszFormat);
    Gate.Win32.wvsprintfA(buf, pszFormat, argList);
    va_end(argList);
    DWORD done;
    Gate.Win32.WriteFile(Gate.Win32.GetStdHandle(STD_OUTPUT_HANDLE), buf, StringLengthA(buf), &done, NULL);
}

void VxPrintBytes(const PUCHAR data, SIZE_T length) {
    size_t bytes_per_row = 16; // Number of bytes per row for structured output
    for (size_t i = 0; i < length; i += bytes_per_row) {
        unsigned char buf[16] = { 0 };

        // Print the hex representation
        for (size_t j = 0; j < bytes_per_row; ++j) {
            if (i + j < length)
                buf[j] = data[i + j];
        }

        VxPrintf("%08zx  %02x", i, buf);
    }
}
#endif

PTEB GetTeb(VOID)
{
    #if defined(_WIN64)
        return (PTEB)__readgsqword(0x30);
    #elif defined(_WIN32)
        return (PTEB)__readfsdword(0x18);
    #endif
}

PVOID WINAPI MyMemcpy(
    _Out_ PVOID Destination,
    _In_ const PVOID Source,
    _In_ SIZE_T Length
)
{
    PUCHAR dest = (PUCHAR)Destination;
    PUCHAR src = (const PUCHAR)Source;

    while (Length--)
    {
        *dest++ = *src++;
    }

    return Destination;
}
