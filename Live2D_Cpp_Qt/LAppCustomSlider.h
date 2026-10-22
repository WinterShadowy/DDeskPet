#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QSlider>
#include <QPushButton>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QVBoxLayout>

// ========== 自定义增强型滑动条-浮点型 ===========
class DDeskPet_FloatSlider : public QWidget {
    Q_OBJECT
public:
    explicit DDeskPet_FloatSlider(QWidget* parent = nullptr);

    // 设置范围
    void setRange(double min, double max);
    // 设置当前值
    void setValue(double value);
    // 设置步长
    void setStep(double step);
    // 设置精度
    void setDecimals(int decimals);
    // 设置刻度精度
    void setTickCount(int tickCount);
    // 获取当前值
    double getValue();
    // 获取最小值
    double getMinValue();
    // 获取最大值
    double getMaxValue();
    // 设置是否禁用
    void setDisabled(bool disabled);
signals:
    void valueChanged(double value);
    void userValueChanged(double value);

private slots:
    void onSliderValueChanged(int intValue);
    void onSliderReleased();
    void onDecrementClicked();
    void onIncrementClicked();
    void onTextEditingFinished();

private:
    void updateDisplay();
    void updateSlider();
    double intToFloat(int intValue) const;
    int floatToInt(double floatValue) const;
    double clamp(double value) const;

private:
    // UI组件
    QLineEdit* m_valueEdit;     // 数值编辑框
    QPushButton* m_decrementBtn;    // 减少按钮编辑框
    QSlider* m_slider;      // 滑动条
    QPushButton* m_incrementBtn;    // 增加按钮编辑框

    // 数据
    double m_min = 0.0;
    double m_max = 100.0;
    double m_value = 50.0;
    double m_step = 1.0;
    int m_decimals = 2;
    int m_tickCount = 1000;

    bool m_updating = false;  // 防止递归更新
};

// ========== end ==========

// ========== 自定义增强型滑动条-整型 =========== 
class DDeskPet_IntegerSlider : public QWidget {
    Q_OBJECT
public:
    explicit DDeskPet_IntegerSlider(QWidget* parent = nullptr);

    // 设置范围
    void setRange(int min, int max);
    // 设置当前值
    void setValue(int value);
    // 设置步长
    void setStep(int step);
    // 设置刻度精度
    void setTickCount(int tickCount);
    // 获取当前值
    int getValue();
    // 获取最小值
    int getMinValue();
    // 获取最大值
    int getMaxValue();
    // 设置是否启用
    void setEnabled(bool enabled);

signals:
    void valueChanged(int value);
    void lineeditValueChanged(int value);

private slots:
    void onSliderValueChanged(int intValue);
    void onSliderReleased();
    void onDecrementClicked();
    void onIncrementClicked();
    void onTextEditingFinished();

private:
    void updateDisplay();
    void updateSlider();
    int clamp(int value) const;

private:
    // UI组件
    QLineEdit* m_valueEdit;     // 数值编辑框
    QPushButton* m_decrementBtn;    // 减少按钮编辑框
    QSlider* m_slider;      // 滑动条
    QPushButton* m_incrementBtn;    // 增加按钮编辑框

    // 数据
    int m_min = 0;      // 最小值
    int m_max = 100;    // 最大值
    int m_value = 50;   // 当前值
    int m_step = 1;     // 步长
    int m_tickCount = 1000; // 刻度精度

    bool m_updating = false;  // 防止递归更新
};

// ========== end ==========