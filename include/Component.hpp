#pragma once

#include <qframe.h>

namespace Ui {
	class Component;
}

class Component : public QFrame
{
public:
	explicit Component(QWidget* parent);
	explicit Component(QWidget* parent, const QString& resource);

	void mouseMoveEvent(QMouseEvent* event) override;

private:
	Ui::Component* ui;
};