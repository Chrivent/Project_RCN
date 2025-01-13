// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RCN_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RCN_API ARCN_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARCN_GameModeBase();

// �α��� ���� �Լ�
	// Ŭ���̾�Ʈ�� ���� ��û�� ó���ϴ� �Լ�
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	
	// ������ ����� Ŭ���̾�Ʈ�� �����ϴ� �÷��̾� ��Ʈ�ѷ��� ����� �Լ�
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	// �÷��̾� ������ ���� �÷��̾ �ʿ��� �⺻ ������ ��� �������ϴ� �Լ�
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// ������ ������ �����ϴ� �Լ�
	virtual void StartPlay() override;
};
