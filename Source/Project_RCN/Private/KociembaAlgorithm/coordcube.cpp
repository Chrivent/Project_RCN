#include "KociembaAlgorithm/coordcube.h"
#include "KociembaAlgorithm/prunetable_helpers.h"
#include "KociembaAlgorithm/cubiecube.h"

short TwistMove[N_TWIST][N_MOVE];
short FlipMove[N_FLIP][N_MOVE];
short ParityMove[2][18] = {
    { 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1 },
    { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 }
};
short FRtoBR_Move[N_FRtoBR][N_MOVE];
short URFtoDLF_Move[N_URFtoDLF][N_MOVE] = {{0}};
short URtoDF_Move[N_URtoDF][N_MOVE] = {{0}};
short URtoUL_Move[N_URtoUL][N_MOVE] = {{0}};
short UBtoDF_Move[N_UBtoDF][N_MOVE] = {{0}};
short MergeURtoULandUBtoDF[336][336] = {{0}};
signed char Slice_URFtoDLF_Parity_Prun[N_SLICE2 * N_URFtoDLF * N_PARITY / 2] = {0};
signed char Slice_URtoDF_Parity_Prun[N_SLICE2 * N_URtoDF * N_PARITY / 2] = {0};
signed char Slice_Twist_Prun[N_SLICE1 * N_TWIST / 2 + 1] = {0};
signed char Slice_Flip_Prun[N_SLICE1 * N_FLIP / 2] = {0};

int PRUNING_INITED = 0;

void move(FCoordCube* CoordCube, int m, const char *cache_dir)
{
    if (PRUNING_INITED == 0) {
        initPruning(cache_dir);
    }
    CoordCube->Twist = TwistMove[CoordCube->Twist][m];
    CoordCube->Flip = FlipMove[CoordCube->Flip][m];
    CoordCube->Parity = ParityMove[CoordCube->Parity][m];
    CoordCube->FRtoBR = FRtoBR_Move[CoordCube->FRtoBR][m];
    CoordCube->URFtoDLF = URFtoDLF_Move[CoordCube->URFtoDLF][m];
    CoordCube->URtoUL = URtoUL_Move[CoordCube->URtoUL][m];
    CoordCube->UBtoDF = UBtoDF_Move[CoordCube->UBtoDF][m];
    if (CoordCube->URtoUL < 336 && CoordCube->UBtoDF < 336)// updated only if UR,UF,UL,UB,DR,DF
        // are not in UD-slice
        CoordCube->URtoDF = MergeURtoULandUBtoDF[CoordCube->URtoUL][CoordCube->UBtoDF];
}

FCoordCube* get_coordcube(FCubieCube* CubieCube)
{
    FCoordCube* result = new FCoordCube();

    result->Twist       = getTwist(CubieCube);
    result->Flip        = getFlip(CubieCube);
    result->Parity      = cornerParity(CubieCube);
    result->FRtoBR      = getFRtoBR(CubieCube);
    result->URFtoDLF    = getURFtoDLF(CubieCube);
    result->URtoUL      = getURtoUL(CubieCube);
    result->UBtoDF      = getUBtoDF(CubieCube);
    result->URtoDF      = getURtoDF(CubieCube);// only needed in phase2

    return result;
}

