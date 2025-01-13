// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RCN_GameModeBase.generated.h"

class URCN_GameModeBaseDataAsset;
/**
 * 
 */
UCLASS()
class PROJECT_RCN_API ARCN_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARCN_GameModeBase();

protected:
// 로그인 관련 함수
	// 클라이언트의 접속 요청을 처리하는 함수
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	
	// 접속을 허용한 클라이언트에 대응하는 플레이어 컨트롤러를 만드는 함수
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	// 플레이어 입장을 위해 플레이어에 필요한 기본 설정을 모두 마무리하는 함수
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// 게임의 시작을 지시하는 함수
	virtual void StartPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URCN_GameModeBaseDataAsset> GameModeBaseDataAsset;
};
