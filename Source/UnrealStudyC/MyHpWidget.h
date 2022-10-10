// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyHpWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSTUDYC_API UMyHpWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

	TWeakObjectPtr<class UMyStatComponent>CurrentStatComp;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_HpBar;


public:

	void BindHp(class UMyStatComponent* MyStat);

	void UpdateHp();	
	
};
