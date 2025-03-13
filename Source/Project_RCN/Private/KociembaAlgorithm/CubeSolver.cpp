#include "KociembaAlgorithm/CubeSolver.h"

#include "KociembaAlgorithm/CoordCube.h"

const TArray<TArray<EFaceletType>> UCubeSolver::CornerFacelet = {
    { EFaceletType::U9, EFaceletType::R1, EFaceletType::F3 },
    { EFaceletType::U7, EFaceletType::F1, EFaceletType::L3 },
    { EFaceletType::U1, EFaceletType::L1, EFaceletType::B3 },
    { EFaceletType::U3, EFaceletType::B1, EFaceletType::R3 },
    { EFaceletType::D3, EFaceletType::F9, EFaceletType::R7 },
    { EFaceletType::D1, EFaceletType::L9, EFaceletType::F7 },
    { EFaceletType::D7, EFaceletType::B9, EFaceletType::L7 },
    { EFaceletType::D9, EFaceletType::R9, EFaceletType::B7 }
};

const TArray<TArray<EFaceletType>> UCubeSolver::EdgeFacelet = {
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

const TArray<TArray<EColorType>> UCubeSolver::CornerColor = {
    { EColorType::U, EColorType::R, EColorType::F },
    { EColorType::U, EColorType::F, EColorType::L },
    { EColorType::U, EColorType::L, EColorType::B },
    { EColorType::U, EColorType::B, EColorType::R },
    { EColorType::D, EColorType::F, EColorType::R },
    { EColorType::D, EColorType::L, EColorType::F },
    { EColorType::D, EColorType::B, EColorType::L },
    { EColorType::D, EColorType::R, EColorType::B }
};

const TArray<TArray<EColorType>> UCubeSolver::EdgeColor = {
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

FString UCubeSolver::SolveCube(const FString& Facelets, const int32 MaxDepth, const double TimeOut, const FString& CacheDir)
{
    FSearch Search;
    
    TArray<int32> Count;
    Count.SetNum(6);

    if (PRUNING_INITED == 0)
    {
        InitPruning(CacheDir);
    }
    
    for (const auto Facelet : Facelets)
    {
        switch (Facelet)
        {
        case 'U': Count[0]++; break;
        case 'R': Count[1]++; break;
        case 'F': Count[2]++; break;
        case 'D': Count[3]++; break;
        case 'L': Count[4]++; break;
        case 'B': Count[5]++; break;
        default:
            return FString("ERROR: Invalid facelet input");
        }
    }
    
    if (Count.ContainsByPredicate([](int32 C) { return C != 9; }))
    {
        return FString("ERROR: Invalid cube state");
    }
    
    FCubieCube Cc = ToCubieCube(Facelets);
    if (Cc.Verify())
    {
        return FString("ERROR: Unsolvable cube");
    }

    Search.Po[0] = 0;
    Search.Ax[0] = 0;
    Search.Flip[0] = Cc.GetFlip();
    Search.Twist[0] = Cc.GetTwist();
    Search.Parity[0] = Cc.CornerParity();
    Search.Slice[0] = Cc.GetFRtoBR() / 24;
    Search.URFtoDLF[0] = Cc.GetURFtoDLF();
    Search.FRtoBR[0] = Cc.GetFRtoBR();
    Search.URtoUL[0] = Cc.GetURtoUL();
    Search.UBtoDF[0] = Cc.GetUBtoDF();

    Search.MinDistPhase1[1] = 1;
    int32 N = 0, Busy = 0, DepthPhase1 = 1;
    const double Time = FPlatformTime::Seconds();
    
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
                        if (FPlatformTime::Seconds() - Time > TimeOut)
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

        const int32 Move = 3 * Search.Ax[N] + Search.Po[N] - 1;
        Search.Flip[N + 1] = FlipMove[Search.Flip[N]][Move];
        Search.Twist[N + 1] = TwistMove[Search.Twist[N]][Move];
        Search.Slice[N + 1] = FRtoBR_Move[Search.Slice[N] * 24][Move] / 24;
    
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
                if (S >= 0 && (S == DepthPhase1 || (Search.Ax[DepthPhase1 - 1] != Search.Ax[DepthPhase1] && Search.Ax[DepthPhase1 - 1] != Search.Ax[DepthPhase1] + 3)))
                {
                    return SolutionToString(Search, S);
                }
            }
        }
    } while (true);
}

