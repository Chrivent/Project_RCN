#pragma once

#include "cubiecube.generated.h"

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
FCubieCube* get_moveCube(void);
FCubieCube* get_cubiecube(void);

// n choose k
int Cnk(int n, int k);
void rotateLeft_corner(ECornerType* arr, int l, int r);
void rotateRight_corner(ECornerType* arr, int l, int r);
void rotateLeft_edge(EEdgeType* arr, int l, int r);
void rotateRight_edge(EEdgeType* arr, int l, int r);
struct FFaceCube* toFaceCube(FCubieCube* cubiecube);
void cornerMultiply(FCubieCube* cubiecube, FCubieCube* b);
void edgeMultiply(FCubieCube* cubiecube, FCubieCube* b);
void multiply(FCubieCube* cubiecube, FCubieCube* b);
void invCubieCube(FCubieCube* cubiecube, FCubieCube* c);
short getTwist(FCubieCube* cubiecube);
void setTwist(FCubieCube* cubiecube, short twist);
short getFlip(FCubieCube* cubiecube);
void setFlip(FCubieCube* cubiecube, short flip);
short cornerParity(FCubieCube* cubiecube);
short edgeParity(FCubieCube* cubiecube);
short getFRtoBR(FCubieCube* cubiecube);
void setFRtoBR(FCubieCube* cubiecube, short idx);
short getURFtoDLF(FCubieCube* cubiecube);
void setURFtoDLF(FCubieCube* cubiecube, short idx);
int getURtoDF(FCubieCube* cubiecube);
void setURtoDF(FCubieCube* cubiecube, int idx);

short getURtoUL(FCubieCube* cubiecube);
void setURtoUL(FCubieCube* cubiecube, short idx);
short getUBtoDF(FCubieCube* cubiecube);
void setUBtoDF(FCubieCube* cubiecube, short idx);
int getURFtoDLB(FCubieCube* cubiecube);
void setURFtoDLB(FCubieCube* cubiecube, int idx);
int getURtoBR(FCubieCube* cubiecube);
void setURtoBR(FCubieCube* cubiecube, int idx);

int Verify(FCubieCube* cubiecube);

int getURtoDF_standalone(short idx1, short idx2);
