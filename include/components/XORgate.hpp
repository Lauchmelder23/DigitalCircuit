#pragma once

#include <QWidget.h>
#include "../Component.hpp"

class XORGate : public Component
{
public:
	XORGate(QWidget* parent) : Component(parent, ":/components/xor.png") { type = "xor"; }

	bool Action() override {
		return ((connections[0]->Action() || connections[1]->Action()) && !(connections[0]->Action() && connections[1]->Action()));
	}
};