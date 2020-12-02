#pragma once

#include <QWidget.h>
#include "../Component.hpp"

class Lamp : public Component
{
public:
	Lamp(QWidget* parent) : Component(parent, ":/components/lamp_off.png") { type = "lamp"; }

	bool Action() override {
		return connections[0]->Action();
	}

	void Turn(bool state) {
		if (state)
			ui->label->setPixmap(QPixmap(":/components/lamp_on.png"));
		else
			ui->label->setPixmap(QPixmap(":/components/lamp_off.png"));
	}

private:
	bool state = false;
};