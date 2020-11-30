#pragma once

#include <qmainwindow.h>

namespace Ui {
	class Window;
}

class Component;

class Window : public QMainWindow
{
public:
	Window();
	~Window();

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	void ToggleSimulation();

	Ui::Window* ui;
	bool simulating;

	struct {
		Component* component;
		QPoint relativePos;
	} dragInfo;
};