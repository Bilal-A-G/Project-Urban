#pragma once
#include "F_Grid_Cell_Count_Size.h"
#include "F_Grid_Extents_Size.h"

namespace INCSizeTypeConverter
{
	FGridCellCountSize GridExtentsToCellCount(const FGridExtentsSize& extents_size)
	{
		FVector extents_vector = extents_size.size;
		FVector converted_vector = extents_vector * 2 + FVector(1,1,1);
		converted_vector.Z = extents_vector.Z + 1;
		
		return FGridCellCountSize(converted_vector);
	}
	FGridExtentsSize GridCellCountToExtents(const FGridCellCountSize cell_count_size)
	{
		FVector cell_count_vector = cell_count_size.size;
		FVector converted_vector = (cell_count_vector - FVector(1,1,1)) / 2;
		converted_vector.Z = cell_count_vector.Z - 1;

		return FGridExtentsSize(converted_vector);
	}
};
