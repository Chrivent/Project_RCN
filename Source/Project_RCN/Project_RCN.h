// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// ���� �ڵ带 ����ϸ� ������ �߻�
// #define LOG_NETMODEINFO ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT%d"), GPlayInEditorID) : ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : TEXT("SERVER")))

#define LOG_NETMODEINFO (GetWorld() && GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT%d"), GPlayInEditorID != INDEX_NONE ? GPlayInEditorID : 0) : (GetWorld() && GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : TEXT("SERVER")


// �α׸� ���� �������� � �Լ��� �� ��° ������ ������ �ޱ� ���� ��ũ��
#define LOG_CALLINFO ANSI_TO_TCHAR(__FUNCTION__)

// UE_LOG ��ũ�θ� ����� �α� ��ũ�θ� �ϳ� ����
#define RCN_LOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s] %s %s"), LOG_NETMODEINFO, LOG_CALLINFO ,*FString::Printf(Format, ##__VA_ARGS__))

// ��Ʈ��ũ ��Ƽ �÷��̾ ���õ� �α� ��ũ��
// �ҽ� �ڵ带 ������������ �� ��ũ�θ� ����� ���̱� ������
// ��� �ҽ� ������ ���� ������ �� �ִ� Project_RCN.h�� �ۼ�����
DECLARE_LOG_CATEGORY_EXTERN(LogRCNNetwrok, Log, All);