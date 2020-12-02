#pragma once

#include <qmainwindow.h>
#include <qpen.h>

namespace Ui {
	class Window;
}

class Component;
class QActionGroup;
class GhostLabel;

class Window : public QMainWindow
{
public:
	Window();
	~Window();

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;

private:
	void ToggleSimulation();
	void ToggleComponentPlacer();
	void LoadGhostLabel(const QString& resource);
	Component* componentAt(const QPoint& point);
	Component* createComponent(const QPoint& position);

	Ui::Window* ui;
	bool simulating;

	struct {
		Component* component;
		QPoint relativePos;
	} dragInfo;
	bool mouseDown;

	GhostLabel* ghostImage;
	QString resourcePath;
	QPen pen;

	QActionGroup* componentGroup;

	enum class eComponent {
		NOT, AND, NAND, OR, NOR, XOR, XNOR, SWITCH, LAMP
	} componentType;

	std::vector<Component*> components;
	std::vector<QLine> wires;
};