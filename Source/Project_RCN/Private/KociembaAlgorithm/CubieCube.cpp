#include "KociembaAlgorithm/CubieCube.h"

#include "KociembaAlgorithm/CubeSolver.h"

const TArray<FCubieCube> FCubieCube::MoveCube = []
{
    TArray<FCubieCube> TempMoveCube;
    TempMoveCube.SetNum(6);
    constexpr ECornerType TempCp[6][8] = {
        { ECornerType::UBR, ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::DFR, ECornerType::DLF, ECornerType::DBL, ECornerType::DRB },
        { ECornerType::DFR, ECornerType::UFL, ECornerType::ULB, ECornerType::URF, ECornerType::DRB, ECornerType::DLF, ECornerType::DBL, ECornerType::UBR },
        { ECornerType::UFL, ECornerType::DLF, ECornerType::ULB, ECornerType::UBR, ECornerType::URF, ECornerType::DFR, ECornerType::DBL, ECornerType::DRB },
        { ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::UBR, ECornerType::DLF, ECornerType::DBL, ECornerType::DRB, ECornerType::DFR },
        { ECornerType::URF, ECornerType::ULB, ECornerType::DBL, ECornerType::UBR, ECornerType::DFR, ECornerType::UFL, ECornerType::DLF, ECornerType::DRB },
        { ECornerType::URF, ECornerType::UFL, ECornerType::UBR, ECornerType::DRB, ECornerType::DFR, ECornerType::DLF, ECornerType::ULB, ECornerType::DBL }
    };
    constexpr int8 TempCo[6][8] = {
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 2, 0, 0, 1, 1, 0, 0, 2 },
        { 1, 2, 0, 0, 2, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 2, 0, 0, 2, 1, 0 },
        { 0, 0, 1, 2, 0, 0, 2, 1 }
    };
    constexpr EEdgeType TempEp[6][12] = {
        { EEdgeType::UB, EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR },
        { EEdgeType::FR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::BR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, EEdgeType::DR, EEdgeType::FL, EEdgeType::BL, EEdgeType::UR },
        { EEdgeType::UR, EEdgeType::FL, EEdgeType::UL, EEdgeType::UB, EEdgeType::DR, EEdgeType::FR, EEdgeType::DL, EEdgeType::DB, EEdgeType::UF, EEdgeType::DF, EEdgeType::BL, EEdgeType::BR },
        { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, EEdgeType::DR, EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR },
        { EEdgeType::UR, EEdgeType::UF, EEdgeType::BL, EEdgeType::UB, EEdgeType::DR, EEdgeType::DF, EEdgeType::FL, EEdgeType::DB, EEdgeType::FR, EEdgeType::UL, EEdgeType::DL, EEdgeType::BR },
        { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::BR, EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::BL, EEdgeType::FR, EEdgeType::FL, EEdgeType::UB, EEdgeType::DB }
    };
    constexpr int8 TempEo[6][12] = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1 }
    };
    for (int32 i = 0; i < 6; i++)
    {
        TempMoveCube[i].Cp = TArray(TempCp[i], UE_ARRAY_COUNT(TempCp[i]));
        TempMoveCube[i].Co = TArray(TempCo[i], UE_ARRAY_COUNT(TempCo[i]));
        TempMoveCube[i].Ep = TArray(TempEp[i], UE_ARRAY_COUNT(TempEp[i]));
        TempMoveCube[i].Eo = TArray(TempEo[i], UE_ARRAY_COUNT(TempEo[i]));
    }
    return TempMoveCube;
}();

FCubieCube::FCubieCube()
{
    Cp = { 
        ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::UBR, 
        ECornerType::DFR, ECornerType::DLF, ECornerType::DBL, ECornerType::DRB 
    };
    Co  = { 0, 0, 0, 0, 0, 0, 0, 0 };
    Ep = { 
        EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, 
        EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, 
        EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR 
    };
    Eo = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}

int32 FCubieCube::Cnk(const int32 N, int32 K)
{
    if (N < K)
    {
        return 0;
    }
    if (K > N / 2)
    {
        K = N - K;
    }
    int32 Result = 1;
    for (int32 i = 0; i < K; i++)
    {
        Result = Result * (N - i) / (i + 1);
    }
    return Result;
}

template <typename T>
void FCubieCube::Rotate(TArray<T>& Arr, const int32 L, const int32 R)
{
    const T Temp = Arr[L];
    Arr.RemoveAt(L);
    Arr.Insert(Temp, R);
}

