#pragma once

#include "CubieCube.generated.h"

#define CORNER_COUNT 8
#define EDGE_COUNT 12

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

    static const TArray<FCubieCube>& GetMoveCube();
};

FCubieCube GetCubieCube();

int32 Cnk(int32 N, int32 K);
void RotateLeftCorner(ECornerType* arr, int32 l, int32 r);
void RotateRightCorner(ECornerType* arr, int32 l, int32 r);
void RotateLeftEdge(EEdgeType* arr, int32 l, int32 r);
void RotateRightEdge(EEdgeType* arr, int32 l, int32 r);
struct FFaceCube ToFaceCube(FCubieCube& CubieCube);
void CornerMultiply(FCubieCube& CubieCube, FCubieCube* b);
void EdgeMultiply(FCubieCube& CubieCube, FCubieCube* b);
void Multiply(FCubieCube& CubieCube, FCubieCube* b);
void InvCubieCube(FCubieCube& CubieCube, FCubieCube* c);
int16 GetTwist(FCubieCube& CubieCube);
void SetTwist(FCubieCube& CubieCube, int16 twist);
int16 GetFlip(FCubieCube& CubieCube);
void SetFlip(FCubieCube& CubieCube, int16 flip);
int16 CornerParity(FCubieCube& CubieCube);
int16 EdgeParity(FCubieCube& CubieCube);
int16 GetFRtoBR(FCubieCube& CubieCube);
void SetFRtoBR(FCubieCube& CubieCube, int16 idx);
int16 GetURFtoDLF(FCubieCube& CubieCube);
void SetURFtoDLF(FCubieCube& CubieCube, int16 idx);
int32 GetURtoDF(FCubieCube& CubieCube);
void SetURtoDF(FCubieCube& CubieCube, int32 idx);

int16 GetURtoUL(FCubieCube& CubieCube);
void SetURtoUL(FCubieCube& CubieCube, int16 idx);
int16 GetUBtoDF(FCubieCube& CubieCube);
void SetUBtoDF(FCubieCube& CubieCube, int16 idx);
int32 GetURFtoDLB(FCubieCube& CubieCube);
void SetURFtoDLB(FCubieCube& CubieCube, int32 idx);
int32 GetURtoBR(FCubieCube& CubieCube);
void SetURtoBR(FCubieCube& CubieCube, int32 idx);

int32 Verify(FCubieCube& CubieCube);

int32 GetURtoDF_Standalone(int16 idx1, int16 idx2);
