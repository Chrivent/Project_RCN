#include "KociembaAlgorithm/CubeSolver.h"

#include "KociembaAlgorithm/facecube.h"
#include "KociembaAlgorithm/CoordCube.h"

FString UCubeSolver::SolutionToString(const FSearch* Search, const int32 Length, const int32 DepthPhase1)
{
    FString SolutionString;
    
    for (int32 i = 0; i < Length; i++)
    {
        static const TCHAR* MoveAxis[] = { TEXT("U"), TEXT("R"), TEXT("F"), TEXT("D"), TEXT("L"), TEXT("B") };
        SolutionString += MoveAxis[Search->Ax[i]];

        static const TCHAR* MovePower[] = { TEXT(" "), TEXT("2 "), TEXT("' ") };
        SolutionString += MovePower[Search->Po[i] - 1];

        if (i == DepthPhase1 - 1)
        {
            SolutionString += TEXT(". ");
        }
    }
    
    return SolutionString;
}

FString UCubeSolver::SolveCube(const FString& Facelets, int32 MaxDepth, double TimeOut, bool bUseSeparator, const FString& CacheDir)
{
    FSearch* Search = new FSearch();

    TArray<int32, TInlineAllocator<6>> Count = { 0, 0, 0, 0, 0, 0 };

    if (PRUNING_INITED == 0)
    {
        InitPruning(TCHAR_TO_UTF8(*CacheDir));
    }
    
    for (const TCHAR& Facelet : Facelets)
    {
        switch (Facelet)
        {
        case 'U': Count[static_cast<int32>(EColorType::U)]++; break;
        case 'R': Count[static_cast<int32>(EColorType::R)]++; break;
        case 'F': Count[static_cast<int32>(EColorType::F)]++; break;
        case 'D': Count[static_cast<int32>(EColorType::D)]++; break;
        case 'L': Count[static_cast<int32>(EColorType::L)]++; break;
        case 'B': Count[static_cast<int32>(EColorType::B)]++; break;
        default:
            return FString("ERROR: Invalid facelet input");
        }
    }
    
    if (Count.ContainsByPredicate([](int32 C) { return C != 9; }))
    {
        return FString("ERROR: Invalid cube state");
    }
    
    FFaceCube* Fc = GetFaceCubeFromString(TCHAR_TO_UTF8(*Facelets));
    FCubieCube* Cc = ToCubieCube(Fc);
    if (Verify(Cc) != 0)
    {
        return FString("ERROR: Unsolvable cube");
    }

    FCoordCube* C = GetCoordCube(Cc);

    Search->Po[0] = 0;
    Search->Ax[0] = 0;
    Search->Flip[0] = C->Flip;
    Search->Twist[0] = C->Twist;
    Search->Parity[0] = C->Parity;
    Search->Slice[0] = C->FRtoBR / 24;
    Search->URFtoDLF[0] = C->URFtoDLF;
    Search->FRtoBR[0] = C->FRtoBR;
    Search->URtoUL[0] = C->URtoUL;
    Search->UBtoDF[0] = C->UBtoDF;

    Search->MinDistPhase1[1] = 1;
    int32 N = 0, Busy = 0, DepthPhase1 = 1;
    double TStart = FPlatformTime::Seconds();
    int32 S;
    
    while (true)
    {
        while (true)
        {
            if (DepthPhase1 - N > Search->MinDistPhase1[N + 1] && !Busy)
            {
                Search->Ax[++N] = Search->Ax[N - 1] == 0 || Search->Ax[N - 1] == 3 ? 1 : 0;
                Search->Po[N] = 1;
            }
            else if (++Search->Po[N] > 3)
            {
                while (true)
                {
                    if (++Search->Ax[N] > 5)
                    {
                        if (FPlatformTime::Seconds() - TStart > TimeOut)
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
                            Search->Ax[N] = 0;
                            Search->Po[N] = 1;
                            Busy = 0;
                            break;
                        }
                    
                        N--;
                        Busy = 1;
                        break;
                    }
                
                    Search->Po[N] = 1;
                    Busy = 0;

                    if (N == 0 || (Search->Ax[N - 1] != Search->Ax[N] && Search->Ax[N - 1] - 3 != Search->Ax[N]))
                    {
                        break;
                    }
                }
            }
            else
            {
                Busy = 0;
            }

            if (!Busy)
            {
                break;
            }
        }

        int32 Mv = 3 * Search->Ax[N] + Search->Po[N] - 1;
        Search->Flip[N + 1] = FlipMove[Search->Flip[N]][Mv];
        Search->Twist[N + 1] = TwistMove[Search->Twist[N]][Mv];
        Search->Slice[N + 1] = FRtoBR_Move[Search->Slice[N] * 24][Mv] / 24;
    
        Search->MinDistPhase1[N + 1] = FMath::Max(
            GetPruning(Slice_Flip_Prun, N_SLICE1 * Search->Flip[N + 1] + Search->Slice[N + 1]),
            GetPruning(Slice_Twist_Prun, N_SLICE1 * Search->Twist[N + 1] + Search->Slice[N + 1])
        );

        if (Search->MinDistPhase1[N + 1] == 0 && N >= DepthPhase1 - 5)
        {
            Search->MinDistPhase1[N + 1] = 10;
        
            if (N == DepthPhase1 - 1 && (S = TotalDepth(*Search, DepthPhase1, MaxDepth)) >= 0)
            {
                if (S == DepthPhase1 || 
                    (Search->Ax[DepthPhase1 - 1] != Search->Ax[DepthPhase1] && Search->Ax[DepthPhase1 - 1] != Search->Ax[DepthPhase1] + 3))
                {
                    return SolutionToString(Search, S, bUseSeparator ? DepthPhase1 : -1);
                }
            }
        }
    }
}

