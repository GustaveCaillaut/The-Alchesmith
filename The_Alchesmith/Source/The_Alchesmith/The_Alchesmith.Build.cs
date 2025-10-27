// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;

public class The_Alchesmith : ModuleRules
{
	public The_Alchesmith(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        // --- ONNX Runtime Integration ---
        string ThirdPartyPath = Path.Combine(ModuleDirectory, "..", "ThirdParty", "ONNXRuntime");

        string IncludePath = Path.Combine(ThirdPartyPath, "include");
        string LibPath = Path.Combine(ThirdPartyPath, "lib");
        string BinPath = Path.Combine(ThirdPartyPath, "bin");

        // Include directory for headers
        PublicIncludePaths.Add(IncludePath);

        // Library for linking
        PublicAdditionalLibraries.Add(Path.Combine(LibPath, "onnxruntime.lib"));

        // Delay-load DLL so Unreal Editor starts even if it's missing (safeguard)
        PublicDelayLoadDLLs.Add("onnxruntime.dll");

        // Ensure DLL gets copied to the packaged build directory
        RuntimeDependencies.Add("$(TargetOutputDir)/onnxruntime.dll",
                                Path.Combine(BinPath, "onnxruntime.dll"));

        // Also copy the ONNX model to packaged build directory
        string ModelPath = Path.Combine(ModuleDirectory, "../../Content/Models/rune_cnn.onnx");
        RuntimeDependencies.Add("$(TargetOutputDir)/rune_cnn.onnx", ModelPath);


        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