void FCubieCube::CornerMultiply(const int32 MoveCubeIdx)
{
    TArray<ECornerType> CornPerm;
    TArray<int8> CornOri;
    CornPerm.SetNum(CORNER_COUNT);
    CornOri.SetNum(CORNER_COUNT);
    const FCubieCube& Move = MoveCube[MoveCubeIdx];
    for (int32 i = 0; i < CORNER_COUNT; i++)
    {
        const int32 Idx = static_cast<int32>(Move.Cp[i]);
        CornPerm[i] = Cp[Idx];
        CornOri[i] = (Co[Idx] + (Move.Co[i] < 3 ? Move.Co[i] : Move.Co[i] - 3) + 3) % 3;
    }
    Cp = CornPerm;
    Co = CornOri;
}

void FCubieCube::EdgeMultiply(const int32 MoveCubeIdx)
{
    TArray<EEdgeType> EdgePerm;
    TArray<int8> EdgeOri;
    EdgePerm.SetNum(EDGE_COUNT);
    EdgeOri.SetNum(EDGE_COUNT);
    const FCubieCube& Move = MoveCube[MoveCubeIdx];
    for (int32 i = 0; i < EDGE_COUNT; i++)
    {
        const int32 Idx = static_cast<int32>(Move.Ep[i]);
        EdgePerm[i] = Ep[Idx];
        EdgeOri[i] = (Move.Eo[i] + Eo[Idx]) % 2;
    }
    Ep = EdgePerm;
    Eo = EdgeOri;
}

int16 FCubieCube::GetTwist()
{
    int16 Twist = 0;
    for (int32 i = 0; i < CORNER_COUNT - 1; i++)
    {
        Twist = Twist * 3 + Co[i];
    }
    return Twist;
}

void FCubieCube::SetTwist(int16 Twist)
{
    int32 TempTwist = 0;
    for (int32 i = CORNER_COUNT - 2; i >= 0; i--)
    {
        Co[i] = Twist % 3;
        TempTwist += Co[i];
        Twist /= 3;
    }
    Co[CORNER_COUNT - 1] = (3 - TempTwist % 3) % 3;
}

int16 FCubieCube::GetFlip()
{
    int16 Flip = 0;
    for (int32 i = 0; i < EDGE_COUNT - 1; i++)
    {
        Flip = Flip * 2 + Eo[i];
    }
    return Flip;
}

void FCubieCube::SetFlip(int16 Flip)
{
    int32 TempFlip = 0;
    for (int32 i = EDGE_COUNT - 2; i >= 0; i--)
    {
        Eo[i] = Flip % 2;
        TempFlip += Eo[i];
        Flip /= 2;
    }
    Eo[EDGE_COUNT - 1] = (2 - TempFlip % 2) % 2;
}

int16 FCubieCube::CornerParity()
{
    int16 s = 0;
    for (int32 i = CORNER_COUNT - 1; i >= 1; i--)
    {
        for (int32 j = i - 1; j >= 0; j--)
        {
            if (Cp[j] > Cp[i])
            {
                s++;
            }
        }
    }
    return s % 2;
}

int16 FCubieCube::EdgeParity()
{
    int16 s = 0;
    for (int32 i = EDGE_COUNT - 1; i >= 1; i--)
    {
        for (int32 j = i - 1; j >= 0; j--)
        {
            if (Ep[j] > Ep[i])
            {
                s++;
            }
        }
    }
    return s % 2;
}

int16 FCubieCube::GetFRtoBR()
{
    int16 a = 0, b = 0;
    TArray<EEdgeType> Edge4;
    Edge4.SetNum(4);
    int32 x = 0;
    for (int32 i = EDGE_COUNT - 1; i >= 0; i--)
    {
        if (EEdgeType::FR <= Ep[i] && Ep[i] <= EEdgeType::BR)
        {
            a += Cnk(11 - i, x + 1);
            Edge4[3 - x++] = Ep[i];
        }
    }
    for (int32 i = 3; i > 0; i--)
    {
        int32 k = 0;
        while (static_cast<int32>(Edge4[i]) != i + 8)
        {
            Rotate(Edge4, 0, i);
            k++;
        }
        b = (i + 1) * b + k;
    }
    return 24 * a + b;
}

void FCubieCube::SetFRtoBR(const int16 Idx)
{
    int32 a = Idx / 24, b = Idx % 24;
    TArray Edge4 = { EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR };
    TArray OtherEdge = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB };
    for (int32 i = 0; i < EDGE_COUNT; i++)
    {
        Ep[i] = EEdgeType::DB;
    }
    for (int32 i = 1; i < 4; i++)
    {
        int32 k = b % (i + 1);
        b /= i + 1;
        while (k-- > 0)
        {
            Rotate(Edge4, i, 0);
        }
    }
    int32 x = 3;
    for (int32 i = 0; i <= EDGE_COUNT - 1; i++)
    {
        if (a - Cnk(11 - i, x + 1) >= 0)
        {
            Ep[i] = Edge4[3 - x];
            a -= Cnk(11 - i, x-- + 1);
        }
    }
    x = 0;
    for (int32 i = 0; i <= EDGE_COUNT - 1; i++)
    {
        if (Ep[i] == EEdgeType::DB)
        {
            Ep[i] = OtherEdge[x++];
        }
    }
}

