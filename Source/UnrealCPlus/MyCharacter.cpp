// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MyAnimInstance.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Arrow.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSparrow/Characters/Heroes/Sparrow/Meshes/Sparrow.Sparrow'"));

	if (SM.Succeeded())
	{

		GetMesh()->SetSkeletalMesh(SM.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));

	}

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	CameraBoom->SetupAttachment(RootComponent);
	FollowCamera->SetupAttachment(CameraBoom);

	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.f, 120.f, 75.f); // ����

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
										  //UAnimInstance*
	AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);


		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AMyCharacter::Fire);

	}
}



void AMyCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMyCharacter::Fire(const FInputActionValue& Value)
{


	if (IsValid(AnimInstance))
	{
		if (!IsShooting)
		{
			IsShooting = true;

			AnimInstance->PlayAttackMontage();

			float AttackRange = 10000.f;

			FHitResult HitResult;

			//auto
			APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);

			FVector AimLocation = CameraManager->GetCameraLocation();
			FVector TargetLocation = AimLocation + CameraManager->GetActorForwardVector() * AttackRange;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);

			bool Result = GetWorld()->LineTraceSingleByChannel
			(
				OUT HitResult,
				AimLocation,
				TargetLocation,
				ECollisionChannel::ECC_GameTraceChannel1,
				Params

			);

			if (Result)
			{
				TargetLocation = HitResult.ImpactPoint;
				DrawDebugLine(GetWorld(), AimLocation, TargetLocation, FColor::Green, true);
			}
			else
			{
				DrawDebugLine(GetWorld(), AimLocation, TargetLocation, FColor::Red, true);
			}


			FTransform SocketTransform = GetMesh()->GetSocketTransform(FName("ArrowSocket"));
			SocketLocation = SocketTransform.GetLocation();
			FVector DeltaVector = TargetLocation - SocketLocation;
			SocketRotation = FRotationMatrix::MakeFromX(DeltaVector).Rotator();
		}

	}
}

void AMyCharacter::PlayerAttack()
{

	FActorSpawnParameters params;
	params.Owner = this;
	params.Instigator = this;

	auto MyArrow = GetWorld()->SpawnActor<AArrow>(SocketLocation, SocketRotation, params);
}

void AMyCharacter::PlayerReload()
{
	IsShooting = false;
}

