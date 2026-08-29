#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoulDropVisual.generated.h"

class UPaperFlipbook;
class UPaperFlipbookComponent;

UCLASS()
class A1_API ASoulDropVisual : public AActor
{
	GENERATED_BODY()

public:
	ASoulDropVisual();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soul Drop")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soul Drop")
	TObjectPtr<UPaperFlipbookComponent> FlipbookComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soul Drop")
	float LifeSeconds = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soul Drop")
	FVector FloatOffset = FVector(0.0f, 0.0f, 80.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soul Drop")
	float StartScale = 0.65f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soul Drop")
	float EndScale = 1.15f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soul Drop")
	TObjectPtr<UPaperFlipbook> SoulFlipbook = nullptr;

private:
	FVector StartLocation = FVector::ZeroVector;
	float AgeSeconds = 0.0f;

	UFUNCTION()
	void FinishDrop();
};
