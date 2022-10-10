// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "MyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHit);

/**
 * 
 */


UCLASS()
class UNREALSTUDYC_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess= true))
	bool IsFalling;

public:
	UMyAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

	void PlayAttackMontage();
	void PlayDeadMontage();

	void JumpToSection(int32 SectionIndex);
	FName GetAttackMontageName(int32 SectionIndex);
	UAnimMontage* GetAttackMontage()
	{
		return AttackMontage;
	}
	UAnimMontage* GetDeadMontage()
	{
		return DeadMontage;
	}


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = true))
	UAnimMontage* DeadMontage;
	
private:
	UFUNCTION()
	void AnimNotify_AttackHit();

public:
	FOnAttackHit OnAttackHit;
	
};
