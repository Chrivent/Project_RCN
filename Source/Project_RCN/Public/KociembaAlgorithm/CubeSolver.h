#pragma once

#include "CoreMinimal.h"
#include "CubeSolver.generated.h"

USTRUCT(BlueprintType)
struct FSearch
{
    GENERATED_BODY()

    int32 Ax[31];
    int32 Po[31];
    int32 Flip[31];
    int32 Twist[31];
    int32 Slice[31];
    int32 Parity[31];
    int32 URFtoDLF[31];
    int32 FRtoBR[31];
    int32 URtoUL[31];
    int32 UBtoDF[31];
    int32 URtoDF[31];
    int32 MinDistPhase1[31];
    int32 MinDistPhase2[31];
};

UCLASS()
class PROJECT_RCN_API UCubeSolver : public UObject
{
    GENERATED_BODY()

public:
    /**
     * Generate the solution string from the array data including a separator between phase1 and phase2 moves
     */
    static FString SolutionToString(const FSearch* Search, int32 Length, int32 DepthPhase1);

    /**
     * Computes the solver string for a given cube.
     * 
     * @param Facelets
     * The cube definition string
     * 
     * @param MaxDepth
     * defines the maximal allowed maneuver length. For random cubes, a maxDepth of 21 usually will return a
     * solution in less than 0.5 seconds. With a maxDepth of 20 it takes a few seconds on average to find a
     * solution, but it may take much longer for specific cubes.
     * 
     * @param TimeOut
     * defines the maximum computing time of the method in seconds. If it does not return with a solution, it returns with
     * an error code.
     * 
     * @param bUseSeparator
     * determines if a " . " separates the phase1 and phase2 parts of the solver string like in F' R B R L2 F .
     * U2 U D for example.
     * 
     * @param CacheDir
     * Directory to cache results
     * 
     * @return Solution string or error code<br>
     * Error 1: There is not exactly one facelet of each colour<br>
     * Error 2: Not all 12 edges exist exactly once<br>
     * Error 3: Flip error: One edge has to be flipped<br>
     * Error 4: Not all corners exist exactly once<br>
     * Error 5: Twist error: One corner has to be twisted<br>
     * Error 6: Parity error: Two corners or two edges have to be exchanged<br>
     * Error 7: No solution exists for the given maxDepth<br>
     * Error 8: Timeout, no solution within given time
     */
    static FString SolveCube(const FString& Facelets, int32 MaxDepth, double TimeOut, bool bUseSeparator, const FString& CacheDir);

    /**
     * Apply phase2 of algorithm and return the combined phase1 and phase2 depth.
     * In phase2, only moves U, D, R2, F2, L2, B2 are allowed.
     */
    static int32 TotalDepth(FSearch& Search, int32 DepthPhase1, int32 MaxDepth);

    /**
     * Add a pattern to the cube state, so that solving `NewFacelets` applied to `Facelets` results in `Pattern`
     */
    static void Patternize(const FString& Facelets, const FString& Pattern, FString& Patternized);
};
