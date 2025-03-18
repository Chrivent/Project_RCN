#pragma once

#include "CoreMinimal.h"
#include "CubeSolver.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCubeSolver, Log, All)

/**
 * The names of the facelet positions of the cube
 * <pre>
 *             |************|
 *             |*U1**U2**U3*|
 *             |************|
 *             |*U4**U5**U6*|
 *             |************|
 *             |*U7**U8**U9*|
 *             |************|
 * ************|************|************|************|
 * *L1**L2**L3*|*F1**F2**F3*|*R1**R2**F3*|*B1**B2**B3*|
 * ************|************|************|************|
 * *L4**L5**L6*|*F4**F5**F6*|*R4**R5**R6*|*B4**B5**B6*|
 * ************|************|************|************|
 * *L7**L8**L9*|*F7**F8**F9*|*R7**R8**R9*|*B7**B8**B9*|
 * ************|************|************|************|
 *             |************|
 *             |*D1**D2**D3*|
 *             |************|
 *             |*D4**D5**D6*|
 *             |************|
 *             |*D7**D8**D9*|
 *             |************|
 * </pre>
 * A cube definition string "UBL..." means for example: In position U1 we have the U-color, in position U2 we have the
 * B-color, in position U3 we have the L color etc. according to the order U1, U2, U3, U4, U5, U6, U7, U8, U9, R1, R2,
 * R3, R4, R5, R6, R7, R8, R9, F1, F2, F3, F4, F5, F6, F7, F8, F9, D1, D2, D3, D4, D5, D6, D7, D8, D9, L1, L2, L3, L4,
 * L5, L6, L7, L8, L9, B1, B2, B3, B4, B5, B6, B7, B8, B9 of the enum constants.
 */
UCLASS()
class UCubeSolver : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
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
    static FString SolveCube(FString Facelets, const int32 MaxDepth = 24, double TimeOut = 1.000f, const FString& CacheDir = TEXT("cache"));
};