int16 FCubieCube::GetURFtoDLF()
{
    int16 a = 0, b = 0;
    TArray<ECornerType> Corner6;
    Corner6.SetNum(6);
    int32 x = 0;
    for (int32 i = 0; i <= CORNER_COUNT - 1; i++)
    {
        if (Cp[i] <= ECornerType::DLF)
        {
            a += Cnk(i, x + 1);
            Corner6[x++] = Cp[i];
        }
    }
    for (int32 i = 5; i > 0; i--)
    {
        int32 k = 0;
        while (static_cast<int32>(Corner6[i]) != i)
        {
            Rotate(Corner6, 0, i);
            k++;
        }
        b = (i + 1) * b + k;
    }
    return 720 * a + b;
}

void FCubieCube::SetURFtoDLF(const int16 Idx)
{
    int32 a = Idx / 720, b = Idx % 720;
    TArray Corner6 = { ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::UBR, ECornerType::DFR, ECornerType::DLF };
    TArray OtherCorner = { ECornerType::DBL, ECornerType::DRB };
    for (int32 i = 0; i < CORNER_COUNT; i++)
    {
        Cp[i] = ECornerType::DRB;
    }
    for (int32 i = 1; i < 6; i++)
    {
        int32 k = b % (i + 1);
        b /= i + 1;
        while (k-- > 0)
        {
            Rotate(Corner6, i, 0);
        }
    }
    int32 x = 5;
    for (int32 i = CORNER_COUNT - 1; i >= 0; i--)
    {
        if (a - Cnk(i, x + 1) >= 0)
        {
            Cp[i] = Corner6[x];
            a -= Cnk(i, x-- + 1);
        }
    }
    x = 0;
    for (int32 i = 0; i <= CORNER_COUNT - 1; i++)
    {
        if (Cp[i] == ECornerType::DRB)
        {
            Cp[i] = OtherCorner[x++];
        }
    }
}

int32 FCubieCube::GetURtoDF()
{
    int16 a = 0, b = 0;
    TArray<EEdgeType> Edge6;
    Edge6.SetNum(6);
    int32 x = 0;
    for (int32 i = 0; i <= EDGE_COUNT - 1; i++)
    {
        if (Ep[i] <= EEdgeType::DF)
        {
            a += Cnk(i, x + 1);
            Edge6[x++] = Ep[i];
        }
    }
    for (int32 i = 5; i > 0; i--)
    {
        int32 k = 0;
        while (static_cast<int32>(Edge6[i]) != i)
        {
            Rotate(Edge6, 0, i);
            k++;
        }
        b = (i + 1) * b + k;
    }
    return 720 * a + b;
}

void FCubieCube::SetURtoDF(const int32 Idx)
{
    int32 a = Idx / 720, b = Idx % 720;
    TArray Edge6 = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::DR, EEdgeType::DF };
    TArray OtherEdge = { EEdgeType::DL, EEdgeType::DB, EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR };
    for (int32 i = 0; i < EDGE_COUNT; i++)
    {
        Ep[i] = EEdgeType::BR;
    }
    for (int32 i = 1; i < 6; i++)
    {
        int32 k = b % (i + 1);
        b /= i + 1;
        while (k-- > 0)
        {
            Rotate(Edge6, i, 0);
        }
    }
    int32 x = 5;
    for (int32 i = EDGE_COUNT - 1; i >= 0; i--)
    {
        if (a - Cnk(i, x + 1) >= 0)
        {
            Ep[i] = Edge6[x];
            a -= Cnk(i, x-- + 1);
        }
    }
    x = 0;
    for (int32 i = 0; i <= EDGE_COUNT - 1; i++)
    {
        if (Ep[i] == EEdgeType::BR)
        {
            Ep[i] = OtherEdge[x++];
        }
    }
}

int16 FCubieCube::GetURtoUL()
{
    int16 a = 0, b = 0;
    TArray<EEdgeType> Edge3;
    Edge3.SetNum(3);
    int32 x = 0;
    for (int32 i = 0; i <= EDGE_COUNT - 1; i++)
    {
        if (Ep[i] <= EEdgeType::UL)
        {
            a += Cnk(i, x + 1);
            Edge3[x++] = Ep[i];
        }
    }
    for (int32 i = 2; i > 0; i--)
    {
        int32 k = 0;
        while (static_cast<int32>(Edge3[i]) != i)
        {
            Rotate(Edge3, 0, i);
            k++;
        }
        b = (i + 1) * b + k;
    }
    return 6 * a + b;
}

