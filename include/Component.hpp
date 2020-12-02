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

	virtual void mouseMoveEvent(QMouseEvent* event) override;

	QPoint CenterPos();
	void Connect(Component* component);
	const QString& Type();

	virtual bool Action() = 0;
	

protected:
	Ui::Component* ui;

	QString type;
	std::vector<Component*> connections;
};