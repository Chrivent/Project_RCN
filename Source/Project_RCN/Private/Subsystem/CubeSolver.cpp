#include "Subsystem//CubeSolver.h"

DEFINE_LOG_CATEGORY(LogCubeSolver)

#define CORNER_COUNT 8
#define EDGE_COUNT 12

UENUM()
enum class EFaceletType : uint8
{
    U1, U2, U3, U4, U5, U6, U7, U8, U9,
    R1, R2, R3, R4, R5, R6, R7, R8, R9,
    F1, F2, F3, F4, F5, F6, F7, F8, F9,
    D1, D2, D3, D4, D5, D6, D7, D8, D9,
    L1, L2, L3, L4, L5, L6, L7, L8, L9,
    B1, B2, B3, B4, B5, B6, B7, B8, B9
};

UENUM()
enum class EColorType : uint8
{
    U,
    R,
    F,
    D,
    L,
    B
};

UENUM()
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

UENUM()
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

USTRUCT()
struct FSearch
{
    FSearch();

    TArray<int32> Ax;
    TArray<int32> Po;
    TArray<int32> Flip;
    TArray<int32> Twist;
    TArray<int32> Slice;
    TArray<int32> Parity;
    TArray<int32> URFtoDLF;
    TArray<int32> FRtoBR;
    TArray<int32> URtoUL;
    TArray<int32> UBtoDF;
    TArray<int32> URtoDF;
    TArray<int32> MinDistPhase1;
    TArray<int32> MinDistPhase2;
};

USTRUCT()
struct FCubieCube
{
    FCubieCube();
    explicit FCubieCube(const FString& Facelets);

    void InitializeSearch(FSearch& Search);
    
    void CornerMultiply(const int32 MoveCubeIdx);
    void EdgeMultiply(const int32 MoveCubeIdx);
    
    int16 GetTwist();
    int16 GetFlip();
    int16 GetFRtoBR();
    int16 GetURFtoDLF();
    int32 GetURtoDF();
    int16 GetURtoUL();
    int16 GetUBtoDF();
    
    static int32 GetURtoDF_Standalone(const int16 Idx1, const int16 Idx2);
    
    void SetTwist(int16 Twist);
    void SetFlip(int16 Flip);
    void SetFRtoBR(const int16 Idx);
    void SetURFtoDLF(const int16 Idx);
    void SetURtoDF(const int32 Idx);
    void SetURtoUL(const int16 Idx);
    void SetUBtoDF(const int16 Idx);
    
    int32 Verify();

protected:
    static int32 Cnk(const int32 N, int32 K);

    template <typename T>
    void Rotate(TArray<T>& Arr, const int32 L, const int32 R);
    
    int16 CornerParity();
    int16 EdgeParity();

    static const TArray<TArray<EFaceletType>> CornerFacelet;
    static const TArray<TArray<EFaceletType>> EdgeFacelet;
    static const TArray<TArray<EColorType>> CornerColor;
    static const TArray<TArray<EColorType>> EdgeColor;
    static const TArray<FCubieCube> MoveCube;

    TArray<ECornerType> Cp;
    TArray<int8> Co;
    TArray<EEdgeType> Ep;
    TArray<int8> Eo;
};

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

