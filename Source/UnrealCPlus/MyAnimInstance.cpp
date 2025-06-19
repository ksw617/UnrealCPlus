// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
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

	}
}
