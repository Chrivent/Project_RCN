// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RCN_Player.generated.h"

class UBoxComponent;
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

	FORCEINLINE void SetRubikCube(ARCN_RubikCube* InRubikCube) { NetworkRubikCube = InRubikCube; }
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

	void InitCube();
	void UpdateCubeLocation(const FVector& TargetLocation);
	void UpdateCubeRotation(const FRotator& TargetRotation);
	void RenewalRubikCubeLocationAndRotation();
	void RenewalRubikCubePattern();
	
protected:
	void SetControl() const;
	void RotateSwitchStarted(const FInputActionValue& Value);
	void RotateSwitchCompleted(const FInputActionValue& Value);
	void RotateCube(const FInputActionValue& Value);
	void ScrambleCube(const FInputActionValue& Value);
	void SolveCube(const FInputActionValue& Value);
	void SpinDragStarted(const FInputActionValue& Value);
	void SpinDragTriggered(const FInputActionValue& Value);
	void SpinDragCompleted(const FInputActionValue& Value);
	void SpinInput(const FInputActionValue& Value);

	FVector GetClosestSpinDirection(const FVector& SelectedButtonPosition, const FVector& Direction) const;
	void SpinCube(const FVector& SelectedButtonPosition, const FVector& SpinDirection);
	
	UFUNCTION()
	void SpinHandle(const FString& Command);

	UFUNCTION()
	void PatternChangedHandle(const FString& Pattern);

	UFUNCTION()
	void FinishScrambleHandle();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URCN_PlayerDataAsset> PlayerDataAsset;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> PitchComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> YawComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> SelectedButtonBoxComponent;

	UPROPERTY(VisibleAnywhere)
	FVector DragStartHitLocation;

	UPROPERTY(VisibleAnywhere)
	FVector DragEndHitLocation;

	UPROPERTY(VisibleAnywhere)
	uint8 bRotateSwitchStarted : 1;

	// 네트워크 로직
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnActorChannelOpen(FInBunch& InBunch, UNetConnection* Connection) override;

	UFUNCTION()
	void OnRep_Command() const;

	UFUNCTION()
	void OnRep_Pattern() const;
	
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

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpinCube(const FString& Command);

	UFUNCTION(Server, Reliable)
	void ServerRPC_FinishScramble();

	UPROPERTY(Replicated)
	TObjectPtr<ARCN_RubikCube> NetworkRubikCube;

	UPROPERTY(Replicated)
	FString NetworkCommand;

	UPROPERTY(ReplicatedUsing = OnRep_Command)
	uint8 bNetworkCommandFlag : 1;

	UPROPERTY(Replicated)
	FString NetworkPattern;
	
	UPROPERTY(ReplicatedUsing = OnRep_Pattern)
    uint8 bNetworkPatternFlag : 1;
};

