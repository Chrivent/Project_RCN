#pragma once

#include "CubieCube.generated.h"

UENUM(BlueprintType)
enum class ECornerType : uint8
{
    URF,
    UFL,
    ULB,
    UBR,
    DFR,
    DLF,
    DBL,
    DRB
};

#define CORNER_COUNT 8

UENUM(BlueprintType)
enum class EEdgeType : uint8
{
    UR,
    UF,
    UL,
    UB,
    DR,
    DF,
    DL,
    DB,
    FR,
    FL,
    BL,
    BR
};

#define EDGE_COUNT 12

USTRUCT(BlueprintType)
struct FCubieCube
{
    GENERATED_BODY()

    TArray<ECornerType> Cp;
    TArray<int8> Co;
    TArray<EEdgeType> Ep;
    TArray<int8> Eo;

    FCubieCube()
    {
        Cp.Init(ECornerType::URF, 8);
        Co.Init(0, 8);
        Ep.Init(EEdgeType::UR, 12);
        Eo.Init(0, 12);
    }
};

// this CubieCube array represents the 6 basic cube moves
FCubieCube* GetMoveCube(void);
FCubieCube* GetCubieCube(void);

// n choose k
int Cnk(int n, int k);
void RotateLeftCorner(ECornerType* arr, int l, int r);
void RotateRightCorner(ECornerType* arr, int l, int r);
void RotateLeftEdge(EEdgeType* arr, int l, int r);
void RotateRightEdge(EEdgeType* arr, int l, int r);
struct FFaceCube* ToFaceCube(FCubieCube* CubieCube);
void CornerMultiply(FCubieCube* CubieCube, FCubieCube* b);
void EdgeMultiply(FCubieCube* CubieCube, FCubieCube* b);
void Multiply(FCubieCube* CubieCube, FCubieCube* b);
void InvCubieCube(FCubieCube* CubieCube, FCubieCube* c);
short GetTwist(FCubieCube* CubieCube);
void SetTwist(FCubieCube* CubieCube, short twist);
short GetFlip(FCubieCube* CubieCube);
void SetFlip(FCubieCube* CubieCube, short flip);
short CornerParity(FCubieCube* CubieCube);
short EdgeParity(FCubieCube* CubieCube);
short GetFRtoBR(FCubieCube* CubieCube);
void SetFRtoBR(FCubieCube* CubieCube, short idx);
short GetURFtoDLF(FCubieCube* CubieCube);
void SetURFtoDLF(FCubieCube* CubieCube, short idx);
int GetURtoDF(FCubieCube* CubieCube);
void SetURtoDF(FCubieCube* CubieCube, int idx);

short GetURtoUL(FCubieCube* CubieCube);
void SetURtoUL(FCubieCube* CubieCube, short idx);
short GetUBtoDF(FCubieCube* CubieCube);
void SetUBtoDF(FCubieCube* CubieCube, short idx);
int GetURFtoDLB(FCubieCube* CubieCube);
void SetURFtoDLB(FCubieCube* CubieCube, int idx);
int GetURtoBR(FCubieCube* CubieCube);
void SetURtoBR(FCubieCube* CubieCube, int idx);

int Verify(FCubieCube* CubieCube);

int GetURtoDF_Standalone(short idx1, short idx2);
