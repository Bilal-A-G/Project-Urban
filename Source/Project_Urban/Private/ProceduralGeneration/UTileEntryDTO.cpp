#include "ProceduralGeneration/UTileEntryDTO.h"
#include "ProceduralGeneration/UGenerationRuleset.h"

void UTileEntryDTO::PostInitProperties()
{
	Super::PostInitProperties();
	this->tileRuleset = NewObject<UGenerationRuleset>();
}
