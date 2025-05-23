#pragma once

UENUM(BlueprintType)
enum class EAdjacency : uint8
{
	INVALID,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	FORWARD,
	BACKWARD
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
