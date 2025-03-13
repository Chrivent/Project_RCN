#pragma once

#include "CubieCube.generated.h"

enum class EEdgeType : uint8;
enum class ECornerType : uint8;

USTRUCT(BlueprintType)
struct FCubieCube
{
    GENERATED_BODY()
    
    FCubieCube();

    static int32 Cnk(const int32 N, int32 K);

    template <typename T>
    void Rotate(TArray<T>& Arr, const int32 L, const int32 R);

    void CornerMultiply(const int32 MoveCubeIdx);
    void EdgeMultiply(const int32 MoveCubeIdx);
    
    int16 GetTwist();
    void SetTwist(int16 Twist);
    int16 GetFlip();
    void SetFlip(int16 Flip);
    int16 CornerParity();
    int16 EdgeParity();
    int16 GetFRtoBR();
    void SetFRtoBR(const int16 Idx);
    int16 GetURFtoDLF();
    void SetURFtoDLF(const int16 Idx);
    int32 GetURtoDF();
    void SetURtoDF(const int32 Idx);

    int16 GetURtoUL();
    void SetURtoUL(const int16 Idx);
    int16 GetUBtoDF();
    void SetUBtoDF(const int16 Idx);

    int32 Verify();

    static int32 GetURtoDF_Standalone(const int16 Idx1, const int16 Idx2);
    
    TArray<ECornerType> Cp;
    TArray<int8> Co;
    TArray<EEdgeType> Ep;
    TArray<int8> Eo;

    static const TArray<FCubieCube> MoveCube;
};
