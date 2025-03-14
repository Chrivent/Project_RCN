#include "KociembaAlgorithm/CubeSolver.h"

#include "KociembaAlgorithm/CoordCube.h"

const TArray<TArray<EFaceletType>> FCubieCube::CornerFacelet = {
    { EFaceletType::U9, EFaceletType::R1, EFaceletType::F3 },
    { EFaceletType::U7, EFaceletType::F1, EFaceletType::L3 },
    { EFaceletType::U1, EFaceletType::L1, EFaceletType::B3 },
    { EFaceletType::U3, EFaceletType::B1, EFaceletType::R3 },
    { EFaceletType::D3, EFaceletType::F9, EFaceletType::R7 },
    { EFaceletType::D1, EFaceletType::L9, EFaceletType::F7 },
    { EFaceletType::D7, EFaceletType::B9, EFaceletType::L7 },
    { EFaceletType::D9, EFaceletType::R9, EFaceletType::B7 }
};

const TArray<TArray<EFaceletType>> FCubieCube::EdgeFacelet = {
    { EFaceletType::U6, EFaceletType::R2 },
    { EFaceletType::U8, EFaceletType::F2 },
    { EFaceletType::U4, EFaceletType::L2 },
    { EFaceletType::U2, EFaceletType::B2 },
    { EFaceletType::D6, EFaceletType::R8 },
    { EFaceletType::D2, EFaceletType::F8 },
    { EFaceletType::D4, EFaceletType::L8 },
    { EFaceletType::D8, EFaceletType::B8 },
    { EFaceletType::F6, EFaceletType::R4 },
    { EFaceletType::F4, EFaceletType::L6 },
    { EFaceletType::B6, EFaceletType::L4 },
    { EFaceletType::B4, EFaceletType::R6 }
};

const TArray<TArray<EColorType>> FCubieCube::CornerColor = {
    { EColorType::U, EColorType::R, EColorType::F },
    { EColorType::U, EColorType::F, EColorType::L },
    { EColorType::U, EColorType::L, EColorType::B },
    { EColorType::U, EColorType::B, EColorType::R },
    { EColorType::D, EColorType::F, EColorType::R },
    { EColorType::D, EColorType::L, EColorType::F },
    { EColorType::D, EColorType::B, EColorType::L },
    { EColorType::D, EColorType::R, EColorType::B }
};

const TArray<TArray<EColorType>> FCubieCube::EdgeColor = {
    { EColorType::U, EColorType::R }, 
    { EColorType::U, EColorType::F }, 
    { EColorType::U, EColorType::L }, 
    { EColorType::U, EColorType::B }, 
    { EColorType::D, EColorType::R }, 
    { EColorType::D, EColorType::F }, 
    { EColorType::D, EColorType::L }, 
    { EColorType::D, EColorType::B },
    { EColorType::F, EColorType::R }, 
    { EColorType::F, EColorType::L }, 
    { EColorType::B, EColorType::L }, 
    { EColorType::B, EColorType::R }
};