int UCubeSolver::TotalDepth(FSearch& Search, int32 DepthPhase1, int32 MaxDepth)
{
    constexpr int32 MaxPhase2Moves = 10;
    int32 MaxDepthPhase2 = FMath::Min(MaxPhase2Moves, MaxDepth - DepthPhase1);
    int32 DepthPhase2 = 1;
    int32 N = DepthPhase1;
    int32 Busy = 0;

    for (int32 i = 0; i < DepthPhase1; i++)
    {
        int32 Move = 3 * Search.Ax[i] + Search.Po[i] - 1;
        Search.URFtoDLF[i + 1] = URFtoDLF_Move[Search.URFtoDLF[i]][Move];
        Search.FRtoBR[i + 1] = FRtoBR_Move[Search.FRtoBR[i]][Move];
        Search.Parity[i + 1] = ParityMove[Search.Parity[i]][Move];
    }

    int32 D1 = GetPruning(Slice_URFtoDLF_Parity_Prun,
        (N_SLICE2 * Search.URFtoDLF[DepthPhase1] + Search.FRtoBR[DepthPhase1]) * 2 + Search.Parity[DepthPhase1]);
    
    if (D1 > MaxDepthPhase2)
    {
        return -1;
    }

    for (int32 i = 0; i < DepthPhase1; i++)
    {
        int32 Move = 3 * Search.Ax[i] + Search.Po[i] - 1;
        Search.URtoUL[i + 1] = URtoUL_Move[Search.URtoUL[i]][Move];
        Search.UBtoDF[i + 1] = UBtoDF_Move[Search.UBtoDF[i]][Move];
    }

    Search.URtoDF[DepthPhase1] = MergeURtoULandUBtoDF[Search.URtoUL[DepthPhase1]][Search.UBtoDF[DepthPhase1]];

    int32 D2 = GetPruning(Slice_URtoDF_Parity_Prun,
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
                Search.Ax[++N] = Search.Ax[N - 1] == 0 || Search.Ax[N - 1] == 3 ? 1 : 0;
                Search.Po[N] = Search.Ax[N] == 0 || Search.Ax[N] == 3 ? 1 : 2;
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

        int32 Move = 3 * Search.Ax[N] + Search.Po[N] - 1;

        Search.URFtoDLF[N + 1] = URFtoDLF_Move[Search.URFtoDLF[N]][Move];
        Search.FRtoBR[N + 1] = FRtoBR_Move[Search.FRtoBR[N]][Move];
        Search.Parity[N + 1] = ParityMove[Search.Parity[N]][Move];
        Search.URtoDF[N + 1] = URtoDF_Move[Search.URtoDF[N]][Move];

        Search.MinDistPhase2[N + 1] = FMath::Max(
            GetPruning(Slice_URtoDF_Parity_Prun,
                (N_SLICE2 * Search.URtoDF[N + 1] + Search.FRtoBR[N + 1]) * 2 + Search.Parity[N + 1]),
            GetPruning(Slice_URFtoDLF_Parity_Prun,
                (N_SLICE2 * Search.URFtoDLF[N + 1] + Search.FRtoBR[N + 1]) * 2 + Search.Parity[N + 1])
        );

    } while (Search.MinDistPhase2[N + 1] != 0);

    return DepthPhase1 + DepthPhase2;
}

