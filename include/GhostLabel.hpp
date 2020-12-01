#pragma once

#include <qlabel.h>

class GhostLabel : public QLabel
{
public:
	explicit GhostLabel(QWidget* parent) :
		QLabel(parent)
	{
		setMouseTracking(true);
	}

	void mouseMoveEvent(QMouseEvent* event) override
	{
		// Ignore all mouse move events, who cares tbh
		event->ignore();
	}
};