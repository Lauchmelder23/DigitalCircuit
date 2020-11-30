#pragma once

#include <qframe.h>

namespace Ui {
	class Component;
}

class Component : public QFrame
{
public:
	explicit Component(QWidget* parent);

private:
	Ui::Component* ui;
};