// Copyright 2015 afuzzyllama. All Rights Reserved.

// namespace UnrealBuildTool.Rules
using UnrealBuildTool;

	public class VoronoiDiagram : ModuleRules
	{
		public VoronoiDiagram(ReadOnlyTargetRules Target) : base(Target)
        {
            PrivatePCHHeaderFile = "Private/VoronoiDiagramPrivatePCH.h";

            PrivateIncludePaths.AddRange(
				new string[] {
					"VoronoiDiagram/Private"
				}
      		);

            PrivateDependencyModuleNames.AddRange(
                new string[] {
                    "Core",
                    "CoreUObject",
                    "Engine"
                }
            );
        }
	}
