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

    int16 Twist;
    int16 Flip;
    int16 Parity;
    int16 FRtoBR;
    int16 URFtoDLF;
    int16 URtoUL;
    int16 UBtoDF;
    int32 URtoDF;
};

// ******************************************Phase 1 move tables*****************************************************

// Move table for the twists of the corners
// twist < 2187 in phase 2.
// twist = 0 in phase 2.
extern short TwistMove[N_TWIST][N_MOVE];

// Move table for the flips of the edges
// flip < 2048 in phase 1
// flip = 0 in phase 2.
extern short FlipMove[N_FLIP][N_MOVE];

// Parity of the corner permutation. This is the same as the parity for the edge permutation of a valid cube.
// parity has values 0 and 1
extern short ParityMove[2][18];

// ***********************************Phase 1 and 2 movetable********************************************************

// Move table for the four UD-slice edges FR, FL, Bl and BR
// FRtoBRMove < 11880 in phase 1
// FRtoBRMove < 24 in phase 2
// FRtoBRMove = 0 for solved cube
extern short FRtoBR_Move[N_FRtoBR][N_MOVE];

// Move table for permutation of six corners. The positions of the DBL and DRB corners are determined by the parity.
// URFtoDLF < 20160 in phase 1
// URFtoDLF < 20160 in phase 2
// URFtoDLF = 0 for solved cube.
extern short URFtoDLF_Move[N_URFtoDLF][N_MOVE];

// Move table for the permutation of six U-face and D-face edges in phase2. The positions of the DL and DB edges are
// determined by the parity.
// URtoDF < 665280 in phase 1
// URtoDF < 20160 in phase 2
// URtoDF = 0 for solved cube.
extern short URtoDF_Move[N_URtoDF][N_MOVE];

// **************************helper move tables to compute URtoDF for the beginning of phase2************************

// Move table for the three edges UR,UF and UL in phase1.
extern short URtoUL_Move[N_URtoUL][N_MOVE];

// Move table for the three edges UB,DR and DF in phase1.
extern short UBtoDF_Move[N_UBtoDF][N_MOVE];

// Table to merge the coordinates of the UR,UF,UL and UB,DR,DF edges at the beginning of phase2
extern short MergeURtoULandUBtoDF[336][336];

// ****************************************Pruning tables for the search*********************************************

// Pruning table for the permutation of the corners and the UD-slice edges in phase2.
// The pruning table entries give a lower estimation for the number of moves to reach the solved cube.
extern signed char Slice_URFtoDLF_Parity_Prun[N_SLICE2 * N_URFtoDLF * N_PARITY / 2];

// Pruning table for the permutation of the edges in phase2.
// The pruning table entries give a lower estimation for the number of moves to reach the solved cube.
extern signed char Slice_URtoDF_Parity_Prun[N_SLICE2 * N_URtoDF * N_PARITY / 2];

// Pruning table for the twist of the corners and the position (not permutation) of the UD-slice edges in phase1
// The pruning table entries give a lower estimation for the number of moves to reach the H-subgroup.
extern signed char Slice_Twist_Prun[N_SLICE1 * N_TWIST / 2 + 1];

// Pruning table for the flip of the edges and the position (not permutation) of the UD-slice edges in phase1
// The pruning table entries give a lower estimation for the number of moves to reach the H-subgroup.
extern signed char Slice_Flip_Prun[N_SLICE1 * N_FLIP / 2];

extern int PRUNING_INITED;
void InitPruning(const FString& CacheDir);

// Set pruning value in table. Two values are stored in one char.
void SetPruning(signed char *table, int index, signed char value);

// Extract pruning value
signed char GetPruning(const signed char *table, int index);

FCoordCube* GetCoordCube(FCubieCube* CubieCube);
void Move(FCoordCube* CoordCube, int32 MoveIndex, const FString& CacheDir);

static bool CheckCachedTable(const FString& Name, void* Ptr, int32 Len, const FString& CacheDir);
static void DumpToFile(void* Ptr, int32 Len, const FString& Name, const FString& CacheDir);
