#include "KociembaAlgorithm/cubiecube.h"
#include "KociembaAlgorithm/facecube.h"

FCubieCube * get_moveCube()
{
    static FCubieCube MoveCube[6];
    static bool MoveCubeInitialized = false;
    
    static const ECornerType cpU[8]  = { ECornerType::UBR, ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::DFR, ECornerType::DLF, ECornerType::DBL, ECornerType::DRB };
    static const signed char coU[8]  = { 0, 0, 0, 0, 0, 0, 0, 0 };
    static const EEdgeType epU[12] = { EEdgeType::UB, EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR };
    static const signed char eoU[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    static const ECornerType cpR[8]  = { ECornerType::DFR, ECornerType::UFL, ECornerType::ULB, ECornerType::URF, ECornerType::DRB, ECornerType::DLF, ECornerType::DBL, ECornerType::UBR };
    static const signed char coR[8]  = { 2, 0, 0, 1, 1, 0, 0, 2 };
    static const EEdgeType epR[12] = { EEdgeType::FR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::BR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, EEdgeType::DR, EEdgeType::FL, EEdgeType::BL, EEdgeType::UR };
    static const signed char eoR[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    static const ECornerType cpF[8]  = { ECornerType::UFL, ECornerType::DLF, ECornerType::ULB, ECornerType::UBR, ECornerType::URF, ECornerType::DFR, ECornerType::DBL, ECornerType::DRB };
    static const signed char coF[8]  = { 1, 2, 0, 0, 2, 1, 0, 0 };
    static const EEdgeType epF[12] = { EEdgeType::UR, EEdgeType::FL, EEdgeType::UL, EEdgeType::UB, EEdgeType::DR, EEdgeType::FR, EEdgeType::DL, EEdgeType::DB, EEdgeType::UF, EEdgeType::DF, EEdgeType::BL, EEdgeType::BR };
    static const signed char eoF[12] = { 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0 };

    static const ECornerType cpD[8]  = { ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::UBR, ECornerType::DLF, ECornerType::DBL, ECornerType::DRB, ECornerType::DFR };
    static const signed char coD[8]  = { 0, 0, 0, 0, 0, 0, 0, 0 };
    static const EEdgeType epD[12] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, EEdgeType::DR, EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR };
    static const signed char eoD[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    static const ECornerType cpL[8]  = { ECornerType::URF, ECornerType::ULB, ECornerType::DBL, ECornerType::UBR, ECornerType::DFR, ECornerType::UFL, ECornerType::DLF, ECornerType::DRB };
    static const signed char coL[8]  = { 0, 1, 2, 0, 0, 2, 1, 0 };
    static const EEdgeType epL[12] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::BL, EEdgeType::UB, EEdgeType::DR, EEdgeType::DF, EEdgeType::FL, EEdgeType::DB, EEdgeType::FR, EEdgeType::UL, EEdgeType::DL, EEdgeType::BR };
    static const signed char eoL[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    static const ECornerType cpB[8]  = { ECornerType::URF, ECornerType::UFL, ECornerType::UBR, ECornerType::DRB, ECornerType::DFR, ECornerType::DLF, ECornerType::ULB, ECornerType::DBL };
    static const signed char coB[8]  = { 0, 0, 1, 2, 0, 0, 2, 1 };
    static const EEdgeType epB[12] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::BR, EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::BL, EEdgeType::FR, EEdgeType::FL, EEdgeType::UB, EEdgeType::DB };
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

FCubieCube* get_cubiecube()
{
    FCubieCube* result = new FCubieCube();

    static const ECornerType  cp[8]  = { 
        ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::UBR, 
        ECornerType::DFR, ECornerType::DLF, ECornerType::DBL, ECornerType::DRB 
    };

    static const int8         co[8]  = { 0, 0, 0, 0, 0, 0, 0, 0 };

    static const EEdgeType    ep[12] = { 
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

void rotateLeft_corner(ECornerType* arr, int l, int r)
// Left rotation of all array elements between l and r
{
    int i;
    ECornerType temp = arr[l];
    for (i = l; i < r; i++)
        arr[i] = arr[i + 1];
    arr[r] = temp;
}

void rotateRight_corner(ECornerType* arr, int l, int r)
// Right rotation of all array elements between l and r
{
    int i;
    ECornerType temp = arr[r];
    for (i = r; i > l; i--)
        arr[i] = arr[i - 1];
    arr[l] = temp;
}


void rotateLeft_edge(EEdgeType* arr, int l, int r)
// Left rotation of all array elements between l and r
{
    int i;
    EEdgeType temp = arr[l];
    for (i = l; i < r; i++)
        arr[i] = arr[i + 1];
    arr[r] = temp;
}

void rotateRight_edge(EEdgeType* arr, int l, int r)
// Right rotation of all array elements between l and r
{
    int i;
    EEdgeType temp = arr[r];
    for (i = r; i > l; i--)
        arr[i] = arr[i - 1];
    arr[l] = temp;
}

FFaceCube* toFaceCube(FCubieCube* cubiecube)
{
    int i, j, n;
    signed char ori;
    FFaceCube* fcRet = get_facecube();
    for(i = 0; i < CORNER_COUNT; i++) {
        j = static_cast<int32>(cubiecube->Cp[i]);
        ori = cubiecube->Co[i];
        for (n = 0; n < 3; n++)
            fcRet->Facelets[static_cast<int32>(CornerFacelet[i][(n + ori) % 3])] = CornerColor[j][n];
    }
    for(i = 0; i < EDGE_COUNT; i++)
    {
        j = static_cast<int32>(cubiecube->Ep[i]);
        ori = cubiecube->Eo[i];
        for (n = 0; n < 2; n++)
            fcRet->Facelets[static_cast<int32>(EdgeFacelet[i][(n + ori) % 2])] = EdgeColor[j][n];
    }
    return fcRet;
}

void cornerMultiply(FCubieCube* cubiecube, FCubieCube* b)
{
    int corn;
    signed char oriA, oriB, ori;
    ECornerType cPerm[8] = {(ECornerType)0};
    signed char cOri[8] = {0};
    for (corn = 0; corn < CORNER_COUNT; corn++) {
        cPerm[corn] = cubiecube->Cp[static_cast<int32>(b->Cp[corn])];

        oriA = cubiecube->Co[static_cast<int32>(b->Cp[corn])];
        oriB = b->Co[corn];
        ori = 0;

        if (oriA < 3 && oriB < 3) // if both cubes are regular cubes...
        {
            ori = oriA + oriB; // just do an addition modulo 3 here
            if (ori >= 3)
                ori -= 3; // the composition is a regular cube

            // +++++++++++++++++++++not used in this implementation +++++++++++++++++++++++++++++++++++
        } else if (oriA < 3 && oriB >= 3) // if cube b is in a mirrored
        // state...
        {
            ori = oriA + oriB;
            if (ori >= 6)
                ori -= 3; // the composition is a mirrored cube
        } else if (oriA >= 3 && oriB < 3) // if cube a is an a mirrored
        // state...
        {
            ori = oriA - oriB;
            if (ori < 3)
                ori += 3; // the composition is a mirrored cube
        } else if (oriA >= 3 && oriB >= 3) // if both cubes are in mirrored
        // states...
        {
            ori = oriA - oriB;
            if (ori < 0)
                ori += 3; // the composition is a regular cube
            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        }
        cOri[corn] = ori;
    }
    for(corn = 0; corn < CORNER_COUNT; corn++) {
        cubiecube->Cp[corn] = cPerm[corn];
        cubiecube->Co[corn] = cOri[corn];
    }
}

void edgeMultiply(FCubieCube* cubiecube, FCubieCube* b)
{
    int edge;
    EEdgeType ePerm[12] = {(EEdgeType)0};
    signed char eOri[12] = {0};

    for(edge = 0; edge < EDGE_COUNT; edge++) {
        ePerm[edge] = cubiecube->Ep[static_cast<int32>(b->Ep[edge])];
        eOri[edge] = (b->Eo[edge] + cubiecube->Eo[static_cast<int32>(b->Ep[edge])]) % 2;
    }
    for(edge = 0; edge < EDGE_COUNT; edge++) {
        cubiecube->Ep[edge] = ePerm[edge];
        cubiecube->Eo[edge] = eOri[edge];
    }
}

void multiply(FCubieCube* cubiecube, FCubieCube* b)
{
    cornerMultiply(cubiecube, b);
    edgeMultiply(cubiecube, b);
}

void invCubieCube(FCubieCube* cubiecube, FCubieCube* c)
{
    int edge, corn;
    for (edge = 0; edge < EDGE_COUNT; edge++)
        c->Ep[static_cast<int32>(cubiecube->Ep[edge])] = (EEdgeType)edge;
    for (edge = 0; edge < EDGE_COUNT; edge++)
        c->Eo[edge] = cubiecube->Eo[static_cast<int32>(c->Ep[edge])];
    for (corn = 0; corn < CORNER_COUNT; corn++)
        c->Cp[static_cast<int32>(cubiecube->Cp[corn])] = (ECornerType)corn;
    for (corn = 0; corn < CORNER_COUNT; corn++) {
        signed char ori = cubiecube->Co[static_cast<int32>(c->Cp[corn])];
        if (ori >= 3)// Just for completeness. We do not invert mirrored
            // cubes in the program.
            c->Co[corn] = ori;
        else {// the standard case
            c->Co[corn] = -ori;
            if (c->Co[corn] < 0)
                c->Co[corn] += 3;
        }
    }
}

short getTwist(FCubieCube* cubiecube)
{
    short ret = 0;
    int i;
    for (i = static_cast<int32>(ECornerType::URF); i < static_cast<int32>(ECornerType::DRB); i++)
        ret = (short) (3 * ret + cubiecube->Co[i]);
    return ret;
}

void setTwist(FCubieCube* cubiecube, short twist)
{
    int twistParity = 0;
    int i;
    for (i = static_cast<int32>(ECornerType::DRB) - 1; i >= static_cast<int32>(ECornerType::URF); i--) {
        twistParity += cubiecube->Co[i] = (signed char) (twist % 3);
        twist /= 3;
    }
    cubiecube->Co[static_cast<int32>(ECornerType::DRB)] = (signed char) ((3 - twistParity % 3) % 3);
}

short getFlip(FCubieCube* cubiecube)
{
    int i;
    short ret = 0;
    for (i = static_cast<int32>(EEdgeType::UR); i < static_cast<int32>(EEdgeType::BR); i++)
        ret = (short) (2 * ret + cubiecube->Eo[i]);
    return ret;
}

void setFlip(FCubieCube* cubiecube, short flip)
{
    int i;
    int flipParity = 0;
    for (i = static_cast<int32>(EEdgeType::BR) - 1; i >= static_cast<int32>(EEdgeType::UR); i--) {
        flipParity += cubiecube->Eo[i] = (signed char) (flip % 2);
        flip /= 2;
    }
    cubiecube->Eo[static_cast<int32>(EEdgeType::BR)] = (signed char) ((2 - flipParity % 2) % 2);
}

short cornerParity(FCubieCube* cubiecube)
{
    int i, j;
    int s = 0;
    for (i = static_cast<int32>(ECornerType::DRB); i >= static_cast<int32>(ECornerType::URF) + 1; i--)
        for (j = i - 1; j >= static_cast<int32>(ECornerType::URF); j--)
            if (cubiecube->Cp[j] > cubiecube->Cp[i])
                s++;
    return (short) (s % 2);
}

short edgeParity(FCubieCube* cubiecube)
{
    int i, j;
    int s = 0;
    for (i = static_cast<int32>(EEdgeType::BR); i >= static_cast<int32>(EEdgeType::UR) + 1; i--)
        for (j = i - 1; j >= static_cast<int32>(EEdgeType::UR); j--)
            if (cubiecube->Ep[j] > cubiecube->Ep[i])
                s++;
    return (short) (s % 2);
}

short getFRtoBR(FCubieCube* cubiecube)
{
    int a = 0, x = 0, j;
    int b = 0;
    EEdgeType edge4[4] = {(EEdgeType)0};
    // compute the index a < (12 choose 4) and the permutation array perm.
    for (j = static_cast<int32>(EEdgeType::BR); j >= static_cast<int32>(EEdgeType::UR); j--)
        if (EEdgeType::FR <= cubiecube->Ep[j] && cubiecube->Ep[j] <= EEdgeType::BR) {
            a += Cnk(11 - j, x + 1);
            edge4[3 - x++] = cubiecube->Ep[j];
        }

    for (j = 3; j > 0; j--)// compute the index b < 4! for the
    // permutation in perm
    {
        int k = 0;
        while (static_cast<int32>(edge4[j]) != j + 8) {
            rotateLeft_edge(edge4, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return (short) (24 * a + b);
}
void setFRtoBR(FCubieCube* cubiecube, short idx)
{
    int x, j, k, e;
    EEdgeType sliceEdge[4] = { EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR };
    EEdgeType otherEdge[8] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB };
    int b = idx % 24; // Permutation
    int a = idx / 24; // Combination
    for (e = 0; e < EDGE_COUNT; e++)
        cubiecube->Ep[e] = EEdgeType::DB;// Use UR to invalidate all edges

    for (j = 1; j < 4; j++)// generate permutation from index b
    {
        k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            rotateRight_edge(sliceEdge, 0, j);
    }

    x = 3;// generate combination and set slice edges
    for (j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (a - Cnk(11 - j, x + 1) >= 0) {
            cubiecube->Ep[j] = sliceEdge[3 - x];
            a -= Cnk(11 - j, x-- + 1);
        }
    x = 0; // set the remaining edges UR..DB
    for (j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (cubiecube->Ep[j] == EEdgeType::DB)
            cubiecube->Ep[j] = otherEdge[x++];
}

short getURFtoDLF(FCubieCube* cubiecube)
{
    int a = 0, x = 0, j, b = 0;
    ECornerType corner6[6] = {(ECornerType)0};
    // compute the index a < (8 choose 6) and the corner permutation.
    for (j = static_cast<int32>(ECornerType::URF); j <= static_cast<int32>(ECornerType::DRB); j++)
        if (cubiecube->Cp[j] <= ECornerType::DLF) {
            a += Cnk(j, x + 1);
            corner6[x++] = cubiecube->Cp[j];
        }

    for (j = 5; j > 0; j--)// compute the index b < 6! for the
    // permutation in corner6
    {
        int k = 0;
        while (static_cast<int32>(corner6[j]) != j) {
            rotateLeft_corner(corner6, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return (short) (720 * a + b);
}

void setURFtoDLF(FCubieCube* cubiecube, short idx)
{
    int x;
    ECornerType corner6[6] = { ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::UBR, ECornerType::DFR, ECornerType::DLF };
    ECornerType otherCorner[2] = { ECornerType::DBL, ECornerType::DRB };
    int b = idx % 720; // Permutation
    int a = idx / 720; // Combination
    int c, j, k;
    for(c = 0; c < CORNER_COUNT; c++)
        cubiecube->Cp[c] = ECornerType::DRB;// Use DRB to invalidate all corners

    for (j = 1; j < 6; j++)// generate permutation from index b
    {
        k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            rotateRight_corner(corner6, 0, j);
    }
    x = 5;// generate combination and set corners
    for (j = static_cast<int32>(ECornerType::DRB); j >= 0; j--)
        if (a - Cnk(j, x + 1) >= 0) {
            cubiecube->Cp[j] = corner6[x];
            a -= Cnk(j, x-- + 1);
        }
    x = 0;
    for (j = static_cast<int32>(ECornerType::URF); j <= static_cast<int32>(ECornerType::DRB); j++)
        if (cubiecube->Cp[j] == ECornerType::DRB)
            cubiecube->Cp[j] = otherCorner[x++];
}

int getURtoDF(FCubieCube* cubiecube)
{
    int a = 0, x = 0;
    int b = 0, j;
    EEdgeType edge6[6] = {(EEdgeType)0};
    // compute the index a < (12 choose 6) and the edge permutation.
    for (j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (cubiecube->Ep[j] <= EEdgeType::DF) {
            a += Cnk(j, x + 1);
            edge6[x++] = cubiecube->Ep[j];
        }

    for (j = 5; j > 0; j--)// compute the index b < 6! for the
    // permutation in edge6
    {
        int k = 0;
        while (static_cast<int32>(edge6[j]) != j) {
            rotateLeft_edge(edge6, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return 720 * a + b;
}

void setURtoDF(FCubieCube* cubiecube, int idx)
{
    int x, e, j, k;
    EEdgeType edge6[6] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::DR, EEdgeType::DF };
    EEdgeType otherEdge[6] = { EEdgeType::DL, EEdgeType::DB, EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR };
    int b = idx % 720; // Permutation
    int a = idx / 720; // Combination

    for(e = 0; e < EDGE_COUNT; e++)
        cubiecube->Ep[e] = EEdgeType::BR;// Use BR to invalidate all edges

    for (j = 1; j < 6; j++)// generate permutation from index b
    {
        k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            rotateRight_edge(edge6, 0, j);
    }
    x = 5;// generate combination and set edges
    for (j = static_cast<int32>(EEdgeType::BR); j >= 0; j--)
        if (a - Cnk(j, x + 1) >= 0) {
            cubiecube->Ep[j] = edge6[x];
            a -= Cnk(j, x-- + 1);
        }
    x = 0; // set the remaining edges DL..BR
    for (j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (cubiecube->Ep[j] == EEdgeType::BR)
            cubiecube->Ep[j] = otherEdge[x++];
}

short getURtoUL(FCubieCube* cubiecube)
{
    int a = 0, b = 0, x = 0, j;
    EEdgeType edge3[3] = {(EEdgeType)0};
    // compute the index a < (12 choose 3) and the edge permutation.
    for (j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (cubiecube->Ep[j] <= EEdgeType::UL) {
            a += Cnk(j, x + 1);
            edge3[x++] = cubiecube->Ep[j];
        }

    for (j = 2; j > 0; j--)// compute the index b < 3! for the
    // permutation in edge3
    {
        int k = 0;
        while (static_cast<int32>(edge3[j]) != j) {
            rotateLeft_edge(edge3, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return (short) (6 * a + b);
}

void setURtoUL(FCubieCube* cubiecube, short idx)
{
    int x, e, j, k;
    EEdgeType edge3[3] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL };
    int b = idx % 6; // Permutation
    int a = idx / 6; // Combination
    for(e = 0; e < EDGE_COUNT; e++) {
        cubiecube->Ep[e] = EEdgeType::BR;// Use BR to invalidate all edges
    }

    for (j = 1; j < 3; j++) {// generate permutation from index b
        k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            rotateRight_edge(edge3, 0, j);
    }
    x = 2;// generate combination and set edges
    for (j = static_cast<int32>(EEdgeType::BR); j >= 0; j--) {
        if (a - Cnk(j, x + 1) >= 0) {
            cubiecube->Ep[j] = edge3[x];
            a -= Cnk(j, x-- + 1);
        }
    }
}

short getUBtoDF(FCubieCube* cubiecube)
{
    int a = 0, x = 0, b = 0, j;
    EEdgeType edge3[3] = {(EEdgeType)0};
    // compute the index a < (12 choose 3) and the edge permutation.
    for (j = static_cast<int32>(EEdgeType::UR); j <= static_cast<int32>(EEdgeType::BR); j++)
        if (EEdgeType::UB <= cubiecube->Ep[j] && cubiecube->Ep[j] <= EEdgeType::DF) {
            a += Cnk(j, x + 1);
            edge3[x++] = cubiecube->Ep[j];
        }

    for (j = 2; j > 0; j--)// compute the index b < 3! for the
    // permutation in edge3
    {
        int k = 0;
        while (static_cast<int32>(edge3[j]) != static_cast<int32>(EEdgeType::UB) + j) {
            rotateLeft_edge(edge3, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return (short) (6 * a + b);
}

void setUBtoDF(FCubieCube* cubiecube, short idx)
{
    int x, e, j, k;
    EEdgeType edge3[3] = { EEdgeType::UB, EEdgeType::DR, EEdgeType::DF };
    int b = idx % 6; // Permutation
    int a = idx / 6; // Combination
    for (e = 0; e < EDGE_COUNT; e++)
        cubiecube->Ep[e] = EEdgeType::BR;// Use BR to invalidate all edges

    for (j = 1; j < 3; j++)// generate permutation from index b
    {
        k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            rotateRight_edge(edge3, 0, j);
    }
    x = 2;// generate combination and set edges
    for (j = static_cast<int32>(EEdgeType::BR); j >= 0; j--)
        if (a - Cnk(j, x + 1) >= 0) {
            cubiecube->Ep[j] = edge3[x];
            a -= Cnk(j, x-- + 1);
        }
}

int getURFtoDLB(FCubieCube* cubiecube)
{
    ECornerType perm[8] = {(ECornerType)0};
    int b = 0, i, j;
    for (i = 0; i < 8; i++)
        perm[i] = cubiecube->Cp[i];
    for (j = 7; j > 0; j--)// compute the index b < 8! for the permutation in perm
    {
        int k = 0;
        while (static_cast<int32>(perm[j]) != j) {
            rotateLeft_corner(perm, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return b;
}

void setURFtoDLB(FCubieCube* cubiecube, int idx)
{
    ECornerType perm[8] = { ECornerType::URF, ECornerType::UFL, ECornerType::ULB, ECornerType::UBR, ECornerType::DFR, ECornerType::DLF, ECornerType::DBL, ECornerType::DRB };
    int k, j;
    int x = 7;// set corners
    for (j = 1; j < 8; j++) {
        k = idx % (j + 1);
        idx /= j + 1;
        while (k-- > 0)
            rotateRight_corner(perm, 0, j);
    }

    for (j = 7; j >= 0; j--)
        cubiecube->Cp[j] = perm[x--];
}

int getURtoBR(FCubieCube* cubiecube)
{
    EEdgeType perm[12] = {(EEdgeType)0};
    int b = 0, i, j;
    for (i = 0; i < 12; i++)
        perm[i] = cubiecube->Ep[i];
    for (j = 11; j > 0; j--)// compute the index b < 12! for the permutation in perm
    {
        int k = 0;
        while (static_cast<int32>(perm[j]) != j) {
            rotateLeft_edge(perm, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return b;
}

void setURtoBR(FCubieCube* cubiecube, int idx)
{
    EEdgeType perm[12] = { EEdgeType::UR, EEdgeType::UF, EEdgeType::UL, EEdgeType::UB, EEdgeType::DR, EEdgeType::DF, EEdgeType::DL, EEdgeType::DB, EEdgeType::FR, EEdgeType::FL, EEdgeType::BL, EEdgeType::BR };
    int k, j;
    int x = 11;// set edges
    for (j = 1; j < 12; j++) {
        k = idx % (j + 1);
        idx /= j + 1;
        while (k-- > 0)
            rotateRight_edge(perm, 0, j);
    }
    for (j = 11; j >= 0; j--)
        cubiecube->Ep[j] = perm[x--];
}

int Verify(FCubieCube* cubiecube)
{
    int sum = 0, e, i, c;
    int edgeCount[12] = {0};
    int cornerCount[8] = {0};

    for(e = 0; e < EDGE_COUNT; e++)
        edgeCount[static_cast<int32>(cubiecube->Ep[e])]++;
    for (i = 0; i < 12; i++)
        if (edgeCount[i] != 1)
            return -2;

    for (i = 0; i < 12; i++)
        sum += cubiecube->Eo[i];
    if (sum % 2 != 0)
        return -3;

    for(c = 0; c < CORNER_COUNT; c++)
        cornerCount[static_cast<int32>(cubiecube->Cp[c])]++;
    for (i = 0; i < 8; i++)
        if (cornerCount[i] != 1)
            return -4;// missing corners

    sum = 0;
    for (i = 0; i < 8; i++)
        sum += cubiecube->Co[i];
    if (sum % 3 != 0)
        return -5;// twisted corner

    if ((edgeParity(cubiecube) ^ cornerParity(cubiecube)) != 0)
        return -6;// parity error

    return 0;// cube ok
}

int getURtoDF_standalone(short idx1, short idx2)
{
    int res, i;
    FCubieCube *a = get_cubiecube();
    FCubieCube *b = get_cubiecube();
    setURtoUL(a, idx1);
    setUBtoDF(b, idx2);
    for (i = 0; i < 8; i++) {
        if (a->Ep[i] != EEdgeType::BR) {
            if (b->Ep[i] != EEdgeType::BR) {// collision
                return -1;
            } else {
                b->Ep[i] = a->Ep[i];
            }
        }
    }
    res = getURtoDF(b);
    free(a);
    free(b);
    return res;
}
