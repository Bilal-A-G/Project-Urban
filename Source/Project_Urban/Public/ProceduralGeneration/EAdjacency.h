#pragma once

UENUM(BlueprintType)
enum class EAdjacency : uint8
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	FORWARD,
	BACKWARD,
	//Just here so we know how many valid entries are in here
	LAST,
	INVALID
};

namespace PUrban
{
	static EAdjacency Opposite(EAdjacency current)
	{
		switch (current)
		{
		case EAdjacency::UP:
			return EAdjacency::DOWN;
		case EAdjacency::DOWN:
			return EAdjacency::UP;
		case EAdjacency::LEFT:
			return EAdjacency::RIGHT;
		case EAdjacency::RIGHT:
			return EAdjacency::LEFT;
		case EAdjacency::FORWARD:
			return EAdjacency::BACKWARD;
		case EAdjacency::BACKWARD:
			return EAdjacency::FORWARD;
		default:
			return EAdjacency::INVALID;
		}
	}
}
