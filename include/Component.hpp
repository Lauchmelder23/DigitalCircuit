#pragma once

#include <qframe.h>

namespace Ui {
	class Component;
}

typedef struct {
	QLine* line;
	enum class eType {
		SOURCE,
		TARGET
	} type;
} Wire;

class Component : public QFrame
{
public:
	explicit Component(QWidget* parent);
	explicit Component(QWidget* parent, const QString& resource);
	~Component();

	virtual void mouseMoveEvent(QMouseEvent* event) override;

	QPoint CenterPos();
	void Connect(Component* component, QLine* wire);
	const QString& Type();
	void UpdateWires();

	virtual bool Action() = 0;
	

protected:
	Ui::Component* ui;

	QString type;
	std::vector<Component*> connections;
	std::vector<Wire*> wires;
};