#include "KociembaAlgorithm/CubieCube.h"

const TArray<FCubieCube> FCubieCube::MoveCube = []
{
    TArray<FCubieCube> TempMoveCube;
    TempMoveCube.SetNum(6);

    constexpr ECornerType cp[6][8] = {
        { ECornerType::UBR, ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::DFR, ECornerType::DLF, ECornerType::DBL, ECornerType::DRB },
        { ECornerType::DFR, ECornerType::UFL, ECornerType::ULB, ECornerType::URF, ECornerType::DRB, ECornerType::DLF, ECornerType::DBL, ECornerType::UBR },
        { ECornerType::UFL, ECornerType::DLF, ECornerType::ULB, ECornerType::UBR, ECornerType::URF, ECornerType::DFR, ECornerType::DBL, ECornerType::DRB },
        { ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::UBR, ECornerType::DLF, ECornerType::DBL, ECornerType::DRB, ECornerType::DFR },
        { ECornerType::URF, ECornerType::ULB, ECornerType::DBL, ECornerType::UBR, ECornerType::DFR, ECornerType::UFL, ECornerType::DLF, ECornerType::DRB },
        { ECornerType::URF, ECornerType::UFL, ECornerType::UBR, ECornerType::DRB, ECornerType::DFR, ECornerType::DLF, ECornerType::ULB, ECornerType::DBL }
    };

    constexpr int8 co[6][8] = {
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 2, 0, 0, 1, 1, 0, 0, 2 },
        { 1, 2, 0, 0, 2, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 2, 0, 0, 2, 1, 0 },
        { 0, 0, 1, 2, 0, 0, 2, 1 }
    };

    constexpr EEdgeType ep[6][12] = {
        { EEdgeType::UB, EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR },
        { EEdgeType::FR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::BR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, EEdgeType::DR, EEdgeType::FL, EEdgeType::BL, EEdgeType::UR },
        { EEdgeType::UR, EEdgeType::FL, EEdgeType::UL, EEdgeType::UB, EEdgeType::DR, EEdgeType::FR, EEdgeType::DL, EEdgeType::DB, EEdgeType::UF, EEdgeType::DF, EEdgeType::BL, EEdgeType::BR },
        { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, EEdgeType::DR, EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR },
        { EEdgeType::UR, EEdgeType::UF, EEdgeType::BL, EEdgeType::UB, EEdgeType::DR, EEdgeType::DF, EEdgeType::FL, EEdgeType::DB, EEdgeType::FR, EEdgeType::UL, EEdgeType::DL, EEdgeType::BR },
        { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::BR, EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::BL, EEdgeType::FR, EEdgeType::FL, EEdgeType::UB, EEdgeType::DB }
    };

    constexpr int8 eo[6][12] = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1 }
    };

    for (int32 i = 0; i < 6; i++)
    {
        TempMoveCube[i].Cp = TArray(cp[i], UE_ARRAY_COUNT(cp[i]));
        TempMoveCube[i].Co = TArray(co[i], UE_ARRAY_COUNT(co[i]));
        TempMoveCube[i].Ep = TArray(ep[i], UE_ARRAY_COUNT(ep[i]));
        TempMoveCube[i].Eo = TArray(eo[i], UE_ARRAY_COUNT(eo[i]));
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

int32 FCubieCube::Cnk(int32 N, int32 K)
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

void FCubieCube::RotateLeftCorner(ECornerType* arr, int32 l, int32 r)
{
    ECornerType temp = arr[l];
    for (int32 i = l; i < r; i++)
        arr[i] = arr[i + 1];
    arr[r] = temp;
}

void FCubieCube::RotateRightCorner(ECornerType* arr, int32 l, int32 r)
{
    ECornerType temp = arr[r];
    for (int32 i = r; i > l; i--)
        arr[i] = arr[i - 1];
    arr[l] = temp;
}


void FCubieCube::RotateLeftEdge(EEdgeType* arr, int32 l, int32 r)
{
    EEdgeType temp = arr[l];
    for (int32 i = l; i < r; i++)
        arr[i] = arr[i + 1];
    arr[r] = temp;
}

void FCubieCube::RotateRightEdge(EEdgeType* arr, int32 l, int32 r)
{
    EEdgeType temp = arr[r];
    for (int32 i = r; i > l; i--)
        arr[i] = arr[i - 1];
    arr[l] = temp;
}

void FCubieCube::CornerMultiply(int32 MoveCubeIdx)
{
    ECornerType cPerm[8] = { static_cast<ECornerType>(0) };
    int8 cOri[8] = {0};
    for (int32 corn = 0; corn < CORNER_COUNT; corn++)
    {
        cPerm[corn] = Cp[static_cast<int32>(MoveCube[MoveCubeIdx].Cp[corn])];

        int8 oriA = Co[static_cast<int32>(MoveCube[MoveCubeIdx].Cp[corn])];
        int8 oriB = MoveCube[MoveCubeIdx].Co[corn];
        int8 ori = 0;

        if (oriA < 3 && oriB < 3)
        {
            ori = oriA + oriB;
            if (ori >= 3)
                ori -= 3;
        } else if (oriA < 3 && oriB >= 3)
        {
            ori = oriA + oriB;
            if (ori >= 6)
                ori -= 3;
        } else if (oriA >= 3 && oriB < 3)
        {
            ori = oriA - oriB;
            if (ori < 3)
                ori += 3;
        } else if (oriA >= 3 && oriB >= 3)
        {
            ori = oriA - oriB;
            if (ori < 0)
                ori += 3;
        }
        cOri[corn] = ori;
    }
    for (int32 corn = 0; corn < CORNER_COUNT; corn++)
    {
        Cp[corn] = cPerm[corn];
        Co[corn] = cOri[corn];
    }
}

void FCubieCube::EdgeMultiply(int32 MoveCubeIdx)
{
    EEdgeType ePerm[12] = { static_cast<EEdgeType>(0) };
    int8 eOri[12] = {0};

    for (int32 edge = 0; edge < EDGE_COUNT; edge++) {
        ePerm[edge] = Ep[static_cast<int32>(MoveCube[MoveCubeIdx].Ep[edge])];
        eOri[edge] = (MoveCube[MoveCubeIdx].Eo[edge] + Eo[static_cast<int32>(MoveCube[MoveCubeIdx].Ep[edge])]) % 2;
    }
    for (int32 edge = 0; edge < EDGE_COUNT; edge++) {
        Ep[edge] = ePerm[edge];
        Eo[edge] = eOri[edge];
    }
}

int16 FCubieCube::GetTwist()
{
    int16 ret = 0;
    for (int32 i = static_cast<int32>(ECornerType::URF); i < static_cast<int32>(ECornerType::DRB); i++)
        ret = static_cast<int16>(3 * ret + Co[i]);
    return ret;
}

void FCubieCube::SetTwist(int16 Twist)
{
    int32 twistParity = 0;
    for (int32 i = static_cast<int32>(ECornerType::DRB) - 1; i >= static_cast<int32>(ECornerType::URF); i--) {
        twistParity += Co[i] = static_cast<int8>(Twist % 3);
        Twist /= 3;
    }
    Co[static_cast<int32>(ECornerType::DRB)] = static_cast<int8>((3 - twistParity % 3) % 3);
}

int16 FCubieCube::GetFlip()
{
    int16 ret = 0;
    for (int32 i = static_cast<int32>(EEdgeType::UR); i < static_cast<int32>(EEdgeType::BR); i++)
        ret = static_cast<int16>(2 * ret + Eo[i]);
    return ret;
}

void FCubieCube::SetFlip(int16 Flip)
{
    int32 flipParity = 0;
    for (int32 i = static_cast<int32>(EEdgeType::BR) - 1; i >= static_cast<int32>(EEdgeType::UR); i--) {
        flipParity += Eo[i] = static_cast<int8>(Flip % 2);
        Flip /= 2;
    }
    Eo[static_cast<int32>(EEdgeType::BR)] = static_cast<int8>((2 - flipParity % 2) % 2);
}

int16 FCubieCube::CornerParity()
{
    int32 s = 0;
    for (int32 i = static_cast<int32>(ECornerType::DRB); i >= static_cast<int32>(ECornerType::URF) + 1; i--)
        for (int32 j = i - 1; j >= static_cast<int32>(ECornerType::URF); j--)
            if (Cp[j] > Cp[i])
                s++;
    return static_cast<int16>(s % 2);
}

int16 FCubieCube::EdgeParity()
{
    int32 s = 0;
    for (int32 i = static_cast<int32>(EEdgeType::BR); i >= static_cast<int32>(EEdgeType::UR) + 1; i--)
        for (int32 j = i - 1; j >= static_cast<int32>(EEdgeType::UR); j--)
            if (Ep[j] > Ep[i])
                s++;
    return static_cast<int16>(s % 2);
}

int16 FCubieCube::GetFRtoBR()
{
    int32 a = 0, x = 0;
    int32 b = 0;
    EEdgeType edge4[4] = { static_cast<EEdgeType>(0) };
    for (int32 j = static_cast<int32>(EEdgeType::BR); j >= static_cast<int32>(EEdgeType::UR); j--)
        if (EEdgeType::FR <= Ep[j] && Ep[j] <= EEdgeType::BR) {
            a += Cnk(11 - j, x + 1);
            edge4[3 - x++] = Ep[j];
        }

    for (int32 j = 3; j > 0; j--)
    {
        int32 k = 0;
        while (static_cast<int32>(edge4[j]) != j + 8) {
            RotateLeftEdge(edge4, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return static_cast<int16>(24 * a + b);
}
void FCubieCube::SetFRtoBR(int16 Idx)
{
    EEdgeType sliceEdge[4] = { EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR };
    EEdgeType otherEdge[8] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB };
    int32 b = Idx % 24;
    int32 a = Idx / 24;
    for (int32 e = 0; e < EDGE_COUNT; e++)
        Ep[e] = EEdgeType::DB;

    for (int32 j = 1; j < 4; j++)
    {
        int32 k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            RotateRightEdge(sliceEdge, 0, j);
    }

    int32 x = 3;
    for (int32 j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (a - Cnk(11 - j, x + 1) >= 0) {
            Ep[j] = sliceEdge[3 - x];
            a -= Cnk(11 - j, x-- + 1);
        }
    x = 0;
    for (int32 j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (Ep[j] == EEdgeType::DB)
            Ep[j] = otherEdge[x++];
}

int16 FCubieCube::GetURFtoDLF()
{
    int32 a = 0, x = 0, b = 0;
    ECornerType corner6[6] = { static_cast<ECornerType>(0) };
    for (int32 j = static_cast<int32>(ECornerType::URF); j <= static_cast<int32>(ECornerType::DRB); j++)
        if (Cp[j] <= ECornerType::DLF) {
            a += Cnk(j, x + 1);
            corner6[x++] = Cp[j];
        }

    for (int32 j = 5; j > 0; j--)
    {
        int32 k = 0;
        while (static_cast<int32>(corner6[j]) != j) {
            RotateLeftCorner(corner6, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return static_cast<int16>(720 * a + b);
}

void FCubieCube::SetURFtoDLF(int16 Idx)
{
    ECornerType corner6[6] = { ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::UBR, ECornerType::DFR, ECornerType::DLF };
    ECornerType otherCorner[2] = { ECornerType::DBL, ECornerType::DRB };
    int32 b = Idx % 720;
    int32 a = Idx / 720;
    for (int32 c = 0; c < CORNER_COUNT; c++)
        Cp[c] = ECornerType::DRB;

    for (int32 j = 1; j < 6; j++)
    {
        int32 k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            RotateRightCorner(corner6, 0, j);
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
    EEdgeType edge6[6] = { static_cast<EEdgeType>(0) };
    for (int32 j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (Ep[j] <= EEdgeType::DF) {
            a += Cnk(j, x + 1);
            edge6[x++] = Ep[j];
        }

    for (int32 j = 5; j > 0; j--)
    {
        int32 k = 0;
        while (static_cast<int32>(edge6[j]) != j) {
            RotateLeftEdge(edge6, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return 720 * a + b;
}

void FCubieCube::SetURtoDF(int32 Idx)
{
    EEdgeType edge6[6] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::DR, EEdgeType::DF };
    EEdgeType otherEdge[6] = { EEdgeType::DL, EEdgeType::DB, EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR };
    int32 b = Idx % 720;
    int32 a = Idx / 720;

    for (int32 e = 0; e < EDGE_COUNT; e++)
        Ep[e] = EEdgeType::BR;

    for (int32 j = 1; j < 6; j++)
    {
        int32 k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            RotateRightEdge(edge6, 0, j);
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
    EEdgeType edge3[3] = { static_cast<EEdgeType>(0) };
    for (int32 j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (Ep[j] <= EEdgeType::UL) {
            a += Cnk(j, x + 1);
            edge3[x++] = Ep[j];
        }

    for (int32 j = 2; j > 0; j--)
    {
        int32 k = 0;
        while (static_cast<int32>(edge3[j]) != j) {
            RotateLeftEdge(edge3, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return static_cast<int16>(6 * a + b);
}

void FCubieCube::SetURtoUL(int16 Idx)
{
    EEdgeType edge3[3] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL };
    int32 b = Idx % 6;
    int32 a = Idx / 6;
    for(int32 e = 0; e < EDGE_COUNT; e++) {
        Ep[e] = EEdgeType::BR;
    }

    for (int32 j = 1; j < 3; j++) {
        int32 k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            RotateRightEdge(edge3, 0, j);
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
    EEdgeType edge3[3] = { static_cast<EEdgeType>(0) };
    for (int32 j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (EEdgeType::UB <= Ep[j] && Ep[j] <= EEdgeType::DF) {
            a += Cnk(j, x + 1);
            edge3[x++] = Ep[j];
        }

    for (int32 j = 2; j > 0; j--)
    {
        int32 k = 0;
        while (static_cast<int32>(edge3[j]) != static_cast<int32>(EEdgeType::UB) + j) {
            RotateLeftEdge(edge3, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return static_cast<int16>(6 * a + b);
}

void FCubieCube::SetUBtoDF(int16 Idx)
{
    EEdgeType edge3[3] = { EEdgeType::UB, EEdgeType::DR, EEdgeType::DF };
    int32 b = Idx % 6;
    int32 a = Idx / 6;
    for (int32 e = 0; e < EDGE_COUNT; e++)
        Ep[e] = EEdgeType::BR;

    for (int32 j = 1; j < 3; j++)
    {
        int32 k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            RotateRightEdge(edge3, 0, j);
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
