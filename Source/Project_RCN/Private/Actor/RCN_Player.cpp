// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RCN_Player.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actor/RCN_RubikCube.h"
#include "Camera/CameraComponent.h"
#include "Data/RCN_PlayerDataAsset.h"
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
		RCN_LOG(LogRCNNetwork, Error, TEXT("데이터 에셋 로드 실패"))
		return;
	}

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.0f;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	RotateAction = PlayerDataAsset->RotateAction;
	HoldAction = PlayerDataAsset->HoldAction;
}

// Called when the game starts or when spawned
void ARCN_Player::BeginPlay()
{
	RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("Begin"));
	
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		EnableInput(PlayerController);
	}

	SetControl();

	if (HasAuthority())
	{
		FTimerHandle TestTimerHandle;
		GetWorldTimerManager().SetTimer(TestTimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]
		{
			Scramble();

			FTimerHandle TestTimerHandle2;
			GetWorldTimerManager().SetTimer(TestTimerHandle2, FTimerDelegate::CreateWeakLambda(this, [=, this]
			{
				Solve();
			}), 4.0f, false);
		}), 8.0f, true);
	}

	RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_Player::PossessedBy(AController* NewController)
{
	RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("Begin"));

	AActor* OwnerActor = GetOwner();
	if (IsValid(OwnerActor))
	{
		RCN_LOG(LogRCNNetwork, Log, TEXT("오너 : %s"), *OwnerActor->GetName());
	}
	else
	{
		RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("오너가 없음."));
	}

	Super::PossessedBy(NewController);

	OwnerActor = GetOwner();
	if (IsValid(OwnerActor))
	{
		RCN_LOG(LogRCNNetwork, Log, TEXT("오너 : %s"), *OwnerActor->GetName());
	}
	else
	{
		RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("오너가 없음."));
	}

	RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_Player::OnRep_Owner()
{
	RCN_LOG(LogRCNNetwork, Log, TEXT("%s %s"), *GetName(), TEXT("Begin"));

	Super::OnRep_Owner();

	AActor* OwnerActor = GetOwner();
	if (IsValid(OwnerActor))
	{
		RCN_LOG(LogRCNNetwork, Log, TEXT("오너 : %s"), *OwnerActor->GetName());
	}
	else
	{
		RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("오너가 없음."));
	}

	RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_Player::PostNetInit()
{
	RCN_LOG(LogRCNNetwork, Log, TEXT("%s %s"), TEXT("Begin"), *GetName());

	Super::PostNetInit();

	RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("End"));
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

	EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ARCN_Player::Rotate);
	EnhancedInputComponent->BindAction(HoldAction, ETriggerEvent::Triggered, this, &ARCN_Player::HoldTriggered);
	EnhancedInputComponent->BindAction(HoldAction, ETriggerEvent::Completed, this, &ARCN_Player::HoldCompleted);
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

void ARCN_Player::Rotate(const FInputActionValue& Value)
{
	if (!bIsHolding)
	{
		return;
	}
	
	FVector2D RotateAxisVector = Value.Get<FVector2D>();
	RotateAxisVector *= PlayerDataAsset->RotateSensitivity;

	ServerRPC_Rotate(RotateAxisVector);
}

void ARCN_Player::Scramble()
{
	ServerRPC_Scramble();
}

void ARCN_Player::Solve()
{
	ServerRPC_Solve();
}

void ARCN_Player::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARCN_Player, RubikCube);
}

void ARCN_Player::OnActorChannelOpen(FInBunch& InBunch, UNetConnection* Connection)
{
	RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("Begin"));
	
	Super::OnActorChannelOpen(InBunch, Connection);
	
	RCN_LOG(LogRCNNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ARCN_Player::ServerRPC_Rotate_Implementation(const FVector2D RotateAxisVector)
{
	RubikCube->Rotate(RotateAxisVector);
	
	ClientRPC_Rotate(RotateAxisVector);
}

void ARCN_Player::ClientRPC_Rotate_Implementation(const FVector2D RotateAxisVector)
{
	RubikCube->Rotate(RotateAxisVector);
}

void ARCN_Player::ServerRPC_Scramble_Implementation()
{
	RubikCube->Scramble();
	
	ClientRPC_Scramble();
}

void ARCN_Player::ClientRPC_Scramble_Implementation()
{
	RubikCube->Scramble();
}

void ARCN_Player::ServerRPC_Solve_Implementation()
{
	RubikCube->Solve();
	
	ClientRPC_Solve();
}

void ARCN_Player::ClientRPC_Solve_Implementation()
{
	RubikCube->Solve();
}