FCubieCube::FCubieCube(const FString& Facelets)
{
    Cp.SetNum(CORNER_COUNT);
    Co.SetNum(CORNER_COUNT);
    Ep.SetNum(EDGE_COUNT);
    Eo.SetNum(EDGE_COUNT);
    TArray<EColorType> Colors;
    Colors.SetNum(54);
    for (int32 i = 0; i < 54; ++i)
    {
        switch (Facelets[i])
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

void FCubieCube::InitializeSearch(FSearch& Search)
{
    Search.Po[0] = 0;
    Search.Ax[0] = 0;
    Search.Flip[0] = GetFlip();
    Search.Twist[0] = GetTwist();
    Search.Parity[0] = CornerParity();
    Search.Slice[0] = GetFRtoBR() / 24;
    Search.URFtoDLF[0] = GetURFtoDLF();
    Search.FRtoBR[0] = GetFRtoBR();
    Search.URtoUL[0] = GetURtoUL();
    Search.UBtoDF[0] = GetUBtoDF();
    Search.MinDistPhase1[1] = 1;
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

int16 FCubieCube::GetFlip()
{
    int16 Flip = 0;
    for (int32 i = 0; i < EDGE_COUNT - 1; i++)
    {
        Flip = Flip * 2 + Eo[i];
    }
    return Flip;
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
            return 2;
        }
    }
    int32 Sum = 0;
    for (int32 i = 0; i < EDGE_COUNT; i++)
    {
        Sum += Eo[i];
    }
    if (Sum % 2 != 0)
    {
        return 3;
    }
    for (int32 i = 0; i < CORNER_COUNT; i++)
    {
        CornerCount[static_cast<int32>(Cp[i])]++;
    }
    for (int32 i = 0; i < 8; i++)
    {
        if (CornerCount[i] != 1)
        {
            return 4;
        }
    }
    Sum = 0;
    for (int32 i = 0; i < CORNER_COUNT; i++)
    {
        Sum += Co[i];
    }
    if (Sum % 3 != 0)
    {
        return 5;
    }
    if (EdgeParity() != CornerParity())
    {
        return 6;
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

int16 TwistMove[N_TWIST][N_MOVE];
int16 FlipMove[N_FLIP][N_MOVE];
int16 FRtoBR_Move[N_FRtoBR][N_MOVE];
int16 URFtoDLF_Move[N_URFtoDLF][N_MOVE] = {{}};
int16 URtoDF_Move[N_URtoDF][N_MOVE] = {{}};
int16 URtoUL_Move[N_URtoUL][N_MOVE] = {{}};
int16 UBtoDF_Move[N_UBtoDF][N_MOVE] = {{}};
int16 MergeURtoULandUBtoDF[336][336] = {{}};
int8 Slice_URFtoDLF_Parity_Pruning[N_SLICE2 * N_URFtoDLF * N_PARITY / 2] = {};
int8 Slice_URtoDF_Parity_Pruning[N_SLICE2 * N_URtoDF * N_PARITY / 2] = {};
int8 Slice_Twist_Pruning[N_SLICE1 * N_TWIST / 2 + 1] = {};
int8 Slice_Flip_Pruning[N_SLICE1 * N_FLIP / 2] = {};
int16 ParityMove[2][18] = {
    { 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1 },
    { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 }
};
int32 PruningInitialized = 0;

bool CheckCachedTable(const FString& Name, void* Ptr, const int32 Length, const FString& CacheDir)
{
    const FString FilePath = FPaths::Combine(CacheDir, Name);
    if (FPaths::FileExists(FilePath))
    {
        TArray<uint8> FileData;
        if (FFileHelper::LoadFileToArray(FileData, *FilePath))
        {
            FMemory::Memcpy(Ptr, FileData.GetData(), FMath::Min(FileData.Num(), Length));
        }
        else
        {
            UE_LOG(LogCubeSolver, Error, TEXT("Failed to read file: %s"), *FilePath);
        }
        return true;
    }
    UE_LOG(LogCubeSolver, Warning, TEXT("Cache table %s was not found. Recalculating."), *FilePath);
    return false;
}

void DumpToFile(void* Ptr, const int32 Length, const FString& Name, const FString& CacheDir)
{
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (PlatformFile.CreateDirectoryTree(*CacheDir))
    {
        const FString FilePath = FPaths::Combine(CacheDir, Name);
        TArray<uint8> FileData;
        FileData.Append(static_cast<uint8*>(Ptr), Length);

        if (FFileHelper::SaveArrayToFile(FileData, *FilePath))
        {
            UE_LOG(LogCubeSolver, Log, TEXT("Successfully wrote cache table: %s"), *FilePath);
        }
        else
        {
            UE_LOG(LogCubeSolver, Error, TEXT("Failed to write cache table: %s"), *FilePath);
        }
    }
    else
    {
        UE_LOG(LogCubeSolver, Error, TEXT("Cannot create cache tables directory: %s"), *CacheDir);
    }
}

void SetPruning(int8* Table, const int32 Idx, const int8 Value)
{
    if ((Idx & 1) == 0)
    {
        Table[Idx / 2] &= 0xf0 | Value;
    }
    else
    {
        Table[Idx / 2] &= 0x0f | Value << 4;
    }
}

int8 GetPruning(const int8* Table, const int32 Idx)
{
    int8 Result;
    if ((Idx & 1) == 0)
    {
        Result = Table[Idx / 2] & 0x0f;
    }
    else
    {
        Result = Table[Idx / 2] >> 4 & 0x0f;
    }
    return Result;
}

template <size_t Row, size_t Col, typename SetFunc, typename MultiplyFunc, typename GetFunc>
void ComputeMoveTable(const FString& TableName, int16 (&MoveTable)[Row][Col], 
                      SetFunc SetFunction, MultiplyFunc MultiplyFunction, GetFunc GetFunction, 
                      FCubieCube& CubieCube, const FString& CacheDir)
{
    if (CheckCachedTable(TableName, MoveTable, sizeof(MoveTable), CacheDir))
    {
        for (int32 i = 0; i < Row; i++)
        {
            (CubieCube.*SetFunction)(i);
            for (int32 j = 0; j < 6; j++)
            {
                for (int32 k = 0; k < 3; k++)
                {
                    (CubieCube.*MultiplyFunction)(j);
                    MoveTable[i][3 * j + k] = (CubieCube.*GetFunction)();
                }
                (CubieCube.*MultiplyFunction)(j);
            }
        }
        DumpToFile(MoveTable, sizeof(MoveTable), TableName, CacheDir);
    }
}

template <size_t Size, typename MoveTable1, typename MoveTable2>
void ComputeParityPruningTable(const FString& TableName, int8 (&PruningTable)[Size], 
                         MoveTable1& MoveTableA, MoveTable2& MoveTableB, 
                         const int16 (&ParityMove)[2][18], const FString& CacheDir)
{
    if (CheckCachedTable(TableName, PruningTable, sizeof(PruningTable), CacheDir))
    {
        int32 Depth = 0, Done = 1;
        FMemory::Memset(PruningTable, -1, sizeof(PruningTable));

        SetPruning(PruningTable, 0, 0);

        while (Done != Size * 2)
        {
            for (int32 i = 0; i < Size * 2; i++)
            {
                const int32 Parity = i % 2;
                int32 StateA = i / 2 / N_SLICE2;
                int32 Slice = i / 2 % N_SLICE2;

                if (GetPruning(PruningTable, i) == Depth)
                {
                    for (int32 j = 0; j < 18; j++)
                    {
                        switch (j)
                        {
                        case 3: case 5: case 6: case 8:
                        case 12: case 14: case 15: case 17:
                            continue;
                        default:
                            const int32 NewSlice = MoveTableA[Slice][j];
                            const int32 NewStateA = MoveTableB[StateA][j];
                            const int32 NewParity = ParityMove[Parity][j];

                            int32 NewIndex = (N_SLICE2 * NewStateA + NewSlice) * 2 + NewParity;
                            if (GetPruning(PruningTable, NewIndex) == 0x0f)
                            {
                                SetPruning(PruningTable, NewIndex, Depth + 1);
                                Done++;
                            }
                        }
                    }
                }
            }
            Depth++;
        }
        DumpToFile(PruningTable, sizeof(PruningTable), TableName, CacheDir);
    }
}

template <size_t Size, size_t N_State, typename MoveTable>
void ComputePruningTable(const FString& TableName, int8 (&PruningTable)[Size], 
                              MoveTable& MoveData, const int16 (&MoveTableA)[N_State][N_MOVE], 
                              const FString& CacheDir)
{
    if (CheckCachedTable(TableName, PruningTable, sizeof(PruningTable), CacheDir))
    {
        int32 Depth = 0, Done = 1;
        FMemory::Memset(PruningTable, -1, sizeof(PruningTable));

        SetPruning(PruningTable, 0, 0);

        while (Done != N_SLICE1 * N_State)
        {
            for (int32 i = 0; i < N_SLICE1 * N_State; i++)
            {
                int32 State = i / N_SLICE1;
                const int32 Slice = i % N_SLICE1;

                if (GetPruning(PruningTable, i) == Depth)
                {
                    for (int32 j = 0; j < 18; j++)
                    {
                        const int32 NewSlice = MoveData[Slice * 24][j] / 24;
                        const int32 NewState = MoveTableA[State][j];

                        int32 NewIndex = N_SLICE1 * NewState + NewSlice;
                        if (GetPruning(PruningTable, NewIndex) == 0x0f)
                        {
                            SetPruning(PruningTable, NewIndex, Depth + 1);
                            Done++;
                        }
                    }
                }
            }
            Depth++;
        }
        DumpToFile(PruningTable, sizeof(PruningTable), TableName, CacheDir);
    }
}

void InitPruning(const FString& CacheDir)
{
    FCubieCube CubieCube;

    ComputeMoveTable("twistMove", TwistMove, 
                     &FCubieCube::SetTwist, &FCubieCube::CornerMultiply, &FCubieCube::GetTwist, 
                     CubieCube, CacheDir);

    ComputeMoveTable("flipMove", FlipMove, 
                     &FCubieCube::SetFlip, &FCubieCube::EdgeMultiply, &FCubieCube::GetFlip, 
                     CubieCube, CacheDir);

    ComputeMoveTable("FRtoBR_Move", FRtoBR_Move, 
                     &FCubieCube::SetFRtoBR, &FCubieCube::EdgeMultiply, &FCubieCube::GetFRtoBR, 
                     CubieCube, CacheDir);

    ComputeMoveTable("URFtoDLF_Move", URFtoDLF_Move, 
                     &FCubieCube::SetURFtoDLF, &FCubieCube::CornerMultiply, &FCubieCube::GetURFtoDLF, 
                     CubieCube, CacheDir);

    ComputeMoveTable("URtoDF_Move", URtoDF_Move, 
                     &FCubieCube::SetURtoDF, &FCubieCube::EdgeMultiply, &FCubieCube::GetURtoDF, 
                     CubieCube, CacheDir);

    ComputeMoveTable("URtoUL_Move", URtoUL_Move, 
                     &FCubieCube::SetURtoUL, &FCubieCube::EdgeMultiply, &FCubieCube::GetURtoUL, 
                     CubieCube, CacheDir);

    ComputeMoveTable("UBtoDF_Move", UBtoDF_Move, 
                     &FCubieCube::SetUBtoDF, &FCubieCube::EdgeMultiply, &FCubieCube::GetUBtoDF, 
                     CubieCube, CacheDir);

    if (CheckCachedTable("MergeURtoULandUBtoDF", MergeURtoULandUBtoDF, sizeof(MergeURtoULandUBtoDF), CacheDir))
    {
        for (int16 URtoUL = 0; URtoUL < 336; URtoUL++)
        {
            for (int16 UBtoDF = 0; UBtoDF < 336; UBtoDF++)
            {
                MergeURtoULandUBtoDF[URtoUL][UBtoDF] = FCubieCube::GetURtoDF_Standalone(URtoUL, UBtoDF);
            }
        }
        DumpToFile(MergeURtoULandUBtoDF, sizeof(MergeURtoULandUBtoDF), "MergeURtoULandUBtoDF", CacheDir);
    }

    ComputeParityPruningTable("Slice_URFtoDLF_Parity_Prun", Slice_URFtoDLF_Parity_Pruning,
                       FRtoBR_Move, URFtoDLF_Move, ParityMove, CacheDir);

    ComputeParityPruningTable("Slice_URtoDF_Parity_Prun", Slice_URtoDF_Parity_Pruning,
                        FRtoBR_Move, URtoDF_Move, ParityMove, CacheDir);

    ComputePruningTable<N_SLICE1 * N_TWIST / 2 + 1, N_TWIST>(
    "Slice_Twist_Prun", Slice_Twist_Pruning, FRtoBR_Move, TwistMove, CacheDir);

    ComputePruningTable<N_SLICE1 * N_FLIP / 2, N_FLIP>(
    "Slice_Flip_Prun", Slice_Flip_Pruning, FRtoBR_Move, FlipMove, CacheDir);

    PruningInitialized = 1;
}

FString SolutionToString(const FSearch& Search, const int32 Length)
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

int32 TotalDepth(FSearch& Search, const int32 DepthPhase1, const int32 MaxDepth)
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

FString UCubeSolver::SolveCube(FString Facelets, const int32 MaxDepth, const double TimeOut, const FString& CacheDir)
{
    FSearch Search;

    if (PruningInitialized == 0)
    {
        InitPruning(CacheDir);
    }

    TMap<TCHAR, TCHAR> ReplacementInfo = {
        { Facelets[4], TEXT('U') },
        { Facelets[13], TEXT('R') },
        { Facelets[22], TEXT('F') },
        { Facelets[31], TEXT('D') },
        { Facelets[40], TEXT('L') },
        { Facelets[49], TEXT('B') }
    };
    for (auto& Facelet : Facelets)
    {
        Facelet = ReplacementInfo[Facelet];
    }

    TArray<int32> Count;
    Count.SetNum(6);
    for (const auto Facelet : Facelets)
    {
        if (Facelet == 'U') { Count[0]++; }
        else if (Facelet == 'R') { Count[1]++; }
        else if (Facelet == 'F') { Count[2]++; }
        else if (Facelet == 'D') { Count[3]++; }
        else if (Facelet == 'L') { Count[4]++; }
        else if (Facelet == 'B') { Count[5]++; }
    }
    
    if (Count.ContainsByPredicate([](const int32 C) { return C != 9; }))
    {
        return TEXT("ERROR 1: There is not exactly one facelet of each colour");
    }
    
    FCubieCube Cc(Facelets);
    switch (Cc.Verify())
    {
    case 2:
        return TEXT("ERROR 2: Not all 12 edges exist exactly once");
    case 3:
        return TEXT("ERROR 3: Flip error: One edge has to be flipped");
    case 4:
        return TEXT("ERROR 4: Not all corners exist exactly once");
    case 5:
        return TEXT("ERROR 5: Twist error: One corner has to be twisted");
    case 6:
        return TEXT("ERROR 6: Parity error: Two corners or two edges have to be exchanged");
    default:
        break;
    }
    Cc.InitializeSearch(Search);
    
    int32 N = 0, Busy = 0, DepthPhase1 = 1;
    const double StartTime = FPlatformTime::Seconds();
    
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
                        if (FPlatformTime::Seconds() - StartTime > TimeOut)
                        {
                            return TEXT("ERROR 7: Timeout, no solution within given time");
                        }

                        if (N == 0)
                        {
                            if (DepthPhase1 >= MaxDepth)
                            {
                                return TEXT("ERROR 8: No solution exists for the given maxDepth");
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
