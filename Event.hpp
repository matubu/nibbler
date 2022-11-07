#pragma once

struct Event {
	enum EventType {
		UP, LEFT, DOWN, RIGHT,

		W, A, S, D,

		SPEED_UP, SPEED_DOWN,

		RESET,
	
		LIB1, LIB2, LIB3,

		QUIT
	};

	EventType	type;
	int 		data;

	Event(EventType type, int data = 0) : type(type), data(data) {}
};