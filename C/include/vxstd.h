//
// Created by ronan on 11/19/24.
//

#ifndef OFFENSIVE_DEV_VXSTD_H
#define OFFENSIVE_DEV_VXSTD_H

INT StringCompareA(_In_ LPCSTR String1, _In_ LPCSTR String2);
SIZE_T StringLengthW(_In_ LPCWSTR String);
INT StringCompareW(_In_ LPCWSTR String1, _In_ LPCWSTR String2);
INT StringNCompareA(_In_ LPCSTR String1, _In_ LPCSTR String2, _In_ INT num);
size_t StringLengthA(_In_ LPCSTR String);
INT VxMemCompare(_In_ LPVOID First, _In_ LPVOID Second, size_t num);
VOID VxZeroMemoryEx(_Inout_ PVOID Destination, _In_ SIZE_T Size);

#ifdef DEBUG
void VxPrintf(const char* pszFormat, ...);
void VxPrintBytes(const PUCHAR data, SIZE_T length);
#endif

PTEB GetTeb(VOID);

PVOID WINAPI MyMemcpy(
    _Out_ PVOID Destination,
    _In_ const PVOID Source,
    _In_ SIZE_T Length
);

#endif //OFFENSIVE_DEV_VXSTD_H
