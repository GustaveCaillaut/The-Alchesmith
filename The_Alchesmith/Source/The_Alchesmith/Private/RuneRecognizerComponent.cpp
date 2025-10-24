#include "RuneRecognizerComponent.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

URuneRecognizerComponent::URuneRecognizerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void URuneRecognizerComponent::BeginPlay()
{
    Super::BeginPlay();
    InitModel(); // Automatically load at start
}

bool URuneRecognizerComponent::InitModel()
{
    FString AbsPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / ModelRelativePath);

    // For packaged builds, fall back to the executable directory
    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsPath))
    {
        AbsPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPlatformProcess::BaseDir(), TEXT("rune_cnn.onnx")));
    }

    return LoadSession(AbsPath);
}

bool URuneRecognizerComponent::LoadSession(const FString& AbsPath)
{
    try
    {
        Env = MakeUnique<Ort::Env>(ORT_LOGGING_LEVEL_WARNING, "RuneEnv");
        SessionOpts = MakeUnique<Ort::SessionOptions>();
        SessionOpts->SetIntraOpNumThreads(1);
        SessionOpts->SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

        std::wstring WModelPath = std::wstring(*AbsPath);
        Session = MakeUnique<Ort::Session>(*Env, WModelPath.c_str(), *SessionOpts);
        Alloc = MakeUnique<Ort::AllocatorWithDefaultOptions>();

        // Cache first input/output names
        if (Session->GetInputCount() == 0 || Session->GetOutputCount() == 0)
        {
            UE_LOG(LogTemp, Error, TEXT("ONNX model has no inputs or outputs."));
            return false;
        }

        Ort::AllocatedStringPtr inName = Session->GetInputNameAllocated(0, *Alloc);
        Ort::AllocatedStringPtr outName = Session->GetOutputNameAllocated(0, *Alloc);

        InputName = UTF8_TO_TCHAR(inName.get());
        OutputName = UTF8_TO_TCHAR(outName.get());

        bReady = true;
        UE_LOG(LogTemp, Log, TEXT("Loaded ONNX model: %s"), *AbsPath);
        UE_LOG(LogTemp, Log, TEXT("Input: %s, Output: %s"), *InputName, *OutputName);
        return true;
    }
    catch (const Ort::Exception& e)
    {
        UE_LOG(LogTemp, Error, TEXT("ONNX load error: %s"), UTF8_TO_TCHAR(e.what()));
        bReady = false;
        return false;
    }
}

TArray<float> URuneRecognizerComponent::PredictRune(const TArray<float>& InputFloats,
    int32& OutBestIndex, float& OutBestConfidence)
{
    TArray<float> Empty;
    OutBestIndex = -1;
    OutBestConfidence = 0.f;

    if (!bReady)
    {
        UE_LOG(LogTemp, Error, TEXT("ONNX model not initialized."));
        return Empty;
    }

    if (InputFloats.Num() != 64 * 64)
    {
        UE_LOG(LogTemp, Error, TEXT("PredictRune expects 4096 floats (64x64). Got %d"), InputFloats.Num());
        return Empty;
    }

    try
    {
        // Create input tensor [1,1,64,64]
        std::vector<int64_t> shape = { 1, 1, 64, 64 };
        std::vector<float>   data(InputFloats.GetData(), InputFloats.GetData() + InputFloats.Num());

        Ort::MemoryInfo memInfo = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
        Ort::Value inputTensor = Ort::Value::CreateTensor<float>(
            memInfo, data.data(), data.size(), shape.data(), shape.size());

        const char* inNames[] = { TCHAR_TO_UTF8(*InputName) };
        const char* outNames[] = { TCHAR_TO_UTF8(*OutputName) };

        auto output = Session->Run(Ort::RunOptions{ nullptr },
            inNames, &inputTensor, 1,
            outNames, 1);

        if (output.size() != 1 || !output[0].IsTensor())
        {
            UE_LOG(LogTemp, Error, TEXT("Unexpected ONNX output."));
            return Empty;
        }

        float* outData = output[0].GetTensorMutableData<float>();
        size_t count = output[0].GetTensorTypeAndShapeInfo().GetElementCount();

        TArray<float> Probs;
        Probs.SetNumUninitialized((int32)count);
        FMemory::Memcpy(Probs.GetData(), outData, sizeof(float) * count);

        // Argmax
        int32 bestIdx = 0;
        float bestVal = (Probs.Num() > 0) ? Probs[0] : 0.f;
        for (int32 i = 1; i < Probs.Num(); ++i)
        {
            if (Probs[i] > bestVal)
            {
                bestVal = Probs[i];
                bestIdx = i;
            }
        }

        OutBestIndex = bestIdx;
        OutBestConfidence = bestVal;
        return Probs;
    }
    catch (const Ort::Exception& e)
    {
        UE_LOG(LogTemp, Error, TEXT("ONNX inference error: %s"), UTF8_TO_TCHAR(e.what()));
        return Empty;
    }
}