const TArray<FCubieCube> FCubieCube::MoveCube = []
{
    TArray<FCubieCube> TempMoveCube;
    TempMoveCube.SetNum(6);
    static const TArray<TArray<ECornerType>> TempCp = {
        { ECornerType::UBR, ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::DFR, ECornerType::DLF, ECornerType::DBL, ECornerType::DRB },
        { ECornerType::DFR, ECornerType::UFL, ECornerType::ULB, ECornerType::URF, ECornerType::DRB, ECornerType::DLF, ECornerType::DBL, ECornerType::UBR },
        { ECornerType::UFL, ECornerType::DLF, ECornerType::ULB, ECornerType::UBR, ECornerType::URF, ECornerType::DFR, ECornerType::DBL, ECornerType::DRB },
        { ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::UBR, ECornerType::DLF, ECornerType::DBL, ECornerType::DRB, ECornerType::DFR },
        { ECornerType::URF, ECornerType::ULB, ECornerType::DBL, ECornerType::UBR, ECornerType::DFR, ECornerType::UFL, ECornerType::DLF, ECornerType::DRB },
        { ECornerType::URF, ECornerType::UFL, ECornerType::UBR, ECornerType::DRB, ECornerType::DFR, ECornerType::DLF, ECornerType::ULB, ECornerType::DBL }
    };
    static const TArray<TArray<int8>> TempCo = {
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 2, 0, 0, 1, 1, 0, 0, 2 },
        { 1, 2, 0, 0, 2, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 2, 0, 0, 2, 1, 0 },
        { 0, 0, 1, 2, 0, 0, 2, 1 }
    };
    static const TArray<TArray<EEdgeType>> TempEp = {
        { EEdgeType::UB, EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR },
        { EEdgeType::FR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::BR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, EEdgeType::DR, EEdgeType::FL, EEdgeType::BL, EEdgeType::UR },
        { EEdgeType::UR, EEdgeType::FL, EEdgeType::UL, EEdgeType::UB, EEdgeType::DR, EEdgeType::FR, EEdgeType::DL, EEdgeType::DB, EEdgeType::UF, EEdgeType::DF, EEdgeType::BL, EEdgeType::BR },
        { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, EEdgeType::DR, EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR },
        { EEdgeType::UR, EEdgeType::UF, EEdgeType::BL, EEdgeType::UB, EEdgeType::DR, EEdgeType::DF, EEdgeType::FL, EEdgeType::DB, EEdgeType::FR, EEdgeType::UL, EEdgeType::DL, EEdgeType::BR },
        { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::BR, EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::BL, EEdgeType::FR, EEdgeType::FL, EEdgeType::UB, EEdgeType::DB }
    };
    static const TArray<TArray<int8>> TempEo = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1 }
    };
    for (int32 i = 0; i < 6; i++)
    {
        TempMoveCube[i].Cp = TempCp[i];
        TempMoveCube[i].Co = TempCo[i];
        TempMoveCube[i].Ep = TempEp[i];
        TempMoveCube[i].Eo = TempEo[i];
    }
    return TempMoveCube;
}();

FSearch::FSearch()
{
    Ax.SetNum(31);
    Po.SetNum(31);
    Flip.SetNum(31);
    Twist.SetNum(31);
    Slice.SetNum(31);
    Parity.SetNum(31);
    URFtoDLF.SetNum(31);
    FRtoBR.SetNum(31);
    URtoUL.SetNum(31);
    UBtoDF.SetNum(31);
    URtoDF.SetNum(31);
    MinDistPhase1.SetNum(31);
    MinDistPhase2.SetNum(31);
}

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

FCubieCube::FCubieCube(const FString& CubeString)
{
    Cp.SetNum(CORNER_COUNT);
    Co.SetNum(CORNER_COUNT);
    Ep.SetNum(EDGE_COUNT);
    Eo.SetNum(EDGE_COUNT);
    TArray<EColorType> Colors;
    Colors.SetNum(54);
    for (int32 i = 0; i < 54; ++i)
    {
        switch (CubeString[i])
        {
        case 'U': Colors[i] = EColorType::U; break;
        case 'R': Colors[i] = EColorType::R; break;
        case 'F': Colors[i] = EColorType::F; break;
        case 'D': Colors[i] = EColorType::D; break;
        case 'L': Colors[i] = EColorType::L; break;
        case 'B': Colors[i] = EColorType::B; break;
        default:  break;
        }
    }
    for (int32 i = 0; i < CORNER_COUNT; i++)
    {
        int8 Orientation;
        for (Orientation = 0; Orientation < 3; Orientation++)
        {
            if (Colors[static_cast<int32>(CornerFacelet[i][Orientation])] == EColorType::U ||
                Colors[static_cast<int32>(CornerFacelet[i][Orientation])] == EColorType::D)
            {
                break;
            }
        }
        const EColorType Color1 = Colors[static_cast<int32>(CornerFacelet[i][(Orientation + 1) % 3])];
        const EColorType Color2 = Colors[static_cast<int32>(CornerFacelet[i][(Orientation + 2) % 3])];
        for (int32 j = 0; j < CORNER_COUNT; j++)
        {
            if (Color1 == CornerColor[j][1] && Color2 == CornerColor[j][2])
            {
                Cp[i] = static_cast<ECornerType>(j);
                Co[i] = Orientation;
                break;
            }
        }
    }
    for (int32 i = 0; i < EDGE_COUNT; i++)
    {
        const EColorType Color1 = Colors[static_cast<int32>(EdgeFacelet[i][0])];
        const EColorType Color2 = Colors[static_cast<int32>(EdgeFacelet[i][1])];
        for (int32 j = 0; j < EDGE_COUNT; j++)
        {
            if (Color1 == EdgeColor[j][0] && Color2 == EdgeColor[j][1])
            {
                Ep[i] = static_cast<EEdgeType>(j);
                Eo[i] = 0;
                break;
            }
            if (Color1 == EdgeColor[j][1] && Color2 == EdgeColor[j][0])
            {
                Ep[i] = static_cast<EEdgeType>(j);
                Eo[i] = 1;
                break;
            }
        }
    }
}

