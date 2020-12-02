#pragma once

#include <QWidget.h>
#include "../Component.hpp"

class ORGate : public Component
{
public:
	ORGate(QWidget* parent) : Component(parent, ":/components/or.png") { type = "or"; }

	bool Action() override {
		return (connections[0]->Action() || connections[1]->Action());
	}
};