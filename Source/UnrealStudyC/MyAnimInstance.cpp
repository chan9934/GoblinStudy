// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyCharacter.h"
#include "GameFramework/PawnMovementComponent.h"

UMyAnimInstance::UMyAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage>AM(TEXT("AnimMontage'/Game/Animation/MyMontage.MyMontage'"));
	if (AM.Succeeded())
	{
		AttackMontage = AM.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage>DM(TEXT("AnimMontage'/Game/Animation/DeadMontage.DeadMontage'"));
	if (DM.Succeeded())
	{
		DeadMontage = DM.Object;
	}

}
void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto Pawn = TryGetPawnOwner();

	if (IsValid(Pawn))
	{
		Speed = Pawn->GetVelocity().Size();
		
		auto Character = Cast<AMyCharacter>(Pawn);

		if (IsValid(Character))
		{
			IsFalling = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UMyAnimInstance::PlayAttackMontage()
{
	
	Montage_Play(AttackMontage, 1.f);
	
}

void UMyAnimInstance::PlayDeadMontage()
{
	Montage_Play(DeadMontage, 1.f);
}

void UMyAnimInstance::JumpToSection(int32 SectionIndex)
{
	FName Name = GetAttackMontageName(SectionIndex);
	Montage_JumpToSection(Name, AttackMontage);
}

FName UMyAnimInstance::GetAttackMontageName(int32 SectionIndex)
{
	return FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));
}

void UMyAnimInstance::AnimNotify_AttackHit()
{	
	OnAttackHit.Broadcast();
}
