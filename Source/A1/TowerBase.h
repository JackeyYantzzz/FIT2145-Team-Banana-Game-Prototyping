// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerBase.generated.h"

UCLASS()
class A1_API ATowerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ATowerBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower")
	TObjectPtr<USceneComponent> Root;
};
