#include "Component.hpp"
#include "ui_Component.h"

#include <iostream>
#include <qevent.h>

Component::Component(QWidget* parent) :
	Component(parent, ":/components/and.png")
{

}

Component::Component(QWidget* parent, const QString& resource) :
	QFrame(parent), ui(nullptr)
{
	ui = new Ui::Component();
	ui->setupUi(this);

	ui->label->setPixmap(QPixmap(resource));
}

Component::~Component()
{
	for (Wire* w : wires)
	{
		delete w;
		w = nullptr;
	}
}

void Component::mouseMoveEvent(QMouseEvent* event)
{
	event->ignore();
}

QPoint Component::CenterPos()
{
	return pos() + QPoint(size().width() / 2, size().height() / 2);
}

void Component::Connect(Component* component, QLine* wire)
{
	connections.push_back(component);
	this->wires.push_back(new Wire{ wire, Wire::eType::TARGET});
	component->wires.push_back(new Wire{ wire, Wire::eType::SOURCE});
}

const QString& Component::Type()
{
	return type;
}

void Component::UpdateWires()
{
	for (Wire* wire : wires)
	{
		if (wire->type == Wire::eType::SOURCE)
		{
			wire->line->setP1(this->CenterPos());
		}
		else
		{
			wire->line->setP2(this->CenterPos());
		}
	}
}
