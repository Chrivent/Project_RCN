#include "KociembaAlgorithm/CubeSolver.h"

#include "KociembaAlgorithm/facecube.h"
#include "KociembaAlgorithm/CoordCube.h"

FString UCubeSolver::SolveCube(const FString& Facelets, int32 MaxDepth, double TimeOut, bool bUseSeparator, const FString& CacheDir)
{
    FSearch Search;
    
    TArray<int32> Count;
    Count.Init(0, 6);

    if (PRUNING_INITED == 0)
    {
        InitPruning(CacheDir);
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
    
    FFaceCube Fc(Facelets);
    FCubieCube Cc = Fc.ToCubieCube();
    if (Verify(Cc))
    {
        return FString("ERROR: Unsolvable cube");
    }

    FCoordCube C(Cc);

    Search.Po[0] = 0;
    Search.Ax[0] = 0;
    Search.Flip[0] = C.Flip;
    Search.Twist[0] = C.Twist;
    Search.Parity[0] = C.Parity;
    Search.Slice[0] = C.FRtoBR / 24;
    Search.URFtoDLF[0] = C.URFtoDLF;
    Search.FRtoBR[0] = C.FRtoBR;
    Search.URtoUL[0] = C.URtoUL;
    Search.UBtoDF[0] = C.UBtoDF;

    Search.MinDistPhase1[1] = 1;
    int32 N = 0, Busy = 0, DepthPhase1 = 1;
    double TStart = FPlatformTime::Seconds();
    
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

        int32 Mv = 3 * Search.Ax[N] + Search.Po[N] - 1;
        Search.Flip[N + 1] = FlipMove[Search.Flip[N]][Mv];
        Search.Twist[N + 1] = TwistMove[Search.Twist[N]][Mv];
        Search.Slice[N + 1] = FRtoBR_Move[Search.Slice[N] * 24][Mv] / 24;
    
        Search.MinDistPhase1[N + 1] = FMath::Max(
            GetPruning(Slice_Flip_Prun, N_SLICE1 * Search.Flip[N + 1] + Search.Slice[N + 1]),
            GetPruning(Slice_Twist_Prun, N_SLICE1 * Search.Twist[N + 1] + Search.Slice[N + 1])
        );

        if (Search.MinDistPhase1[N + 1] == 0 && N >= DepthPhase1 - 5)
        {
            Search.MinDistPhase1[N + 1] = 10;

            if (N == DepthPhase1 - 1)
            {
                int32 S = TotalDepth(Search, DepthPhase1, MaxDepth);
                if (S >= 0 && 
                    (S == DepthPhase1 || 
                    (Search.Ax[DepthPhase1 - 1] != Search.Ax[DepthPhase1] && Search.Ax[DepthPhase1 - 1] != Search.Ax[DepthPhase1] + 3)))
                {
                    return SolutionToString(Search, S, bUseSeparator ? DepthPhase1 : -1);
                }
            }
        }
    } while (true);
}

int32 UCubeSolver::TotalDepth(FSearch& Search, int32 DepthPhase1, int32 MaxDepth)
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

FString UCubeSolver::SolutionToString(const FSearch& Search, const int32 Length, const int32 DepthPhase1)
{
    FString SolutionString;
    
    for (int32 i = 0; i < Length; i++)
    {
        static const TCHAR* MoveAxis[] = { TEXT("U"), TEXT("R"), TEXT("F"), TEXT("D"), TEXT("L"), TEXT("B") };
        SolutionString += MoveAxis[Search.Ax[i]];

        static const TCHAR* MovePower[] = { TEXT(" "), TEXT("2 "), TEXT("' ") };
        SolutionString += MovePower[Search.Po[i] - 1];

        if (i == DepthPhase1 - 1)
        {
            SolutionString += TEXT(". ");
        }
    }
    
    return SolutionString;
}
