// Copyright 2015 afuzzyllama. All Rights Reserved.

#include "VoronoiDiagramVertex.h"
#include "VoronoiDiagramPrivatePCH.h"

TSharedPtr<FVoronoiDiagramVertex, ESPMode::ThreadSafe> FVoronoiDiagramVertex::CreatePtr(int32 Index, FVector2D Coordinate)
{
	return TSharedPtr<FVoronoiDiagramVertex, ESPMode::ThreadSafe>(new FVoronoiDiagramVertex(Index, Coordinate));
}

TSharedPtr<FVoronoiDiagramVertex, ESPMode::ThreadSafe> FVoronoiDiagramVertex::Intersect(TSharedPtr<FVoronoiDiagramHalfEdge, ESPMode::ThreadSafe> HalfEdgeA, TSharedPtr<FVoronoiDiagramHalfEdge, ESPMode::ThreadSafe> HalfEdgeB)
{
	TSharedPtr<FVoronoiDiagramEdge, ESPMode::ThreadSafe> EdgeA, EdgeB, Edge;
	TSharedPtr<FVoronoiDiagramHalfEdge, ESPMode::ThreadSafe> HalfEdge;
    float Determinant, IntersectionX, IntersectionY;
    bool bRightOfSite;
    
    EdgeA = HalfEdgeA->Edge;
    EdgeB = HalfEdgeB->Edge;
    
    if(!EdgeA.IsValid() || !EdgeB.IsValid())
    {
        return nullptr;
    }
    
    if(EdgeA->RightSite.Get() == EdgeB->RightSite.Get())
    {
        return nullptr;
    }
    
    Determinant = (EdgeA->a * EdgeB->b) - (EdgeA->b * EdgeB->a);
    
    if (FMath::IsNearlyZero(Determinant))
    {
        // The edges are parallel
        return nullptr;
    }
    
    IntersectionX = (EdgeA->c * EdgeB->b - EdgeB->c * EdgeA->b) / Determinant;
    IntersectionY = (EdgeB->c * EdgeA->a - EdgeA->c * EdgeB->a) / Determinant;

    if(
        EdgeA->RightSite->GetCoordinate().Y < EdgeB->RightSite->GetCoordinate().Y ||
        (
            FMath::IsNearlyEqual(EdgeA->RightSite->GetCoordinate().Y, EdgeB->RightSite->GetCoordinate().Y) &&
            EdgeA->RightSite->GetCoordinate().X < EdgeB->RightSite->GetCoordinate().X
        )
    )
    {
        HalfEdge = HalfEdgeA;
        Edge = EdgeA;
    }
    else
    {
        HalfEdge = HalfEdgeB;
        Edge = EdgeB;
    }
    
    bRightOfSite = IntersectionX >= Edge->RightSite->GetCoordinate().X;
    if(
        ( bRightOfSite && HalfEdge->EdgeType == EVoronoiDiagramEdge::Left) ||
        (!bRightOfSite && HalfEdge->EdgeType == EVoronoiDiagramEdge::Right)
    )
    {
        return nullptr;
    }
    
    return FVoronoiDiagramVertex::CreatePtr(-1, FVector2D(IntersectionX, IntersectionY));
}

FVoronoiDiagramVertex::FVoronoiDiagramVertex(int32 InIndex, FVector2D InCoordinate)
: Index(InIndex)
, Coordinate(InCoordinate)
{
	// float != float == NAN
	if (Coordinate.X != Coordinate.X || Coordinate.Y != Coordinate.Y)
    {
        // This probably should not happen, but it will alert in the logs if it does
        UE_LOG(LogVoronoiDiagram, Error, TEXT("Contains NaN"));
    }
}

FVector2D FVoronoiDiagramVertex::GetCoordinate() const
{
    return Coordinate;
}
