// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RCN_Player.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actor/RCN_PlayerController.h"
#include "Actor/RCN_RubikCube.h"
#include "Camera/CameraComponent.h"
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

	HoldAction = PlayerDataAsset->HoldAction;
	RotateAction = PlayerDataAsset->RotateAction;
	ScrambleAction = PlayerDataAsset->ScrambleAction;
	SolveAction = PlayerDataAsset->SolveAction;
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

	EnhancedInputComponent->BindAction(HoldAction, ETriggerEvent::Triggered, this, &ARCN_Player::HoldTriggered);
	EnhancedInputComponent->BindAction(HoldAction, ETriggerEvent::Completed, this, &ARCN_Player::HoldCompleted);
	EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ARCN_Player::RotateCube);
	EnhancedInputComponent->BindAction(ScrambleAction, ETriggerEvent::Triggered, this, &ARCN_Player::ScrambleCube);
	EnhancedInputComponent->BindAction(SolveAction, ETriggerEvent::Triggered, this, &ARCN_Player::SolveCube);
}

void ARCN_Player::InitCube()
{
	NetworkRubikCube->AttachToComponent(YawComponent, FAttachmentTransformRules::KeepWorldTransform);
	NetworkRubikCube->SetActorRelativeLocation(FVector::ZeroVector);
	NetworkRubikCube->SetActorRelativeRotation(GetActorRotation());

	NetworkRubikCube->SpinDelegate.AddUObject(this, &ARCN_Player::SpinHandle);
	NetworkRubikCube->PatternChangedDelegate.AddUObject(this, &ARCN_Player::PatternChangedHandle);
	NetworkRubikCube->FinishScrambleDelegate.AddUObject(this, &ARCN_Player::FinishScrambleHandle);
}

void ARCN_Player::UpdateCubeLocation(const FVector& TargetLocation)
{
	const FVector CurrentLocation = PitchComponent->GetRelativeLocation();
	const FVector NewLocation = FMath::Lerp(CurrentLocation, TargetLocation, PlayerDataAsset->LocationSpeed);
	PitchComponent->SetRelativeLocation(NewLocation);

	RenewalRubikCubeLocationAndRotation();

	if (NewLocation.Equals(TargetLocation,PlayerDataAsset->LocationTolerance))
	{
		PitchComponent->SetRelativeLocation(TargetLocation);
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
	PitchComponent->SetRelativeRotation(FRotator(NewRotator.Pitch, 0.0f, 0.0f));
	YawComponent->SetRelativeRotation(FRotator(0.0f, NewRotator.Yaw, 0.0f));

	RenewalRubikCubeLocationAndRotation();

	if (NewRotator.Equals(TargetRotation, PlayerDataAsset->RotationTolerance))
	{
		PitchComponent->SetRelativeRotation(FRotator(TargetRotation.Pitch, 0.0f, 0.0f));
		YawComponent->SetRelativeRotation(FRotator(0.0f, TargetRotation.Yaw, 0.0f));
		return;
	}

	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		UpdateCubeRotation(TargetRotation);
	}));
}

void ARCN_Player::RenewalRubikCubeLocationAndRotation()
{
	ServerRPC_SetCubeLocation(PitchComponent->GetRelativeLocation());

	FRotator Rotator = FRotator::ZeroRotator;
	Rotator.Pitch = PitchComponent->GetRelativeRotation().Pitch;
	Rotator.Yaw = YawComponent->GetRelativeRotation().Yaw;
	
	ServerRPC_SetCubeRotation(Rotator);
}

void ARCN_Player::RenewalRubikCubePattern()
{
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

void ARCN_Player::HoldTriggered(const FInputActionValue& Value)
{
	bIsHolding = true;
}

void ARCN_Player::HoldCompleted(const FInputActionValue& Value)
{
	bIsHolding = false;
}

void ARCN_Player::RotateCube(const FInputActionValue& Value)
{
	if (!bIsHolding)
	{
		return;
	}
	
	FVector2D RotateAxisVector = Value.Get<FVector2D>();
	RotateAxisVector *= PlayerDataAsset->RotateSensitivity;

	FRotator Rotator;
	Rotator.Pitch = FMath::Clamp(PitchComponent->GetRelativeRotation().Pitch + RotateAxisVector.Y, -89.0f, 89.0f);
	Rotator.Yaw = YawComponent->GetRelativeRotation().Yaw + RotateAxisVector.X;
	
	PitchComponent->SetRelativeRotation(FRotator(Rotator.Pitch, 0.0f, 0.0f));
	YawComponent->SetRelativeRotation(FRotator(0.0f, Rotator.Yaw, 0.0f));

	RenewalRubikCubeLocationAndRotation();
}

void ARCN_Player::ScrambleCube(const FInputActionValue& Value)
{
	ServerRPC_ScrambleCube();
}

void ARCN_Player::SolveCube(const FInputActionValue& Value)
{
	ServerRPC_SolveCube();
}

void ARCN_Player::SpinHandle(const FString& Command)
{
	NetworkCommand = Command;
	bNetworkCommandFlag = !bNetworkCommandFlag;
}

void ARCN_Player::PatternChangedHandle(const FString& Pattern)
{
	NetworkPattern = Pattern;
	bNetworkPatternFlag = !bNetworkPatternFlag;
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

void ARCN_Player::ServerRPC_SetCubeLocation_Implementation(const FVector Location)
{
	//RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));
	
	MulticastRPC_SetCubeLocation(Location);
	
	//RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_Player::MulticastRPC_SetCubeLocation_Implementation(const FVector Location)
{
	//RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	PitchComponent->SetRelativeLocation(Location);
	
	//RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_Player::ServerRPC_SetCubeRotation_Implementation(const FRotator Rotator)
{
	//RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	MulticastRPC_SetCubeRotation(Rotator);

	//RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_Player::MulticastRPC_SetCubeRotation_Implementation(const FRotator Rotator)
{
	//RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	PitchComponent->SetRelativeRotation(FRotator(Rotator.Pitch, 0.0f, 0.0f));
	YawComponent->SetRelativeRotation(FRotator(0.0f, Rotator.Yaw, 0.0f));
	
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

void ARCN_Player::ServerRPC_FinishScramble_Implementation()
{
	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	if (ARCN_GameModeBase* GameModeBase = Cast<ARCN_GameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GameModeBase->FinishScramble();
	}

	RCN_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

