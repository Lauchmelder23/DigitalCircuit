#include "Window.hpp"
#include "ui_Window.h"

#include <iostream>
#include <qevent.h>

Window::Window() :
	QMainWindow(), ui(nullptr), simulating(false)
{
	ui = new Ui::Window();
	ui->setupUi(this);

	connect(ui->actionStart, &QAction::triggered, this, &Window::ToggleSimulation);
}

Window::~Window()
{
	delete ui;
	ui = nullptr;
}

void Window::mousePressEvent(QMouseEvent* event)
{
	ui->centralwidget->findChildren<Component*>();
	QFrame* child = static_cast<QFrame*>(ui->centralwidget->childAt(event->pos()));
	if (child == nullptr)
		return;

	if (!child->isAncestorOf(ui->centralwidget))
		child = static_cast<QFrame*>(child->parentWidget());

	dragInfo.component = static_cast<Component*>(child);
	dragInfo.relativePos = event->pos() - child->pos();
}

void Window::mouseMoveEvent(QMouseEvent* event)
{
	if (dragInfo.component != nullptr)
		dragInfo.component->move(event->pos() - dragInfo.relativePos);
}

void Window::mouseReleaseEvent(QMouseEvent* event)
{
	dragInfo.component = nullptr;
}

void Window::ToggleSimulation()
{
	simulating = !simulating;
	ui->actionStart->setIcon(QIcon(simulating ? ":/toolbar/stop.png" : ":/toolbar/start.png"));
}
