// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStatComponent.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMyStatComponent::UMyStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	bWantsInitializeComponent = true;
	
	Level = 1;

	// ...
}


// Called when the game starts
void UMyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMyStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetLevel(Level);
}

void UMyStatComponent::SetLevel(int32 NewLevel)
{
	auto MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance)
	{
		auto StatData = MyGameInstance->GetStatData(NewLevel);
		Level = StatData->Level;

		Attack = StatData->Attack;

		MaxHp = StatData->MaxHp;
		SetUp(StatData->MaxHp);
	}
}

void UMyStatComponent::OnAttacked(float DamageMount)
{
	int32 NewHp = Hp - DamageMount;
	SetUp(NewHp);

	UE_LOG(LogTemp, Log, TEXT("Hp : %d"), NewHp);
}

void UMyStatComponent::SetUp(int32 NewHp)
{
	Hp = NewHp;
	if (Hp < 5)
	{
		Hp = 0;	
		
		OnDead.Broadcast();
	}
	OnHpChanged.Broadcast();
}


