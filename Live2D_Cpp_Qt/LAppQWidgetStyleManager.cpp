#include "LAppQWidgetStyleManager.h"

bool LAppStyleManager::SetLabelStyle(QLabel* label, int index)
{
    if (index < 0 || index > 27)
    {
        RuntimeLog_Error(QString("Index out of bounds:%1").arg(index).toStdString().c_str());
        return false;
    }
    static const QString tbl[40] = {
        // 0
        "QLabel{ background:#ffffff; color:#202020; font:8pt; border:none; }",
        // 1
        "QLabel{ background:#f5f5f5; color:#202020; font:8pt; border-radius:4px; padding:4px; }",
        // 2
        "QLabel{ background:#e3f2fd; color:#1565c0; font:8pt; border:1px solid #90caf9; border-radius:0px; padding:0px; }",
        // 3
        "QLabel{ background:#e8f5e9; color:#2e7d32; font:8pt; border:1px solid #a5d6a7; border-radius:0px; padding:0px; }",
        // 4
        "QLabel{ background:#fff8e1; color:#f57c00; font:8pt; border:1px solid #ffe082; border-radius:0px; padding:0px; }",
        // 5
        "QLabel{ background:#ffebee; color:#c62828; font:7pt; border:1px solid #ef9a9a; padding:0px; }",
        // 6
        "QLabel{ background:transparent; color:#303030; font: bold 12pt; }",
        // 7
        "QLabel{ background:#dcdcdc; max-height:1px; }",
        // 8
        "QLabel{ background:#424242; color:#fafafa; font:9pt; border-radius:4px; padding:4px; }",
        // 9
        "QLabel{ background:#1976d2; color:#ffffff; font:9pt; border-radius:4px; padding:4px; }",
        //10
        "QLabel{ background:#e0e0e0; color:#424242; font:9pt; border-radius:8px; padding:4px 8px; }",
        //11
        "QLabel{ background:#fff; color:#757575; font:9pt; border:1px solid #bdbdbd; border-radius:3px; }",
        //12
        "QLabel{ background:#fafafa; color:#202020; font:9pt; border:1px solid #e0e0e0; border-radius:3px; padding:3px; }",
        //13
        "QLabel{ background:transparent; color:#202020; font:9pt; border-bottom:1px solid #e0e0e0; padding:4px 0px; }",
        //14
        "QLabel{ background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #64b5f6,stop:1 #1976d2); color:#ffffff; font: bold 7pt; border-radius:4px; padding:2px; }",
        // 15: "?" Label`s tooltip style
        "QLabel{ border: 1px solid #d0d0d0; border-radius: 4px; padding: 1px; background-color: #f5f5f5; font-size: 10px;}",
        // 16 ÔËÐÐ/Æô¶¯
        "QLabel{ background:#4caf50; color:#ffffff; font:bold 6pt; border:1px solid #388e3c; border-radius:12px;}",
        // 17 Í£Ö¹/ÀëÏß
        "QLabel{ background:#9e9e9e; color:#ffffff; font:bold 6pt; border:1px solid #616161; border-radius:12px;}",
        // 18 ¸æ¾¯
        "QLabel{ background:#ff9800; color:#ffffff; font:bold 6pt; border:1px solid #f57c00; border-radius:12px;}",
        // 19 ¹ÊÕÏ
        "QLabel{ background:#f44336; color:#ffffff; font:bold 6pt; border:1px solid #d32f2f; border-radius:12px;}",
        // 20 Î´³õÊ¼»¯
        "QLabel{ background:#eceff1; color:#546e7a; font:bold 8pt; border:1px solid #90a4ae; border-radius:12px; qproperty-alignment: AlignCenter;}",
        // 21 Çá»ÒÀäµ­·ç
        "QLabel{background:#eceff1;color:#546e7a;font:bold 8pt;border:1px solid #90a4ae;border-radius:12px;padding:2px 6px;qproperty-alignment:AlignCenter;}",
        // 22 Éî¿ÕÒ¹À¶
        "QLabel{background:#263238;color:#eceff1;font:bold 8pt;border:1px solid #37474f;border-radius:12px;padding:2px 6px;qproperty-alignment:AlignCenter;}",
        // 23 ±¡ºÉÇåË¬
        "QLabel{background:#e0f2f1;color:#00796b;font:bold 8pt;border:1px solid #80cbc4;border-radius:12px;padding:2px 6px;qproperty-alignment:AlignCenter;}",
        // 24 Ó£»¨Å¯É«
        "QLabel{background:#fce4ec;color:#c2185b;font:bold 8pt;border:1px solid #f48fb1;border-radius:12px;padding:2px 6px;qproperty-alignment:AlignCenter;}",
        // 25 ¼«¼ò´¿°×
        "QLabel{background:#ffffff;color:#424242;font:bold 8pt;border:1px solid #bdbdbd;border-radius:12px;padding:2px 6px;qproperty-alignment:AlignCenter;}",
        // 26 Èü²©ÄÞºç
        "QLabel{background:#1a1a2e;color:#00f9ff;font:bold 8pt;border:1px solid #00f9ff;border-radius:12px;padding:2px 6px;qproperty-alignment:AlignCenter;}",
        // 27 ÎÊºÅ×¨ÓÃ
        "QLabel{border: 1px solid #d0d0d0;border-radius: 4px;padding: 1px;background-color: #f5f5f5;}"
    };

    label->setStyleSheet(tbl[index]);
    return true;
}

