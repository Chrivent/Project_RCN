#include "KociembaAlgorithm/CoordCube.h"

#include "KociembaAlgorithm/CubeSolver.h"

int16 TwistMove[N_TWIST][N_MOVE];
int16 FlipMove[N_FLIP][N_MOVE];
int16 FRtoBR_Move[N_FRtoBR][N_MOVE];
int16 URFtoDLF_Move[N_URFtoDLF][N_MOVE] = {{0}};
int16 URtoDF_Move[N_URtoDF][N_MOVE] = {{0}};
int16 URtoUL_Move[N_URtoUL][N_MOVE] = {{0}};
int16 UBtoDF_Move[N_UBtoDF][N_MOVE] = {{0}};

int16 MergeURtoULandUBtoDF[336][336] = {{0}};

int8 Slice_URFtoDLF_Parity_Pruning[N_SLICE2 * N_URFtoDLF * N_PARITY / 2] = {0};
int8 Slice_URtoDF_Parity_Pruning[N_SLICE2 * N_URtoDF * N_PARITY / 2] = {0};

int8 Slice_Twist_Pruning[N_SLICE1 * N_TWIST / 2 + 1] = {0};
int8 Slice_Flip_Pruning[N_SLICE1 * N_FLIP / 2] = {0};

int16 ParityMove[2][18] = {
    { 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1 },
    { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 }
};

int32 PRUNING_INITED = 0;

template <typename T, size_t Row, size_t Col, typename SetFunc, typename MultiplyFunc, typename GetFunc>
void ComputeMoveTable(const FString& TableName, T (&MoveTable)[Row][Col], 
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
                (CubieCube.*MultiplyFunction)(j); // 원상복구
            }
        }
        DumpToFile(MoveTable, sizeof(MoveTable), TableName, CacheDir);
    }
}

template <size_t Size, typename MoveTable1, typename MoveTable2>
void ComputePruningTable(const FString& TableName, int8 (&PruningTable)[Size], 
                         MoveTable1& MoveTableA, MoveTable2& MoveTableB, 
                         const int16 (&ParityMove)[2][18], const FString& CacheDir)
{
    if (CheckCachedTable(TableName, PruningTable, sizeof(PruningTable), CacheDir))
    {
        int32 depth = 0, done = 1;
        FMemory::Memset(PruningTable, -1, sizeof(PruningTable)); // 전체를 -1로 초기화

        SetPruning(PruningTable, 0, 0);

        while (done != Size * 2)
        {
            for (int32 i = 0; i < Size * 2; i++)
            {
                int32 parity = i % 2;
                int32 stateA = i / 2 / N_SLICE2;
                int32 slice = i / 2 % N_SLICE2;

                if (GetPruning(PruningTable, i) == depth)
                {
                    for (int32 j = 0; j < 18; j++)
                    {
                        switch (j)
                        {
                        case 3: case 5: case 6: case 8:
                        case 12: case 14: case 15: case 17:
                            continue;
                        default:
                            int32 newSlice = MoveTableA[slice][j];
                            int32 newStateA = MoveTableB[stateA][j];
                            int32 newParity = ParityMove[parity][j];

                            int32 newIndex = (N_SLICE2 * newStateA + newSlice) * 2 + newParity;
                            if (GetPruning(PruningTable, newIndex) == 0x0f)
                            {
                                SetPruning(PruningTable, newIndex, depth + 1);
                                done++;
                            }
                        }
                    }
                }
            }
            depth++;
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
        int16 uRtoUL, uBtoDF;
        for (uRtoUL = 0; uRtoUL < 336; uRtoUL++)
        {
            for (uBtoDF = 0; uBtoDF < 336; uBtoDF++)
            {
                MergeURtoULandUBtoDF[uRtoUL][uBtoDF] = FCubieCube::GetURtoDF_Standalone(uRtoUL, uBtoDF);
            }
        }
        DumpToFile(MergeURtoULandUBtoDF, sizeof(MergeURtoULandUBtoDF), "MergeURtoULandUBtoDF", CacheDir);
    }

    ComputePruningTable("Slice_URFtoDLF_Parity_Prun", Slice_URFtoDLF_Parity_Pruning,
                       FRtoBR_Move, URFtoDLF_Move, ParityMove, CacheDir);

    ComputePruningTable("Slice_URtoDF_Parity_Prun", Slice_URtoDF_Parity_Pruning,
                        FRtoBR_Move, URtoDF_Move, ParityMove, CacheDir);

    if (CheckCachedTable("Slice_Twist_Prun", Slice_Twist_Pruning, sizeof(Slice_Twist_Pruning), CacheDir))
    {
        int32 depth = 0, done = 1;
        for (int32 i = 0; i < N_SLICE1 * N_TWIST / 2 + 1; i++)
        {
            Slice_Twist_Pruning[i] = -1;
        }
        SetPruning(Slice_Twist_Pruning, 0, 0);
        while (done != N_SLICE1 * N_TWIST)
        {
            for (int32 i = 0; i < N_SLICE1 * N_TWIST; i++)
            {
                int32 twist = i / N_SLICE1, slice = i % N_SLICE1;
                if (GetPruning(Slice_Twist_Pruning, i) == depth)
                {
                    for (int32 j = 0; j < 18; j++)
                    {
                        int32 newSlice = FRtoBR_Move[slice * 24][j] / 24;
                        int32 newTwist = TwistMove[twist][j];
                        if (GetPruning(Slice_Twist_Pruning, N_SLICE1 * newTwist + newSlice) == 0x0f)
                        {
                            SetPruning(Slice_Twist_Pruning, N_SLICE1 * newTwist + newSlice, depth + 1);
                            done++;
                        }
                    }
                }
            }
            depth++;
        }
        DumpToFile(Slice_Twist_Pruning, sizeof(Slice_Twist_Pruning), "Slice_Twist_Prun", CacheDir);
    }

    if (CheckCachedTable("Slice_Flip_Prun", Slice_Flip_Pruning, sizeof(Slice_Flip_Pruning), CacheDir) != 0)
    {
        int32 depth = 0, done = 1;
        for (int32 i = 0; i < N_SLICE1 * N_FLIP / 2; i++)
        {
            Slice_Flip_Pruning[i] = -1;
        }
        SetPruning(Slice_Flip_Pruning, 0, 0);
        while (done != N_SLICE1 * N_FLIP)
        {
            for (int32 i = 0; i < N_SLICE1 * N_FLIP; i++)
            {
                int32 flip = i / N_SLICE1, slice = i % N_SLICE1;
                if (GetPruning(Slice_Flip_Pruning, i) == depth)
                {
                    for (int32 j = 0; j < 18; j++)
                    {
                        int32 newSlice = FRtoBR_Move[slice * 24][j] / 24;
                        int32 newFlip = FlipMove[flip][j];
                        if (GetPruning(Slice_Flip_Pruning, N_SLICE1 * newFlip + newSlice) == 0x0f)
                        {
                            SetPruning(Slice_Flip_Pruning, N_SLICE1 * newFlip + newSlice, depth + 1);
                            done++;
                        }
                    }
                }
            }
            depth++;
        }
        DumpToFile(Slice_Flip_Pruning, sizeof(Slice_Flip_Pruning), "Slice_Flip_Prun", CacheDir);
    }

    PRUNING_INITED = 1;
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
            UE_LOG(LogTemp, Error, TEXT("Failed to read file: %s"), *FilePath);
        }
        return true;
    }
    UE_LOG(LogTemp, Warning, TEXT("Cache table %s was not found. Recalculating."), *FilePath);
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
