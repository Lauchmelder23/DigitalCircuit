#include "Window.hpp"
#include "ui_Window.h"

#include "Component.hpp"

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
	componentGroup->addAction(ui->actionAddNOT);
	componentGroup->addAction(ui->actionAddAND);
	componentGroup->addAction(ui->actionAddNAND);
	componentGroup->addAction(ui->actionAddOR);
	componentGroup->addAction(ui->actionAddNOR);
	componentGroup->addAction(ui->actionAddXOR);
	componentGroup->addAction(ui->actionAddXNOR);
	componentGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::ExclusiveOptional);

	connect(ui->actionStart,	&QAction::triggered, this, &Window::ToggleSimulation);
	connect(ui->actionAddNOT,	&QAction::triggered, this, [this]() {resourcePath = ":/components/not.png"; ToggleComponentPlacer(); });
	connect(ui->actionAddAND,	&QAction::triggered, this, [this]() {resourcePath = ":/components/and.png"; ToggleComponentPlacer(); });
	connect(ui->actionAddNAND,	&QAction::triggered, this, [this]() {resourcePath = ":/components/nand.png"; ToggleComponentPlacer(); });
	connect(ui->actionAddOR,	&QAction::triggered, this, [this]() {resourcePath = ":/components/or.png"; ToggleComponentPlacer(); });
	connect(ui->actionAddNOR,	&QAction::triggered, this, [this]() {resourcePath = ":/components/nor.png"; ToggleComponentPlacer(); });
	connect(ui->actionAddXOR,	&QAction::triggered, this, [this]() {resourcePath = ":/components/xor.png"; ToggleComponentPlacer(); });
	connect(ui->actionAddXNOR,	&QAction::triggered, this, [this]() {resourcePath = ":/components/xnor.png"; ToggleComponentPlacer(); });
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
	if (simulating) return;

	Component* child = componentAt(event->pos());

	QAction* toggledAction = componentGroup->checkedAction();
	if (toggledAction == nullptr)
	{
		if (child == nullptr)
			return;

		dragInfo.component = static_cast<Component*>(child);
		dragInfo.relativePos = event->pos() - child->pos();
	}
	else
	{
		if (child != nullptr)
			return;

		Component* leak = new Component(this, resourcePath);
		QPoint pos = event->pos() - QPoint {50, 25};
		leak->setGeometry(QRect(pos.x(), pos.y(), 100, 50));
		leak->setFrameShape(QFrame::NoFrame);
		leak->setFrameShadow(QFrame::Plain);
		leak->setVisible(true);
		leak->setAttribute(Qt::WA_TransparentForMouseEvents);

		components.push_back(leak);
	}
}

void Window::mouseMoveEvent(QMouseEvent* event)
{
	if (dragInfo.component != nullptr)
		dragInfo.component->move(event->pos() - dragInfo.relativePos);

	if (ghostImage != nullptr)
		ghostImage->move(event->pos() - QPoint{50, ui->toolBar->height() + 25});	// TODO: Hardcoded

	event->accept();
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

void Window::ToggleComponentPlacer()
{
	QAction* toggledAction = componentGroup->checkedAction();
	if (toggledAction == nullptr)
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
