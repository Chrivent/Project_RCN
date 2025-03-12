#include "KociembaAlgorithm/CubieCube.h"

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
    TArray<ECornerType> cPerm;
    TArray<int8> cOri;
    cPerm.SetNum(CORNER_COUNT);
    cOri.SetNum(CORNER_COUNT);
    const FCubieCube& Move = MoveCube[MoveCubeIdx];
    for (int32 corn = 0; corn < CORNER_COUNT; corn++)
    {
        const int32 Idx = static_cast<int32>(Move.Cp[corn]);
        cPerm[corn] = Cp[Idx];
        cOri[corn] = (Co[Idx] + (Move.Co[corn] < 3 ? Move.Co[corn] : Move.Co[corn] - 3) + 3) % 3;
    }
    Cp = cPerm;
    Co = cOri;
}

void FCubieCube::EdgeMultiply(int32 MoveCubeIdx)
{
    TArray<EEdgeType> ePerm;
    TArray<int8> eOri;
    ePerm.SetNum(EDGE_COUNT);
    eOri.SetNum(EDGE_COUNT);
    const FCubieCube& Move = MoveCube[MoveCubeIdx];
    for (int32 edge = 0; edge < EDGE_COUNT; edge++)
    {
        const int32 Idx = static_cast<int32>(Move.Ep[edge]);
        ePerm[edge] = Ep[Idx];
        eOri[edge] = (Move.Eo[edge] + Eo[Idx]) % 2;
    }
    Ep = ePerm;
    Eo = eOri;
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
    int32 a = 0, x = 0;
    int32 b = 0;
    TArray<EEdgeType> edge4;
    edge4.SetNum(4);
    for (int32 j = EDGE_COUNT - 1; j >= 0; j--)
    {
        if (EEdgeType::FR <= Ep[j] && Ep[j] <= EEdgeType::BR)
        {
            a += Cnk(11 - j, x + 1);
            edge4[3 - x++] = Ep[j];
        }
    }
    for (int32 j = 3; j > 0; j--)
    {
        int32 k = 0;
        while (static_cast<int32>(edge4[j]) != j + 8)
        {
            Rotate(edge4, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return static_cast<int16>(24 * a + b);
}

void FCubieCube::SetFRtoBR(int16 Idx)
{
    TArray sliceEdge = { EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR };
    TArray otherEdge = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB };
    int32 b = Idx % 24;
    int32 a = Idx / 24;
    for (int32 e = 0; e < EDGE_COUNT; e++)
        Ep[e] = EEdgeType::DB;
    for (int32 j = 1; j < 4; j++)
    {
        int32 k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            Rotate(sliceEdge, j, 0);
    }
    int32 x = 3;
    for (int32 j = 0; j <= EDGE_COUNT - 1; j++)
        if (a - Cnk(11 - j, x + 1) >= 0) {
            Ep[j] = sliceEdge[3 - x];
            a -= Cnk(11 - j, x-- + 1);
        }
    x = 0;
    for (int32 j = 0; j <= EDGE_COUNT - 1; j++)
        if (Ep[j] == EEdgeType::DB)
            Ep[j] = otherEdge[x++];
}

int16 FCubieCube::GetURFtoDLF()
{
    int32 a = 0, x = 0, b = 0;
    TArray<ECornerType> corner6;
    corner6.Init(static_cast<ECornerType>(0), 6);
    for (int32 j = static_cast<int32>(ECornerType::URF); j <= static_cast<int32>(ECornerType::DRB); j++)
        if (Cp[j] <= ECornerType::DLF) {
            a += Cnk(j, x + 1);
            corner6[x++] = Cp[j];
        }

    for (int32 j = 5; j > 0; j--)
    {
        int32 k = 0;
        while (static_cast<int32>(corner6[j]) != j) {
            Rotate(corner6, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return static_cast<int16>(720 * a + b);
}

void FCubieCube::SetURFtoDLF(int16 Idx)
{
    TArray corner6 = { ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::UBR, ECornerType::DFR, ECornerType::DLF };
    TArray otherCorner = { ECornerType::DBL, ECornerType::DRB };
    int32 b = Idx % 720;
    int32 a = Idx / 720;
    for (int32 c = 0; c < CORNER_COUNT; c++)
        Cp[c] = ECornerType::DRB;

    for (int32 j = 1; j < 6; j++)
    {
        int32 k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            Rotate(corner6, j, 0);
    }
    int32 x = 5;
    for (int32 j = static_cast<int32>(ECornerType::DRB); j >= 0; j--)
        if (a - Cnk(j, x + 1) >= 0) {
            Cp[j] = corner6[x];
            a -= Cnk(j, x-- + 1);
        }
    x = 0;
    for (int32 j = static_cast<int32>(ECornerType::URF); j <= static_cast<int32>(ECornerType::DRB); j++)
        if (Cp[j] == ECornerType::DRB)
            Cp[j] = otherCorner[x++];
}

int32 FCubieCube::GetURtoDF()
{
    int32 a = 0, x = 0;
    int32 b = 0;
    TArray<EEdgeType> edge6;
    edge6.Init(static_cast<EEdgeType>(0), 6);
    for (int32 j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (Ep[j] <= EEdgeType::DF) {
            a += Cnk(j, x + 1);
            edge6[x++] = Ep[j];
        }

    for (int32 j = 5; j > 0; j--)
    {
        int32 k = 0;
        while (static_cast<int32>(edge6[j]) != j) {
            Rotate(edge6, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return 720 * a + b;
}

void FCubieCube::SetURtoDF(int32 Idx)
{
    TArray edge6 = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::DR, EEdgeType::DF };
    TArray otherEdge = { EEdgeType::DL, EEdgeType::DB, EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR };
    int32 b = Idx % 720;
    int32 a = Idx / 720;

    for (int32 e = 0; e < EDGE_COUNT; e++)
        Ep[e] = EEdgeType::BR;

    for (int32 j = 1; j < 6; j++)
    {
        int32 k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            Rotate(edge6, j, 0);
    }
    int32 x = 5;
    for (int32 j = static_cast<int32>(EEdgeType::BR); j >= 0; j--)
        if (a - Cnk(j, x + 1) >= 0) {
            Ep[j] = edge6[x];
            a -= Cnk(j, x-- + 1);
        }
    x = 0;
    for (int32 j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (Ep[j] == EEdgeType::BR)
            Ep[j] = otherEdge[x++];
}

int16 FCubieCube::GetURtoUL()
{
    int32 a = 0, b = 0, x = 0;
    TArray<EEdgeType> edge3;
    edge3.Init(static_cast<EEdgeType>(0), 3);
    for (int32 j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (Ep[j] <= EEdgeType::UL) {
            a += Cnk(j, x + 1);
            edge3[x++] = Ep[j];
        }

    for (int32 j = 2; j > 0; j--)
    {
        int32 k = 0;
        while (static_cast<int32>(edge3[j]) != j) {
            Rotate(edge3, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return static_cast<int16>(6 * a + b);
}

void FCubieCube::SetURtoUL(int16 Idx)
{
    TArray edge3 = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL };
    int32 b = Idx % 6;
    int32 a = Idx / 6;
    for(int32 e = 0; e < EDGE_COUNT; e++) {
        Ep[e] = EEdgeType::BR;
    }

    for (int32 j = 1; j < 3; j++) {
        int32 k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            Rotate(edge3, j, 0);
    }
    int32 x = 2;
    for (int32 j = static_cast<int32>(EEdgeType::BR); j >= 0; j--) {
        if (a - Cnk(j, x + 1) >= 0) {
            Ep[j] = edge3[x];
            a -= Cnk(j, x-- + 1);
        }
    }
}

int16 FCubieCube::GetUBtoDF()
{
    int32 a = 0, x = 0, b = 0;
    TArray<EEdgeType> edge3;
    edge3.Init(static_cast<EEdgeType>(0), 3);
    for (int32 j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (EEdgeType::UB <= Ep[j] && Ep[j] <= EEdgeType::DF) {
            a += Cnk(j, x + 1);
            edge3[x++] = Ep[j];
        }

    for (int32 j = 2; j > 0; j--)
    {
        int32 k = 0;
        while (static_cast<int32>(edge3[j]) != static_cast<int32>(EEdgeType::UB) + j) {
            Rotate(edge3, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return static_cast<int16>(6 * a + b);
}

void FCubieCube::SetUBtoDF(int16 Idx)
{
    TArray edge3 = { EEdgeType::UB, EEdgeType::DR, EEdgeType::DF };
    int32 b = Idx % 6;
    int32 a = Idx / 6;
    for (int32 e = 0; e < EDGE_COUNT; e++)
        Ep[e] = EEdgeType::BR;

    for (int32 j = 1; j < 3; j++)
    {
        int32 k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            Rotate(edge3, j, 0);
    }
    int32 x = 2;
    for (int32 j = static_cast<int32>(EEdgeType::BR); j >= 0; j--)
        if (a - Cnk(j, x + 1) >= 0) {
            Ep[j] = edge3[x];
            a -= Cnk(j, x-- + 1);
        }
}

int32 FCubieCube::Verify()
{
    int32 sum = 0, i;
    int32 edgeCount[12] = {0};
    int32 cornerCount[8] = {0};

    for(int32 e = 0; e < EDGE_COUNT; e++)
        edgeCount[static_cast<int32>(Ep[e])]++;
    for (i = 0; i < 12; i++)
        if (edgeCount[i] != 1)
            return -2;

    for (i = 0; i < 12; i++)
        sum += Eo[i];
    if (sum % 2 != 0)
        return -3;

    for(int32 c = 0; c < CORNER_COUNT; c++)
        cornerCount[static_cast<int32>(Cp[c])]++;
    for (i = 0; i < 8; i++)
        if (cornerCount[i] != 1)
            return -4;

    sum = 0;
    for (i = 0; i < 8; i++)
        sum += Co[i];
    if (sum % 3 != 0)
        return -5;

    if ((EdgeParity() ^ CornerParity()) != 0)
        return -6;

    return 0;
}

int32 FCubieCube::GetURtoDF_Standalone(int16 idx1, int16 idx2)
{
    FCubieCube a;
    FCubieCube b;
    a.SetURtoUL(idx1);
    b.SetUBtoDF(idx2);
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
