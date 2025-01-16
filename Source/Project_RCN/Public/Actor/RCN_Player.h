// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RCN_Player.generated.h"

class ARCN_RubikCube;
class URCN_PlayerDataAsset;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class UInputAction;

UCLASS()
class PROJECT_RCN_API ARCN_Player : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARCN_Player();

	FORCEINLINE ARCN_RubikCube* GetRubikCube() const { return NetworkRubikCube; }

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

	void SetRubikCube(ARCN_RubikCube* InRubikCube);
	void RenewalRubikCubeLocationAndRotation();
	
protected:
	void SetControl() const;
	void HoldTriggered(const FInputActionValue& Value);
	void HoldCompleted(const FInputActionValue& Value);
	void RotateCube(const FInputActionValue& Value);
	void ScrambleCube(const FInputActionValue& Value);
	void SolveCube(const FInputActionValue& Value);
	
	UFUNCTION()
	void SpinStartHandle(const FString& Command);

	UFUNCTION()
	void SpinEndHandle(const FString& Pattern);

	UFUNCTION()
	void FinishScrambleHandle();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URCN_PlayerDataAsset> PlayerDataAsset;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> PitchComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> YawComponent;

	UPROPERTY(VisibleAnywhere)
	uint8 bIsHolding : 1;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> HoldAction;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> RotateAction;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> ScrambleAction;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> SolveAction;

	// 네트워크 로직
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnActorChannelOpen(FInBunch& InBunch, UNetConnection* Connection) override;

	UFUNCTION()
	void OnRep_SpinStart() const;

	UFUNCTION()
	void OnRep_SpinEnd() const;
	
	UFUNCTION(Server, Unreliable)
	void ServerRPC_SetCubeRotation(FRotator Rotator);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPC_SetCubeRotation(FRotator Rotator);

	UFUNCTION(Server, Unreliable)
	void ServerRPC_SetCubeLocation(FVector Location);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPC_SetCubeLocation(FVector Location);

	UFUNCTION(Server, Reliable)
	void ServerRPC_ScrambleCube();
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_SolveCube();

	UPROPERTY(Replicated)
	TObjectPtr<ARCN_RubikCube> NetworkRubikCube;

	UPROPERTY(Replicated)
	FString NetworkCommand;

	UPROPERTY(Replicated)
	FString NetworkPattern;

	UPROPERTY(ReplicatedUsing = OnRep_SpinStart)
	uint8 bNetworkSpinStartFlag : 1;

	UPROPERTY(ReplicatedUsing = OnRep_SpinEnd)
	uint8 bNetworkSpinEndFlag : 1;
};
