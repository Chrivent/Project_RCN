#pragma once

#include "CoreMinimal.h"

class FPruneTableHelpers
{
public:
    static FString JoinPath(const FString& Dir, const FString& Filename);
    static bool CheckCachedTable(const FString& Name, void* Ptr, int32 Len, const FString& CacheDir);
    static void ReadFromFile(void* Ptr, int32 Len, const FString& FilePath);
    static bool MakeDir(const FString& CacheDir);
    static void DumpToFile(void* Ptr, int32 Len, const FString& Name, const FString& CacheDir);
};
