#include "KociembaAlgorithm/CoordCube.h"

int16 TwistMove[N_TWIST][N_MOVE];
int16 FlipMove[N_FLIP][N_MOVE];
int16 ParityMove[2][18] = {
    { 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1 },
    { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 }
};
int16 FRtoBR_Move[N_FRtoBR][N_MOVE];
int16 URFtoDLF_Move[N_URFtoDLF][N_MOVE] = {{0}};
int16 URtoDF_Move[N_URtoDF][N_MOVE] = {{0}};
int16 URtoUL_Move[N_URtoUL][N_MOVE] = {{0}};
int16 UBtoDF_Move[N_UBtoDF][N_MOVE] = {{0}};
int16 MergeURtoULandUBtoDF[336][336] = {{0}};
int8 Slice_URFtoDLF_Parity_Prun[N_SLICE2 * N_URFtoDLF * N_PARITY / 2] = {0};
int8 Slice_URtoDF_Parity_Prun[N_SLICE2 * N_URtoDF * N_PARITY / 2] = {0};
int8 Slice_Twist_Prun[N_SLICE1 * N_TWIST / 2 + 1] = {0};
int8 Slice_Flip_Prun[N_SLICE1 * N_FLIP / 2] = {0};

int32 PRUNING_INITED = 0;

FCoordCube::FCoordCube(FCubieCube& CubieCube)
{
    Twist = CubieCube.GetTwist();
    Flip = CubieCube.GetFlip();
    Parity = CubieCube.CornerParity();
    FRtoBR = CubieCube.GetFRtoBR();
    URFtoDLF = CubieCube.GetURFtoDLF();
    URtoUL = CubieCube.GetURtoUL();
    UBtoDF = CubieCube.GetUBtoDF();
    URtoDF = CubieCube.GetURtoDF();
}

