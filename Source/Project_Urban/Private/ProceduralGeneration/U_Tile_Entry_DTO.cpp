#include "ProceduralGeneration/U_Tile_Entry_DTO.h"

void UTileEntryDTO::PostInitProperties()
{
	Super::PostInitProperties();
	tileRuleset_ = NewObject<UGenerationRuleset>();
}
