// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// 강의 코드를 사용하면 에러가 발생
// #define LOG_NETMODEINFO ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT%d"), GPlayInEditorID) : ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : TEXT("SERVER")))

#define LOG_NETMODEINFO (GetWorld() && GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT%d"), GPlayInEditorID != INDEX_NONE ? GPlayInEditorID : 0) : (GetWorld() && GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : TEXT("SERVER")


// 로그를 콜한 시점에서 어떤 함수의 몇 번째 줄인지 정보를 받기 위한 매크로
#define LOG_CALLINFO ANSI_TO_TCHAR(__FUNCTION__)

// UE_LOG 매크로를 대신할 로그 매크로를 하나 지정
#define RCN_LOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s] %s %s"), LOG_NETMODEINFO, LOG_CALLINFO ,*FString::Printf(Format, ##__VA_ARGS__))

// 네트워크 멀티 플레이어에 관련된 로그 매크로
// 소스 코드를 전방위적으로 이 매크로를 사용할 것이기 때문에
// 모든 소스 파일을 쉽게 참조할 수 있는 Project_RCN.h에 작성했음
DECLARE_LOG_CATEGORY_EXTERN(LogRCNNetwrok, Log, All);