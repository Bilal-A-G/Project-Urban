#pragma once

namespace PUrban
{
	enum Adjacency
	{
		INVALID,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		FORWARD,
		BACKWARD
	};

	static Adjacency Opposite(Adjacency current)
	{
		switch (current)
		{
		case UP:
			return DOWN;
		case DOWN:
			return UP;
		case LEFT:
			return RIGHT;
		case RIGHT:
			return LEFT;
		case FORWARD:
			return BACKWARD;
		case BACKWARD:
			return FORWARD;
		default:
			return INVALID;
		}
	}
}