bool LAppStyleManager::SetPushButtonStyle(QPushButton* pushbutton, int index)
{
    if (index < 0 || index > 6)
    {
		RuntimeLog_Error(QString("Index out of bounds:%1").arg(index).toStdString().c_str());
        return false;
    }
    static const QString tbl[40] = {
        // 0 ±¡ºÉ½ºÄÒ
        "QPushButton{background:#26a69a;color:#fff;font:9pt;border:none;border-radius:14px;padding:6px 16px;}QPushButton:hover{background:#29b6f6;}QPushButton:pressed{background:#00897b;}",
        // 1 Éî¿ÕÀ¶Ïß
        "QPushButton{background:#37474f;color:#eceff1;font:9pt;border:1px solid #546e7a;border-radius:4px;padding:5px 12px;}QPushButton:hover{background:#455a64;}QPushButton:pressed{background:#263238;}",
        // 2 ¼«¼ò´¿°×
        "QPushButton{background:#ffffff;color:#424242;font:9pt;border:1px solid #bdbdbd;border-radius:4px;padding:5px 12px;}QPushButton:hover{background:#fafafa;}QPushButton:pressed{background:#e0e0e0;}",
        // 3 Ó£»¨Ô²µã
        "QPushButton{background:#fce4ec;color:#c2185b;font:9pt;border:none;border-radius:10px;padding:6px 14px;}QPushButton:hover{background:#f8bbd9;}QPushButton:pressed{background:#f48fb1;}",
        // 4 Èü²©ÄÞºç
        "QPushButton{background:#1a1a2e;color:#00f9ff;font:9pt;border:1px solid #00f9ff;border-radius:12px;padding:6px 16px;}QPushButton:hover{color:#ffffff;background:#16213e;}QPushButton:pressed{background:#0f3460;}",
        // 5 ²à±ßÀ¸Button
        "QPushButton {text-align: left;font-size: 14px;background: rgb(255, 255, 255);border: none;padding-left:6px;padding-right:12px;}QPushButton:checked {font: bold 14px;background: rgb(240, 240, 240);padding-left:6px;padding-right:12px;}"
    };

    pushbutton->setStyleSheet(tbl[index]);
    return true;
}

bool LAppStyleManager::SetCheckBoxStyle(QCheckBox* checkbox, int index)
{
    return false;
}

bool LAppStyleManager::SetRadioButtonStyle(QRadioButton* radiobutton, int index)
{
    return false;
}

