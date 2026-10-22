#include "Live2D_Cpp_Qt.h"

Live2D_Cpp_Qt::Live2D_Cpp_Qt(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Live2D_Cpp_QtClass())
{
    ui->setupUi(this);
}

Live2D_Cpp_Qt::~Live2D_Cpp_Qt()
{
    delete ui;
}

