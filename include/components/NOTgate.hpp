#pragma once

#include <QWidget.h>
#include "../Component.hpp"

class NOTGate : public Component
{
public:
	NOTGate(QWidget* parent) : Component(parent, ":/components/not.png") { type = "not"; }

	bool Action() override {
		return !connections[0]->Action();
	}
};