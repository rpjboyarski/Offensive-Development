//
// Created by ronan on 11/19/24.
//

#ifndef OFFENSIVE_DEV_LOADER_H
#define OFFENSIVE_DEV_LOADER_H

#include <common.h>

PVOID LdrModulePeb(
        _In_ ULONG Hash
);

PVOID LdrFunction(
        _In_ PVOID Module,
        _In_ ULONG Function
);

#endif //OFFENSIVE_DEV_LOADER_H
