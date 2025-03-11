#include "KociembaAlgorithm/facecube.h"
#include "KociembaAlgorithm/cubiecube.h"


EFaceletType CornerFacelet[8][3] = {
    { EFaceletType::U9, EFaceletType::R1, EFaceletType::F3 },
    { EFaceletType::U7, EFaceletType::F1, EFaceletType::L3 },
    { EFaceletType::U1, EFaceletType::L1, EFaceletType::B3 },
    { EFaceletType::U3, EFaceletType::B1, EFaceletType::R3 },
    { EFaceletType::D3, EFaceletType::F9, EFaceletType::R7 },
    { EFaceletType::D1, EFaceletType::L9, EFaceletType::F7 },
    { EFaceletType::D7, EFaceletType::B9, EFaceletType::L7 },
    { EFaceletType::D9, EFaceletType::R9, EFaceletType::B7 }
};

EFaceletType EdgeFacelet[12][2] = {
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

EColorType CornerColor[8][3] = { 
    { EColorType::U, EColorType::R, EColorType::F }, 
    { EColorType::U, EColorType::F, EColorType::L }, 
    { EColorType::U, EColorType::L, EColorType::B }, 
    { EColorType::U, EColorType::B, EColorType::R }, 
    { EColorType::D, EColorType::F, EColorType::R }, 
    { EColorType::D, EColorType::L, EColorType::F },
    { EColorType::D, EColorType::B, EColorType::L }, 
    { EColorType::D, EColorType::R, EColorType::B } 
};

EColorType EdgeColor[12][2] = { 
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


FFaceCube* get_facecube()
{
    FFaceCube* res = new FFaceCube();
    static const EColorType f[54] = {
        EColorType::U, EColorType::U, EColorType::U, EColorType::U, EColorType::U, EColorType::U, EColorType::U, EColorType::U, EColorType::U,
        EColorType::R, EColorType::R, EColorType::R, EColorType::R, EColorType::R, EColorType::R, EColorType::R, EColorType::R, EColorType::R,
        EColorType::F, EColorType::F, EColorType::F, EColorType::F, EColorType::F, EColorType::F, EColorType::F, EColorType::F, EColorType::F,
        EColorType::D, EColorType::D, EColorType::D, EColorType::D, EColorType::D, EColorType::D, EColorType::D, EColorType::D, EColorType::D,
        EColorType::L, EColorType::L, EColorType::L, EColorType::L, EColorType::L, EColorType::L, EColorType::L, EColorType::L, EColorType::L,
        EColorType::B, EColorType::B, EColorType::B, EColorType::B, EColorType::B, EColorType::B, EColorType::B, EColorType::B, EColorType::B
    };
    res->Facelets = TArray(f, UE_ARRAY_COUNT(f));
    return res;
}

FFaceCube* get_facecube_fromstring(char* cubeString)
{
    FFaceCube* res = new FFaceCube();
    for (int32 i = 0; i < 54; i++)
    {
        switch(cubeString[i])
        {
        case 'U':
            res->Facelets[i] = EColorType::U;
            break;
        case 'R':
            res->Facelets[i] = EColorType::R;
            break;
        case 'F':
            res->Facelets[i] = EColorType::F;
            break;
        case 'D':
            res->Facelets[i] = EColorType::D;
            break;
        case 'L':
            res->Facelets[i] = EColorType::L;
            break;
        case 'B':
            res->Facelets[i] = EColorType::B;
            break;
        default:
            break;
        }
    }
    return res;
}

void to_String(FFaceCube* FaceCube, char* res)
{
    for (int32 i = 0; i < 54; i++)
    {
        switch(FaceCube->Facelets[i])
        {
        case EColorType::U:
            res[i] = 'U';
            break;
        case EColorType::R:
            res[i] = 'R';
            break;
        case EColorType::F:
            res[i] = 'F';
            break;
        case EColorType::D:
            res[i] = 'D';
            break;
        case EColorType::L:
            res[i] = 'L';
            break;
        case EColorType::B:
            res[i] = 'B';
            break;
        }
    }
    res[54] = 0;
}

FCubieCube* toCubieCube(FFaceCube* FaceCube)
{
    int i, j;
    signed char ori;
    EColorType col1, col2;
    FCubieCube* ccRet = new FCubieCube();
    for (i = 0; i < 8; i++)
        ccRet->Cp[i] = ECornerType::URF;// invalidate corners
    for (i = 0; i < 12; i++)
        ccRet->Ep[i] = EEdgeType::UR;// and edges

    for(i = 0; i < CORNER_COUNT; i++) {
        // get the colors of the cubie at corner i, starting with U/D
        for (ori = 0; ori < 3; ori++)
            if (FaceCube->Facelets[static_cast<int32>(CornerFacelet[i][ori])] == EColorType::U || FaceCube->Facelets[static_cast<int32>(CornerFacelet[i][ori])] == EColorType::D)
                break;
        col1 = FaceCube->Facelets[static_cast<int32>(CornerFacelet[i][(ori + 1) % 3])];
        col2 = FaceCube->Facelets[static_cast<int32>(CornerFacelet[i][(ori + 2) % 3])];

        for (j = 0; j < CORNER_COUNT; j++) {
            if (col1 == CornerColor[j][1] && col2 == CornerColor[j][2]) {
                // in cornerposition i we have cornercubie j
                ccRet->Cp[i] = (ECornerType)j;
                ccRet->Co[i] = ori % 3;
                break;
            }
        }
    }

    for (i = 0; i < EDGE_COUNT; i++) {
        for (j = 0; j < EDGE_COUNT; j++) {
            if (FaceCube->Facelets[static_cast<int32>(EdgeFacelet[i][0])] == EdgeColor[j][0]
                    && FaceCube->Facelets[static_cast<int32>(EdgeFacelet[i][1])] == EdgeColor[j][1]) {
                ccRet->Ep[i] = (EEdgeType)j;
                ccRet->Eo[i] = 0;
                break;
            }
            if (FaceCube->Facelets[static_cast<int32>(EdgeFacelet[i][0])] == EdgeColor[j][1]
                    && FaceCube->Facelets[static_cast<int32>(EdgeFacelet[i][1])] == EdgeColor[j][0]) {
                ccRet->Ep[i] = (EEdgeType)j;
                ccRet->Eo[i] = 1;
                break;
            }
        }
    }
    return ccRet;
}
