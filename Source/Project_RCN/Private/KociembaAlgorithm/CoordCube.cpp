#include "KociembaAlgorithm/CoordCube.h"

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

void Move(FCoordCube* CoordCube, int32 MoveIndex, const FString& CacheDir)
{
    if (PRUNING_INITED == 0)
    {
        InitPruning(CacheDir);
    }
    
    CoordCube->Twist = TwistMove[CoordCube->Twist][MoveIndex];
    CoordCube->Flip = FlipMove[CoordCube->Flip][MoveIndex];
    CoordCube->Parity = ParityMove[CoordCube->Parity][MoveIndex];
    CoordCube->FRtoBR = FRtoBR_Move[CoordCube->FRtoBR][MoveIndex];
    CoordCube->URFtoDLF = URFtoDLF_Move[CoordCube->URFtoDLF][MoveIndex];
    CoordCube->URtoUL = URtoUL_Move[CoordCube->URtoUL][MoveIndex];
    CoordCube->UBtoDF = UBtoDF_Move[CoordCube->UBtoDF][MoveIndex];
    
    if (CoordCube->URtoUL < 336 && CoordCube->UBtoDF < 336)
    {
        CoordCube->URtoDF = MergeURtoULandUBtoDF[CoordCube->URtoUL][CoordCube->UBtoDF];
    }
}

FCoordCube* GetCoordCube(FCubieCube* CubieCube)
{
    return new FCoordCube{
        GetTwist(CubieCube),
        GetFlip(CubieCube),
        CornerParity(CubieCube),
        GetFRtoBR(CubieCube),
        GetURFtoDLF(CubieCube),
        GetURtoUL(CubieCube),
        GetUBtoDF(CubieCube),
        GetURtoDF(CubieCube)
    };
}

