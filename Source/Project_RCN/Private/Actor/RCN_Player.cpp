// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RCN_Player.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actor/RCN_PlayerController.h"
#include "Actor/RCN_RubikCube.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Data/RCN_PlayerDataAsset.h"
#include "Game/RCN_GameModeBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Project_RCN/Project_RCN.h"

// Sets default values
ARCN_Player::ARCN_Player()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<URCN_PlayerDataAsset> PlayerDataAssetRef(TEXT("/Script/Project_RCN.RCN_PlayerDataAsset'/Game/Data/DA_Player.DA_Player'"));
	if (PlayerDataAssetRef.Object)
	{
		PlayerDataAsset = PlayerDataAssetRef.Object;
	}
	else
	{
		RCN_LOG(LogNetwork, Error, TEXT("데이터 에셋 로드 실패"))
		return;
	}

	DefaultComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultComponent"));
	RootComponent = DefaultComponent;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.0f;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	
	PitchComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PitchComponent"));
	PitchComponent->SetupAttachment(RootComponent);

	YawComponent = CreateDefaultSubobject<USceneComponent>(TEXT("YawComponent"));
	YawComponent->SetupAttachment(PitchComponent);

	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->SetupAttachment(RootComponent);
	SceneCaptureComponent->TextureTarget = PlayerDataAsset->RenderTarget;

	RenderTarget = PlayerDataAsset->RenderTarget;
}

// Called when the game starts or when spawned
void ARCN_Player::BeginPlay()
{
	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));
	
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		EnableInput(PlayerController);
	}

	SetControl();

	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_Player::PossessedBy(AController* NewController)
{
	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	AActor* OwnerActor = GetOwner();
	if (IsValid(OwnerActor))
	{
		RCN_LOG(LogNetwork, Log, TEXT("오너 : %s"), *OwnerActor->GetName());
	}
	else
	{
		RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("오너가 없음."));
	}

	Super::PossessedBy(NewController);

	OwnerActor = GetOwner();
	if (IsValid(OwnerActor))
	{
		RCN_LOG(LogNetwork, Log, TEXT("오너 : %s"), *OwnerActor->GetName());
	}
	else
	{
		RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("오너가 없음."));
	}

	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_Player::OnRep_Owner()
{
	RCN_LOG(LogNetwork, Log, TEXT("%s %s"), *GetName(), TEXT("Begin"));

	Super::OnRep_Owner();

	AActor* OwnerActor = GetOwner();
	if (IsValid(OwnerActor))
	{
		RCN_LOG(LogNetwork, Log, TEXT("오너 : %s"), *OwnerActor->GetName());
	}
	else
	{
		RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("오너가 없음."));
	}

	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_Player::PostNetInit()
{
	RCN_LOG(LogNetwork, Log, TEXT("%s %s"), TEXT("Begin"), *GetName());

	Super::PostNetInit();

	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

// Called every frame
void ARCN_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARCN_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(PlayerDataAsset->RotateSwitchAction, ETriggerEvent::Started, this, &ARCN_Player::RotateSwitchStarted);
	EnhancedInputComponent->BindAction(PlayerDataAsset->RotateSwitchAction, ETriggerEvent::Completed, this, &ARCN_Player::RotateSwitchCompleted);
	EnhancedInputComponent->BindAction(PlayerDataAsset->RotateAction, ETriggerEvent::Triggered, this, &ARCN_Player::RotateCube);
	EnhancedInputComponent->BindAction(PlayerDataAsset->ScrambleAction, ETriggerEvent::Started, this, &ARCN_Player::ScrambleCube);
	EnhancedInputComponent->BindAction(PlayerDataAsset->SolveAction, ETriggerEvent::Started, this, &ARCN_Player::SolveCube);
	EnhancedInputComponent->BindAction(PlayerDataAsset->SpinDragAction, ETriggerEvent::Started, this, &ARCN_Player::SpinDragStarted);
	EnhancedInputComponent->BindAction(PlayerDataAsset->SpinDragAction, ETriggerEvent::Triggered, this, &ARCN_Player::SpinDragTriggered);
	EnhancedInputComponent->BindAction(PlayerDataAsset->SpinDragAction, ETriggerEvent::Completed, this, &ARCN_Player::SpinDragCompleted);
	EnhancedInputComponent->BindAction(PlayerDataAsset->SpinInputUpAction, ETriggerEvent::Triggered, this, &ARCN_Player::SpinInput);
	EnhancedInputComponent->BindAction(PlayerDataAsset->SpinInputLeftAction, ETriggerEvent::Triggered, this, &ARCN_Player::SpinInput);
	EnhancedInputComponent->BindAction(PlayerDataAsset->SpinInputDownAction, ETriggerEvent::Triggered, this, &ARCN_Player::SpinInput);
	EnhancedInputComponent->BindAction(PlayerDataAsset->SpinInputRightAction, ETriggerEvent::Triggered, this, &ARCN_Player::SpinInput);
}