FString Solution(char* Facelets, int MaxDepth, long TimeOut, bool bUseSeparator, const char* CacheDir)
{
    FSearch* search = new FSearch();
    FFaceCube* fc;
    FCubieCube* cc;
    FCoordCube* c;

    int s, i;
    int mv, n;
    int busy;
    int depthPhase1;
    time_t tStart;
    // +++++++++++++++++++++check for wrong input +++++++++++++++++++++++++++++
    int count[6] = {0};

    if (PRUNING_INITED == 0) {
        InitPruning(CacheDir);
    }

    for (i = 0; i < 54; i++)
        switch(Facelets[i]) {
    case 'U': count[static_cast<int32>(EColorType::U)]++; break;
    case 'R': count[static_cast<int32>(EColorType::R)]++; break;
    case 'F': count[static_cast<int32>(EColorType::F)]++; break;
    case 'D': count[static_cast<int32>(EColorType::D)]++; break;
    case 'L': count[static_cast<int32>(EColorType::L)]++; break;
    case 'B': count[static_cast<int32>(EColorType::B)]++; break;
        }

    for (i = 0; i < 6; i++)
        if (count[i] != 9) {
            free(search);
            return FString();
        }

    fc = GetFaceCubeFromString(Facelets);
    cc = ToCubieCube(fc);
    if ((s = Verify(cc)) != 0) {
        free(search);
        return FString();
    }

    // +++++++++++++++++++++++ initialization +++++++++++++++++++++++++++++++++
    c = GetCoordCube(cc);

    search->Po[0] = 0;
    search->Ax[0] = 0;
    search->Flip[0] = c->Flip;
    search->Twist[0] = c->Twist;
    search->Parity[0] = c->Parity;
    search->Slice[0] = c->FRtoBR / 24;
    search->URFtoDLF[0] = c->URFtoDLF;
    search->FRtoBR[0] = c->FRtoBR;
    search->URtoUL[0] = c->URtoUL;
    search->UBtoDF[0] = c->UBtoDF;

    search->MinDistPhase1[1] = 1;// else failure for depth=1, n=0
    mv = 0;
    n = 0;
    busy = 0;
    depthPhase1 = 1;

    tStart = time(NULL);

    // +++++++++++++++++++ Main loop ++++++++++++++++++++++++++++++++++++++++++
    do {
        do {
            if ((depthPhase1 - n > search->MinDistPhase1[n + 1]) && !busy) {

                if (search->Ax[n] == 0 || search->Ax[n] == 3)// Initialize next move
                    search->Ax[++n] = 1;
                else
                    search->Ax[++n] = 0;
                search->Po[n] = 1;
            } else if (++search->Po[n] > 3) {
                do {// increment axis
                    if (++search->Ax[n] > 5) {

                        if (time(NULL) - tStart > TimeOut)
                            return FString();

                        if (n == 0) {
                            if (depthPhase1 >= MaxDepth)
                                return FString();
                            else {
                                depthPhase1++;
                                search->Ax[n] = 0;
                                search->Po[n] = 1;
                                busy = 0;
                                break;
                            }
                        } else {
                            n--;
                            busy = 1;
                            break;
                        }

                    } else {
                        search->Po[n] = 1;
                        busy = 0;
                    }
                } while (n != 0 && (search->Ax[n - 1] == search->Ax[n] || search->Ax[n - 1] - 3 == search->Ax[n]));
            } else
                busy = 0;
        } while (busy);

        // +++++++++++++ compute new coordinates and new minDistPhase1 ++++++++++
        // if minDistPhase1 =0, the H subgroup is reached
        mv = 3 * search->Ax[n] + search->Po[n] - 1;
        search->Flip[n + 1] = FlipMove[search->Flip[n]][mv];
        search->Twist[n + 1] = TwistMove[search->Twist[n]][mv];
        search->Slice[n + 1] = FRtoBR_Move[search->Slice[n] * 24][mv] / 24;
        search->MinDistPhase1[n + 1] = FMath::Max(
            GetPruning(Slice_Flip_Prun, N_SLICE1 * search->Flip[n + 1] + search->Slice[n + 1]),
            GetPruning(Slice_Twist_Prun, N_SLICE1 * search->Twist[n + 1] + search->Slice[n + 1])
        );
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // System.out.format("%d %d\n", n, depthPhase1);
        if (search->MinDistPhase1[n + 1] == 0 && n >= depthPhase1 - 5) {
            search->MinDistPhase1[n + 1] = 10;// instead of 10 any value >5 is possible
            if (n == depthPhase1 - 1 && (s = UCubeSolver::TotalDepth(*search, depthPhase1, MaxDepth)) >= 0) {
                if (s == depthPhase1
                        || (search->Ax[depthPhase1 - 1] != search->Ax[depthPhase1] && search->Ax[depthPhase1 - 1] != search->Ax[depthPhase1] + 3)) {
                    FString res;
                    if (bUseSeparator) {
                        res = UCubeSolver::SolutionToString(search, s, depthPhase1);
                    } else {
                        res = UCubeSolver::SolutionToString(search, s, -1);
                    }
                    return res;
                }
            }

        }
    } while (1);
}
