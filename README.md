# VoronoiDiagram
Updated this repository to work with at least 4.21.2 -> https://github.com/afuzzyllama/VoronoiDiagramUE4

Hey, i uploaded a little modified and updated version from this guy.

All credits go to him. Feel free to use it and improve it.

Just copy and install the folder into the plugins section of your project.

The template :

###### file.cpp
```
void AMyActorForVoronoi::MyFunction(int width, int height, bool random, TArray<FVector> InPoints, TArray<FVector2D>& VoronoiPoints, TArray<int32>& NbrVertices)
{

	TSharedPtr<FVoronoiDiagram> VoronoiDiagram(new FVoronoiDiagram(FIntRect(0, 0, width, height)));
	TArray<FIntPoint> Points;


	if (random == true) {
		for (int32 i = 0; i < 20; ++i)
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
		UE_LOG(LogTemp, Error, TEXT("%d"), VoronoiDiagram->GeneratedSites[i].Vertices.Num());
		//VoronoiDiagram->GeneratedSites[i].Vertices;
		NbrVertices.Add(VoronoiDiagram->GeneratedSites[i].Vertices.Num());
		VoronoiPoints.Append(test);
		
	}
	
	

	// FVoronoiDiagramHelper::GenerateTexture(VoronoiDiagram, MyTexture);
}
```

###### File.h
```
UFUNCTION(BlueprintCallable, Category = "Custom")
		static void MyFunction(int width, int height, bool random, TArray<FVector> InPoints, TArray<FVector2D>& VoronoiPoints, TArray<int32>& NbrVertices);
```

###### Blueprint

![blueprint](https://i.gyazo.com/63476e2728f29c9d751b109b63f5bf45.png)

###### And the result
![result](https://i.gyazo.com/bb62d634ea8e10c20ff9383746a0c9ba.png)
