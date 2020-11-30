#include "Component.hpp"
#include "ui_Component.h"

Component::Component(QWidget* parent) :
	QFrame(parent), ui(nullptr)
{
	ui = new Ui::Component();
	ui->setupUi(this);
}