// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// 로컬인지 리모트인지 구분하는 매크로
#define LOG_LOCALROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetLocalRole()))
#define LOG_REMOTEROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetRemoteRole()))

// 서버인지 클라이언트인지 구분하는 매크로
#define LOG_NETMODEINFO ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT%d"), UE::GetPlayInEditorID) : ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : TEXT("SERVER")))

// 로그를 콜한 시점에서 어떤 함수의 몇 번째 줄인지 정보를 받기 위한 매크로
#define LOG_CALLINFO ANSI_TO_TCHAR(__FUNCTION__)

// UE_LOG 매크로를 대신할 로그 매크로를 하나 지정
#define RCN_LOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s] %s %s"), LOG_NETMODEINFO, LOG_LOCALROLEINFO, LOG_REMOTEROLEINFO, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

// 네트워크 멀티 플레이어에 관련된 로그 매크로
// 소스 코드를 전방위적으로 이 매크로를 사용할 것이기 때문에
// 모든 소스 파일을 쉽게 참조할 수 있는 Project_RCN.h에 작성했음
DECLARE_LOG_CATEGORY_EXTERN(LogNetwork, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogRubikCube, Log, All);

DECLARE_LOG_CATEGORY_EXTERN(LogSingleModeBase, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogTimerStart, Log, All);