// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;
using Tools.DotNETCommon;


public class ExWorldTest : ModuleRules
{
	public ExWorldTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "GameplayAbilities", "GameplayTags",	"GameplayTasks", });

		AddIncludePath(ModuleDirectory);

		AddIncludePath(Path.Combine(Path.GetFullPath(Target.RelativeEnginePath), "Source", "Editor", "Blutility", "Private"));

	}

	private void AddIncludePath(string include_path)
	{
		if (!Directory.Exists(include_path))
		{
			Fail("Unable to locate include path: " + include_path);
		}

		PublicIncludePaths.Add(include_path);
		Trace("Added include path: {0}", include_path);
	}

	private void Trace(string msg)
	{
		Log.TraceInformation(Name + ": " + msg);
	}

	private void Trace(string format, params object[] args)
	{
		Trace(string.Format(format, args));
	}

	private void Fail(string message)
	{
		Trace(message);
		throw new Exception(message);
	}

}