void InitPruning(const FString& CacheDir)
{
    FCubieCube* CubieCube;
    FCubieCube* MoveCube = GetMoveCube();

    if (CheckCachedTable("twistMove", TwistMove, sizeof(TwistMove), CacheDir) != 0) {
        CubieCube = GetCubieCube();
        for (int32 i = 0; i < N_TWIST; i++) {
            SetTwist(CubieCube, i);
            for (int32 j = 0; j < 6; j++) {
                for (int32 k = 0; k < 3; k++) {
                    CornerMultiply(CubieCube, &MoveCube[j]);
                    TwistMove[i][3 * j + k] = GetTwist(CubieCube);
                }
                CornerMultiply(CubieCube, &MoveCube[j]);
            }
        }
        DumpToFile(TwistMove, sizeof(TwistMove), "twistMove", CacheDir);
    }

    if (CheckCachedTable("flipMove", FlipMove, sizeof(FlipMove), CacheDir) != 0) {
        CubieCube = GetCubieCube();
        for (int32 i = 0; i < N_FLIP; i++) {
            SetFlip(CubieCube, i);
            for (int32 j = 0; j < 6; j++) {
                for (int32 k = 0; k < 3; k++) {
                    EdgeMultiply(CubieCube, &MoveCube[j]);
                    FlipMove[i][3 * j + k] = GetFlip(CubieCube);
                }
                EdgeMultiply(CubieCube, &MoveCube[j]);
            }
        }
        DumpToFile(FlipMove, sizeof(FlipMove), "flipMove", CacheDir);
    }

    if (CheckCachedTable("FRtoBR_Move", FRtoBR_Move, sizeof(FRtoBR_Move), CacheDir) != 0) {
        CubieCube = GetCubieCube();
        for (int32 i = 0; i < N_FRtoBR; i++) {
            SetFRtoBR(CubieCube, i);
            for (int32 j = 0; j < 6; j++) {
                for (int32 k = 0; k < 3; k++) {
                    EdgeMultiply(CubieCube, &MoveCube[j]);
                    FRtoBR_Move[i][3 * j + k] = GetFRtoBR(CubieCube);
                }
                EdgeMultiply(CubieCube, &MoveCube[j]);
            }
        }
        DumpToFile(FRtoBR_Move, sizeof(FRtoBR_Move), "FRtoBR_Move", CacheDir);
    }

    if (CheckCachedTable("URFtoDLF_Move", URFtoDLF_Move, sizeof(URFtoDLF_Move), CacheDir) != 0) {
        CubieCube = GetCubieCube();
        for (int32 i = 0; i < N_URFtoDLF; i++) {
            SetURFtoDLF(CubieCube, i);
            for (int32 j = 0; j < 6; j++) {
                for (int32 k = 0; k < 3; k++) {
                    CornerMultiply(CubieCube, &MoveCube[j]);
                    URFtoDLF_Move[i][3 * j + k] = GetURFtoDLF(CubieCube);
                }
                CornerMultiply(CubieCube, &MoveCube[j]);
            }
        }
        DumpToFile(URFtoDLF_Move, sizeof(URFtoDLF_Move), "URFtoDLF_Move", CacheDir);
    }

    if (CheckCachedTable("URtoDF_Move", URtoDF_Move, sizeof(URtoDF_Move), CacheDir) != 0) {
        CubieCube = GetCubieCube();
        for (int32 i = 0; i < N_URtoDF; i++) {
            SetURtoDF(CubieCube, i);
            for (int32 j = 0; j < 6; j++) {
                for (int32 k = 0; k < 3; k++) {
                    EdgeMultiply(CubieCube, &MoveCube[j]);
                    URtoDF_Move[i][3 * j + k] = static_cast<short>(GetURtoDF(CubieCube));
                }
                EdgeMultiply(CubieCube, &MoveCube[j]);
            }
        }
        DumpToFile(URtoDF_Move, sizeof(URtoDF_Move), "URtoDF_Move", CacheDir);
    }

    if (CheckCachedTable("URtoUL_Move", URtoUL_Move, sizeof(URtoUL_Move), CacheDir) != 0) {
        CubieCube = GetCubieCube();
        for (int32 i = 0; i < N_URtoUL; i++) {
            SetURtoUL(CubieCube, i);
            for (int32 j = 0; j < 6; j++) {
                for (int32 k = 0; k < 3; k++) {
                    EdgeMultiply(CubieCube, &MoveCube[j]);
                    URtoUL_Move[i][3 * j + k] = GetURtoUL(CubieCube);
                }
                EdgeMultiply(CubieCube, &MoveCube[j]);
            }
        }
        DumpToFile(URtoUL_Move, sizeof(URtoUL_Move), "URtoUL_Move", CacheDir);
    }

    if (CheckCachedTable("UBtoDF_Move", UBtoDF_Move, sizeof(UBtoDF_Move), CacheDir) != 0) {
        CubieCube = GetCubieCube();
        for (int32 i = 0; i < N_UBtoDF; i++) {
            SetUBtoDF(CubieCube, i);
            for (int32 j = 0; j < 6; j++) {
                for (int32 k = 0; k < 3; k++) {
                    EdgeMultiply(CubieCube, &MoveCube[j]);
                    UBtoDF_Move[i][3 * j + k] = GetUBtoDF(CubieCube);
                }
                EdgeMultiply(CubieCube, &MoveCube[j]);
            }
        }
        DumpToFile(UBtoDF_Move, sizeof(UBtoDF_Move), "UBtoDF_Move", CacheDir);
    }

    if (CheckCachedTable("MergeURtoULandUBtoDF", MergeURtoULandUBtoDF, sizeof(MergeURtoULandUBtoDF), CacheDir) != 0)
    {
        short uRtoUL, uBtoDF;
        for (uRtoUL = 0; uRtoUL < 336; uRtoUL++) {
            for (uBtoDF = 0; uBtoDF < 336; uBtoDF++) {
                MergeURtoULandUBtoDF[uRtoUL][uBtoDF] = static_cast<short>(GetURtoDF_Standalone(uRtoUL, uBtoDF));
            }
        }
        DumpToFile(MergeURtoULandUBtoDF, sizeof(MergeURtoULandUBtoDF), "MergeURtoULandUBtoDF", CacheDir);
    }

    if (CheckCachedTable("Slice_URFtoDLF_Parity_Prun", Slice_URFtoDLF_Parity_Prun, sizeof(Slice_URFtoDLF_Parity_Prun), CacheDir) != 0) {
        int depth = 0, done = 1;
        for (int32 i = 0; i < N_SLICE2 * N_URFtoDLF * N_PARITY / 2; i++)
        {
            Slice_URFtoDLF_Parity_Prun[i] = -1;
        }
        SetPruning(Slice_URFtoDLF_Parity_Prun, 0, 0);
        while (done != N_SLICE2 * N_URFtoDLF * N_PARITY)
        {
            for (int32 i = 0; i < N_SLICE2 * N_URFtoDLF * N_PARITY; i++)
            {
                int parity = i % 2;
                int URFtoDLF = i / 2 / N_SLICE2;
                int slice = i / 2 % N_SLICE2;
                if (GetPruning(Slice_URFtoDLF_Parity_Prun, i) == depth)
                {
                    for (int32 j = 0; j < 18; j++)
                    {
                        int newSlice;
                        int newURFtoDLF;
                        int newParity;
                        switch (j)
                        {
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
                            if (GetPruning(Slice_URFtoDLF_Parity_Prun, (N_SLICE2 * newURFtoDLF + newSlice) * 2 + newParity) == 0x0f) {
                                SetPruning(Slice_URFtoDLF_Parity_Prun, (N_SLICE2 * newURFtoDLF + newSlice) * 2 + newParity,
                                        static_cast<signed char>(depth + 1));
                                done++;
                            }
                        }
                    }
                }
            }
            depth++;
        }
        DumpToFile(Slice_URFtoDLF_Parity_Prun, sizeof(Slice_URFtoDLF_Parity_Prun), "Slice_URFtoDLF_Parity_Prun", CacheDir);
    }

    if (CheckCachedTable("Slice_URtoDF_Parity_Prun", Slice_URtoDF_Parity_Prun, sizeof(Slice_URtoDF_Parity_Prun), CacheDir) != 0) {
        int depth = 0, done = 1;
        for (int32 i = 0; i < N_SLICE2 * N_URtoDF * N_PARITY / 2; i++)
        {
            Slice_URtoDF_Parity_Prun[i] = -1;
        }
        SetPruning(Slice_URtoDF_Parity_Prun, 0, 0);
        while (done != N_SLICE2 * N_URtoDF * N_PARITY)
        {
            for (int32 i = 0; i < N_SLICE2 * N_URtoDF * N_PARITY; i++)
            {
                int parity = i % 2;
                int URtoDF = i / 2 / N_SLICE2;
                int slice = i / 2 % N_SLICE2;
                if (GetPruning(Slice_URtoDF_Parity_Prun, i) == depth)
                {
                    for (int32 j = 0; j < 18; j++)
                    {
                        int newSlice;
                        int newURtoDF;
                        int newParity;
                        switch (j)
                        {
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
                            if (GetPruning(Slice_URtoDF_Parity_Prun, (N_SLICE2 * newURtoDF + newSlice) * 2 + newParity) == 0x0f) {
                                SetPruning(Slice_URtoDF_Parity_Prun, (N_SLICE2 * newURtoDF + newSlice) * 2 + newParity,
                                        static_cast<signed char>(depth + 1));
                                done++;
                            }
                        }
                    }
                }
            }
            depth++;
        }
        DumpToFile(Slice_URtoDF_Parity_Prun, sizeof(Slice_URtoDF_Parity_Prun), "Slice_URtoDF_Parity_Prun", CacheDir);
    }

    if (CheckCachedTable("Slice_Twist_Prun", Slice_Twist_Prun, sizeof(Slice_Twist_Prun), CacheDir) != 0) {
        int depth = 0, done = 1;
        for (int32 i = 0; i < N_SLICE1 * N_TWIST / 2 + 1; i++)
        {
            Slice_Twist_Prun[i] = -1;
        }
        SetPruning(Slice_Twist_Prun, 0, 0);
        while (done != N_SLICE1 * N_TWIST) {
            for (int32 i = 0; i < N_SLICE1 * N_TWIST; i++) {
                int twist = i / N_SLICE1, slice = i % N_SLICE1;
                if (GetPruning(Slice_Twist_Prun, i) == depth) {
                    for (int32 j = 0; j < 18; j++) {
                        int newSlice = FRtoBR_Move[slice * 24][j] / 24;
                        int newTwist = TwistMove[twist][j];
                        if (GetPruning(Slice_Twist_Prun, N_SLICE1 * newTwist + newSlice) == 0x0f) {
                            SetPruning(Slice_Twist_Prun, N_SLICE1 * newTwist + newSlice, static_cast<signed char>(depth + 1));
                            done++;
                        }
                    }
                }
            }
            depth++;
        }
        DumpToFile(Slice_Twist_Prun, sizeof(Slice_Twist_Prun), "Slice_Twist_Prun", CacheDir);
    }

    if (CheckCachedTable("Slice_Flip_Prun", Slice_Flip_Prun, sizeof(Slice_Flip_Prun), CacheDir) != 0) {
        int depth = 0, done = 1;
        for (int32 i = 0; i < N_SLICE1 * N_FLIP / 2; i++)
        {
            Slice_Flip_Prun[i] = -1;
        }
        SetPruning(Slice_Flip_Prun, 0, 0);
        while (done != N_SLICE1 * N_FLIP) {
            for (int32 i = 0; i < N_SLICE1 * N_FLIP; i++) {
                int flip = i / N_SLICE1, slice = i % N_SLICE1;
                if (GetPruning(Slice_Flip_Prun, i) == depth) {
                    for (int32 j = 0; j < 18; j++)
                    {
                        int newSlice = FRtoBR_Move[slice * 24][j] / 24;
                        int newFlip = FlipMove[flip][j];
                        if (GetPruning(Slice_Flip_Prun, N_SLICE1 * newFlip + newSlice) == 0x0f) {
                            SetPruning(Slice_Flip_Prun, N_SLICE1 * newFlip + newSlice, static_cast<signed char>(depth + 1));
                            done++;
                        }
                    }
                }
            }
            depth++;
        }
        DumpToFile(Slice_Flip_Prun, sizeof(Slice_Flip_Prun), "Slice_Flip_Prun", CacheDir);
    }

    PRUNING_INITED = 1;
}

