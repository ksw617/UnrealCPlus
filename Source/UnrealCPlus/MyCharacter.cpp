// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MyAnimInstance.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));

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

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
										  //UAnimInstance*
	AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	//Todo
	
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

	PlayerInputComponent->BindAxis(TEXT("ForwardBackward"), this, &AMyCharacter::KeyUpdown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyCharacter::KeyLeftRight);

	PlayerInputComponent->BindAxis(TEXT("LookLeftRight"), this, &AMyCharacter::LookLeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUpDown"), this, &AMyCharacter::LookUpDown);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::KeyAttack);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);

}

void AMyCharacter::KeyUpdown(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value, false);
}

void AMyCharacter::KeyLeftRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value, false);
}

void AMyCharacter::KeyAttack()
{
	if (IsValid(AnimInstance))
	{
		AnimInstance->PlayAttackMontage();
	}
}

void AMyCharacter::LookLeftRight(float Value)
{
	AddControllerYawInput(Value);
}

void AMyCharacter::LookUpDown(float Value)
{
	AddControllerPitchInput(Value);
}

void AMyCharacter::PlayerAttack()
{
	FHitResult HitResult;
	FCollisionQueryParams Params;

	float AttackRange = 100.f;
	float AttackRadius = 50.f;
	FVector StartPos = GetActorLocation();
	FVector EndPos = GetActorLocation() + GetActorForwardVector() * AttackRange;

	bool Result = GetWorld()->SweepSingleByChannel
	(
		HitResult,									//충돌 결과를 저장할 변수					
		StartPos,									//시작 지점
		EndPos,										//끝 지점
		FQuat::Identity,							//회전 (기본 값)
		ECC_GameTraceChannel2,						//충돌 채널 (Visibilirty)
		FCollisionShape::MakeSphere(AttackRange),	//형태 : Sphere(구) MakeSphere(반지름)
		Params										//충돌 쿼리 파라미터
	);


	//
	FVector Vec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + Vec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;
	
	DrawColor = Result ? FColor::Green : FColor::Red;

	//if (Result)
	//{
	//	DrawColor = FColor::Green;
	//}
	//else
	//{
	//	DrawColor = FColor::Red;
	//}

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, Rotation, DrawColor, false, 2.f);

	if (Result && HitResult.GetActor())
	{
		UE_LOG(LogTemp, Log, TEXT("Hit : %s"), *HitResult.GetActor()->GetName());

	}


}

