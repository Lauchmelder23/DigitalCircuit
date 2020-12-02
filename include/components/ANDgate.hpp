#pragma once

#include <QWidget.h>
#include "../Component.hpp"

class ANDGate : public Component
{
public:
	ANDGate(QWidget* parent) : Component(parent, ":/components/and.png") { type = "and"; }

	bool Action() override {
		return (connections[0]->Action() && connections[1]->Action());
	}
};