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

	LoadGhostLabel(":/components/and.png");

	componentGroup = new QActionGroup(this);
	componentGroup->addAction(ui->actionAddAND);
	componentGroup->addAction(ui->actionAddOR);
	componentGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::ExclusiveOptional);

	connect(ui->actionStart, &QAction::triggered, this, &Window::ToggleSimulation);
	connect(ui->actionAddAND, &QAction::triggered, this, [this]() {resourcePath = ":/components/and.png"; ToggleComponentPlacer(); });
	connect(ui->actionAddOR, &QAction::triggered, this, [this]() {resourcePath = ":/components/or.png"; ToggleComponentPlacer(); });
}

Window::~Window()
{
	delete ui;
	ui = nullptr;
}

void Window::mousePressEvent(QMouseEvent* event)
{
	if (simulating) return;

	ui->centralwidget->findChildren<Component*>();
	QFrame* child = static_cast<QFrame*>(ui->centralwidget->childAt(event->pos()));

	QAction* toggledAction = componentGroup->checkedAction();
	if (toggledAction == nullptr)
	{
		if (child == nullptr)
			return;

		if (!child->isAncestorOf(ui->centralwidget))
			child = static_cast<QFrame*>(child->parentWidget());

		dragInfo.component = static_cast<Component*>(child);
		dragInfo.relativePos = event->pos() - child->pos();
	}
	else
	{
		if (child != nullptr)
			return;

		// So Qt can't add widgets outside of the constructor apparently
		// So component cannot be a QFrame, it will have to become just a normal object
		// And I need one of these open gl gsjdööööööööööööööl<
		// TODO: I'll just let this leak for now, I just wanna eat dinner...
		Component* leak = new Component(this, resourcePath);
		QPoint pos = event->pos() - QPoint {50, 25};
		leak->setGeometry(QRect(280, 160, 100, 50));
		leak->setFrameShape(QFrame::StyledPanel);
		leak->setFrameShadow(QFrame::Raised);

		std::cout << leak->pos().x() << ", " << leak->pos().y() << std::endl;
	}
}

void Window::mouseMoveEvent(QMouseEvent* event)
{
	if (dragInfo.component != nullptr)
		dragInfo.component->move(event->pos() - dragInfo.relativePos);

	if (ghostImage != nullptr)
		ghostImage->move(event->pos() - QPoint{50, 25});	// TODO: Hardcoded

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
	}
	else
	{
		LoadGhostLabel(resourcePath);
	}
}

void Window::LoadGhostLabel(const QString& resource)
{
	if (ghostImage == nullptr) {
		ghostImage = new GhostLabel(this);
		ghostImage->setGeometry({ 0, 0, 100, 50 });
		ghostImage->setScaledContents(true);
	}

	QPainter p;
	QImage image(resource);

	p.begin(&image);
	p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
	p.fillRect(image.rect(), QColor(0, 0, 0, 50));
	p.end();

	ghostImage->setPixmap(QPixmap::fromImage(image));
}
