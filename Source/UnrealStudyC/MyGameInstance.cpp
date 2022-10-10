// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable>DT(TEXT("DataTable'/Game/Data/StatTable.StatTable'"));

	if (DT.Succeeded())
	{
		MyStats = DT.Object;
	}
}

void UMyGameInstance::Init()
{
}

FMyCharacterData* UMyGameInstance::GetStatData(int32 Level)
{
	return MyStats->FindRow<FMyCharacterData>(*FString::FromInt(Level), TEXT(""));
}
