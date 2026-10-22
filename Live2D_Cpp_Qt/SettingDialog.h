#pragma once

#include "SettingVTabPageBar.h"
#include "SettingFuncPage.h"
#include "ServerUser.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QTabBar>
#include <QStackedWidget>
#include <QtConcurrent/QtConcurrentRun>


class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget* parent = nullptr);
    ~SettingsDialog() = default;

    void resizeEvent(QResizeEvent* event) override;

public:
    static void CB_SetToolButtonStyle(QPushButton* btn);
private:
    VTabPageBar* m_vtab_Side                      = nullptr;  // 侧边栏
    QWidget* m_widget_PageUser                    = nullptr;  // 账户
    QWidget* m_widget_PagePersonalization         = nullptr;  // 个性化
    QWidget* m_widget_PageFuntionWindows          = nullptr;  // 个性化功能窗口
    QWidget* m_widget_PageSetAI                   = nullptr;  // AI设置
    QWidget* m_widget_PageChat                    = nullptr;  // AI聊天
	QWidget* m_widget_PageModel                   = nullptr;  // 模型
    QWidget* m_widget_PageDeveloper               = nullptr;  // 开发者功能
    QVector<QWidget*> m_SettingPageInfoPool;

};