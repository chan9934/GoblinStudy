// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpChanged);
DECLARE_MULTICAST_DELEGATE(FDead);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALSTUDYC_API UMyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	
	// Called every frame

public:
	int32 GetLevel()
	{
		return Level;
	}

	int32 GetAttack()
	{
		return Attack;
	}

	int32 GetHp()
	{
		return Hp;
	}
	
	void SetLevel(int32 Level);
	void OnAttacked(float DamageMount);
private:
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 Level;
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 Attack;
	UPROPERTY(EditAnywhere, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 Hp;
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 MaxHp;

public:
	int32 GetMaxHp()
	{
		return MaxHp;
	}

	float GetHpRatio()
	{
		return (float)Hp / (float)MaxHp;
	}

	void SetUp(int32 NewHp);

public:
	FOnHpChanged OnHpChanged;
	FDead OnDead;
		
};