void initPruning(const char *cache_dir)
{
    FCubieCube* a;
    FCubieCube* moveCube = get_moveCube();

    if(FPruneTableHelpers::CheckCachedTable("twistMove", TwistMove, sizeof(TwistMove), cache_dir) != 0) {
        short i;
        int k, j;
        a = get_cubiecube();
        for (i = 0; i < N_TWIST; i++) {
            setTwist(a, i);
            for (j = 0; j < 6; j++) {
                for (k = 0; k < 3; k++) {
                    cornerMultiply(a, &moveCube[j]);
                    TwistMove[i][3 * j + k] = getTwist(a);
                }
                cornerMultiply(a, &moveCube[j]);// 4. faceturn restores
            }
        }
        //free(a);
        FPruneTableHelpers::DumpToFile(TwistMove, sizeof(TwistMove), "twistMove", cache_dir);
    }

    if(FPruneTableHelpers::CheckCachedTable("flipMove", FlipMove, sizeof(FlipMove), cache_dir) != 0) {
        short i;
        int k, j;
        a = get_cubiecube();
        for (i = 0; i < N_FLIP; i++) {
            setFlip(a, i);
            for (j = 0; j < 6; j++) {
                for (k = 0; k < 3; k++) {
                    edgeMultiply(a, &moveCube[j]);
                    FlipMove[i][3 * j + k] = getFlip(a);
                }
                edgeMultiply(a, &moveCube[j]);
            }
        }
        //free(a);
        FPruneTableHelpers::DumpToFile(FlipMove, sizeof(FlipMove), "flipMove", cache_dir);
    }

    if(FPruneTableHelpers::CheckCachedTable("FRtoBR_Move", FRtoBR_Move, sizeof(FRtoBR_Move), cache_dir) != 0) {
        short i;
        int k, j;
        a = get_cubiecube();
        for (i = 0; i < N_FRtoBR; i++) {
            setFRtoBR(a, i);
            for (j = 0; j < 6; j++) {
                for (k = 0; k < 3; k++) {
                    edgeMultiply(a, &moveCube[j]);
                    FRtoBR_Move[i][3 * j + k] = getFRtoBR(a);
                }
                edgeMultiply(a, &moveCube[j]);
            }
        }
        //free(a);
        FPruneTableHelpers::DumpToFile(FRtoBR_Move, sizeof(FRtoBR_Move), "FRtoBR_Move", cache_dir);
    }

    if(FPruneTableHelpers::CheckCachedTable("URFtoDLF_Move", URFtoDLF_Move, sizeof(URFtoDLF_Move), cache_dir) != 0) {
        short i;
        int k, j;
        a = get_cubiecube();
        for (i = 0; i < N_URFtoDLF; i++) {
            setURFtoDLF(a, i);
            for (j = 0; j < 6; j++) {
                for (k = 0; k < 3; k++) {
                    cornerMultiply(a, &moveCube[j]);
                    URFtoDLF_Move[i][3 * j + k] = getURFtoDLF(a);
                }
                cornerMultiply(a, &moveCube[j]);
            }
        }
        //free(a);
        FPruneTableHelpers::DumpToFile(URFtoDLF_Move, sizeof(URFtoDLF_Move), "URFtoDLF_Move", cache_dir);
    }

    if(FPruneTableHelpers::CheckCachedTable("URtoDF_Move", URtoDF_Move, sizeof(URtoDF_Move), cache_dir) != 0) {
        short i;
        int k, j;
        a = get_cubiecube();
        for (i = 0; i < N_URtoDF; i++) {
            setURtoDF(a, i);
            for (j = 0; j < 6; j++) {
                for (k = 0; k < 3; k++) {
                    edgeMultiply(a, &moveCube[j]);
                    URtoDF_Move[i][3 * j + k] = static_cast<short>(getURtoDF(a));
                }
                edgeMultiply(a, &moveCube[j]);
            }
        }
        //free(a);
        FPruneTableHelpers::DumpToFile(URtoDF_Move, sizeof(URtoDF_Move), "URtoDF_Move", cache_dir);
    }

    if(FPruneTableHelpers::CheckCachedTable("URtoUL_Move", URtoUL_Move, sizeof(URtoUL_Move), cache_dir) != 0) {
        short i;
        int k, j;
        a = get_cubiecube();
        for (i = 0; i < N_URtoUL; i++) {
            setURtoUL(a, i);
            for (j = 0; j < 6; j++) {
                for (k = 0; k < 3; k++) {
                    edgeMultiply(a, &moveCube[j]);
                    URtoUL_Move[i][3 * j + k] = getURtoUL(a);
                }
                edgeMultiply(a, &moveCube[j]);
            }
        }
        //free(a);
        FPruneTableHelpers::DumpToFile(URtoUL_Move, sizeof(URtoUL_Move), "URtoUL_Move", cache_dir);
    }

    if(FPruneTableHelpers::CheckCachedTable("UBtoDF_Move", UBtoDF_Move, sizeof(UBtoDF_Move), cache_dir) != 0) {
        short i;
        int k, j;
        a = get_cubiecube();
        for (i = 0; i < N_UBtoDF; i++) {
            setUBtoDF(a, i);
            for (j = 0; j < 6; j++) {
                for (k = 0; k < 3; k++) {
                    edgeMultiply(a, &moveCube[j]);
                    UBtoDF_Move[i][3 * j + k] = getUBtoDF(a);
                }
                edgeMultiply(a, &moveCube[j]);
            }
        }
        //free(a);
        FPruneTableHelpers::DumpToFile(UBtoDF_Move, sizeof(UBtoDF_Move), "UBtoDF_Move", cache_dir);
    }

    if(FPruneTableHelpers::CheckCachedTable("MergeURtoULandUBtoDF", MergeURtoULandUBtoDF, sizeof(MergeURtoULandUBtoDF), cache_dir) != 0) {
        // for i, j <336 the six edges UR,UF,UL,UB,DR,DF are not in the
        // UD-slice and the index is <20160
        short uRtoUL, uBtoDF;
        for (uRtoUL = 0; uRtoUL < 336; uRtoUL++) {
            for (uBtoDF = 0; uBtoDF < 336; uBtoDF++) {
                MergeURtoULandUBtoDF[uRtoUL][uBtoDF] = static_cast<short>(getURtoDF_standalone(uRtoUL, uBtoDF));
            }
        }
        FPruneTableHelpers::DumpToFile(MergeURtoULandUBtoDF, sizeof(MergeURtoULandUBtoDF), "MergeURtoULandUBtoDF", cache_dir);
    }

    if(FPruneTableHelpers::CheckCachedTable("Slice_URFtoDLF_Parity_Prun", Slice_URFtoDLF_Parity_Prun, sizeof(Slice_URFtoDLF_Parity_Prun), cache_dir) != 0) {
        int depth = 0, done = 1;
        int i, j;
        for (i = 0; i < N_SLICE2 * N_URFtoDLF * N_PARITY / 2; i++)
            Slice_URFtoDLF_Parity_Prun[i] = -1;
        setPruning(Slice_URFtoDLF_Parity_Prun, 0, 0);
        printf("1\n");
        while (done != N_SLICE2 * N_URFtoDLF * N_PARITY) {
            // printf("%d %d %d\n", done, N_SLICE2 * N_URFtoDLF * N_PARITY, depth);
            for (i = 0; i < N_SLICE2 * N_URFtoDLF * N_PARITY; i++) {
                int parity = i % 2;
                int URFtoDLF = (i / 2) / N_SLICE2;
                int slice = (i / 2) % N_SLICE2;
                if (getPruning(Slice_URFtoDLF_Parity_Prun, i) == depth) {
                    for (j = 0; j < 18; j++) {
                        int newSlice;
                        int newURFtoDLF;
                        int newParity;
                        switch (j) {
                        case 3:
                        case 5:
                        case 6:
                        case 8:
                        case 12:
                        case 14:
                        case 15:
                        case 17:
                            continue;
                        default:
                            newSlice = FRtoBR_Move[slice][j];
                            newURFtoDLF = URFtoDLF_Move[URFtoDLF][j];
                            newParity = ParityMove[parity][j];
                            if (getPruning(Slice_URFtoDLF_Parity_Prun, (N_SLICE2 * newURFtoDLF + newSlice) * 2 + newParity) == 0x0f) {
                                setPruning(Slice_URFtoDLF_Parity_Prun, (N_SLICE2 * newURFtoDLF + newSlice) * 2 + newParity,
                                        static_cast<signed char>(depth + 1));
                                done++;
                            }
                        }
                    }
                }
            }
            depth++;
        }
        printf("2\n");
        FPruneTableHelpers::DumpToFile(Slice_URFtoDLF_Parity_Prun, sizeof(Slice_URFtoDLF_Parity_Prun), "Slice_URFtoDLF_Parity_Prun", cache_dir);
        printf("3\n");
    }

    if(FPruneTableHelpers::CheckCachedTable("Slice_URtoDF_Parity_Prun", Slice_URtoDF_Parity_Prun, sizeof(Slice_URtoDF_Parity_Prun), cache_dir) != 0) {
        int depth = 0, done = 1;
        int i, j;
        for (i = 0; i < N_SLICE2 * N_URtoDF * N_PARITY / 2; i++)
            Slice_URtoDF_Parity_Prun[i] = -1;
        setPruning(Slice_URtoDF_Parity_Prun, 0, 0);
        while (done != N_SLICE2 * N_URtoDF * N_PARITY) {
            for (i = 0; i < N_SLICE2 * N_URtoDF * N_PARITY; i++) {
                int parity = i % 2;
                int URtoDF = i / 2 / N_SLICE2;
                int slice = i / 2 % N_SLICE2;
                if (getPruning(Slice_URtoDF_Parity_Prun, i) == depth) {
                    for (j = 0; j < 18; j++) {
                        int newSlice;
                        int newURtoDF;
                        int newParity;
                        switch (j) {
                        case 3:
                        case 5:
                        case 6:
                        case 8:
                        case 12:
                        case 14:
                        case 15:
                        case 17:
                            continue;
                        default:
                            newSlice = FRtoBR_Move[slice][j];
                            newURtoDF = URtoDF_Move[URtoDF][j];
                            newParity = ParityMove[parity][j];
                            if (getPruning(Slice_URtoDF_Parity_Prun, (N_SLICE2 * newURtoDF + newSlice) * 2 + newParity) == 0x0f) {
                                setPruning(Slice_URtoDF_Parity_Prun, (N_SLICE2 * newURtoDF + newSlice) * 2 + newParity,
                                        static_cast<signed char>(depth + 1));
                                done++;
                            }
                        }
                    }
                }
            }
            depth++;
        }
        FPruneTableHelpers::DumpToFile(Slice_URtoDF_Parity_Prun, sizeof(Slice_URtoDF_Parity_Prun), "Slice_URtoDF_Parity_Prun", cache_dir);
    }

    if(FPruneTableHelpers::CheckCachedTable("Slice_Twist_Prun", Slice_Twist_Prun, sizeof(Slice_Twist_Prun), cache_dir) != 0) {
        int depth = 0, done = 1;
        int i, j;
        for (i = 0; i < N_SLICE1 * N_TWIST / 2 + 1; i++)
            Slice_Twist_Prun[i] = -1;
        setPruning(Slice_Twist_Prun, 0, 0);
        while (done != N_SLICE1 * N_TWIST) {
            for (i = 0; i < N_SLICE1 * N_TWIST; i++) {
                int twist = i / N_SLICE1, slice = i % N_SLICE1;
                if (getPruning(Slice_Twist_Prun, i) == depth) {
                    for (j = 0; j < 18; j++) {
                        int newSlice = FRtoBR_Move[slice * 24][j] / 24;
                        int newTwist = TwistMove[twist][j];
                        if (getPruning(Slice_Twist_Prun, N_SLICE1 * newTwist + newSlice) == 0x0f) {
                            setPruning(Slice_Twist_Prun, N_SLICE1 * newTwist + newSlice, static_cast<signed char>(depth + 1));
                            done++;
                        }
                    }
                }
            }
            depth++;
        }
        FPruneTableHelpers::DumpToFile(Slice_Twist_Prun, sizeof(Slice_Twist_Prun), "Slice_Twist_Prun", cache_dir);
    }

    if(FPruneTableHelpers::CheckCachedTable("Slice_Flip_Prun", Slice_Flip_Prun, sizeof(Slice_Flip_Prun), cache_dir) != 0) {
        int depth = 0, done = 1;
        int i, j;
        for (i = 0; i < N_SLICE1 * N_FLIP / 2; i++)
            Slice_Flip_Prun[i] = -1;
        setPruning(Slice_Flip_Prun, 0, 0);
        while (done != N_SLICE1 * N_FLIP) {
            for (i = 0; i < N_SLICE1 * N_FLIP; i++) {
                int flip = i / N_SLICE1, slice = i % N_SLICE1;
                if (getPruning(Slice_Flip_Prun, i) == depth) {
                    for (j = 0; j < 18; j++) {
                        int newSlice = FRtoBR_Move[slice * 24][j] / 24;
                        int newFlip = FlipMove[flip][j];
                        if (getPruning(Slice_Flip_Prun, N_SLICE1 * newFlip + newSlice) == 0x0f) {
                            setPruning(Slice_Flip_Prun, N_SLICE1 * newFlip + newSlice, static_cast<signed char>(depth + 1));
                            done++;
                        }
                    }
                }
            }
            depth++;
        }
        FPruneTableHelpers::DumpToFile(Slice_Flip_Prun, sizeof(Slice_Flip_Prun), "Slice_Flip_Prun", cache_dir);
    }

    PRUNING_INITED = 1;
}

void setPruning(signed char *table, int index, signed char value) {
    if ((index & 1) == 0)
        table[index / 2] &= 0xf0 | value;
    else
        table[index / 2] &= 0x0f | (value << 4);
}

// Extract pruning value
signed char getPruning(const signed char *table, int index) {
    signed char res;

    if ((index & 1) == 0)
        res = table[index / 2] & 0x0f;
    else
        res = table[index / 2] >> 4 & 0x0f;

    return res;
}
