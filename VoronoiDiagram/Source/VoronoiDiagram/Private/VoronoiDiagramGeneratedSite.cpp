// Copyright 2015 afuzzyllama. All Rights Reserved.

#include "VoronoiDiagramGeneratedSite.h"
#include "VoronoiDiagramPrivatePCH.h"

FVoronoiDiagramGeneratedSite::FVoronoiDiagramGeneratedSite(int32 InIndex, FVector2D InCoordinate, FVector2D InCentroid, FColor InColor, bool InIsCorner, bool InIsEdge)
: Index(InIndex)
, Color(InColor)
, Coordinate(InCoordinate)
, Centroid(InCentroid)
, bIsCorner(InIsCorner)
, bIsEdge(InIsEdge)
{}