void FCubieCube::SetURtoUL(const int16 Idx)
{
    int32 a = Idx / 6, b = Idx % 6;
    TArray Edge3 = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL };
    for(int32 i = 0; i < EDGE_COUNT; i++)
    {
        Ep[i] = EEdgeType::BR;
    }
    for (int32 i = 1; i < 3; i++)
    {
        int32 k = b % (i + 1);
        b /= i + 1;
        while (k-- > 0)
        {
            Rotate(Edge3, i, 0);
        }
    }
    int32 x = 2;
    for (int32 i = EDGE_COUNT - 1; i >= 0; i--)
    {
        if (a - Cnk(i, x + 1) >= 0)
        {
            Ep[i] = Edge3[x];
            a -= Cnk(i, x-- + 1);
        }
    }
}

int16 FCubieCube::GetUBtoDF()
{
    int16 a = 0, b = 0;
    TArray<EEdgeType> Edge3;
    Edge3.SetNum(3);
    int32 x = 0;
    for (int32 i = 0; i <= EDGE_COUNT - 1; i++)
    {
        if (EEdgeType::UB <= Ep[i] && Ep[i] <= EEdgeType::DF)
        {
            a += Cnk(i, x + 1);
            Edge3[x++] = Ep[i];
        }
    }
    for (int32 i = 2; i > 0; i--)
    {
        int32 k = 0;
        while (static_cast<int32>(Edge3[i]) != 3 + i)
        {
            Rotate(Edge3, 0, i);
            k++;
        }
        b = (i + 1) * b + k;
    }
    return 6 * a + b;
}

void FCubieCube::SetUBtoDF(const int16 Idx)
{
    int32 a = Idx / 6, b = Idx % 6;
    TArray Edge3 = { EEdgeType::UB, EEdgeType::DR, EEdgeType::DF };
    for (int32 i = 0; i < EDGE_COUNT; i++)
    {
        Ep[i] = EEdgeType::BR;
    }
    for (int32 i = 1; i < 3; i++)
    {
        int32 k = b % (i + 1);
        b /= i + 1;
        while (k-- > 0)
        {
            Rotate(Edge3, i, 0);
        }
    }
    int32 x = 2;
    for (int32 i = EDGE_COUNT - 1; i >= 0; i--)
    {
        if (a - Cnk(i, x + 1) >= 0)
        {
            Ep[i] = Edge3[x];
            a -= Cnk(i, x-- + 1);
        }
    }
}

int32 FCubieCube::Verify()
{
    TArray<int32> EdgeCount;
    TArray<int32> CornerCount;
    EdgeCount.SetNum(EDGE_COUNT);
    CornerCount.SetNum(CORNER_COUNT);
    for(int32 i = 0; i < EDGE_COUNT; i++)
    {
        EdgeCount[static_cast<int32>(Ep[i])]++;
    }
    for (int32 i = 0; i < EDGE_COUNT; i++)
    {
        if (EdgeCount[i] != 1)
        {
            return -2;
        }
    }
    int32 Sum = 0;
    for (int32 i = 0; i < EDGE_COUNT; i++)
    {
        Sum += Eo[i];
    }
    if (Sum % 2 != 0)
    {
        return -3;
    }
    for (int32 i = 0; i < CORNER_COUNT; i++)
    {
        CornerCount[static_cast<int32>(Cp[i])]++;
    }
    for (int32 i = 0; i < 8; i++)
    {
        if (CornerCount[i] != 1)
        {
            return -4;
        }
    }
    Sum = 0;
    for (int32 i = 0; i < CORNER_COUNT; i++)
    {
        Sum += Co[i];
    }
    if (Sum % 3 != 0)
    {
        return -5;
    }
    if (EdgeParity() != CornerParity())
    {
        return -6;
    }
    return 0;
}

int32 FCubieCube::GetURtoDF_Standalone(const int16 Idx1, const int16 Idx2)
{
    FCubieCube a;
    FCubieCube b;
    a.SetURtoUL(Idx1);
    b.SetUBtoDF(Idx2);
    for (int32 i = 0; i < 8; i++)
    {
        if (a.Ep[i] != EEdgeType::BR)
        {
            if (b.Ep[i] != EEdgeType::BR)
            {
                return -1;
            }
            b.Ep[i] = a.Ep[i];
        }
    }
    return b.GetURtoDF();
}
