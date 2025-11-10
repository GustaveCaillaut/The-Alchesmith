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
	LIGHTNING UMETA(DisplayName = "Rune of Lightning"),
	MOUNTAIN UMETA(DisplayName = "Rune of Mountain"),
	MAGNETISM UMETA(DisplayName = "Rune of Magnetism"),
	LIGHT UMETA(DisplayName = "Rune of Light"),
	DARKNESS UMETA(DisplayName = "Rune of Darkness"),
	STEEL UMETA(DisplayName = "Rune of Steel"),
	INVERSION UMETA(DisplayName = "Rune of Inversion"),
	GRAVITY UMETA(DisplayName = "Rune of Gravity"),
	SEA UMETA(DisplayName = "Rune of Sea"),
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float physDmg = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float magDmg = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float physPen = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float magPen = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float precision = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor drawColor;

	UFUNCTION(BlueprintCallable)
	int32 GetFusionID(ERuneID j);



	UFUNCTION(BlueprintCallable)
	void Init(ERuneID idToSet);
	URune() { id = ERuneID::NONE; };
	~URune();

private:

	UPROPERTY()
	TArray<bool> drawing;
	//XXX
};
