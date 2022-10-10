// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);
UCLASS()
class UNREALSTUDYC_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Attack();

	UPROPERTY()
		USkeletalMeshComponent* Weapon;

private:
	UPROPERTY()
		class UMyStatComponent* Stat;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UpDown(float Value);
	void LeftRight(float Value);
	void Yaw(float Value);

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void AttackCheck();
private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

private:
	UPROPERTY(VisibleAnywhere)
		bool IsAttacking = false;

	UPROPERTY()
		class UMyAnimInstance* AnimInstance;



public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
	UPROPERTY()
		int32 AttackIndex = 0;

private:
	class UWidgetComponent* HpBar;
public:
	bool CharacterGrab = false;

public:
	FOnAttackEnd OnAttackEnd;

private:
	void DeadBinding();

};