void ARCN_Player::InitCube()
{
	NetworkRubikCube->AttachToComponent(YawComponent, FAttachmentTransformRules::KeepWorldTransform);
	NetworkRubikCube->SetActorRelativeLocation(FVector::ZeroVector);
	NetworkRubikCube->SetActorRelativeRotation(GetActorRotation());

	NetworkRubikCube->SpinStartDelegate.AddUObject(this, &ARCN_Player::SpinStartHandle);
	NetworkRubikCube->SpinEndDelegate.AddUObject(this, &ARCN_Player::SpinEndHandle);
	NetworkRubikCube->FinishScrambleDelegate.AddUObject(this, &ARCN_Player::FinishScrambleHandle);
}

void ARCN_Player::UpdateCubeLocation(const FVector& TargetLocation)
{
	const FVector CurrentLocation = PitchComponent->GetRelativeLocation();
	const FVector NewLocation = FMath::Lerp(CurrentLocation, TargetLocation, PlayerDataAsset->LocationSpeed);
	MulticastRPC_SetCubeLocation(NewLocation);

	if (NewLocation.Equals(TargetLocation, PlayerDataAsset->LocationTolerance))
	{
		MulticastRPC_SetCubeLocation(TargetLocation);
		return;
	}

	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		UpdateCubeLocation(TargetLocation);
	}));
}

void ARCN_Player::UpdateCubeRotation(const FRotator& TargetRotation)
{
	const FRotator CurrentRotator = FRotator(PitchComponent->GetRelativeRotation().Pitch, YawComponent->GetRelativeRotation().Yaw, 0.0f);
	const FRotator NewRotator = FMath::Lerp(CurrentRotator, TargetRotation, PlayerDataAsset->RotationSpeed);
	MulticastRPC_SetCubeRotation(NewRotator);

	if (NewRotator.Equals(TargetRotation, PlayerDataAsset->RotationTolerance))
	{
		MulticastRPC_SetCubeRotation(TargetRotation);
		return;
	}

	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		UpdateCubeRotation(TargetRotation);
	}));
}

void ARCN_Player::RenewalCube()
{
	MulticastRPC_SetCubeLocation(PitchComponent->GetRelativeLocation());

	FRotator Rotator = FRotator::ZeroRotator;
	Rotator.Pitch = PitchComponent->GetRelativeRotation().Pitch;
	Rotator.Yaw = YawComponent->GetRelativeRotation().Yaw;
	
	MulticastRPC_SetCubeRotation(Rotator);

	NetworkPattern = NetworkRubikCube->GetPattern();
	bNetworkPatternFlag = !bNetworkPatternFlag;
}

