#pragma once

#include <tuple>
enum class EVENT_TYPE {
	TRANSFORM,
	ROTATION,
	SCALE,
	RATIO,
};


class Event
{
public:

	EVENT_TYPE type;
	std::tuple<float, float, float> data;

	Event(const EVENT_TYPE& type, const std::tuple<float, float, float>& data) : type(type), data(data) {	}
};

class Renderable {
public:
	virtual void render() = 0;
	virtual void update(Event  e) = 0;
};