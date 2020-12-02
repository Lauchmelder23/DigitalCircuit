#include "Window.hpp"
#include "ui_Window.h"

#include "Component.hpp"
#include "components/NOTgate.hpp"
#include "components/ANDgate.hpp"
#include "components/NANDgate.hpp"
#include "components/ORgate.hpp"
#include "components/NORgate.hpp"
#include "components/XORgate.hpp"
#include "components/XNORgate.hpp"
#include "components/Switch.hpp"
#include "components/Lamp.hpp"

#include <iostream>
#include <qevent.h>
#include "GhostLabel.hpp"
#include <qlayout.h>
#include <qpainter.h>
#include <qactiongroup.h>

Window::Window() :
	QMainWindow(), ui(nullptr), simulating(false), dragInfo{nullptr, QPoint()}, 
	ghostImage(nullptr), componentGroup(nullptr)
{
	ui = new Ui::Window();
	ui->setupUi(this);

	ui->centralwidget->setAttribute(Qt::WA_TransparentForMouseEvents);
	setMouseTracking(true);

	componentGroup = new QActionGroup(this);
	componentGroup->addAction(ui->actionCursor);
	componentGroup->addAction(ui->actionWiring);
	componentGroup->addAction(ui->actionAddNOT);
	componentGroup->addAction(ui->actionAddAND);
	componentGroup->addAction(ui->actionAddNAND);
	componentGroup->addAction(ui->actionAddOR);
	componentGroup->addAction(ui->actionAddNOR);
	componentGroup->addAction(ui->actionAddXOR);
	componentGroup->addAction(ui->actionAddXNOR);
	componentGroup->addAction(ui->actionAddSwitch);
	componentGroup->addAction(ui->actionAddLamp);
	componentGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);

	connect(ui->actionStart,	&QAction::triggered, this, &Window::ToggleSimulation);
	connect(ui->actionCursor, &QAction::triggered, this, &Window::ToggleComponentPlacer);
	connect(ui->actionWiring, &QAction::triggered, this, &Window::ToggleComponentPlacer);
	connect(ui->actionAddNOT,	&QAction::triggered, this, [this]() {componentType = eComponent::NOT , resourcePath = ":/components/not.png"; ToggleComponentPlacer(); });
	connect(ui->actionAddAND,	&QAction::triggered, this, [this]() {componentType = eComponent::AND, resourcePath = ":/components/and.png"; ToggleComponentPlacer(); });
	connect(ui->actionAddNAND,	&QAction::triggered, this, [this]() {componentType = eComponent::NAND , resourcePath = ":/components/nand.png"; ToggleComponentPlacer(); });
	connect(ui->actionAddOR,	&QAction::triggered, this, [this]() {componentType = eComponent::OR , resourcePath = ":/components/or.png"; ToggleComponentPlacer(); });
	connect(ui->actionAddNOR,	&QAction::triggered, this, [this]() {componentType = eComponent::NOR , resourcePath = ":/components/nor.png"; ToggleComponentPlacer(); });
	connect(ui->actionAddXOR,	&QAction::triggered, this, [this]() {componentType = eComponent::XOR, resourcePath = ":/components/xor.png"; ToggleComponentPlacer(); });
	connect(ui->actionAddXNOR,	&QAction::triggered, this, [this]() {componentType = eComponent::XNOR , resourcePath = ":/components/xnor.png"; ToggleComponentPlacer(); });
	connect(ui->actionAddSwitch,&QAction::triggered, this, [this]() {componentType = eComponent::SWITCH , resourcePath = ":/components/switch_off.png"; ToggleComponentPlacer(); });
	connect(ui->actionAddLamp,	&QAction::triggered, this, [this]() {componentType = eComponent::LAMP , resourcePath = ":/components/lamp_off.png"; ToggleComponentPlacer(); });

	pen.setColor(Qt::black);
	pen.setWidth(3);
}

Window::~Window()
{
	delete ui;
	ui = nullptr;

	delete componentGroup;
	componentGroup = nullptr;

	delete ghostImage;
	ghostImage = nullptr;

	for (Component* c : components)
	{
		delete c;
		c = nullptr;
	}
}

void Window::mousePressEvent(QMouseEvent* event)
{
	mouseDown = true;
	if (simulating) return;

	Component* child = componentAt(event->pos());

	QAction* toggledAction = componentGroup->checkedAction();
	if (toggledAction == ui->actionCursor)
	{
		if (child == nullptr)
			return;

		dragInfo.component = static_cast<Component*>(child);
		dragInfo.relativePos = event->pos() - child->pos();
	}
	else if (toggledAction == ui->actionWiring)
	{
		if (child == nullptr)
			return;

		QPoint mousePos = ui->centralwidget->mapFromParent(event->pos());
		mousePos.setY(mousePos.y() + ui->toolBar->height());
		wires.push_back(QLine(child->CenterPos(), mousePos));
		// std::cout << "(" << child->x() << ", " << child->y() << ") --> (" << mousePos.x() << ", " << mousePos.y() << ")" << std::endl;

	}
	else
	{
		if (child != nullptr)
			return;

		createComponent(event->pos());
	}
}