bool LAppStyleManager::SetGroupBoxStyle(QGroupBox* gruopbox, int index)
{
    return false;
}

bool LAppStyleManager::SetLineEditStyle(QLineEdit* lineedit, int index)
{
    return false;
}

bool LAppStyleManager::SetTextEditStyle(QTextEdit* textedit, int index)
{
    return false;
}

bool LAppStyleManager::SetPlainTextEditStyle(QPlainTextEdit* plaintextedit, int index)
{
    return false;
}

bool LAppStyleManager::SetTabWidgetStyle(QTabWidget* tabwidget, int index)
{
    if (index < 0 || index > 4)
    {
        RuntimeLog_Error(QString("Index out of bounds:%1").arg(index).toStdString().c_str());
        return false;
    }
    static const QString tbl[4] = {
        // 0 µ­»Ò¸¡µñ
        "QTabWidget::pane{ border:1px solid #c8c8c8; border-radius:6px; background:#fdfdfd; margin:4px 0 0 0; }"
        "QTabBar::tab{ min-width:80px; height:24px; margin:0 1px 0 0; padding:0 10px; "
        "background:#f0f0f0; color:#555; font:9pt; border:1px solid #d5d5d5; border-bottom:none; border-radius:4px 4px 0 0; }"
        "QTabBar::tab:selected{ background:#fdfdfd; border-color:#c8c8c8; }"
        "QTabBar::tab:!selected{ margin-top:3px; }"
        "QTabBar::tab:hover:!selected{ background:#e8e8e8; }",

        // 1 Î¢Í¹°×¿¨
        "QTabWidget::pane{ border:1px solid #e0e0e0; border-radius:5px; background:#ffffff; "
        "margin:3px 0 0 0; box-shadow:inset 0 1px 2px rgba(0,0,0,0.03); }"
        "QTabBar::tab{ min-width:80px; height:24px; margin:0 2px 0 0; padding:0 10px; "
        "background:#fafafa; color:#444; font:9pt; border:1px solid #e0e0e0; border-bottom:none; border-radius:5px 5px 0 0; }"
        "QTabBar::tab:selected{ background:#ffffff; border-color:#e0e0e0; }"
        "QTabBar::tab:!selected{ margin-top:2px; }"
        "QTabBar::tab:hover:!selected{ background:#f5f5f5; }",

        // 2 Àä»ÒÄÚ°¼
        "QTabWidget::pane{ border:1px solid #b8b8b8; border-radius:4px; background:#f8f8f8; "
        "margin:4px 0 0 0; box-shadow:inset 0 2px 4px rgba(0,0,0,0.05); }"
        "QTabBar::tab{ min-width:80px; height:24px; margin:0 1px 0 0; padding:0 10px; "
        "background:#e8e8e8; color:#444; font:9pt; border:1px solid #b8b8b8; border-bottom:none; border-radius:4px 4px 0 0; }"
        "QTabBar::tab:selected{ background:#f8f8f8; border-color:#b8b8b8; }"
        "QTabBar::tab:!selected{ margin-top:3px; }"
        "QTabBar::tab:hover:!selected{ background:#dee0e2; }",

        // 3 Ç³À¶Ãè±ß
        "QTabWidget::pane{ border:1px solid #a0c6e8; border-radius:6px; background:#fbfdff; margin:4px 0 0 0; }"
        "QTabBar::tab{ min-width:80px; height:24px; margin:0 1px 0 0; padding:0 10px; "
        "background:#eef4fb; color:#3e5a8c; font:9pt; border:1px solid #a0c6e8; border-bottom:none; border-radius:5px 5px 0 0; }"
        "QTabBar::tab:selected{ background:#fbfdff; border-color:#a0c6e8; }"
        "QTabBar::tab:!selected{ margin-top:3px; }"
        "QTabBar::tab:hover:!selected{ background:#e2ebf8; }"
    };

    tabwidget->setStyleSheet(tbl[index]);
    return true;
}
