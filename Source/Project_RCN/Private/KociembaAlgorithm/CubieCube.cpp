#include "KociembaAlgorithm/CubieCube.h"

#include "KociembaAlgorithm/facecube.h"

const TArray<FCubieCube>& FCubieCube::GetMoveCube()
{
    static const TArray<FCubieCube> MoveCube = []
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
            TempMoveCube[i].Cp = TArray<ECornerType>(cp[i], UE_ARRAY_COUNT(cp[i]));
            TempMoveCube[i].Co = TArray<int8>(co[i], UE_ARRAY_COUNT(co[i]));
            TempMoveCube[i].Ep = TArray<EEdgeType>(ep[i], UE_ARRAY_COUNT(ep[i]));
            TempMoveCube[i].Eo = TArray<int8>(eo[i], UE_ARRAY_COUNT(eo[i]));
        }

        return TempMoveCube;
    }();

    return MoveCube;
}

FCubieCube GetCubieCube()
{
    FCubieCube result;

    static constexpr ECornerType cp[8]  = { 
        ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::UBR, 
        ECornerType::DFR, ECornerType::DLF, ECornerType::DBL, ECornerType::DRB 
    };

    static const int8 co[8]  = { 0, 0, 0, 0, 0, 0, 0, 0 };

    static constexpr EEdgeType ep[12] = { 
        EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, 
        EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, 
        EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR 
    };

    static const int8 eo[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    result.Cp = TArray(cp, UE_ARRAY_COUNT(cp));
    result.Co = TArray(co, UE_ARRAY_COUNT(co));
    result.Ep = TArray(ep, UE_ARRAY_COUNT(ep));
    result.Eo = TArray(eo, UE_ARRAY_COUNT(eo));

    return result;
}

FCubieCube ToCubieCube(FFaceCube& FaceCube)
{
    int8 ori;
    FCubieCube ccRet;
    for (int32 i = 0; i < 8; i++)
    {
        ccRet.Cp[i] = ECornerType::URF;
    }
    for (int32 i = 0; i < 12; i++)
    {
        ccRet.Ep[i] = EEdgeType::UR;
    }
    for (int32 i = 0; i < CORNER_COUNT; i++)
    {
        for (ori = 0; ori < 3; ori++)
        {
            if (FaceCube.Facelets[static_cast<int32>(FFaceCube::CornerFacelet()[i][ori])] == EColorType::U || FaceCube.Facelets[static_cast<int32>(FFaceCube::CornerFacelet()[i][ori])] == EColorType::D)
            {
                break;
            }
        }
        EColorType col1 = FaceCube.Facelets[static_cast<int32>(FFaceCube::CornerFacelet()[i][(ori + 1) % 3])];
        EColorType col2 = FaceCube.Facelets[static_cast<int32>(FFaceCube::CornerFacelet()[i][(ori + 2) % 3])];
        for (int32 j = 0; j < CORNER_COUNT; j++)
        {
            if (col1 == FFaceCube::CornerColor()[j][1] && col2 == FFaceCube::CornerColor()[j][2])
            {
                ccRet.Cp[i] = static_cast<ECornerType>(j);
                ccRet.Co[i] = ori % 3;
                break;
            }
        }
    }
    for (int32 i = 0; i < EDGE_COUNT; i++)
    {
        for (int32 j = 0; j < EDGE_COUNT; j++)
        {
            if (FaceCube.Facelets[static_cast<int32>(FFaceCube::EdgeFacelet()[i][0])] == FFaceCube::EdgeColor()[j][0]
                && FaceCube.Facelets[static_cast<int32>(FFaceCube::EdgeFacelet()[i][1])] == FFaceCube::EdgeColor()[j][1])
            {
                ccRet.Ep[i] = static_cast<EEdgeType>(j);
                ccRet.Eo[i] = 0;
                break;
            }
            if (FaceCube.Facelets[static_cast<int32>(FFaceCube::EdgeFacelet()[i][0])] == FFaceCube::EdgeColor()[j][1]
                && FaceCube.Facelets[static_cast<int32>(FFaceCube::EdgeFacelet()[i][1])] == FFaceCube::EdgeColor()[j][0])
            {
                ccRet.Ep[i] = static_cast<EEdgeType>(j);
                ccRet.Eo[i] = 1;
                break;
            }
        }
    }
    return ccRet;
}

int32 Cnk(const int32 N, int32 K)
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

void RotateLeftCorner(ECornerType* arr, int32 l, int32 r)
{
    ECornerType temp = arr[l];
    for (int32 i = l; i < r; i++)
        arr[i] = arr[i + 1];
    arr[r] = temp;
}

void RotateRightCorner(ECornerType* arr, int32 l, int32 r)
{
    ECornerType temp = arr[r];
    for (int32 i = r; i > l; i--)
        arr[i] = arr[i - 1];
    arr[l] = temp;
}


void RotateLeftEdge(EEdgeType* arr, int32 l, int32 r)
{
    EEdgeType temp = arr[l];
    for (int32 i = l; i < r; i++)
        arr[i] = arr[i + 1];
    arr[r] = temp;
}

void RotateRightEdge(EEdgeType* arr, int32 l, int32 r)
{
    EEdgeType temp = arr[r];
    for (int32 i = r; i > l; i--)
        arr[i] = arr[i - 1];
    arr[l] = temp;
}

FFaceCube ToFaceCube(FCubieCube& CubieCube)
{
    int32 j;
    int8 ori;
    FFaceCube fcRet;
    for (int32 i = 0; i < CORNER_COUNT; i++) {
        j = static_cast<int32>(CubieCube.Cp[i]);
        ori = CubieCube.Co[i];
        for (int32 n = 0; n < 3; n++)
            fcRet.Facelets[static_cast<int32>(FFaceCube::CornerFacelet()[i][(n + ori) % 3])] = FFaceCube::CornerColor()[j][n];
    }
    for (int32 i = 0; i < EDGE_COUNT; i++)
    {
        j = static_cast<int32>(CubieCube.Ep[i]);
        ori = CubieCube.Eo[i];
        for (int32 n = 0; n < 2; n++)
            fcRet.Facelets[static_cast<int32>(FFaceCube::EdgeFacelet()[i][(n + ori) % 2])] = FFaceCube::EdgeColor()[j][n];
    }
    return fcRet;
}

void CornerMultiply(FCubieCube& CubieCube, FCubieCube* b)
{
    ECornerType cPerm[8] = { static_cast<ECornerType>(0) };
    int8 cOri[8] = {0};
    for (int32 corn = 0; corn < CORNER_COUNT; corn++)
    {
        cPerm[corn] = CubieCube.Cp[static_cast<int32>(b->Cp[corn])];

        int8 oriA = CubieCube.Co[static_cast<int32>(b->Cp[corn])];
        int8 oriB = b->Co[corn];
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
        CubieCube.Cp[corn] = cPerm[corn];
        CubieCube.Co[corn] = cOri[corn];
    }
}

void EdgeMultiply(FCubieCube& CubieCube, FCubieCube* b)
{
    EEdgeType ePerm[12] = { static_cast<EEdgeType>(0) };
    int8 eOri[12] = {0};

    for (int32 edge = 0; edge < EDGE_COUNT; edge++) {
        ePerm[edge] = CubieCube.Ep[static_cast<int32>(b->Ep[edge])];
        eOri[edge] = (b->Eo[edge] + CubieCube.Eo[static_cast<int32>(b->Ep[edge])]) % 2;
    }
    for (int32 edge = 0; edge < EDGE_COUNT; edge++) {
        CubieCube.Ep[edge] = ePerm[edge];
        CubieCube.Eo[edge] = eOri[edge];
    }
}

void Multiply(FCubieCube& CubieCube, FCubieCube* b)
{
    CornerMultiply(CubieCube, b);
    EdgeMultiply(CubieCube, b);
}

void InvCubieCube(FCubieCube& CubieCube, FCubieCube* c)
{
    for (int32 edge = 0; edge < EDGE_COUNT; edge++)
        c->Ep[static_cast<int32>(CubieCube.Ep[edge])] = static_cast<EEdgeType>(edge);
    for (int32 edge = 0; edge < EDGE_COUNT; edge++)
        c->Eo[edge] = CubieCube.Eo[static_cast<int32>(c->Ep[edge])];
    for (int32 corn = 0; corn < CORNER_COUNT; corn++)
        c->Cp[static_cast<int32>(CubieCube.Cp[corn])] = static_cast<ECornerType>(corn);
    for (int32 corn = 0; corn < CORNER_COUNT; corn++) {
        int8 ori = CubieCube.Co[static_cast<int32>(c->Cp[corn])];
        if (ori >= 3)
            c->Co[corn] = ori;
        else {
            c->Co[corn] = -ori;
            if (c->Co[corn] < 0)
                c->Co[corn] += 3;
        }
    }
}

int16 GetTwist(FCubieCube& CubieCube)
{
    int16 ret = 0;
    for (int32 i = static_cast<int32>(ECornerType::URF); i < static_cast<int32>(ECornerType::DRB); i++)
        ret = static_cast<int16>(3 * ret + CubieCube.Co[i]);
    return ret;
}

void SetTwist(FCubieCube& CubieCube, int16 twist)
{
    int32 twistParity = 0;
    for (int32 i = static_cast<int32>(ECornerType::DRB) - 1; i >= static_cast<int32>(ECornerType::URF); i--) {
        twistParity += CubieCube.Co[i] = static_cast<int8>(twist % 3);
        twist /= 3;
    }
    CubieCube.Co[static_cast<int32>(ECornerType::DRB)] = static_cast<int8>((3 - twistParity % 3) % 3);
}

int16 GetFlip(FCubieCube& CubieCube)
{
    int16 ret = 0;
    for (int32 i = static_cast<int32>(EEdgeType::UR); i < static_cast<int32>(EEdgeType::BR); i++)
        ret = static_cast<int16>(2 * ret + CubieCube.Eo[i]);
    return ret;
}

void SetFlip(FCubieCube& CubieCube, int16 flip)
{
    int32 flipParity = 0;
    for (int32 i = static_cast<int32>(EEdgeType::BR) - 1; i >= static_cast<int32>(EEdgeType::UR); i--) {
        flipParity += CubieCube.Eo[i] = static_cast<int8>(flip % 2);
        flip /= 2;
    }
    CubieCube.Eo[static_cast<int32>(EEdgeType::BR)] = static_cast<int8>((2 - flipParity % 2) % 2);
}

int16 CornerParity(FCubieCube& CubieCube)
{
    int32 s = 0;
    for (int32 i = static_cast<int32>(ECornerType::DRB); i >= static_cast<int32>(ECornerType::URF) + 1; i--)
        for (int32 j = i - 1; j >= static_cast<int32>(ECornerType::URF); j--)
            if (CubieCube.Cp[j] > CubieCube.Cp[i])
                s++;
    return static_cast<int16>(s % 2);
}

int16 EdgeParity(FCubieCube& CubieCube)
{
    int32 s = 0;
    for (int32 i = static_cast<int32>(EEdgeType::BR); i >= static_cast<int32>(EEdgeType::UR) + 1; i--)
        for (int32 j = i - 1; j >= static_cast<int32>(EEdgeType::UR); j--)
            if (CubieCube.Ep[j] > CubieCube.Ep[i])
                s++;
    return static_cast<int16>(s % 2);
}

int16 GetFRtoBR(FCubieCube& CubieCube)
{
    int32 a = 0, x = 0;
    int32 b = 0;
    EEdgeType edge4[4] = { static_cast<EEdgeType>(0) };
    for (int32 j = static_cast<int32>(EEdgeType::BR); j >= static_cast<int32>(EEdgeType::UR); j--)
        if (EEdgeType::FR <= CubieCube.Ep[j] && CubieCube.Ep[j] <= EEdgeType::BR) {
            a += Cnk(11 - j, x + 1);
            edge4[3 - x++] = CubieCube.Ep[j];
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
void SetFRtoBR(FCubieCube& CubieCube, int16 idx)
{
    EEdgeType sliceEdge[4] = { EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR };
    EEdgeType otherEdge[8] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB };
    int32 b = idx % 24;
    int32 a = idx / 24;
    for (int32 e = 0; e < EDGE_COUNT; e++)
        CubieCube.Ep[e] = EEdgeType::DB;

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
            CubieCube.Ep[j] = sliceEdge[3 - x];
            a -= Cnk(11 - j, x-- + 1);
        }
    x = 0;
    for (int32 j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (CubieCube.Ep[j] == EEdgeType::DB)
            CubieCube.Ep[j] = otherEdge[x++];
}

int16 GetURFtoDLF(FCubieCube& CubieCube)
{
    int32 a = 0, x = 0, b = 0;
    ECornerType corner6[6] = { static_cast<ECornerType>(0) };
    for (int32 j = static_cast<int32>(ECornerType::URF); j <= static_cast<int32>(ECornerType::DRB); j++)
        if (CubieCube.Cp[j] <= ECornerType::DLF) {
            a += Cnk(j, x + 1);
            corner6[x++] = CubieCube.Cp[j];
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

void SetURFtoDLF(FCubieCube& CubieCube, int16 idx)
{
    ECornerType corner6[6] = { ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::UBR, ECornerType::DFR, ECornerType::DLF };
    ECornerType otherCorner[2] = { ECornerType::DBL, ECornerType::DRB };
    int32 b = idx % 720;
    int32 a = idx / 720;
    for(int32 c = 0; c < CORNER_COUNT; c++)
        CubieCube.Cp[c] = ECornerType::DRB;

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
            CubieCube.Cp[j] = corner6[x];
            a -= Cnk(j, x-- + 1);
        }
    x = 0;
    for (int32 j = static_cast<int32>(ECornerType::URF); j <= static_cast<int32>(ECornerType::DRB); j++)
        if (CubieCube.Cp[j] == ECornerType::DRB)
            CubieCube.Cp[j] = otherCorner[x++];
}

int32 GetURtoDF(FCubieCube& CubieCube)
{
    int32 a = 0, x = 0;
    int32 b = 0;
    EEdgeType edge6[6] = { static_cast<EEdgeType>(0) };
    for (int32 j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (CubieCube.Ep[j] <= EEdgeType::DF) {
            a += Cnk(j, x + 1);
            edge6[x++] = CubieCube.Ep[j];
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

void SetURtoDF(FCubieCube& CubieCube, int32 idx)
{
    EEdgeType edge6[6] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::DR, EEdgeType::DF };
    EEdgeType otherEdge[6] = { EEdgeType::DL, EEdgeType::DB, EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR };
    int32 b = idx % 720;
    int32 a = idx / 720;

    for(int32 e = 0; e < EDGE_COUNT; e++)
        CubieCube.Ep[e] = EEdgeType::BR;

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
            CubieCube.Ep[j] = edge6[x];
            a -= Cnk(j, x-- + 1);
        }
    x = 0;
    for (int32 j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (CubieCube.Ep[j] == EEdgeType::BR)
            CubieCube.Ep[j] = otherEdge[x++];
}

int16 GetURtoUL(FCubieCube& CubieCube)
{
    int32 a = 0, b = 0, x = 0;
    EEdgeType edge3[3] = { static_cast<EEdgeType>(0) };
    for (int32 j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (CubieCube.Ep[j] <= EEdgeType::UL) {
            a += Cnk(j, x + 1);
            edge3[x++] = CubieCube.Ep[j];
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

void SetURtoUL(FCubieCube& CubieCube, int16 idx)
{
    EEdgeType edge3[3] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL };
    int32 b = idx % 6;
    int32 a = idx / 6;
    for(int32 e = 0; e < EDGE_COUNT; e++) {
        CubieCube.Ep[e] = EEdgeType::BR;
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
            CubieCube.Ep[j] = edge3[x];
            a -= Cnk(j, x-- + 1);
        }
    }
}

int16 GetUBtoDF(FCubieCube& CubieCube)
{
    int32 a = 0, x = 0, b = 0;
    EEdgeType edge3[3] = { static_cast<EEdgeType>(0) };
    for (int32 j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (EEdgeType::UB <= CubieCube.Ep[j] && CubieCube.Ep[j] <= EEdgeType::DF) {
            a += Cnk(j, x + 1);
            edge3[x++] = CubieCube.Ep[j];
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

void SetUBtoDF(FCubieCube& CubieCube, int16 idx)
{
    EEdgeType edge3[3] = { EEdgeType::UB, EEdgeType::DR, EEdgeType::DF };
    int32 b = idx % 6;
    int32 a = idx / 6;
    for (int32 e = 0; e < EDGE_COUNT; e++)
        CubieCube.Ep[e] = EEdgeType::BR;

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
            CubieCube.Ep[j] = edge3[x];
            a -= Cnk(j, x-- + 1);
        }
}

int32 GetURFtoDLB(FCubieCube& CubieCube)
{
    ECornerType perm[8] = { static_cast<ECornerType>(0) };
    int32 b = 0;
    for (int32 i = 0; i < 8; i++)
        perm[i] = CubieCube.Cp[i];
    for (int32 j = 7; j > 0; j--)
    {
        int32 k = 0;
        while (static_cast<int32>(perm[j]) != j) {
            RotateLeftCorner(perm, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return b;
}

void SetURFtoDLB(FCubieCube& CubieCube, int32 idx)
{
    ECornerType perm[8] = { ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::UBR, ECornerType::DFR, ECornerType::DLF, ECornerType::DBL, ECornerType::DRB };
    int32 x = 7;
    for (int32 j = 1; j < 8; j++) {
        int32 k = idx % (j + 1);
        idx /= j + 1;
        while (k-- > 0)
            RotateRightCorner(perm, 0, j);
    }

    for (int32 j = 7; j >= 0; j--)
        CubieCube.Cp[j] = perm[x--];
}

int32 GetURtoBR(FCubieCube& CubieCube)
{
    EEdgeType perm[12] = { static_cast<EEdgeType>(0) };
    int32 b = 0;
    for (int32 i = 0; i < 12; i++)
        perm[i] = CubieCube.Ep[i];
    for (int32 j = 11; j > 0; j--)
    {
        int32 k = 0;
        while (static_cast<int32>(perm[j]) != j) {
            RotateLeftEdge(perm, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return b;
}

void SetURtoBR(FCubieCube& CubieCube, int32 idx)
{
    EEdgeType perm[12] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR };
    int32 x = 11;
    for (int32 j = 1; j < 12; j++) {
        int32 k = idx % (j + 1);
        idx /= j + 1;
        while (k-- > 0)
            RotateRightEdge(perm, 0, j);
    }
    for (int32 j = 11; j >= 0; j--)
        CubieCube.Ep[j] = perm[x--];
}

int32 Verify(FCubieCube& CubieCube)
{
    int32 sum = 0, i;
    int32 edgeCount[12] = {0};
    int32 cornerCount[8] = {0};

    for(int32 e = 0; e < EDGE_COUNT; e++)
        edgeCount[static_cast<int32>(CubieCube.Ep[e])]++;
    for (i = 0; i < 12; i++)
        if (edgeCount[i] != 1)
            return -2;

    for (i = 0; i < 12; i++)
        sum += CubieCube.Eo[i];
    if (sum % 2 != 0)
        return -3;

    for(int32 c = 0; c < CORNER_COUNT; c++)
        cornerCount[static_cast<int32>(CubieCube.Cp[c])]++;
    for (i = 0; i < 8; i++)
        if (cornerCount[i] != 1)
            return -4;

    sum = 0;
    for (i = 0; i < 8; i++)
        sum += CubieCube.Co[i];
    if (sum % 3 != 0)
        return -5;

    if ((EdgeParity(CubieCube) ^ CornerParity(CubieCube)) != 0)
        return -6;

    return 0;
}

int32 GetURtoDF_Standalone(int16 idx1, int16 idx2)
{
    FCubieCube a = GetCubieCube();
    FCubieCube b = GetCubieCube();
    SetURtoUL(a, idx1);
    SetUBtoDF(b, idx2);
    for (int32 i = 0; i < 8; i++) {
        if (a.Ep[i] != EEdgeType::BR)
        {
            if (b.Ep[i] != EEdgeType::BR) {
                return -1;
            }
            b.Ep[i] = a.Ep[i];
        }
    }
    int32 res = GetURtoDF(b);
    return res;
}
