#include "ProceduralGeneration/UGenerationModel.h"
#include "ProceduralGeneration/FModelCell.h"

void UGenerationModel::Init(FVector gridSize, TArray<FLabel*> allPossibleLabels)
{
	grid.SetNum(gridSize.X);

	for (int x = 0; x < gridSize.X; x++)
	{
		grid[x].SetNum(gridSize.Y);
		for (int y = 0; y < gridSize.Y; y++)
		{
			grid[x][y].SetNum(gridSize.Z);
			for (int z = 0; z < gridSize.Z; z++)
			{
				grid[x][y][z] = new FModelCell(allPossibleLabels);
			}
		}
	}
}
