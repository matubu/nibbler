#pragma once

struct Event {
	enum EventType {
		W, A, S, D,

		UP, LEFT, DOWN, RIGHT,

		SPEED_UP, SPEED_DOWN,

		RESET,
	
		LIB1, LIB2, LIB3,

		QUIT
	};

	EventType	type;
	int 		data;

	Event(EventType type, int data = 0) : type(type), data(data) {}
};