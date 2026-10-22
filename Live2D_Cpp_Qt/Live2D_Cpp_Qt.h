#pragma once

#include <QtWidgets/QWidget>
#include "ui_Live2D_Cpp_Qt.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Live2D_Cpp_QtClass; };
QT_END_NAMESPACE

class Live2D_Cpp_Qt : public QWidget
{
    Q_OBJECT

public:
    Live2D_Cpp_Qt(QWidget *parent = nullptr);
    ~Live2D_Cpp_Qt();

private:
    Ui::Live2D_Cpp_QtClass *ui;
};

