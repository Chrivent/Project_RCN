#pragma once

#include "CubieCube.h"
#include "CoordCube.generated.h"

#define N_TWIST     2187
#define N_FLIP      2048
#define N_SLICE1    495
#define N_SLICE2    24
#define N_PARITY    2
#define N_URFtoDLF  20160
#define N_FRtoBR    11880
#define N_URtoUL    1320
#define N_UBtoDF    1320
#define N_URtoDF    20160
#define N_URFtoDLB  40320
#define N_URtoBR    479001600
#define N_MOVE      18

USTRUCT(BlueprintType)
struct FCoordCube
{
    GENERATED_BODY()

    FCoordCube() = default;
    FCoordCube(FCubieCube& CubieCube);

    int16 Twist;
    int16 Flip;
    int16 Parity;
    int16 FRtoBR;
    int16 URFtoDLF;
    int16 URtoUL;
    int16 UBtoDF;
    int32 URtoDF;
};

extern int16 TwistMove[N_TWIST][N_MOVE];
extern int16 FlipMove[N_FLIP][N_MOVE];
extern int16 ParityMove[2][18];

extern int16 FRtoBR_Move[N_FRtoBR][N_MOVE];
extern int16 URFtoDLF_Move[N_URFtoDLF][N_MOVE];
extern int16 URtoDF_Move[N_URtoDF][N_MOVE];

extern int16 URtoUL_Move[N_URtoUL][N_MOVE];
extern int16 UBtoDF_Move[N_UBtoDF][N_MOVE];
extern int16 MergeURtoULandUBtoDF[336][336];

extern int8 Slice_URFtoDLF_Parity_Prun[N_SLICE2 * N_URFtoDLF * N_PARITY / 2];
extern int8 Slice_URtoDF_Parity_Prun[N_SLICE2 * N_URtoDF * N_PARITY / 2];
extern int8 Slice_Twist_Prun[N_SLICE1 * N_TWIST / 2 + 1];
extern int8 Slice_Flip_Prun[N_SLICE1 * N_FLIP / 2];

extern int32 PRUNING_INITED;
void InitPruning(const FString& CacheDir);
void SetPruning(int8* table, int32 index, int8 value);
int8 GetPruning(const int8* table, int32 index);

static bool CheckCachedTable(const FString& Name, void* Ptr, int32 Len, const FString& CacheDir);
static void DumpToFile(void* Ptr, int32 Len, const FString& Name, const FString& CacheDir);
