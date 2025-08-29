#pragma once
#include "F_Grid_Array_Index_Coordinate.h"
#include "F_Grid_Local_Coordinate.h"
#include "F_World_Coordinate.h"
#include "Sizing/F_Grid_Cell_Count_Size.h"
#include "Sizing/F_Grid_Extents_Size.h"

namespace INCCoordinateSpaceConverter 
{
	inline FGridLocalCoordinate GridArrayIndexToLocal(FGridArrayIndexCoordinate coordinate, FGridExtentsSize grid_extents)
	{
		FVector converted = coordinate.vector - grid_extents.size;
		converted.Z = coordinate.vector.Z;
		return FGridLocalCoordinate(converted);
	}
	inline FGridArrayIndexCoordinate GridLocalToArrayIndex(FGridLocalCoordinate coordinate, FGridExtentsSize grid_extents)
	{
		FVector converted =  coordinate.vector + grid_extents.size;
		converted.Z = coordinate.vector.Z;
		return FGridArrayIndexCoordinate(converted);
	}
	
	inline int FlattenGridArrayIndexCoordinate(FGridArrayIndexCoordinate coordinate, FGridCellCountSize grid_size)
	{
		FVector coordinate_vector = coordinate.vector;
		FVector grid_size_vector = grid_size.size;
		return coordinate_vector.X + grid_size_vector.X * (coordinate_vector.Y + grid_size_vector.Y * coordinate_vector.Z);
	}
	inline FGridArrayIndexCoordinate UnFlattenToGridArrayIndexCoordinate(int flattened, FGridCellCountSize grid_size)
	{
		FVector grid_size_vector = grid_size.size;
		int grid_size_x_int = static_cast<int>(grid_size_vector.X);
		int grid_size_y_int = static_cast<int>(grid_size_vector.Y);
		
		int x_coordinate = flattened % grid_size_x_int;
		int y_coordinate =  (flattened / grid_size_x_int) % grid_size_y_int;
		int z_coordinate = flattened / (grid_size_x_int * grid_size_y_int);

		return FGridArrayIndexCoordinate(FVector(x_coordinate, y_coordinate, z_coordinate));
	}

	//Implement when needed: inline FWorldCoordinate GridLocalToWorld(FGridLocalCoordinate coordinate);
	//Implement when needed: inline FGridLocalCoordinate WorldToGridLocal(FWorldCoordinate coordinate);

	//Implement when needed: inline FWorldCoordinate GridArrayIndexToWorld(FGridArrayIndexCoordinate coordinate);
	//Implement when needed: inline FGridArrayIndexCoordinate WorldToGridArrayIndex(FWorldCoordinate coordinate);
}
