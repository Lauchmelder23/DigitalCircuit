#pragma once

#include <QWidget.h>
#include "../Component.hpp"

class NORGate : public Component
{
public:
	NORGate(QWidget* parent) : Component(parent, ":/components/nor.png") { type = "nor"; }

	bool Action() override {
		return !(connections[0]->Action() || connections[1]->Action());
	}
};