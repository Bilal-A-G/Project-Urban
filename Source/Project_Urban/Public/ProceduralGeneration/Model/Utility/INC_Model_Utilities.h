#pragma once
#include "ProceduralGeneration/CoordinateSpaces/F_Grid_Array_Index_Coordinate.h"
#include "ProceduralGeneration/CoordinateSpaces/Sizing/F_Grid_Cell_Count_Size.h"

namespace INCModelUtilities
{
	bool OutOfBounds(FGridArrayIndexCoordinate index, FGridCellCountSize grid_size)
	{
		FVector index_vector = index.vector;
		FVector size_vector = grid_size.size;
		
		bool too_far_left = index_vector.X < 0;
		bool too_far_right = index_vector.X >= size_vector.X;
		bool too_far_down = index_vector.Y < 0;
		bool too_far_up = index_vector.Y >= size_vector.Y;
		bool too_far_back = index_vector.Z < 0;
		bool too_far_forwards = index_vector.Z >= size_vector.Z;
		
		return too_far_left || too_far_right || too_far_down || too_far_up || too_far_back || too_far_forwards;
	}	
};
