
/*
 ============================================================================
 Header: LAppQWidgetStyleManager.h
 Description: QWidget样式管理
 Start time: 2025.10.20
 End time: 2026.01.07
 Maintainer: WangSZ
 ============================================================================
*/

#pragma once

#include <QGroupBox>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QSpinBox>
#include <QComboBox>
#include <QTabBar>
#include <QTabWidget>
#include <QListWidget>
#include <QToolBox>
// button
#include <QToolButton>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QAbstractButton>
// edit
#include <QLineEdit>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QKeySequenceEdit>

#include "LAppDefine.hpp"

namespace LAppStyleManager {
	// 设置Label样式
	bool SetLabelStyle(QLabel* label, int index);
	// 设置Button样式
	bool SetPushButtonStyle(QPushButton* pushbutton, int index);
	// 设置CheckBox样式
	bool SetCheckBoxStyle(QCheckBox* checkbox, int index);
	// 设置RadioButton样式
	bool SetRadioButtonStyle(QRadioButton* radiobutton, int index);
	// 设置GroupBox样式
	bool SetGroupBoxStyle(QGroupBox* gruopbox, int index);
	// 设置LineEdit样式
	bool SetLineEditStyle(QLineEdit* lineedit, int index);
	// 设置TextEdit样式
	bool SetTextEditStyle(QTextEdit* textedit, int index);
	// 设置PlainTextEdit样式
	bool SetPlainTextEditStyle(QPlainTextEdit* plaintextedit, int index);
	// 设置TabWidget样式
	bool SetTabWidgetStyle(QTabWidget* tabwidget, int index);
}

namespace QWidgetStyleManager = LAppStyleManager;