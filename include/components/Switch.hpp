#pragma once

#include <QWidget.h>
#include "ui_Component.h"
#include "../Component.hpp"

class Switch : public Component
{
public:
	Switch(QWidget* parent) : Component(parent, ":/components/switch_off.png") { type = "switch"; }

	bool Action() override {
		return state;
	}

	void Toggle() {
		state = !state;
		if (state)
			ui->label->setPixmap(QPixmap(":/components/switch_on.png"));
		else
			ui->label->setPixmap(QPixmap(":/components/switch_off.png"));
	}

private:
	bool state = false;
};