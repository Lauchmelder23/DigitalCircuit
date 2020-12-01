#pragma once

#include <qmainwindow.h>

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

private:
	void ToggleSimulation();
	void ToggleComponentPlacer();
	void LoadGhostLabel(const QString& resource);

	Ui::Window* ui;
	bool simulating;

	struct {
		Component* component;
		QPoint relativePos;
	} dragInfo;

	GhostLabel* ghostImage;
	QString resourcePath;
	QActionGroup* componentGroup;
};