void FCubieCube::CornerMultiply(const int32 MoveCubeIdx)
{
    TArray<ECornerType> CornPerm;
    TArray<int8> CornOri;
    CornPerm.SetNum(CORNER_COUNT);
    CornOri.SetNum(CORNER_COUNT);
    const FCubieCube Move = MoveCube[MoveCubeIdx];
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
    const FCubieCube Move = MoveCube[MoveCubeIdx];
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

FString UCubeSolver::SolveCube(const FString& Facelets, const int32 MaxDepth, const double TimeOut, const FString& CacheDir)
{
    FSearch Search;

    if (PRUNING_INITED == 0)
    {
        InitPruning(CacheDir);
    }

    if (Facelets.Len() != 54)
    {
        return FString("ERROR: Invalid facelet input");
    }

    FString CubeString = Facelets;
    TMap<TCHAR, TCHAR> ReplacementInfo = {
        { Facelets[4], TEXT('U') },
        { Facelets[13], TEXT('R') },
        { Facelets[22], TEXT('F') },
        { Facelets[31], TEXT('D') },
        { Facelets[40], TEXT('L') },
        { Facelets[49], TEXT('B') }
    };
    for (auto& CubeStringChar : CubeString)
    {
        CubeStringChar = ReplacementInfo[CubeStringChar];
    }

    TArray<int32> Count;
    Count.SetNum(6);
    for (const auto Facelet : CubeString)
    {
        switch (Facelet)
        {
        case 'U': Count[0]++; break;
        case 'R': Count[1]++; break;
        case 'F': Count[2]++; break;
        case 'D': Count[3]++; break;
        case 'L': Count[4]++; break;
        case 'B': Count[5]++; break;
        default:
            return FString("ERROR: Unrecognized String");
        }
    }
    
    if (Count.ContainsByPredicate([](int32 C) { return C != 9; }))
    {
        return FString("ERROR: Invalid cube state");
    }
    
    FCubieCube Cc(CubeString);
    if (Cc.Verify())
    {
        return FString("ERROR: Unsolvable cube");
    }

    Search.Po[0] = 0;
    Search.Ax[0] = 0;
    Search.Flip[0] = Cc.GetFlip();
    Search.Twist[0] = Cc.GetTwist();
    Search.Parity[0] = Cc.CornerParity();
    Search.Slice[0] = Cc.GetFRtoBR() / 24;
    Search.URFtoDLF[0] = Cc.GetURFtoDLF();
    Search.FRtoBR[0] = Cc.GetFRtoBR();
    Search.URtoUL[0] = Cc.GetURtoUL();
    Search.UBtoDF[0] = Cc.GetUBtoDF();

    Search.MinDistPhase1[1] = 1;
    int32 N = 0, Busy = 0, DepthPhase1 = 1;
    const double Time = FPlatformTime::Seconds();
    
    do
    {
        do
        {
            if (DepthPhase1 - N > Search.MinDistPhase1[N + 1] && !Busy)
            {
                if (Search.Ax[N] == 0 || Search.Ax[N] == 3)
                {
                    Search.Ax[++N] = 1;
                }
                else
                {
                    Search.Ax[++N] = 0;
                }
                Search.Po[N] = 1;
            }
            else if (++Search.Po[N] > 3)
            {
                do
                {
                    if (++Search.Ax[N] > 5)
                    {
                        if (FPlatformTime::Seconds() - Time > TimeOut)
                        {
                            return FString("ERROR: Timeout");
                        }

                        if (N == 0)
                        {
                            if (DepthPhase1 >= MaxDepth)
                            {
                                return FString("ERROR: Max depth exceeded");
                            }
                        
                            DepthPhase1++;
                            Search.Ax[N] = 0;
                            Search.Po[N] = 1;
                            Busy = 0;
                            break;
                        }
                    
                        N--;
                        Busy = 1;
                        break;
                    }
                
                    Search.Po[N] = 1;
                    Busy = 0;
                } while (N != 0 && (Search.Ax[N - 1] == Search.Ax[N] || Search.Ax[N - 1] - 3 == Search.Ax[N]));
            }
            else
            {
                Busy = 0;
            }
        } while (Busy);

        const int32 Move = 3 * Search.Ax[N] + Search.Po[N] - 1;
        Search.Flip[N + 1] = FlipMove[Search.Flip[N]][Move];
        Search.Twist[N + 1] = TwistMove[Search.Twist[N]][Move];
        Search.Slice[N + 1] = FRtoBR_Move[Search.Slice[N] * 24][Move] / 24;
    
        Search.MinDistPhase1[N + 1] = FMath::Max(
            GetPruning(Slice_Flip_Pruning, N_SLICE1 * Search.Flip[N + 1] + Search.Slice[N + 1]),
            GetPruning(Slice_Twist_Pruning, N_SLICE1 * Search.Twist[N + 1] + Search.Slice[N + 1])
        );

        if (Search.MinDistPhase1[N + 1] == 0 && N >= DepthPhase1 - 5)
        {
            Search.MinDistPhase1[N + 1] = 10;

            if (N == DepthPhase1 - 1)
            {
                int32 S = TotalDepth(Search, DepthPhase1, MaxDepth);
                if (S >= 0 && (S == DepthPhase1 || (Search.Ax[DepthPhase1 - 1] != Search.Ax[DepthPhase1] && Search.Ax[DepthPhase1 - 1] != Search.Ax[DepthPhase1] + 3)))
                {
                    return SolutionToString(Search, S);
                }
            }
        }
    } while (true);
}

int32 UCubeSolver::TotalDepth(FSearch& Search, const int32 DepthPhase1, const int32 MaxDepth)
{
    const int32 MaxDepthPhase2 = FMath::Min(10, MaxDepth - DepthPhase1);
    int32 DepthPhase2 = 1;
    int32 N = DepthPhase1;
    int32 Busy = 0;

    for (int32 i = 0; i < DepthPhase1; i++)
    {
        const int32 Move = 3 * Search.Ax[i] + Search.Po[i] - 1;
        Search.URFtoDLF[i + 1] = URFtoDLF_Move[Search.URFtoDLF[i]][Move];
        Search.FRtoBR[i + 1] = FRtoBR_Move[Search.FRtoBR[i]][Move];
        Search.Parity[i + 1] = ParityMove[Search.Parity[i]][Move];
    }

    const int32 D1 = GetPruning(Slice_URFtoDLF_Parity_Pruning,
        (N_SLICE2 * Search.URFtoDLF[DepthPhase1] + Search.FRtoBR[DepthPhase1]) * 2 + Search.Parity[DepthPhase1]);
    
    if (D1 > MaxDepthPhase2)
    {
        return -1;
    }

    for (int32 i = 0; i < DepthPhase1; i++)
    {
        const int32 Move = 3 * Search.Ax[i] + Search.Po[i] - 1;
        Search.URtoUL[i + 1] = URtoUL_Move[Search.URtoUL[i]][Move];
        Search.UBtoDF[i + 1] = UBtoDF_Move[Search.UBtoDF[i]][Move];
    }

    Search.URtoDF[DepthPhase1] = MergeURtoULandUBtoDF[Search.URtoUL[DepthPhase1]][Search.UBtoDF[DepthPhase1]];

    const int32 D2 = GetPruning(Slice_URtoDF_Parity_Pruning,
        (N_SLICE2 * Search.URtoDF[DepthPhase1] + Search.FRtoBR[DepthPhase1]) * 2 + Search.Parity[DepthPhase1]);

    if (D2 > MaxDepthPhase2)
    {
        return -1;
    }

    Search.MinDistPhase2[DepthPhase1] = FMath::Max(D1, D2);
    if (Search.MinDistPhase2[DepthPhase1] == 0)
    {
        return DepthPhase1;
    }

    Search.Po[DepthPhase1] = 0;
    Search.Ax[DepthPhase1] = 0;
    Search.MinDistPhase2[N + 1] = 1;

    do
    {
        do
        {
            if (DepthPhase1 + DepthPhase2 - N > Search.MinDistPhase2[N + 1] && !Busy)
            {
                if (Search.Ax[N] == 0 || Search.Ax[N] == 3)
                {
                    Search.Ax[++N] = 1;
                    Search.Po[N] = 2;
                }
                else
                {
                    Search.Ax[++N] = 0;
                    Search.Po[N] = 1;
                }
            }
            else if (Search.Ax[N] == 0 || Search.Ax[N] == 3 ? ++Search.Po[N] > 3 : (Search.Po[N] += 2) > 3)
            {
                do
                {
                    if (++Search.Ax[N] > 5)
                    {
                        if (N == DepthPhase1)
                        {
                            if (DepthPhase2 >= MaxDepthPhase2)
                            {
                                return -1;
                            }

                            DepthPhase2++;
                            Search.Ax[N] = 0;
                            Search.Po[N] = 1;
                            Busy = 0;
                            break;
                        }

                        N--;
                        Busy = 1;
                        break;
                    }

                    Search.Po[N] = Search.Ax[N] == 0 || Search.Ax[N] == 3 ? 1 : 2;
                    Busy = 0;
                } while (N != DepthPhase1 && (Search.Ax[N - 1] == Search.Ax[N] || Search.Ax[N - 1] - 3 == Search.Ax[N]));
            }
            else
            {
                Busy = 0;
            }
        } while (Busy);

        const int32 Move = 3 * Search.Ax[N] + Search.Po[N] - 1;

        Search.URFtoDLF[N + 1] = URFtoDLF_Move[Search.URFtoDLF[N]][Move];
        Search.FRtoBR[N + 1] = FRtoBR_Move[Search.FRtoBR[N]][Move];
        Search.Parity[N + 1] = ParityMove[Search.Parity[N]][Move];
        Search.URtoDF[N + 1] = URtoDF_Move[Search.URtoDF[N]][Move];

        Search.MinDistPhase2[N + 1] = FMath::Max(
            GetPruning(Slice_URtoDF_Parity_Pruning,
                (N_SLICE2 * Search.URtoDF[N + 1] + Search.FRtoBR[N + 1]) * 2 + Search.Parity[N + 1]),
            GetPruning(Slice_URFtoDLF_Parity_Pruning,
                (N_SLICE2 * Search.URFtoDLF[N + 1] + Search.FRtoBR[N + 1]) * 2 + Search.Parity[N + 1])
        );

    } while (Search.MinDistPhase2[N + 1] != 0);

    return DepthPhase1 + DepthPhase2;
}

FString UCubeSolver::SolutionToString(const FSearch& Search, const int32 Length)
{
    FString SolutionString;
    for (int32 i = 0; i < Length; i++)
    {
        TArray MoveAxis = { TEXT("U"), TEXT("R"), TEXT("F"), TEXT("D"), TEXT("L"), TEXT("B") };
        SolutionString += MoveAxis[Search.Ax[i]];
        TArray MovePower = { TEXT(" "), TEXT("2 "), TEXT("' ") };
        SolutionString += MovePower[Search.Po[i] - 1];
    }
    return SolutionString;
}
