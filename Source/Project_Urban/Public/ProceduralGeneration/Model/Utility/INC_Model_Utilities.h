#pragma once
#include "ProceduralGeneration/CoordinateSpaces/F_Grid_Array_Index_Coordinate.h"
#include "ProceduralGeneration/CoordinateSpaces/Sizing/F_Grid_Cell_Count_Size.h"
#include "ProceduralGeneration/CoordinateSpaces/INC_Coordinate_Space_Converter.h"

namespace INCModelUtilities
{
	inline bool OutOfBounds(FGridArrayIndexCoordinate index, FGridCellCountSize grid_size)
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
	
	inline TArray<FGridArrayIndexCoordinate> GetBorderTileIndices(FGridCellCountSize grid_size, bool exclude_z = false)
	{
		TArray<FGridArrayIndexCoordinate> border_tiles = TArray<FGridArrayIndexCoordinate>();
		FVector grid_size_vector = grid_size.size;
		int num_grid_elements = grid_size_vector.X * grid_size_vector.Y * grid_size_vector.Z;
		for(int i = 0; i < num_grid_elements; i++)
		{
			FGridArrayIndexCoordinate un_flattened_index = INCCoordinateSpaceConverter::UnFlattenToGridArrayIndexCoordinate(i, grid_size);
			FVector un_flattened_vector = un_flattened_index.vector;
			bool borders_left = un_flattened_vector.X == 0;
			bool borders_right = un_flattened_vector.X == grid_size_vector.X - 1;
			bool borders_down = un_flattened_vector.Y == 0;
			bool borders_up = un_flattened_vector.Y == grid_size_vector.Y - 1;
			bool borders_back = false;
			bool borders_forwards = false;
			if(!exclude_z)
			{
				borders_back = un_flattened_vector.Z == 0;
				borders_forwards = un_flattened_vector.Z == grid_size_vector.Z - 1;
			}
			if(borders_left || borders_right || borders_down || borders_up || borders_back || borders_forwards)
			{
				border_tiles.Add(un_flattened_index);
			}
		}
		
		return border_tiles;
	} 
};
