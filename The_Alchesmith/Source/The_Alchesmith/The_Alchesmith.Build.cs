// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;

public class The_Alchesmith : ModuleRules
{
    public The_Alchesmith(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnableExceptions = true;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        // --- ONNX Runtime Integration ---
        string ThirdPartyPath = Path.Combine(ModuleDirectory, "../ThirdParty", "ONNXRuntime");
        string IncludePath = Path.Combine(ThirdPartyPath, "include");
        string LibPath = Path.Combine(ThirdPartyPath, "lib");
        string BinPath = Path.Combine(ThirdPartyPath, "bin");

        // Include directory for headers
        PublicIncludePaths.Add(IncludePath);

        // Library for linking
        PublicAdditionalLibraries.Add(Path.Combine(LibPath, "onnxruntime.lib"));

        // Delay-load the DLL (avoid load failures if not found immediately)
        PublicDelayLoadDLLs.Add("onnxruntime.dll");

        // --- Runtime Dependencies ---
        // Copy the ONNX Runtime DLL next to the built module (for both Editor & Packaged builds)
        string ProjectBinDir = Path.Combine(ModuleDirectory, "../../Binaries/Win64");
        RuntimeDependencies.Add(Path.Combine(ProjectBinDir, "onnxruntime.dll"), Path.Combine(BinPath, "onnxruntime.dll"));

        // Also copy the ONNX model to packaged build directory
        string ModelPath = Path.Combine(ModuleDirectory, "../../Content/Models/rune_cnn_9.onnx");
        RuntimeDependencies.Add("$(TargetOutputDir)/rune_cnn_9.onnx", ModelPath);

        // --- Environment Path fix (ensure correct DLL is found first) ---
        string FullBinPath = Path.GetFullPath(BinPath);
        System.Environment.SetEnvironmentVariable(
            "PATH",
            FullBinPath + Path.PathSeparator + System.Environment.GetEnvironmentVariable("PATH")
        );

        PrivateDependencyModuleNames.AddRange(new string[] { });

        // Optional UI / Online subsystems
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");
    }
}
