#pragma once

struct FGridCellCountSize
{
public:
	FGridCellCountSize(FVector new_size)
	{
		size = new_size;
	}
	FGridCellCountSize() = default;
public:
	FVector size;	
};