void SetPruning(signed char *table, int index, signed char value)
{
    if ((index & 1) == 0)
        table[index / 2] &= 0xf0 | value;
    else
        table[index / 2] &= 0x0f | value << 4;
}

// Extract pruning value
signed char GetPruning(const signed char *table, int index)
{
    signed char res;

    if ((index & 1) == 0)
        res = table[index / 2] & 0x0f;
    else
        res = table[index / 2] >> 4 & 0x0f;

    return res;
}

bool CheckCachedTable(const FString& Name, void* Ptr, int32 Len, const FString& CacheDir)
{
    const FString FilePath = FPaths::Combine(CacheDir, Name);

    if (FPaths::FileExists(FilePath))
    {
        TArray<uint8> FileData;
        if (FFileHelper::LoadFileToArray(FileData, *FilePath))
        {
            FMemory::Memcpy(Ptr, FileData.GetData(), FMath::Min(FileData.Num(), Len));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to read file: %s"), *FilePath);
        }
        return true;
    }
	
    UE_LOG(LogTemp, Warning, TEXT("Cache table %s was not found. Recalculating."), *FilePath);
    return false;
}

void DumpToFile(void* Ptr, int32 Len, const FString& Name, const FString& CacheDir)
{
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (PlatformFile.CreateDirectoryTree(*CacheDir))
    {
        const FString FilePath = FPaths::Combine(CacheDir, Name);
        TArray<uint8> FileData;
        FileData.Append(static_cast<uint8*>(Ptr), Len);

        if (FFileHelper::SaveArrayToFile(FileData, *FilePath))
        {
            UE_LOG(LogTemp, Log, TEXT("Successfully wrote cache table: %s"), *FilePath);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to write cache table: %s"), *FilePath);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot create cache tables directory: %s"), *CacheDir);
    }
}