void InitPruning(const FString& CacheDir)
{
    FCubieCube CubieCube;

    if (CheckCachedTable("twistMove", TwistMove, sizeof(TwistMove), CacheDir) != 0) {
        for (int32 i = 0; i < N_TWIST; i++) {
            CubieCube.SetTwist(i);
            for (int32 j = 0; j < 6; j++) {
                for (int32 k = 0; k < 3; k++) {
                    CubieCube.CornerMultiply(j);
                    TwistMove[i][3 * j + k] = CubieCube.GetTwist();
                }
                CubieCube.CornerMultiply(j);
            }
        }
        DumpToFile(TwistMove, sizeof(TwistMove), "twistMove", CacheDir);
    }

    if (CheckCachedTable("flipMove", FlipMove, sizeof(FlipMove), CacheDir) != 0) {
        for (int32 i = 0; i < N_FLIP; i++) {
            CubieCube.SetFlip(i);
            for (int32 j = 0; j < 6; j++) {
                for (int32 k = 0; k < 3; k++) {
                    CubieCube.EdgeMultiply(j);
                    FlipMove[i][3 * j + k] = CubieCube.GetFlip();
                }
                CubieCube.EdgeMultiply(j);
            }
        }
        DumpToFile(FlipMove, sizeof(FlipMove), "flipMove", CacheDir);
    }

    if (CheckCachedTable("FRtoBR_Move", FRtoBR_Move, sizeof(FRtoBR_Move), CacheDir) != 0) {
        for (int32 i = 0; i < N_FRtoBR; i++) {
            CubieCube.SetFRtoBR(i);
            for (int32 j = 0; j < 6; j++) {
                for (int32 k = 0; k < 3; k++) {
                    CubieCube.EdgeMultiply(j);
                    FRtoBR_Move[i][3 * j + k] = CubieCube.GetFRtoBR();
                }
                CubieCube.EdgeMultiply(j);
            }
        }
        DumpToFile(FRtoBR_Move, sizeof(FRtoBR_Move), "FRtoBR_Move", CacheDir);
    }

    if (CheckCachedTable("URFtoDLF_Move", URFtoDLF_Move, sizeof(URFtoDLF_Move), CacheDir) != 0) {
        for (int32 i = 0; i < N_URFtoDLF; i++) {
            CubieCube.SetURFtoDLF(i);
            for (int32 j = 0; j < 6; j++) {
                for (int32 k = 0; k < 3; k++) {
                    CubieCube.CornerMultiply(j);
                    URFtoDLF_Move[i][3 * j + k] = CubieCube.GetURFtoDLF();
                }
                CubieCube.CornerMultiply(j);
            }
        }
        DumpToFile(URFtoDLF_Move, sizeof(URFtoDLF_Move), "URFtoDLF_Move", CacheDir);
    }

    if (CheckCachedTable("URtoDF_Move", URtoDF_Move, sizeof(URtoDF_Move), CacheDir) != 0) {
        for (int32 i = 0; i < N_URtoDF; i++) {
            CubieCube.SetURtoDF(i);
            for (int32 j = 0; j < 6; j++) {
                for (int32 k = 0; k < 3; k++) {
                    CubieCube.EdgeMultiply(j);
                    URtoDF_Move[i][3 * j + k] = CubieCube.GetURtoDF();
                }
                CubieCube.EdgeMultiply(j);
            }
        }
        DumpToFile(URtoDF_Move, sizeof(URtoDF_Move), "URtoDF_Move", CacheDir);
    }

    if (CheckCachedTable("URtoUL_Move", URtoUL_Move, sizeof(URtoUL_Move), CacheDir) != 0) {
        for (int32 i = 0; i < N_URtoUL; i++) {
            CubieCube.SetURtoUL(i);
            for (int32 j = 0; j < 6; j++) {
                for (int32 k = 0; k < 3; k++) {
                    CubieCube.EdgeMultiply(j);
                    URtoUL_Move[i][3 * j + k] = CubieCube.GetURtoUL();
                }
                CubieCube.EdgeMultiply(j);
            }
        }
        DumpToFile(URtoUL_Move, sizeof(URtoUL_Move), "URtoUL_Move", CacheDir);
    }

    if (CheckCachedTable("UBtoDF_Move", UBtoDF_Move, sizeof(UBtoDF_Move), CacheDir) != 0) {
        for (int32 i = 0; i < N_UBtoDF; i++) {
            CubieCube.SetUBtoDF(i);
            for (int32 j = 0; j < 6; j++) {
                for (int32 k = 0; k < 3; k++) {
                    CubieCube.EdgeMultiply(j);
                    UBtoDF_Move[i][3 * j + k] = CubieCube.GetUBtoDF();
                }
                CubieCube.EdgeMultiply(j);
            }
        }
        DumpToFile(UBtoDF_Move, sizeof(UBtoDF_Move), "UBtoDF_Move", CacheDir);
    }

    if (CheckCachedTable("MergeURtoULandUBtoDF", MergeURtoULandUBtoDF, sizeof(MergeURtoULandUBtoDF), CacheDir) != 0)
    {
        int16 uRtoUL, uBtoDF;
        for (uRtoUL = 0; uRtoUL < 336; uRtoUL++) {
            for (uBtoDF = 0; uBtoDF < 336; uBtoDF++) {
                MergeURtoULandUBtoDF[uRtoUL][uBtoDF] = FCubieCube::GetURtoDF_Standalone(uRtoUL, uBtoDF);
            }
        }
        DumpToFile(MergeURtoULandUBtoDF, sizeof(MergeURtoULandUBtoDF), "MergeURtoULandUBtoDF", CacheDir);
    }

    if (CheckCachedTable("Slice_URFtoDLF_Parity_Prun", Slice_URFtoDLF_Parity_Prun, sizeof(Slice_URFtoDLF_Parity_Prun), CacheDir) != 0) {
        int32 depth = 0, done = 1;
        for (int32 i = 0; i < N_SLICE2 * N_URFtoDLF * N_PARITY / 2; i++)
        {
            Slice_URFtoDLF_Parity_Prun[i] = -1;
        }
        SetPruning(Slice_URFtoDLF_Parity_Prun, 0, 0);
        while (done != N_SLICE2 * N_URFtoDLF * N_PARITY)
        {
            for (int32 i = 0; i < N_SLICE2 * N_URFtoDLF * N_PARITY; i++)
            {
                int32 parity = i % 2;
                int32 URFtoDLF = i / 2 / N_SLICE2;
                int32 slice = i / 2 % N_SLICE2;
                if (GetPruning(Slice_URFtoDLF_Parity_Prun, i) == depth)
                {
                    for (int32 j = 0; j < 18; j++)
                    {
                        int32 newSlice;
                        int32 newURFtoDLF;
                        int32 newParity;
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
                                SetPruning(Slice_URFtoDLF_Parity_Prun, (N_SLICE2 * newURFtoDLF + newSlice) * 2 + newParity, depth + 1);
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
        int32 depth = 0, done = 1;
        for (int32 i = 0; i < N_SLICE2 * N_URtoDF * N_PARITY / 2; i++)
        {
            Slice_URtoDF_Parity_Prun[i] = -1;
        }
        SetPruning(Slice_URtoDF_Parity_Prun, 0, 0);
        while (done != N_SLICE2 * N_URtoDF * N_PARITY)
        {
            for (int32 i = 0; i < N_SLICE2 * N_URtoDF * N_PARITY; i++)
            {
                int32 parity = i % 2;
                int32 URtoDF = i / 2 / N_SLICE2;
                int32 slice = i / 2 % N_SLICE2;
                if (GetPruning(Slice_URtoDF_Parity_Prun, i) == depth)
                {
                    for (int32 j = 0; j < 18; j++)
                    {
                        int32 newSlice;
                        int32 newURtoDF;
                        int32 newParity;
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
                                SetPruning(Slice_URtoDF_Parity_Prun, (N_SLICE2 * newURtoDF + newSlice) * 2 + newParity, depth + 1);
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
        int32 depth = 0, done = 1;
        for (int32 i = 0; i < N_SLICE1 * N_TWIST / 2 + 1; i++)
        {
            Slice_Twist_Prun[i] = -1;
        }
        SetPruning(Slice_Twist_Prun, 0, 0);
        while (done != N_SLICE1 * N_TWIST) {
            for (int32 i = 0; i < N_SLICE1 * N_TWIST; i++) {
                int32 twist = i / N_SLICE1, slice = i % N_SLICE1;
                if (GetPruning(Slice_Twist_Prun, i) == depth) {
                    for (int32 j = 0; j < 18; j++) {
                        int32 newSlice = FRtoBR_Move[slice * 24][j] / 24;
                        int32 newTwist = TwistMove[twist][j];
                        if (GetPruning(Slice_Twist_Prun, N_SLICE1 * newTwist + newSlice) == 0x0f) {
                            SetPruning(Slice_Twist_Prun, N_SLICE1 * newTwist + newSlice, depth + 1);
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
        int32 depth = 0, done = 1;
        for (int32 i = 0; i < N_SLICE1 * N_FLIP / 2; i++)
        {
            Slice_Flip_Prun[i] = -1;
        }
        SetPruning(Slice_Flip_Prun, 0, 0);
        while (done != N_SLICE1 * N_FLIP) {
            for (int32 i = 0; i < N_SLICE1 * N_FLIP; i++) {
                int32 flip = i / N_SLICE1, slice = i % N_SLICE1;
                if (GetPruning(Slice_Flip_Prun, i) == depth) {
                    for (int32 j = 0; j < 18; j++)
                    {
                        int32 newSlice = FRtoBR_Move[slice * 24][j] / 24;
                        int32 newFlip = FlipMove[flip][j];
                        if (GetPruning(Slice_Flip_Prun, N_SLICE1 * newFlip + newSlice) == 0x0f) {
                            SetPruning(Slice_Flip_Prun, N_SLICE1 * newFlip + newSlice, depth + 1);
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

void SetPruning(int8* table, int32 index, int8 value)
{
    if ((index & 1) == 0)
        table[index / 2] &= 0xf0 | value;
    else
        table[index / 2] &= 0x0f | value << 4;
}

int8 GetPruning(const int8* table, int32 index)
{
    int8 res;

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
