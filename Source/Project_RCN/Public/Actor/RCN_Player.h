// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interface/RCN_SetCubeInterface.h"
#include "RCN_Player.generated.h"

class URCN_PlayerDataAsset;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class UInputAction;

UCLASS()
class PROJECT_RCN_API ARCN_Player : public APawn, public IRCN_SetCubeInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARCN_Player();

	FORCEINLINE virtual void SetRubikCube(AActor* InRubikCube) override { RubikCube = InRubikCube; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Owner() override;
	virtual void PostNetInit() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	void SetControl() const;
	void HoldTriggered(const FInputActionValue& Value);
	void HoldCompleted(const FInputActionValue& Value);
	void Rotate(const FInputActionValue& Value);
	void Scramble();
	void Solve();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URCN_PlayerDataAsset> PlayerDataAsset;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	uint8 bIsHolding : 1;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> HoldAction;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> RotateAction;

	// 네트워크 로직
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnActorChannelOpen(FInBunch& InBunch, UNetConnection* Connection) override;
	
	UFUNCTION(Server, Unreliable)
	void ServerRPC_Rotate(FVector2D RotateAxisVector);

	UFUNCTION(NetMulticast, Unreliable)
	void MultiRPC_Rotate(FVector2D RotateAxisVector);

	UFUNCTION(Server, Unreliable)
	void ServerRPC_Scramble(FString Command);

	UFUNCTION(NetMulticast, Unreliable)
	void MultiRPC_Scramble(FString Command);

	UFUNCTION(Server, Unreliable)
	void ServerRPC_Solve(FString Command);

	UFUNCTION(NetMulticast, Unreliable)
	void MultiRPC_Solve(FString Command);

	UPROPERTY(Replicated)
	TObjectPtr<AActor> RubikCube;
};
