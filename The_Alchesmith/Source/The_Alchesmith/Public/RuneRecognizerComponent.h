#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// Include the ONNX Runtime C++ API
#include <onnxruntime_cxx_api.h>

#include "RuneRecognizerComponent.generated.h"

/**
 * Component that loads an ONNX model (rune_cnn_9.onnx)
 * and can predict which rune is drawn based on a 64x64 float array.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class THE_ALCHESMITH_API URuneRecognizerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URuneRecognizerComponent();

    /** Relative path to the ONNX model (default: Content/Models/rune_cnn_9.onnx) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rune")
    FString ModelRelativePath = TEXT("Content/Models/rune_cnn_9.onnx");

    /** Confidence threshold for rune recognition */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rune")
    float ConfidenceThreshold = 0.8f;

    /** Load the ONNX model */
    UFUNCTION(BlueprintCallable, Category = "Rune")
    bool InitModel();

    /**
     * Run inference on a flattened 64x64 float array.
     * @param InputFloats  Flattened array of 4096 values (0..1)
     * @param OutBestIndex The index of the predicted rune
     * @param OutBestConfidence The confidence value (0..1)
     * @return Array of all output probabilities
     */
    UFUNCTION(BlueprintCallable, Category = "Rune")
    TArray<float> PredictRune(const TArray<float>& InputFloats, int32& OutBestIndex, float& OutBestConfidence);

protected:
    virtual void BeginPlay() override;

private:
    // ONNX Runtime members
    TUniquePtr<Ort::Env> Env;
    TUniquePtr<Ort::SessionOptions> SessionOpts;
    TUniquePtr<Ort::Session> Session;
    TUniquePtr<Ort::AllocatorWithDefaultOptions> Alloc;

    FString InputName;
    FString OutputName;
    bool bReady = false;

    bool LoadSession(const FString& AbsPath);
};
#pragma once
