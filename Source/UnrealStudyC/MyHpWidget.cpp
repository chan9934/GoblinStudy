// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHpWidget.h"
#include "MyStatComponent.h"
#include "Components/ProgressBar.h"

void UMyHpWidget::BindHp(UMyStatComponent* MyStat)
{
	UE_LOG(LogTemp, Log, TEXT("Binding?"));
	CurrentStatComp = MyStat;
	MyStat->OnHpChanged.AddUObject(this, &UMyHpWidget::UpdateHp);
	
}

void UMyHpWidget::UpdateHp()
{
	float Hphp = CurrentStatComp->GetHpRatio();
	int32 hp = CurrentStatComp->GetHp();
	int32 Max = CurrentStatComp->GetMaxHp();
	
	if (CurrentStatComp.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("HpRatio %f"), Hphp);
		PB_HpBar->SetPercent(CurrentStatComp->GetHpRatio());
		UE_LOG(LogTemp, Log, TEXT("1Hp %d"), hp);
		UE_LOG(LogTemp, Log, TEXT("1Max %d"), Max);
		
	}
}	
