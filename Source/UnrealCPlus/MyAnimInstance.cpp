// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UMyAnimInstance::UMyAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("/Script/Engine.AnimMontage'/Game/ParagonSparrow/Characters/Heroes/Sparrow/Animations/Primary_Fire_Med_Montage.Primary_Fire_Med_Montage'"));

	if (AM.Succeeded())
	{
		AttackMontage = AM.Object;
	}
}

void UMyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		MyCharacter = Cast<AMyCharacter>(Pawn);
		if (IsValid(MyCharacter))
		{
			CharacterMovement = MyCharacter->GetCharacterMovement();
		}
	}


}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(CharacterMovement))
	{
		Velocity = CharacterMovement->Velocity;
		FRotator Rotation = MyCharacter->GetActorRotation();
		FVector UnrotateVector = Rotation.UnrotateVector(Velocity);

		UnrotateVector.Normalize();

		Vertical = UnrotateVector.X;
		Horizontal = UnrotateVector.Y;

		GroundSpeed = Velocity.Size2D();

		auto Acceleration = CharacterMovement->GetCurrentAcceleration();

		ShouldMove = GroundSpeed >= 3.0 && Acceleration != FVector::Zero();

		IsFalling = CharacterMovement->IsFalling();

	}
}

void UMyAnimInstance::PlayAttackMontage()
{
	UE_LOG(LogTemp, Log, TEXT("PlayAttackMontage"));
	if (IsValid(AttackMontage))
	{
		UE_LOG(LogTemp, Log, TEXT("IsValid"));
		if (!Montage_IsPlaying(AttackMontage))
		{
			UE_LOG(LogTemp, Log, TEXT("Is Not Playing"));
			Montage_Play(AttackMontage);
		}

	}
}

void UMyAnimInstance::AnimNotify_Hit()
{
	MyCharacter->PlayerAttack();
}