void Window::mouseMoveEvent(QMouseEvent* event)
{
	if (simulating) return;

	if (dragInfo.component != nullptr)
	{
		dragInfo.component->move(event->pos() - dragInfo.relativePos);
		// Move wires too lol
	}

	if (ghostImage != nullptr)
		ghostImage->move(event->pos() - QPoint{50, ui->toolBar->height() + 25});	// TODO: Hardcoded

	if ((componentGroup->checkedAction() == ui->actionWiring) && mouseDown)
	{
		QPoint mousePos = ui->centralwidget->mapFromParent(event->pos());
		mousePos.setY(mousePos.y() + ui->toolBar->height());
		wires.back().setP2(mousePos);
	}

	event->accept();
}

void Window::mouseReleaseEvent(QMouseEvent* event)
{
	if (simulating) return;
	mouseDown = false;

	if (componentGroup->checkedAction() == ui->actionWiring)
	{
		Component* connectFrom = componentAt(wires.back().p1());
		Component* connectTo = componentAt(event->pos());
		if (connectTo == nullptr || connectTo == connectFrom)
		{ 
			wires.pop_back();
		}
		else
		{
			wires.back().setP2(connectTo->CenterPos());
			connectTo->Connect(connectFrom);
		}
	}
	else if(componentGroup->checkedAction() == ui->actionCursor)
	{
		Component* clicked = componentAt(event->pos());
		if (clicked != nullptr)
			if (clicked->Type() == "switch")
				static_cast<Switch*>(clicked)->Toggle();
	}

	dragInfo.component = nullptr;
}

void Window::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setPen(pen);

	for(QLine line : wires)
		painter.drawLine(line);

	update();
}

void Window::ToggleSimulation()
{
	simulating = !simulating;
	ui->actionStart->setIcon(QIcon(simulating ? ":/toolbar/stop.png" : ":/toolbar/start.png"));

	for (Component* c : components)
	{
		if (c->Type() == "lamp")
			static_cast<Lamp*>(c)->Turn(c->Action());
	}
}

void Window::ToggleComponentPlacer()
{
	QAction* toggledAction = componentGroup->checkedAction();
	if (toggledAction == ui->actionCursor || toggledAction == ui->actionWiring)
	{
		delete ghostImage;
		ghostImage = nullptr;

		for (Component* c : components)
			c->setAttribute(Qt::WA_TransparentForMouseEvents, false);
	}
	else
	{
		LoadGhostLabel(resourcePath);
	}
}

void Window::LoadGhostLabel(const QString& resource)
{
	if (ghostImage == nullptr) {
		ghostImage = new GhostLabel(ui->centralwidget);
		QPoint mousePos = ui->centralwidget->mapFromGlobal(QCursor::pos());
		ghostImage->setGeometry({ mousePos.x() - 50, mousePos.y() - 25, 100, 50 });
		ghostImage->setScaledContents(true);
		ghostImage->setVisible(true);

		for (Component* c : components)
			c->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	}

	QPainter p;
	QImage image(resource);

	p.begin(&image);
	p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
	p.fillRect(image.rect(), QColor(0, 0, 0, 50));
	p.end();

	ghostImage->setPixmap(QPixmap::fromImage(image));
}

Component* Window::componentAt(const QPoint& point)
{
	for (Component* c : components)
	{
		if (c->geometry().contains(point))
			return c;
	}

	return nullptr;
}

Component* Window::createComponent(const QPoint& position)
{
	Component* leak = nullptr;

	switch (componentType)
	{
	case eComponent::NOT:		leak = new NOTGate(this);	break;
	case eComponent::AND:		leak = new ANDGate(this);	break;
	case eComponent::NAND:		leak = new NANDGate(this);	break;
	case eComponent::OR:		leak = new ORGate(this);	break;
	case eComponent::NOR:		leak = new NORGate(this);	break;
	case eComponent::XOR:		leak = new XORGate(this);	break;
	case eComponent::XNOR:		leak = new XNORGate(this);	break;
	case eComponent::SWITCH:	leak = new Switch(this);	break;
	case eComponent::LAMP:		leak = new Lamp(this);		break;
	}

	if (leak != nullptr)
	{
		QPoint pos = position - QPoint{ 50, 25 };
		leak->setGeometry(QRect(pos.x(), pos.y(), 100, 50));
		leak->setFrameShape(QFrame::NoFrame);
		leak->setFrameShadow(QFrame::Plain);
		leak->setVisible(true);
		leak->setAttribute(Qt::WA_TransparentForMouseEvents);

		components.push_back(leak);
	}

	return leak;
}
