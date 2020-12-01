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

void Component::mouseMoveEvent(QMouseEvent* event)
{
	event->ignore();
}