void ARCN_Player::SetControl() const
{
	if (!IsLocallyControlled())
	{
		return;
	}

	const APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		EnhancedInputLocalPlayerSubsystem->ClearAllMappings();
		const UInputMappingContext* InputMappingContext = PlayerDataAsset->InputMappingContext;
		if (PlayerDataAsset->InputMappingContext)
		{
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void ARCN_Player::RotateSwitchStarted(const FInputActionValue& Value)
{
	bRotateSwitchStarted = true;
}

void ARCN_Player::RotateSwitchCompleted(const FInputActionValue& Value)
{
	bRotateSwitchStarted = false;
}

void ARCN_Player::RotateCube(const FInputActionValue& Value)
{
	if (!bRotateSwitchStarted)
	{
		return;
	}
	
	FVector2D RotateAxisVector = Value.Get<FVector2D>();
	RotateAxisVector *= PlayerDataAsset->RotateSensitivity;
	
	ServerRPC_RotateCube(RotateAxisVector);
}

void ARCN_Player::ScrambleCube(const FInputActionValue& Value)
{
	ServerRPC_ScrambleCube();
}

void ARCN_Player::SolveCube(const FInputActionValue& Value)
{
	ServerRPC_SolveCube();
}

void ARCN_Player::SpinDragStarted(const FInputActionValue& Value)
{
	if (const ARCN_PlayerController* PlayerController = CastChecked<ARCN_PlayerController>(GetController()))
	{
		FVector CursorLocation, CursorDirection;
		if (PlayerController->DeprojectMousePositionToWorld(CursorLocation, CursorDirection))
		{
			const FVector TraceStart = CursorLocation;
			const FVector TraceEnd = CursorLocation + CursorDirection * 10000.0f;

			FHitResult HitResult;
			FCollisionQueryParams Params;
			if (GetWorld()->LineTraceSingleByChannel(
				HitResult,
				TraceStart,
				TraceEnd,
				ECC_Visibility,
				Params))
			{
				if (UBoxComponent* ButtonBoxComponent = Cast<UBoxComponent>(HitResult.GetComponent()))
				{
					SelectedButtonBoxComponent = ButtonBoxComponent;
					DragStartHitLocation = HitResult.Location;
				}
			}
		}
	}
}

void ARCN_Player::SpinDragTriggered(const FInputActionValue& Value)
{
	if (!IsValid(SelectedButtonBoxComponent))
	{
		return;
	}
	
	FVector SelectedButtonPosition = NetworkRubikCube->GetButtonPosition(SelectedButtonBoxComponent);
	if (SelectedButtonPosition == FVector::ZeroVector)
	{
		return;
	}
	
	UBoxComponent* CurrentBoxComponent;
	FVector CurrentHitLocation;
	if (const ARCN_PlayerController* PlayerController = CastChecked<ARCN_PlayerController>(GetController()))
	{
		FVector CursorLocation, CursorDirection;
		if (PlayerController->DeprojectMousePositionToWorld(CursorLocation, CursorDirection))
		{
			const FVector TraceStart = CursorLocation;
			const FVector TraceEnd = CursorLocation + CursorDirection * 10000.0f;

			FHitResult HitResult;
			FCollisionQueryParams Params;
			if (GetWorld()->LineTraceSingleByChannel(
				HitResult,
				TraceStart,
				TraceEnd,
				ECC_Visibility,
				Params))
			{
				CurrentBoxComponent = Cast<UBoxComponent>(HitResult.GetComponent());
				CurrentHitLocation = HitResult.Location;
			}
		}
	}

	if (CurrentBoxComponent != SelectedButtonBoxComponent)
	{
		FVector DragDirection = (DragEndHitLocation - DragStartHitLocation).GetSafeNormal();
		FVector SpinDirection = GetClosestSpinDirection(SelectedButtonPosition, DragDirection);
		ServerRPC_SpinCube(SelectedButtonPosition, SpinDirection);

		SelectedButtonBoxComponent = nullptr;
	}

	DragEndHitLocation = CurrentHitLocation;
}

void ARCN_Player::SpinDragCompleted(const FInputActionValue& Value)
{
	SelectedButtonBoxComponent = nullptr;
}

void ARCN_Player::SpinInput(const FInputActionValue& Value)
{
	FVector SelectedButtonPosition;
	FVector InputStartHitLocation;
	float HitDistance;
	if (const ARCN_PlayerController* PlayerController = CastChecked<ARCN_PlayerController>(GetController()))
	{
		FVector CursorLocation, CursorDirection;
		if (PlayerController->DeprojectMousePositionToWorld(CursorLocation, CursorDirection))
		{
			const FVector TraceStart = CursorLocation;
			const FVector TraceEnd = CursorLocation + CursorDirection * 10000.0f;

			FHitResult HitResult;
			FCollisionQueryParams Params;
			if (GetWorld()->LineTraceSingleByChannel(
				HitResult,
				TraceStart,
				TraceEnd,
				ECC_Visibility,
				Params))
			{
				if (UBoxComponent* ButtonBoxComponent = Cast<UBoxComponent>(HitResult.GetComponent()))
				{
					SelectedButtonPosition = NetworkRubikCube->GetButtonPosition(ButtonBoxComponent);
					InputStartHitLocation = HitResult.Location;
					HitDistance = HitResult.Distance;
				}
			}
		}
	}

	if (SelectedButtonPosition == FVector::ZeroVector)
	{
		return;
	}

	FVector SpinDirection;
	if (const ARCN_PlayerController* PlayerController = CastChecked<ARCN_PlayerController>(GetController()))
	{
		FVector2D TargetPosition;
		if (PlayerController->GetMousePosition(TargetPosition.X, TargetPosition.Y))
		{
			TargetPosition += Value.Get<FVector2D>();
			
			FVector TargetLocation, TargetDirection;
			PlayerController->DeprojectScreenPositionToWorld(TargetPosition.X, TargetPosition.Y, TargetLocation, TargetDirection);

			FVector InputEndHitLocation = TargetLocation + TargetDirection * HitDistance;
			FVector InputDirection = (InputEndHitLocation - InputStartHitLocation).GetSafeNormal();

			SpinDirection = GetClosestSpinDirection(SelectedButtonPosition, InputDirection);
		}
	}

	ServerRPC_SpinCube(SelectedButtonPosition, SpinDirection);
}

void ARCN_Player::SpinInputNext()
{
	if (!SelectedButtonPositionQueue.IsEmpty() && !SpinDirectionQueue.IsEmpty())
	{
		FVector SelectedButtonPosition;
		FVector SpinDirection;
		SelectedButtonPositionQueue.Dequeue(SelectedButtonPosition);
		SpinDirectionQueue.Dequeue(SpinDirection);
	
		SpinCube(SelectedButtonPosition, SpinDirection);
	}
}

FVector ARCN_Player::GetClosestSpinDirection(const FVector& SelectedButtonPosition, const FVector& Direction) const
{
	TMap<FVector, FVector> CubeVectors;
	if (FMath::Abs(SelectedButtonPosition.X) != 2)
	{
		CubeVectors.Emplace(FVector(1, 0, 0), NetworkRubikCube->GetActorForwardVector());
		CubeVectors.Emplace(FVector(-1, 0, 0), NetworkRubikCube->GetActorForwardVector() * -1);
	}
		
	if (FMath::Abs(SelectedButtonPosition.Y) != 2)
	{
		CubeVectors.Emplace(FVector(0, 1, 0), NetworkRubikCube->GetActorRightVector());
		CubeVectors.Emplace(FVector(0, -1, 0), NetworkRubikCube->GetActorRightVector() * -1);
	}
		
	if (FMath::Abs(SelectedButtonPosition.Z) != 2)
	{
		CubeVectors.Emplace(FVector(0, 0, 1), NetworkRubikCube->GetActorUpVector());
		CubeVectors.Emplace(FVector(0, 0, -1), NetworkRubikCube->GetActorUpVector() * -1);
	}

	FVector SpinDirection = FVector::ZeroVector;
	float MaxDot = -1.0f;
	for (auto CubeVector : CubeVectors)
	{
		float Dot = Direction.Dot(CubeVector.Value);
		if (Dot > MaxDot)
		{
			MaxDot = Dot;
			SpinDirection = CubeVector.Key;
		}
	}

	return SpinDirection;
}

void ARCN_Player::SpinCube(const FVector& SelectedButtonPosition, const FVector& SpinDirection) const
{
	const FVector NormalVector = FVector(
			FMath::Abs(SelectedButtonPosition.X) == 2 ? SelectedButtonPosition.X / 2.0f : 0,
			FMath::Abs(SelectedButtonPosition.Y) == 2 ? SelectedButtonPosition.Y / 2.0f : 0,
			FMath::Abs(SelectedButtonPosition.Z) == 2 ? SelectedButtonPosition.Z / 2.0f : 0
			);
	const FVector Cross = SpinDirection.Cross(NormalVector).GetSafeNormal();

	FString Command;
	if (FMath::Abs(Cross.X) == 1)
	{
		if (SelectedButtonPosition.X == 1)
		{
			Command.Append(TEXT("R")) += Cross.X > 0.0f ? TEXT("'") : TEXT("");
		}
		else if (SelectedButtonPosition.X == 0)
		{
			Command.Append(TEXT("M")) += Cross.X > 0.0f ? TEXT("") : TEXT("'");
		}
		else if (SelectedButtonPosition.X == -1)
		{
			Command.Append(TEXT("L")) += Cross.X > 0.0f ? TEXT("") : TEXT("'");
		}
	}
	else if (FMath::Abs(Cross.Y) == 1)
	{
		if (SelectedButtonPosition.Y == 1)
		{
			Command.Append(TEXT("F")) += Cross.Y > 0.0f ? TEXT("'") : TEXT("");
		}
		else if (SelectedButtonPosition.Y == 0)
		{
			Command.Append(TEXT("S")) += Cross.Y > 0.0f ? TEXT("'") : TEXT("");
		}
		else if (SelectedButtonPosition.Y == -1)
		{
			Command.Append(TEXT("B")) += Cross.Y > 0.0f ? TEXT("") : TEXT("'");
		}
	}
	else if (FMath::Abs(Cross.Z) == 1)
	{
		if (SelectedButtonPosition.Z == 1)
		{
			Command.Append(TEXT("U")) += Cross.Z > 0.0f ? TEXT("'") : TEXT("");
		}
		else if (SelectedButtonPosition.Z == 0)
		{
			Command.Append(TEXT("E")) += Cross.Z > 0.0f ? TEXT("") : TEXT("'");
		}
		else if (SelectedButtonPosition.Z == -1)
		{
			Command.Append(TEXT("D")) += Cross.Z > 0.0f ? TEXT("") : TEXT("'");
		}
	}
	
	NetworkRubikCube->Spin(Command);
}

void ARCN_Player::SpinStartHandle(const FString& Command)
{
	NetworkCommand = Command;
	bNetworkCommandFlag = !bNetworkCommandFlag;
}

void ARCN_Player::SpinEndHandle(const FString& Pattern)
{
	NetworkPattern = Pattern;
	bNetworkPatternFlag = !bNetworkPatternFlag;

	SpinInputNext();
}

void ARCN_Player::FinishScrambleHandle()
{
	ServerRPC_FinishScramble();
}

void ARCN_Player::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARCN_Player, NetworkRubikCube)
	DOREPLIFETIME(ARCN_Player, NetworkCommand)
	DOREPLIFETIME(ARCN_Player, bNetworkCommandFlag)
	DOREPLIFETIME(ARCN_Player, NetworkPattern)
	DOREPLIFETIME(ARCN_Player, bNetworkPatternFlag)
}

