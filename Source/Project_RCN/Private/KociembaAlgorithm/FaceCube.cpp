#include "KociembaAlgorithm/facecube.h"

#include "KociembaAlgorithm/CubieCube.h"

FFaceCube::FFaceCube(FString CubeString)
{
    Facelets.Init(EColorType::U, 54);
    
    for (int32 i = 0; i < 54; i++)
    {
        switch(CubeString[i])
        {
        case 'U':
            Facelets[i] = EColorType::U;
            break;
        case 'R':
            Facelets[i] = EColorType::R;
            break;
        case 'F':
            Facelets[i] = EColorType::F;
            break;
        case 'D':
            Facelets[i] = EColorType::D;
            break;
        case 'L':
            Facelets[i] = EColorType::L;
            break;
        case 'B':
            Facelets[i] = EColorType::B;
            break;
        default:
            break;
        }
    }
}

const TArray<TArray<EFaceletType>>& FFaceCube::CornerFacelet()
{
    static const TArray<TArray<EFaceletType>> CornerFacelet = {
        { EFaceletType::U9, EFaceletType::R1, EFaceletType::F3 },
        { EFaceletType::U7, EFaceletType::F1, EFaceletType::L3 },
        { EFaceletType::U1, EFaceletType::L1, EFaceletType::B3 },
        { EFaceletType::U3, EFaceletType::B1, EFaceletType::R3 },
        { EFaceletType::D3, EFaceletType::F9, EFaceletType::R7 },
        { EFaceletType::D1, EFaceletType::L9, EFaceletType::F7 },
        { EFaceletType::D7, EFaceletType::B9, EFaceletType::L7 },
        { EFaceletType::D9, EFaceletType::R9, EFaceletType::B7 }
    };
    return CornerFacelet;
}

const TArray<TArray<EFaceletType>>& FFaceCube::EdgeFacelet()
{
    static const TArray<TArray<EFaceletType>> EdgeFacelet = {
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
    return EdgeFacelet;
}

const TArray<TArray<EColorType>>& FFaceCube::CornerColor()
{
    static const TArray<TArray<EColorType>> CornerColor = {
        { EColorType::U, EColorType::R, EColorType::F },
        { EColorType::U, EColorType::F, EColorType::L },
        { EColorType::U, EColorType::L, EColorType::B },
        { EColorType::U, EColorType::B, EColorType::R },
        { EColorType::D, EColorType::F, EColorType::R },
        { EColorType::D, EColorType::L, EColorType::F },
        { EColorType::D, EColorType::B, EColorType::L },
        { EColorType::D, EColorType::R, EColorType::B }
    };
    return CornerColor;
}

const TArray<TArray<EColorType>>& FFaceCube::EdgeColor()
{
    static const TArray<TArray<EColorType>> EdgeColor = {
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
    return EdgeColor;
}

FCubieCube FFaceCube::ToCubieCube()
{
    int8 ori;
    FCubieCube ccRet;
    for (int32 i = 0; i < 8; i++)
    {
        ccRet.Cp[i] = ECornerType::URF;
    }
    for (int32 i = 0; i < 12; i++)
    {
        ccRet.Ep[i] = EEdgeType::UR;
    }
    for (int32 i = 0; i < CORNER_COUNT; i++)
    {
        for (ori = 0; ori < 3; ori++)
        {
            if (Facelets[static_cast<int32>(CornerFacelet()[i][ori])] == EColorType::U || Facelets[static_cast<int32>(CornerFacelet()[i][ori])] == EColorType::D)
            {
                break;
            }
        }
        EColorType col1 = Facelets[static_cast<int32>(CornerFacelet()[i][(ori + 1) % 3])];
        EColorType col2 = Facelets[static_cast<int32>(CornerFacelet()[i][(ori + 2) % 3])];
        for (int32 j = 0; j < CORNER_COUNT; j++)
        {
            if (col1 == CornerColor()[j][1] && col2 == CornerColor()[j][2])
            {
                ccRet.Cp[i] = static_cast<ECornerType>(j);
                ccRet.Co[i] = ori % 3;
                break;
            }
        }
    }
    for (int32 i = 0; i < EDGE_COUNT; i++)
    {
        for (int32 j = 0; j < EDGE_COUNT; j++)
        {
            if (Facelets[static_cast<int32>(EdgeFacelet()[i][0])] == EdgeColor()[j][0]
                && Facelets[static_cast<int32>(EdgeFacelet()[i][1])] == EdgeColor()[j][1])
            {
                ccRet.Ep[i] = static_cast<EEdgeType>(j);
                ccRet.Eo[i] = 0;
                break;
            }
            if (Facelets[static_cast<int32>(EdgeFacelet()[i][0])] == EdgeColor()[j][1]
                && Facelets[static_cast<int32>(EdgeFacelet()[i][1])] == EdgeColor()[j][0])
            {
                ccRet.Ep[i] = static_cast<EEdgeType>(j);
                ccRet.Eo[i] = 1;
                break;
            }
        }
    }
    return ccRet;
}
