// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/TextureRenderTarget2D.h"


#include "RenderTargetExporter.generated.h"

/**
 * 
 */
UCLASS()
class THE_ALCHESMITH_API URenderTargetExporter : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:


	UFUNCTION(BlueprintCallable)
	static TArray<float> ExportRenderTargetToBitmap(UTextureRenderTarget2D* renderTarget);


};