void ARCN_Player::OnActorChannelOpen(FInBunch& InBunch, UNetConnection* Connection)
{
	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));
	
	Super::OnActorChannelOpen(InBunch, Connection);
	
	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_Player::OnRep_Command() const
{
	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	if (IsValid(NetworkRubikCube))
	{
		NetworkRubikCube->Spin(NetworkCommand);
	}
	
	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_Player::OnRep_Pattern() const
{
	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));
	
	if (IsValid(NetworkRubikCube))
	{
		NetworkRubikCube->ChangePattern(NetworkPattern);
	}

	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_Player::MulticastRPC_SetCubeLocation_Implementation(const FVector Location)
{
	//RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	PitchComponent->SetRelativeLocation(Location);
	
	//RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_Player::MulticastRPC_SetCubeRotation_Implementation(const FRotator Rotator)
{
	//RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	PitchComponent->SetRelativeRotation(FRotator(Rotator.Pitch, 0.0f, 0.0f));
	YawComponent->SetRelativeRotation(FRotator(0.0f, Rotator.Yaw, 0.0f));
	
	//RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_Player::ServerRPC_RotateCube_Implementation(const FVector2D RotateAxisVector)
{
	//RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	FRotator Rotator = FRotator::ZeroRotator;
	Rotator.Pitch = FMath::Clamp(PitchComponent->GetRelativeRotation().Pitch + RotateAxisVector.Y, -89.0f, 89.0f);
	Rotator.Yaw = YawComponent->GetRelativeRotation().Yaw + RotateAxisVector.X;

	MulticastRPC_SetCubeRotation(Rotator);
	
	//RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_Player::ServerRPC_ScrambleCube_Implementation()
{
	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	NetworkRubikCube->Scramble();
	
	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_Player::ServerRPC_SolveCube_Implementation()
{
	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));
	
	NetworkRubikCube->Solve();

	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_Player::ServerRPC_SpinCube_Implementation(const FVector& SelectedButtonPosition, const FVector& SpinDirection)
{
	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	if (SelectedButtonPositionQueue.IsEmpty() || SpinDirectionQueue.IsEmpty())
	{
		SelectedButtonPositionQueue.Enqueue(SelectedButtonPosition);
		SpinDirectionQueue.Enqueue(SpinDirection);
		
		SpinInputNext();
	}
	else
	{
		SelectedButtonPositionQueue.Enqueue(SelectedButtonPosition);
		SpinDirectionQueue.Enqueue(SpinDirection);
	}
	
	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_Player::ServerRPC_FinishScramble_Implementation()
{
	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	if (ARCN_GameModeBase* GameModeBase = Cast<ARCN_GameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GameModeBase->FinishScramble();
	}

	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

