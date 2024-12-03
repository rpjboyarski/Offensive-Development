//
// Created by ronan on 11/19/24.
//

#ifndef OFFENSIVE_DEV_APIHASHING_H
#define OFFENSIVE_DEV_APIHASHING_H

#include <common.h>

//
// Hashing defines
//
#define H_MAGIC_KEY       5381
#define H_MAGIC_SEED      5
#define H_MODULE_NTDLL    0x70e61753
#define H_MODULE_KERNEL32 0xadd31df0

ULONG HashString(
    _In_ PVOID  String,
    _In_ SIZE_T Length
);


#endif //OFFENSIVE_DEV_APIHASHING_H
