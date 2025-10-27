// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rune.generated.h"

/**
 * Custom enum to demonstrate how to expose enums to Blueprints.
 */
UENUM(BlueprintType)
enum class ERuneID : uint8 {
	NONE UMETA(DisplayName = "None"),
	FIRE UMETA(DisplayName = "Rune of Fire"),
	AIR UMETA(DisplayName = "Rune of Air"),
	EARTH UMETA(DisplayName = "Rune of Earth"),
	LIGHTNING UMETA(DisplayName = "Rune of Lightning")
};

/**
 *
 */
UCLASS(BlueprintType)
class THE_ALCHESMITH_API URune : public UObject {
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERuneID id = ERuneID::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor drawColor;

	UFUNCTION(BlueprintCallable)
	int32 GetFusionID(ERuneID j);



	UFUNCTION(BlueprintCallable)
	void Init(ERuneID idToSet);
	URune() { id = ERuneID::NONE; };
	~URune();

private:
	void loadDrawing(std::string const& filepath);

	UPROPERTY()
	TArray<bool> drawing;
	//XXX
};
