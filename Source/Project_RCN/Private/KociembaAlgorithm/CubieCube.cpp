#include "KociembaAlgorithm/CubieCube.h"

#include "KociembaAlgorithm/facecube.h"

FCubieCube* GetMoveCube()
{
    static FCubieCube MoveCube[6] = {};
    static bool MoveCubeInitialized = false;
    
    static constexpr ECornerType cpU[8]  = { ECornerType::UBR, ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::DFR, ECornerType::DLF, ECornerType::DBL, ECornerType::DRB };
    static const signed char coU[8]  = { 0, 0, 0, 0, 0, 0, 0, 0 };
    static constexpr EEdgeType epU[12] = { EEdgeType::UB, EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR };
    static const signed char eoU[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    static constexpr ECornerType cpR[8]  = { ECornerType::DFR, ECornerType::UFL, ECornerType::ULB, ECornerType::URF, ECornerType::DRB, ECornerType::DLF, ECornerType::DBL, ECornerType::UBR };
    static const signed char coR[8]  = { 2, 0, 0, 1, 1, 0, 0, 2 };
    static constexpr EEdgeType epR[12] = { EEdgeType::FR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::BR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, EEdgeType::DR, EEdgeType::FL, EEdgeType::BL, EEdgeType::UR };
    static const signed char eoR[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    static constexpr ECornerType cpF[8]  = { ECornerType::UFL, ECornerType::DLF, ECornerType::ULB, ECornerType::UBR, ECornerType::URF, ECornerType::DFR, ECornerType::DBL, ECornerType::DRB };
    static const signed char coF[8]  = { 1, 2, 0, 0, 2, 1, 0, 0 };
    static constexpr EEdgeType epF[12] = { EEdgeType::UR, EEdgeType::FL, EEdgeType::UL, EEdgeType::UB, EEdgeType::DR, EEdgeType::FR, EEdgeType::DL, EEdgeType::DB, EEdgeType::UF, EEdgeType::DF, EEdgeType::BL, EEdgeType::BR };
    static const signed char eoF[12] = { 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0 };

    static constexpr ECornerType cpD[8]  = { ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::UBR, ECornerType::DLF, ECornerType::DBL, ECornerType::DRB, ECornerType::DFR };
    static const signed char coD[8]  = { 0, 0, 0, 0, 0, 0, 0, 0 };
    static constexpr EEdgeType epD[12] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, EEdgeType::DR, EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR };
    static const signed char eoD[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    static constexpr ECornerType cpL[8]  = { ECornerType::URF, ECornerType::ULB, ECornerType::DBL, ECornerType::UBR, ECornerType::DFR, ECornerType::UFL, ECornerType::DLF, ECornerType::DRB };
    static const signed char coL[8]  = { 0, 1, 2, 0, 0, 2, 1, 0 };
    static constexpr EEdgeType epL[12] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::BL, EEdgeType::UB, EEdgeType::DR, EEdgeType::DF, EEdgeType::FL, EEdgeType::DB, EEdgeType::FR, EEdgeType::UL, EEdgeType::DL, EEdgeType::BR };
    static const signed char eoL[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    static constexpr ECornerType cpB[8]  = { ECornerType::URF, ECornerType::UFL, ECornerType::UBR, ECornerType::DRB, ECornerType::DFR, ECornerType::DLF, ECornerType::ULB, ECornerType::DBL };
    static const signed char coB[8]  = { 0, 0, 1, 2, 0, 0, 2, 1 };
    static constexpr EEdgeType epB[12] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::BR, EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::BL, EEdgeType::FR, EEdgeType::FL, EEdgeType::UB, EEdgeType::DB };
    static const signed char eoB[12] = { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1 };

    if (!MoveCubeInitialized)
    {
        MoveCube[0].Cp = TArray(cpU, UE_ARRAY_COUNT(cpU));
        MoveCube[0].Co = TArray(coU, UE_ARRAY_COUNT(coU));
        MoveCube[0].Ep = TArray(epU, UE_ARRAY_COUNT(epU));
        MoveCube[0].Eo = TArray(eoU, UE_ARRAY_COUNT(eoU));

        MoveCube[1].Cp = TArray(cpR, UE_ARRAY_COUNT(cpR));
        MoveCube[1].Co = TArray(coR, UE_ARRAY_COUNT(coR));
        MoveCube[1].Ep = TArray(epR, UE_ARRAY_COUNT(epR));
        MoveCube[1].Eo = TArray(eoR, UE_ARRAY_COUNT(eoR));

        MoveCube[2].Cp = TArray(cpF, UE_ARRAY_COUNT(cpF));
        MoveCube[2].Co = TArray(coF, UE_ARRAY_COUNT(coF));
        MoveCube[2].Ep = TArray(epF, UE_ARRAY_COUNT(epF));
        MoveCube[2].Eo = TArray(eoF, UE_ARRAY_COUNT(eoF));

        MoveCube[3].Cp = TArray(cpD, UE_ARRAY_COUNT(cpD));
        MoveCube[3].Co = TArray(coD, UE_ARRAY_COUNT(coD));
        MoveCube[3].Ep = TArray(epD, UE_ARRAY_COUNT(epD));
        MoveCube[3].Eo = TArray(eoD, UE_ARRAY_COUNT(eoD));

        MoveCube[4].Cp = TArray(cpL, UE_ARRAY_COUNT(cpL));
        MoveCube[4].Co = TArray(coL, UE_ARRAY_COUNT(coL));
        MoveCube[4].Ep = TArray(epL, UE_ARRAY_COUNT(epL));
        MoveCube[4].Eo = TArray(eoL, UE_ARRAY_COUNT(eoL));

        MoveCube[5].Cp = TArray(cpB, UE_ARRAY_COUNT(cpB));
        MoveCube[5].Co = TArray(coB, UE_ARRAY_COUNT(coB));
        MoveCube[5].Ep = TArray(epB, UE_ARRAY_COUNT(epB));
        MoveCube[5].Eo = TArray(eoB, UE_ARRAY_COUNT(eoB));
    }

    return MoveCube;
}

FCubieCube* GetCubieCube()
{
    FCubieCube* result = new FCubieCube();

    static constexpr ECornerType  cp[8]  = { 
        ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::UBR, 
        ECornerType::DFR, ECornerType::DLF, ECornerType::DBL, ECornerType::DRB 
    };

    static const int8         co[8]  = { 0, 0, 0, 0, 0, 0, 0, 0 };

    static constexpr EEdgeType    ep[12] = { 
        EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, 
        EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, 
        EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR 
    };

    static const int8         eo[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    result->Cp = TArray(cp, UE_ARRAY_COUNT(cp));
    result->Co = TArray(co, UE_ARRAY_COUNT(co));
    result->Ep = TArray(ep, UE_ARRAY_COUNT(ep));
    result->Eo = TArray(eo, UE_ARRAY_COUNT(eo));

    return result;
}

int Cnk(int n, int k) {
    int i, j, s;
    if (n < k)
        return 0;
    if (k > n / 2)
        k = n - k;
    for (s = 1, i = n, j = 1; i != n - k; i--, j++) {
        s *= i;
        s /= j;
    }
    return s;
}

void RotateLeftCorner(ECornerType* arr, int l, int r)
{
    ECornerType temp = arr[l];
    for (int32 i = l; i < r; i++)
        arr[i] = arr[i + 1];
    arr[r] = temp;
}

void RotateRightCorner(ECornerType* arr, int l, int r)
{
    ECornerType temp = arr[r];
    for (int32 i = r; i > l; i--)
        arr[i] = arr[i - 1];
    arr[l] = temp;
}


void RotateLeftEdge(EEdgeType* arr, int l, int r)
{
    EEdgeType temp = arr[l];
    for (int32 i = l; i < r; i++)
        arr[i] = arr[i + 1];
    arr[r] = temp;
}

void RotateRightEdge(EEdgeType* arr, int l, int r)
{
    EEdgeType temp = arr[r];
    for (int32 i = r; i > l; i--)
        arr[i] = arr[i - 1];
    arr[l] = temp;
}

FFaceCube* ToFaceCube(FCubieCube* CubieCube)
{
    int j;
    signed char ori;
    FFaceCube* fcRet = GetFaceCube();
    for (int32 i = 0; i < CORNER_COUNT; i++) {
        j = static_cast<int32>(CubieCube->Cp[i]);
        ori = CubieCube->Co[i];
        for (int32 n = 0; n < 3; n++)
            fcRet->Facelets[static_cast<int32>(CornerFacelet[i][(n + ori) % 3])] = CornerColor[j][n];
    }
    for (int32 i = 0; i < EDGE_COUNT; i++)
    {
        j = static_cast<int32>(CubieCube->Ep[i]);
        ori = CubieCube->Eo[i];
        for (int32 n = 0; n < 2; n++)
            fcRet->Facelets[static_cast<int32>(EdgeFacelet[i][(n + ori) % 2])] = EdgeColor[j][n];
    }
    return fcRet;
}

void CornerMultiply(FCubieCube* CubieCube, FCubieCube* b)
{
    ECornerType cPerm[8] = { static_cast<ECornerType>(0) };
    signed char cOri[8] = {0};
    for (int32 corn = 0; corn < CORNER_COUNT; corn++)
    {
        cPerm[corn] = CubieCube->Cp[static_cast<int32>(b->Cp[corn])];

        signed char oriA = CubieCube->Co[static_cast<int32>(b->Cp[corn])];
        signed char oriB = b->Co[corn];
        signed char ori = 0;

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
        CubieCube->Cp[corn] = cPerm[corn];
        CubieCube->Co[corn] = cOri[corn];
    }
}

void EdgeMultiply(FCubieCube* CubieCube, FCubieCube* b)
{
    EEdgeType ePerm[12] = { static_cast<EEdgeType>(0) };
    signed char eOri[12] = {0};

    for (int32 edge = 0; edge < EDGE_COUNT; edge++) {
        ePerm[edge] = CubieCube->Ep[static_cast<int32>(b->Ep[edge])];
        eOri[edge] = (b->Eo[edge] + CubieCube->Eo[static_cast<int32>(b->Ep[edge])]) % 2;
    }
    for (int32 edge = 0; edge < EDGE_COUNT; edge++) {
        CubieCube->Ep[edge] = ePerm[edge];
        CubieCube->Eo[edge] = eOri[edge];
    }
}

void Multiply(FCubieCube* CubieCube, FCubieCube* b)
{
    CornerMultiply(CubieCube, b);
    EdgeMultiply(CubieCube, b);
}

void InvCubieCube(FCubieCube* CubieCube, FCubieCube* c)
{
    for (int32 edge = 0; edge < EDGE_COUNT; edge++)
        c->Ep[static_cast<int32>(CubieCube->Ep[edge])] = static_cast<EEdgeType>(edge);
    for (int32 edge = 0; edge < EDGE_COUNT; edge++)
        c->Eo[edge] = CubieCube->Eo[static_cast<int32>(c->Ep[edge])];
    for (int32 corn = 0; corn < CORNER_COUNT; corn++)
        c->Cp[static_cast<int32>(CubieCube->Cp[corn])] = static_cast<ECornerType>(corn);
    for (int32 corn = 0; corn < CORNER_COUNT; corn++) {
        signed char ori = CubieCube->Co[static_cast<int32>(c->Cp[corn])];
        if (ori >= 3)
            c->Co[corn] = ori;
        else {
            c->Co[corn] = -ori;
            if (c->Co[corn] < 0)
                c->Co[corn] += 3;
        }
    }
}

short GetTwist(FCubieCube* CubieCube)
{
    short ret = 0;
    for (int32 i = static_cast<int32>(ECornerType::URF); i < static_cast<int32>(ECornerType::DRB); i++)
        ret = static_cast<short>(3 * ret + CubieCube->Co[i]);
    return ret;
}

void SetTwist(FCubieCube* CubieCube, short twist)
{
    int twistParity = 0;
    for (int32 i = static_cast<int32>(ECornerType::DRB) - 1; i >= static_cast<int32>(ECornerType::URF); i--) {
        twistParity += CubieCube->Co[i] = static_cast<signed char>(twist % 3);
        twist /= 3;
    }
    CubieCube->Co[static_cast<int32>(ECornerType::DRB)] = static_cast<signed char>((3 - twistParity % 3) % 3);
}

short GetFlip(FCubieCube* CubieCube)
{
    short ret = 0;
    for (int32 i = static_cast<int32>(EEdgeType::UR); i < static_cast<int32>(EEdgeType::BR); i++)
        ret = static_cast<short>(2 * ret + CubieCube->Eo[i]);
    return ret;
}

void SetFlip(FCubieCube* CubieCube, short flip)
{
    int flipParity = 0;
    for (int32 i = static_cast<int32>(EEdgeType::BR) - 1; i >= static_cast<int32>(EEdgeType::UR); i--) {
        flipParity += CubieCube->Eo[i] = static_cast<signed char>(flip % 2);
        flip /= 2;
    }
    CubieCube->Eo[static_cast<int32>(EEdgeType::BR)] = static_cast<signed char>((2 - flipParity % 2) % 2);
}

short CornerParity(FCubieCube* CubieCube)
{
    int s = 0;
    for (int32 i = static_cast<int32>(ECornerType::DRB); i >= static_cast<int32>(ECornerType::URF) + 1; i--)
        for (int32 j = i - 1; j >= static_cast<int32>(ECornerType::URF); j--)
            if (CubieCube->Cp[j] > CubieCube->Cp[i])
                s++;
    return static_cast<short>(s % 2);
}

short EdgeParity(FCubieCube* CubieCube)
{
    int s = 0;
    for (int32 i = static_cast<int32>(EEdgeType::BR); i >= static_cast<int32>(EEdgeType::UR) + 1; i--)
        for (int32 j = i - 1; j >= static_cast<int32>(EEdgeType::UR); j--)
            if (CubieCube->Ep[j] > CubieCube->Ep[i])
                s++;
    return static_cast<short>(s % 2);
}

short GetFRtoBR(FCubieCube* CubieCube)
{
    int a = 0, x = 0;
    int b = 0;
    EEdgeType edge4[4] = { static_cast<EEdgeType>(0) };
    for (int32 j = static_cast<int32>(EEdgeType::BR); j >= static_cast<int32>(EEdgeType::UR); j--)
        if (EEdgeType::FR <= CubieCube->Ep[j] && CubieCube->Ep[j] <= EEdgeType::BR) {
            a += Cnk(11 - j, x + 1);
            edge4[3 - x++] = CubieCube->Ep[j];
        }

    for (int32 j = 3; j > 0; j--)
    {
        int k = 0;
        while (static_cast<int32>(edge4[j]) != j + 8) {
            RotateLeftEdge(edge4, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return static_cast<short>(24 * a + b);
}
void SetFRtoBR(FCubieCube* CubieCube, short idx)
{
    EEdgeType sliceEdge[4] = { EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR };
    EEdgeType otherEdge[8] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB };
    int b = idx % 24;
    int a = idx / 24;
    for (int e = 0; e < EDGE_COUNT; e++)
        CubieCube->Ep[e] = EEdgeType::DB;

    for (int32 j = 1; j < 4; j++)
    {
        int k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            RotateRightEdge(sliceEdge, 0, j);
    }

    int x = 3;
    for (int32 j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (a - Cnk(11 - j, x + 1) >= 0) {
            CubieCube->Ep[j] = sliceEdge[3 - x];
            a -= Cnk(11 - j, x-- + 1);
        }
    x = 0;
    for (int32 j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (CubieCube->Ep[j] == EEdgeType::DB)
            CubieCube->Ep[j] = otherEdge[x++];
}

short GetURFtoDLF(FCubieCube* CubieCube)
{
    int a = 0, x = 0, b = 0;
    ECornerType corner6[6] = { static_cast<ECornerType>(0) };
    for (int32 j = static_cast<int32>(ECornerType::URF); j <= static_cast<int32>(ECornerType::DRB); j++)
        if (CubieCube->Cp[j] <= ECornerType::DLF) {
            a += Cnk(j, x + 1);
            corner6[x++] = CubieCube->Cp[j];
        }

    for (int32 j = 5; j > 0; j--)
    {
        int k = 0;
        while (static_cast<int32>(corner6[j]) != j) {
            RotateLeftCorner(corner6, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return static_cast<short>(720 * a + b);
}

void SetURFtoDLF(FCubieCube* CubieCube, short idx)
{
    ECornerType corner6[6] = { ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::UBR, ECornerType::DFR, ECornerType::DLF };
    ECornerType otherCorner[2] = { ECornerType::DBL, ECornerType::DRB };
    int b = idx % 720;
    int a = idx / 720;
    for(int c = 0; c < CORNER_COUNT; c++)
        CubieCube->Cp[c] = ECornerType::DRB;

    for (int32 j = 1; j < 6; j++)
    {
        int k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            RotateRightCorner(corner6, 0, j);
    }
    int x = 5;
    for (int32 j = static_cast<int32>(ECornerType::DRB); j >= 0; j--)
        if (a - Cnk(j, x + 1) >= 0) {
            CubieCube->Cp[j] = corner6[x];
            a -= Cnk(j, x-- + 1);
        }
    x = 0;
    for (int32 j = static_cast<int32>(ECornerType::URF); j <= static_cast<int32>(ECornerType::DRB); j++)
        if (CubieCube->Cp[j] == ECornerType::DRB)
            CubieCube->Cp[j] = otherCorner[x++];
}

int GetURtoDF(FCubieCube* CubieCube)
{
    int a = 0, x = 0;
    int b = 0;
    EEdgeType edge6[6] = { static_cast<EEdgeType>(0) };
    for (int32 j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (CubieCube->Ep[j] <= EEdgeType::DF) {
            a += Cnk(j, x + 1);
            edge6[x++] = CubieCube->Ep[j];
        }

    for (int32 j = 5; j > 0; j--)
    {
        int k = 0;
        while (static_cast<int32>(edge6[j]) != j) {
            RotateLeftEdge(edge6, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return 720 * a + b;
}

void SetURtoDF(FCubieCube* CubieCube, int idx)
{
    EEdgeType edge6[6] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::DR, EEdgeType::DF };
    EEdgeType otherEdge[6] = { EEdgeType::DL, EEdgeType::DB, EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR };
    int b = idx % 720;
    int a = idx / 720;

    for(int e = 0; e < EDGE_COUNT; e++)
        CubieCube->Ep[e] = EEdgeType::BR;

    for (int32 j = 1; j < 6; j++)
    {
        int k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            RotateRightEdge(edge6, 0, j);
    }
    int x = 5;
    for (int32 j = static_cast<int32>(EEdgeType::BR); j >= 0; j--)
        if (a - Cnk(j, x + 1) >= 0) {
            CubieCube->Ep[j] = edge6[x];
            a -= Cnk(j, x-- + 1);
        }
    x = 0;
    for (int32 j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (CubieCube->Ep[j] == EEdgeType::BR)
            CubieCube->Ep[j] = otherEdge[x++];
}

short GetURtoUL(FCubieCube* CubieCube)
{
    int a = 0, b = 0, x = 0;
    EEdgeType edge3[3] = { static_cast<EEdgeType>(0) };
    for (int32 j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (CubieCube->Ep[j] <= EEdgeType::UL) {
            a += Cnk(j, x + 1);
            edge3[x++] = CubieCube->Ep[j];
        }

    for (int32 j = 2; j > 0; j--)
    {
        int k = 0;
        while (static_cast<int32>(edge3[j]) != j) {
            RotateLeftEdge(edge3, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return static_cast<short>(6 * a + b);
}

void SetURtoUL(FCubieCube* CubieCube, short idx)
{
    EEdgeType edge3[3] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL };
    int b = idx % 6;
    int a = idx / 6;
    for(int e = 0; e < EDGE_COUNT; e++) {
        CubieCube->Ep[e] = EEdgeType::BR;
    }

    for (int32 j = 1; j < 3; j++) {
        int k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            RotateRightEdge(edge3, 0, j);
    }
    int x = 2;
    for (int32 j = static_cast<int32>(EEdgeType::BR); j >= 0; j--) {
        if (a - Cnk(j, x + 1) >= 0) {
            CubieCube->Ep[j] = edge3[x];
            a -= Cnk(j, x-- + 1);
        }
    }
}

short GetUBtoDF(FCubieCube* CubieCube)
{
    int a = 0, x = 0, b = 0;
    EEdgeType edge3[3] = { static_cast<EEdgeType>(0) };
    for (int32 j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (EEdgeType::UB <= CubieCube->Ep[j] && CubieCube->Ep[j] <= EEdgeType::DF) {
            a += Cnk(j, x + 1);
            edge3[x++] = CubieCube->Ep[j];
        }

    for (int32 j = 2; j > 0; j--)
    {
        int k = 0;
        while (static_cast<int32>(edge3[j]) != static_cast<int32>(EEdgeType::UB) + j) {
            RotateLeftEdge(edge3, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return static_cast<short>(6 * a + b);
}

void SetUBtoDF(FCubieCube* CubieCube, short idx)
{
    EEdgeType edge3[3] = { EEdgeType::UB, EEdgeType::DR, EEdgeType::DF };
    int b = idx % 6;
    int a = idx / 6;
    for (int e = 0; e < EDGE_COUNT; e++)
        CubieCube->Ep[e] = EEdgeType::BR;

    for (int32 j = 1; j < 3; j++)
    {
        int k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            RotateRightEdge(edge3, 0, j);
    }
    int x = 2;
    for (int32 j = static_cast<int32>(EEdgeType::BR); j >= 0; j--)
        if (a - Cnk(j, x + 1) >= 0) {
            CubieCube->Ep[j] = edge3[x];
            a -= Cnk(j, x-- + 1);
        }
}

int GetURFtoDLB(FCubieCube* CubieCube)
{
    ECornerType perm[8] = { static_cast<ECornerType>(0) };
    int b = 0;
    for (int32 i = 0; i < 8; i++)
        perm[i] = CubieCube->Cp[i];
    for (int32 j = 7; j > 0; j--)
    {
        int k = 0;
        while (static_cast<int32>(perm[j]) != j) {
            RotateLeftCorner(perm, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return b;
}

void SetURFtoDLB(FCubieCube* CubieCube, int idx)
{
    ECornerType perm[8] = { ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::UBR, ECornerType::DFR, ECornerType::DLF, ECornerType::DBL, ECornerType::DRB };
    int x = 7;
    for (int32 j = 1; j < 8; j++) {
        int k = idx % (j + 1);
        idx /= j + 1;
        while (k-- > 0)
            RotateRightCorner(perm, 0, j);
    }

    for (int32 j = 7; j >= 0; j--)
        CubieCube->Cp[j] = perm[x--];
}

int GetURtoBR(FCubieCube* CubieCube)
{
    EEdgeType perm[12] = { static_cast<EEdgeType>(0) };
    int b = 0;
    for (int32 i = 0; i < 12; i++)
        perm[i] = CubieCube->Ep[i];
    for (int32 j = 11; j > 0; j--)
    {
        int k = 0;
        while (static_cast<int32>(perm[j]) != j) {
            RotateLeftEdge(perm, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return b;
}

void SetURtoBR(FCubieCube* CubieCube, int idx)
{
    EEdgeType perm[12] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR };
    int x = 11;
    for (int32 j = 1; j < 12; j++) {
        int k = idx % (j + 1);
        idx /= j + 1;
        while (k-- > 0)
            RotateRightEdge(perm, 0, j);
    }
    for (int32 j = 11; j >= 0; j--)
        CubieCube->Ep[j] = perm[x--];
}

int Verify(FCubieCube* CubieCube)
{
    int sum = 0, i;
    int32 edgeCount[12] = {0};
    int32 cornerCount[8] = {0};

    for(int e = 0; e < EDGE_COUNT; e++)
        edgeCount[static_cast<int32>(CubieCube->Ep[e])]++;
    for (i = 0; i < 12; i++)
        if (edgeCount[i] != 1)
            return -2;

    for (i = 0; i < 12; i++)
        sum += CubieCube->Eo[i];
    if (sum % 2 != 0)
        return -3;

    for(int c = 0; c < CORNER_COUNT; c++)
        cornerCount[static_cast<int32>(CubieCube->Cp[c])]++;
    for (i = 0; i < 8; i++)
        if (cornerCount[i] != 1)
            return -4;

    sum = 0;
    for (i = 0; i < 8; i++)
        sum += CubieCube->Co[i];
    if (sum % 3 != 0)
        return -5;

    if ((EdgeParity(CubieCube) ^ CornerParity(CubieCube)) != 0)
        return -6;

    return 0;
}

int GetURtoDF_Standalone(short idx1, short idx2)
{
    FCubieCube *a = GetCubieCube();
    FCubieCube *b = GetCubieCube();
    SetURtoUL(a, idx1);
    SetUBtoDF(b, idx2);
    for (int i = 0; i < 8; i++) {
        if (a->Ep[i] != EEdgeType::BR)
        {
            if (b->Ep[i] != EEdgeType::BR) {
                return -1;
            }
            b->Ep[i] = a->Ep[i];
        }
    }
    int res = GetURtoDF(b);
    return res;
}
