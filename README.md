# VoronoiDiagram
Updated this repository to work with at least 4.26 -> https://github.com/afuzzyllama/VoronoiDiagramUE4

Hey, i uploaded a little modified and updated version from this guy.

All credits go to him. Feel free to use it and improve it.

Just copy and install the folder into the plugins section of your project.

The template :
###### Project.build.cs ( add 'VoronoiDiagram' into plubicDependency)
```
PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "VoronoiDiagram" });
```

###### file.cpp
```
// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"

#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "VoronoiDiagram.h"

TArray<FVector2D> VoronoiPointsVar;
TArray<int32> VerticesVar;

void UMyBlueprintFunctionLibrary::MyCustomFunction(int32 RDNNumber, int32 width, int32 height, bool random, TArray<FVector> InPoints, TArray<FVector2D>& VoronoiPoints, TArray<int32>& NbrVertices)
{

	VoronoiPointsVar.Empty();
	VerticesVar.Empty();

	TSharedPtr<FVoronoiDiagram> VoronoiDiagram(new FVoronoiDiagram(FIntRect(0, 0, width, height)));
	TArray<FIntPoint> Points;


	if (random == true) {
		for (int32 i = 0; i < RDNNumber; ++i)
		{
			Points.AddUnique(FIntPoint(FMath::RandRange(0, width - 1), FMath::RandRange(0, height - 1)));
		}
	}
	else {
		for (int32 i = 0; i < InPoints.Num(); ++i)
		{
			Points.AddUnique(FIntPoint(InPoints[i].X, InPoints[i].Y));
		}
	}
	VoronoiDiagram->AddPoints(Points);


	VoronoiDiagram->GenerateSites(2);

	TArray<FVector2D> test;

	for (int32 i = 0; i < VoronoiDiagram->GeneratedSites.Num(); ++i)
	{

		test = VoronoiDiagram->GeneratedSites[i].Vertices;
		// UE_LOG(LogTemp, Error, TEXT("%d"), VoronoiDiagram->GeneratedSites[i].Vertices.Num());
		//VoronoiDiagram->GeneratedSites[i].Vertices;
		NbrVertices.Add(VoronoiDiagram->GeneratedSites[i].Vertices.Num());
		VoronoiPoints.Append(test);
	}

	VoronoiPointsVar = VoronoiPoints;
	VerticesVar = NbrVertices;
	

	// FVoronoiDiagramHelper::GenerateTexture(VoronoiDiagram, MyTexture);
}

void UMyBlueprintFunctionLibrary::calculateAllPoints(TArray<FisNeighbour>& isNeighbourArrayFirst)
{
	int32 FirstIndex = 0;
	int32 loopIt = 0;
	TArray<FVector> CurrentsVectorPoints;
	TArray<FisNeighbour> isNeighbourArray;

	for (int32 i = 0; i < VerticesVar.Num(); ++i)
	{
		// replace if / else
		(i == 0) ? loopIt = VerticesVar[i] : loopIt = FirstIndex + VerticesVar[i];
		
		FVector Centroid = FVector(0.0f, 0.0f, 0.0f);
		CurrentsVectorPoints.Empty();

		

		for (FirstIndex; FirstIndex < loopIt; ++FirstIndex)
		{			
			CurrentsVectorPoints.Add(FVector(FMath::RoundHalfFromZero(VoronoiPointsVar[FirstIndex].X), FMath::RoundHalfFromZero(VoronoiPointsVar[FirstIndex].Y), 0.0f));
			Centroid = Centroid + FVector(FMath::RoundHalfFromZero(VoronoiPointsVar[FirstIndex].X), FMath::RoundHalfFromZero(VoronoiPointsVar[FirstIndex].Y), 0.0f);

			//UE_LOG(LogTemp, Error, TEXT("%d -- %d || %d -- %d ---- %s"), FirstIndex, loopIt, VerticesVar[i], i, *VoronoiPointsVar[FirstIndex].ToString());
		}


		// Add the points into a structure to use it elsewhere.		
		FisNeighbour temp;
		temp.CellID = i;
		temp.Color = FLinearColor(FMath::RandRange(0.0f, 1.0f), FMath::RandRange(0.0f, 1.0f), FMath::RandRange(0.0f, 1.0f), 1.0f);
		temp.Centroid = UKismetMathLibrary::Divide_VectorInt(Centroid, VerticesVar[i]);
		temp.VoronoiPoints = CurrentsVectorPoints;
		temp.NeighbourCellID.Add(0);
		isNeighbourArray.Add(temp);

	}

	isNeighbourArrayFirst = isNeighbourArray;
}

void UMyBlueprintFunctionLibrary::CalculateNeighbour(TArray<FisNeighbour>& isCalculateNeighbourArray) {

	TArray<int32> TmpAdjZone;

	TArray<FisNeighbour> TempStrucArray;
	calculateAllPoints(TempStrucArray);

	TArray<FisNeighbour> secondStruc = TempStrucArray;


	for (int32 i = 0; i < TempStrucArray.Num(); ++i) {

		TempStrucArray[i].NeighbourCellID.Empty();

		for (int32 x = 0; x < TempStrucArray[i].VoronoiPoints.Num(); ++x)
		{

			//UE_LOG(LogTemp, Error, TEXT("%d -- %d -- %s"), x, TempStrucArray[i].VoronoiPoints.Num(), *TempStrucArray[i].VoronoiPoints[x].ToString());
						
			for (int32 y = 0; y < secondStruc.Num(); ++y)
			{

				if (secondStruc[y].VoronoiPoints.Contains(TempStrucArray[i].VoronoiPoints[x]) == true) {
					if (TempStrucArray[i].CellID != secondStruc[y].CellID) {
						TmpAdjZone.AddUnique(secondStruc[y].CellID);
						//UE_LOG(LogTemp, Warning, TEXT("%d, True: %d - %s"), i, y, *TempStrucArray[i].VoronoiPoints[x].ToString());
					}
				}
				else {
					//UE_LOG(LogTemp, Warning, TEXT("%d, False: %d"), i, y);
				}

			}
			//UE_LOG(LogTemp, Warning, TEXT("------"));
			// complete the 3rd loop

		}

		// complete the 2nd loop
		TempStrucArray[i].NeighbourCellID = TmpAdjZone;
		TmpAdjZone.Empty();

	}

	isCalculateNeighbourArray = TempStrucArray;

	// complete the 1st loop

}
```

###### File.h
```
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FisNeighbour
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 CellID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<FVector> VoronoiPoints;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FLinearColor Color;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<int32> NeighbourCellID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FVector Centroid;
};

UCLASS()
class PROCEDURALGENERATION_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
        
        UFUNCTION(BlueprintCallable, Category = "Custom")
            static void MyCustomFunction(int32 RDNNumber, int32 width, int32 height, bool random, TArray<FVector> InPoints, TArray<FVector2D>& VoronoiPoints, TArray<int32>& NbrVertices);
    
        UFUNCTION(BlueprintCallable, Category = "Custom")
            static void calculateAllPoints(TArray<FisNeighbour>& isNeighbourArrayFirst);

        UFUNCTION(BlueprintCallable, Category = "Custom")
            static void CalculateNeighbour(TArray<FisNeighbour>& isCalculateNeighbourArray);
};
```

###### Blueprint

![blueprint](https://i.gyazo.com/a8f9c5f70f97f60ca200efb9638f291b.png)

###### And the result
![result](https://i.gyazo.com/bb62d634ea8e10c20ff9383746a0c9ba.png)
