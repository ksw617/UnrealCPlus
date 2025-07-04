// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "kismet/kismetMathLibrary.h"

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
		GroundSpeed = Velocity.Size2D();

		auto Acceleration = CharacterMovement->GetCurrentAcceleration();

		ShouldMove = GroundSpeed >= 3.0 && Acceleration != FVector::Zero();


		FRotator Rotation = MyCharacter->GetActorRotation();
		IsFalling = CharacterMovement->IsFalling();

		 AimRotation = MyCharacter->GetBaseAimRotation();

		 FRotator RotFromX = UKismetMathLibrary::MakeRotFromX(Velocity);

		 FRotator DeltaRotation = AimRotation - RotFromX;
		 DeltaRotation.Normalize();

		 YawOffset = DeltaRotation.Yaw;



	}
}

void UMyAnimInstance::PlayAttackMontage()
{
	if (IsValid(AttackMontage))
	{
		if (!Montage_IsPlaying(AttackMontage))
		{
			Montage_Play(AttackMontage);
		}

	}
}

void UMyAnimInstance::AnimNotify_SaveAttack()
{
	MyCharacter->PlayerReload();
}

void UMyAnimInstance::AnimNotify_Shoot()
{
	MyCharacter->PlayerAttack();
}

