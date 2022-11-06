#pragma once

struct Event {
	enum EventType {
		UP,
		DOWN,
		LEFT,
		RIGHT,
		RESET,
		LIB1,
		LIB2,
		LIB3,
		QUIT
	};

	EventType	type;
	int 		data;

	Event(EventType type, int data = 0) : type(type), data(data) {}
};