int32 UCubeSolver::TotalDepth(FSearch& Search, const int32 DepthPhase1, const int32 MaxDepth)
{
    constexpr int32 MaxPhase2Moves = 10;
    const int32 MaxDepthPhase2 = FMath::Min(MaxPhase2Moves, MaxDepth - DepthPhase1);
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

    const int32 D1 = GetPruning(Slice_URFtoDLF_Parity_Prun,
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

    const int32 D2 = GetPruning(Slice_URtoDF_Parity_Prun,
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
            GetPruning(Slice_URtoDF_Parity_Prun,
                (N_SLICE2 * Search.URtoDF[N + 1] + Search.FRtoBR[N + 1]) * 2 + Search.Parity[N + 1]),
            GetPruning(Slice_URFtoDLF_Parity_Prun,
                (N_SLICE2 * Search.URFtoDLF[N + 1] + Search.FRtoBR[N + 1]) * 2 + Search.Parity[N + 1])
        );

    } while (Search.MinDistPhase2[N + 1] != 0);

    return DepthPhase1 + DepthPhase2;
}

FString UCubeSolver::SolutionToString(const FSearch& Search, const int32 Length)
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

FCubieCube UCubeSolver::ToCubieCube(const FString& CubeString)
{
    TArray<EColorType> Facelets;
    Facelets.SetNum(54);
    for (int32 i = 0; i < 54; ++i)
    {
        switch (CubeString[i])
        {
        case 'U': Facelets[i] = EColorType::U; break;
        case 'R': Facelets[i] = EColorType::R; break;
        case 'F': Facelets[i] = EColorType::F; break;
        case 'D': Facelets[i] = EColorType::D; break;
        case 'L': Facelets[i] = EColorType::L; break;
        case 'B': Facelets[i] = EColorType::B; break;
        default:  break;
        }
    }
    FCubieCube CubieCube;
    for (int32 i = 0; i < CORNER_COUNT; i++)
    {
        CubieCube.Cp[i] = ECornerType::URF;
    }
    for (int32 i = 0; i < EDGE_COUNT; i++)
    {
        CubieCube.Ep[i] = EEdgeType::UR;
    }
    for (int32 i = 0; i < CORNER_COUNT; i++)
    {
        int8 Orientation;
        for (Orientation = 0; Orientation < 3; Orientation++)
        {
            if (Facelets[static_cast<int32>(CornerFacelet[i][Orientation])] == EColorType::U ||
                Facelets[static_cast<int32>(CornerFacelet[i][Orientation])] == EColorType::D)
            {
                break;
            }
        }
        const EColorType Color1 = Facelets[static_cast<int32>(CornerFacelet[i][(Orientation + 1) % 3])];
        const EColorType Color2 = Facelets[static_cast<int32>(CornerFacelet[i][(Orientation + 2) % 3])];
        for (int32 j = 0; j < CORNER_COUNT; j++)
        {
            if (Color1 == CornerColor[j][1] && Color2 == CornerColor[j][2])
            {
                CubieCube.Cp[i] = static_cast<ECornerType>(j);
                CubieCube.Co[i] = Orientation;
                break;
            }
        }
    }
    for (int32 i = 0; i < EDGE_COUNT; i++)
    {
        const EColorType Color1 = Facelets[static_cast<int32>(EdgeFacelet[i][0])];
        const EColorType Color2 = Facelets[static_cast<int32>(EdgeFacelet[i][1])];
        for (int32 j = 0; j < EDGE_COUNT; j++)
        {
            if (Color1 == EdgeColor[j][0] && Color2 == EdgeColor[j][1])
            {
                CubieCube.Ep[i] = static_cast<EEdgeType>(j);
                CubieCube.Eo[i] = 0;
                break;
            }
            if (Color1 == EdgeColor[j][1] && Color2 == EdgeColor[j][0])
            {
                CubieCube.Ep[i] = static_cast<EEdgeType>(j);
                CubieCube.Eo[i] = 1;
                break;
            }
        }
    }
    return CubieCube;
}
