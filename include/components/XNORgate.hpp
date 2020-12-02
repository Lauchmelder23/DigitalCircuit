#pragma once

#include <QWidget.h>
#include "../Component.hpp"

class XNORGate : public Component
{
public:
	XNORGate(QWidget* parent) : Component(parent, ":/components/xnor.png") { type = "xnor"; }

	bool Action() override {
		return !((connections[0]->Action() || connections[1]->Action()) && !(connections[0]->Action() && connections[1]->Action()));
	}
};