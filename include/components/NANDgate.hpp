#pragma once

#include <QWidget.h>
#include "../Component.hpp"

class NANDGate : public Component
{
public:
	NANDGate(QWidget* parent) : Component(parent, ":/components/nand.png") { type = "nand"; }

	bool Action() override {
		return !(connections[0]->Action() && connections[1]->Action());
	}
};