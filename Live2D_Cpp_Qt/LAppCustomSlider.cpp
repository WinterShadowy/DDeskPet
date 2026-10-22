#include "LAppCustomSlider.h"

// ========== FloatSlider ========== FloatSlider ========== FloatSlider ========== FloatSlider ===========
DDeskPet_FloatSlider::DDeskPet_FloatSlider(QWidget* parent) :
    QWidget(parent)
{
    this->m_valueEdit = new QLineEdit(this);
    this->m_decrementBtn = new QPushButton("−", this);
    this->m_slider = new QSlider(this);
    this->m_incrementBtn = new QPushButton("+", this);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);
    this->m_valueEdit->setFixedWidth(60);
    this->m_valueEdit->setAlignment(Qt::AlignCenter);
    QFont editFont = this->m_valueEdit->font();
    editFont.setPointSize(10);
    this->m_valueEdit->setFont(editFont);

    // 减小按钮
    {
        this->m_decrementBtn->setFixedSize(28, 28);
        this->m_decrementBtn->setToolTip("减小 " + QString::number(m_step));
        QFont btnFont_decrement = this->m_decrementBtn->font();
        btnFont_decrement.setBold(true);
        btnFont_decrement.setPointSize(12);
        this->m_decrementBtn->setFont(btnFont_decrement);
        layout->addWidget(this->m_decrementBtn);
    }

    // 滑块
    {
        this->m_slider->setRange(0, m_tickCount);
        this->m_slider->setSingleStep(1);
        this->m_slider->setPageStep(m_tickCount / 10);
        this->m_slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        this->m_slider->setOrientation(Qt::Horizontal);
        layout->addWidget(this->m_slider, 1);
    }

    // 增大按钮 [+]
    {
        this->m_incrementBtn->setFixedSize(28, 28);
        this->m_incrementBtn->setToolTip("增大" + QString::number(m_step));
        QFont btnFont = m_decrementBtn->font();
        btnFont.setBold(true);
        btnFont.setPointSize(12);
        this->m_incrementBtn->setFont(btnFont);
        layout->addWidget(this->m_incrementBtn);
    }

    QDoubleValidator* validator = new QDoubleValidator(this->m_min, this->m_max, this->m_decimals, this);
    validator->setNotation(QDoubleValidator::StandardNotation);
    this->m_valueEdit->setValidator(validator);
    this->m_valueEdit->adjustSize();
    layout->addWidget(this->m_valueEdit);

    // 连接信号
    connect(this->m_slider, &QSlider::valueChanged, this, &DDeskPet_FloatSlider::onSliderValueChanged);
    connect(this->m_slider, &QSlider::sliderReleased, this, &DDeskPet_FloatSlider::onSliderReleased);
    connect(this->m_decrementBtn, &QPushButton::clicked, this, &DDeskPet_FloatSlider::onDecrementClicked);
    connect(this->m_incrementBtn, &QPushButton::clicked, this, &DDeskPet_FloatSlider::onIncrementClicked);
    connect(this->m_valueEdit, &QLineEdit::editingFinished, this, &DDeskPet_FloatSlider::onTextEditingFinished);

    // 初始化显示
    updateDisplay();
    updateSlider();
}

void DDeskPet_FloatSlider::setRange(double min, double max)
{
    if (min >= max)
    {
        return;
    }
    this->m_min = min;
    this->m_max = max;

    // 更新验证器范围
    QDoubleValidator* validator = qobject_cast<QDoubleValidator*>(const_cast<QValidator*>(this->m_valueEdit->validator()));
    if (validator)
    {
        validator->setRange(this->m_min, this->m_max, this->m_decimals);
    }

    // 限制当前值在范围内
    setValue(clamp(this->m_value));
}

void DDeskPet_FloatSlider::setValue(double value)
{
    double newValue = clamp(value);
    if (std::abs(newValue - this->m_value) < std::pow(10, -m_decimals - 1))
    {
        // 差异太小，只更新显示
        updateDisplay();
        updateSlider();
        return;
    }

    this->m_value = newValue;
    updateDisplay();
    updateSlider();
    emit valueChanged(this->m_value);
}

void DDeskPet_FloatSlider::setStep(double step)
{
    if (step > 0)
    {
        this->m_step = step;
        this->m_decrementBtn->setToolTip("\n减小 " + QString::number(this->m_step));
        this->m_incrementBtn->setToolTip("\n增大 " + QString::number(this->m_step));
    }
}

void DDeskPet_FloatSlider::setDecimals(int decimals)
{
    if (decimals >= 0 && decimals <= 10)
    {
        this->m_decimals = decimals;
        // 更新验证器
        QDoubleValidator* validator = qobject_cast<QDoubleValidator*>(const_cast<QValidator*>(this->m_valueEdit->validator()));
        if (validator)
        {
            validator->setDecimals(this->m_decimals);
        }
        updateDisplay();
    }
}

void DDeskPet_FloatSlider::setTickCount(int tickCount)
{
    if (tickCount > 0)
    {
        m_tickCount = tickCount;
        m_slider->setRange(0, m_tickCount);
        m_slider->setPageStep(m_tickCount / 10);
        updateSlider();
    }
}

double DDeskPet_FloatSlider::getValue()
{
    return this->m_value;
}

double DDeskPet_FloatSlider::getMinValue()
{
    return this->m_min;
}

double DDeskPet_FloatSlider::getMaxValue()
{
    return this->m_max;
}

void DDeskPet_FloatSlider::setDisabled(bool disabled)
{
    this->m_valueEdit->setDisabled(disabled);
    this->m_decrementBtn->setDisabled(disabled);
    this->m_slider->setDisabled(disabled);
    this->m_incrementBtn->setDisabled(disabled);
}

void DDeskPet_FloatSlider::onSliderValueChanged(int intValue)
{
    if (this->m_updating)
    {
        return;
    }

    double newValue = intToFloat(intValue);
    // 根据精度四舍五入
    double factor = std::pow(10, this->m_decimals);
    newValue = std::round(newValue * factor) / factor;
    newValue = clamp(newValue);

    if (std::abs(newValue - this->m_value) >= std::pow(10, -this->m_decimals - 1))
    {
        this->m_value = newValue;
        updateDisplay();
        emit valueChanged(this->m_value);
    }
}

void DDeskPet_FloatSlider::onSliderReleased()
{
    // 用户释放滑块时发射 userValueChanged
    emit userValueChanged(this->m_value);
}

void DDeskPet_FloatSlider::onDecrementClicked()
{
    double newValue = clamp(this->m_value - this->m_step);
    if (newValue != this->m_value)
    {
        setValue(newValue);
        emit userValueChanged(this->m_value);
    }
}

void DDeskPet_FloatSlider::onIncrementClicked()
{
    double newValue = clamp(this->m_value + this->m_step);
    if (newValue != this->m_value)
    {
        setValue(newValue);
        emit userValueChanged(this->m_value);
    }
}

void DDeskPet_FloatSlider::onTextEditingFinished()
{
    bool ok;
    double newValue = this->m_valueEdit->text().toDouble(&ok);
    if (ok)
    {
        newValue = clamp(newValue);
        // 根据精度四舍五入
        double factor = std::pow(10, this->m_decimals);
        newValue = std::round(newValue * factor) / factor;

        if (std::abs(newValue - this->m_value) >= std::pow(10, -this->m_decimals - 1))
        {
            this->m_value = newValue;
            updateDisplay();
            updateSlider();
            emit valueChanged(this->m_value);
            emit userValueChanged(this->m_value);
        }
        else
        {
            updateDisplay(); // 恢复显示
        }
    }
    else
    {
        updateDisplay(); // 输入无效，恢复显示
    }
}

void DDeskPet_FloatSlider::updateDisplay()
{
    this->m_updating = true;
    this->m_valueEdit->setText(QString::number(this->m_value, 'f', this->m_decimals));
    this->m_valueEdit->adjustSize();
    this->m_updating = false;
}

void DDeskPet_FloatSlider::updateSlider()
{
    this->m_updating = true;
    this->m_slider->setValue(floatToInt(this->m_value));
    this->m_updating = false;
}

double DDeskPet_FloatSlider::intToFloat(int intValue) const
{
    if (this->m_tickCount == 0)
    {
        return this->m_min;
    }
    double ratio = static_cast<double>(intValue) / this->m_tickCount;
    return this->m_min + ratio * (this->m_max - this->m_min);
}

int DDeskPet_FloatSlider::floatToInt(double floatValue) const
{
    if (this->m_max == this->m_min)
    {
        return 0;
    }
    double ratio = (floatValue - this->m_min) / (this->m_max - this->m_min);
    return static_cast<int>(std::round(ratio * this->m_tickCount));
}

double DDeskPet_FloatSlider::clamp(double value) const
{
    if (value < m_min)
    {
        return m_min;
    }
    if (value > m_max)
    {
        return m_max;
    }
    return value;
}

// ========== end ========== end ========== end =========== end =========== end =========== end ========== end =========== end ==========

// ========== FloatSlider ========== FloatSlider ========== FloatSlider ========== FloatSlider ===========

DDeskPet_IntegerSlider::DDeskPet_IntegerSlider(QWidget* parent) :
    QWidget(parent)
{
    this->m_valueEdit = new QLineEdit(this);
    this->m_decrementBtn = new QPushButton("−", this);
    this->m_slider = new QSlider(this);
    this->m_incrementBtn = new QPushButton("+", this);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);
    this->m_valueEdit->setFixedWidth(60);
    this->m_valueEdit->setAlignment(Qt::AlignCenter);
    QFont editFont = this->m_valueEdit->font();
    editFont.setPointSize(10);
    this->m_valueEdit->setFont(editFont);

    // 减小按钮
    {
        this->m_decrementBtn->setFixedSize(28, 28);
        this->m_decrementBtn->setToolTip("减小 " + QString::number(m_step));
        QFont btnFont_decrement = this->m_decrementBtn->font();
        btnFont_decrement.setBold(true);
        btnFont_decrement.setPointSize(12);
        this->m_decrementBtn->setFont(btnFont_decrement);
        layout->addWidget(this->m_decrementBtn);
    }

    // 滑块
    {
        this->m_slider->setRange(0, m_tickCount);
        this->m_slider->setSingleStep(1);
        this->m_slider->setPageStep(m_tickCount / 10);
        this->m_slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        this->m_slider->setOrientation(Qt::Horizontal);
        layout->addWidget(this->m_slider, 1);
    }

    // 增大按钮 [+]
    {
        this->m_incrementBtn->setFixedSize(28, 28);
        this->m_incrementBtn->setToolTip("增大" + QString::number(m_step));
        QFont btnFont = m_decrementBtn->font();
        btnFont.setBold(true);
        btnFont.setPointSize(12);
        this->m_incrementBtn->setFont(btnFont);
        layout->addWidget(this->m_incrementBtn);
    }

    QIntValidator* validator = new QIntValidator(this->m_min, this->m_max, this);
    this->m_valueEdit->setValidator(validator);
    this->m_valueEdit->adjustSize();
    layout->addWidget(this->m_valueEdit);

    // 连接信号
    connect(this->m_slider, qOverload<int>(&QSlider::valueChanged), this, &DDeskPet_IntegerSlider::onSliderValueChanged);
    connect(this->m_slider, &QSlider::sliderReleased, this, &DDeskPet_IntegerSlider::onSliderReleased);
    connect(this->m_decrementBtn, &QPushButton::clicked, this, &DDeskPet_IntegerSlider::onDecrementClicked);
    connect(this->m_incrementBtn, &QPushButton::clicked, this, &DDeskPet_IntegerSlider::onIncrementClicked);
    connect(this->m_valueEdit, &QLineEdit::editingFinished, this, &DDeskPet_IntegerSlider::onTextEditingFinished);

    // 初始化显示
    updateDisplay();
    updateSlider();
}

void DDeskPet_IntegerSlider::setRange(int min, int max)
{
    if (min >= max)
    {
        return;
    }
    this->m_min = min;
    this->m_max = max;

    // 更新验证器

    if (QIntValidator* v = qobject_cast<QIntValidator*>(const_cast<QValidator*>(this->m_valueEdit->validator())))
    {
        v->setRange(this->m_min, this->m_max);
    }

    // 更新滑块映射
    this->m_slider->setRange(0, this->m_tickCount);
    setValue(clamp(this->m_value));
}

void DDeskPet_IntegerSlider::setValue(int value)
{
    int newValue = clamp(value);
    if (newValue == this->m_value)
    {
        updateDisplay();
        updateSlider();
        return;
    }
    this->m_value = newValue;
    updateDisplay();
    updateSlider();
    emit valueChanged(this->m_value);
}

void DDeskPet_IntegerSlider::setStep(int step)
{
    if (step > 0)
    {
        this->m_step = step;
        this->m_decrementBtn->setToolTip("\n减小 " + QString::number(this->m_step));
        this->m_incrementBtn->setToolTip("\n增大 " + QString::number(this->m_step));
    }
}

void DDeskPet_IntegerSlider::setTickCount(int tickcount)
{
    if (tickcount > 0)
    {
        this->m_tickCount = tickcount;
        this->m_slider->setRange(0, this->m_tickCount);
        this->m_slider->setPageStep(this->m_tickCount / 10);
        updateSlider();
    }
    return;
}

int DDeskPet_IntegerSlider::getValue()
{
    this->m_value;
    return this->m_value;
}

int DDeskPet_IntegerSlider::getMinValue()
{
    return this->m_min;
}

int DDeskPet_IntegerSlider::getMaxValue()
{
    return this->m_max;
}

void DDeskPet_IntegerSlider::setEnabled(bool enabled)
{
    this->m_valueEdit->setEnabled(enabled);
    this->m_slider->setEnabled(enabled);
    this->m_decrementBtn->setEnabled(enabled);
    this->m_incrementBtn->setEnabled(enabled);
}

void DDeskPet_IntegerSlider::onSliderValueChanged(int intValue)
{
    if (this->m_updating)
    {
        return;
    }

    // 将内部滑块值映射到实际范围
    double ratio = static_cast<double>(intValue) / this->m_tickCount;
    int newValue = static_cast<int>(std::round(this->m_min + ratio * (this->m_max - this->m_min)));
    newValue = clamp(newValue);

    if (newValue != this->m_value)
    {
        this->m_value = newValue;
        updateDisplay();
        emit valueChanged(this->m_value);
    }
    return;
}

void DDeskPet_IntegerSlider::onSliderReleased()
{
    emit lineeditValueChanged(this->m_value);
    return;
}

void DDeskPet_IntegerSlider::onDecrementClicked()
{
    setValue(this->m_value - this->m_step);
    emit lineeditValueChanged(this->m_value);
    return;
}

void DDeskPet_IntegerSlider::onIncrementClicked()
{
    setValue(m_value + m_step);
    emit lineeditValueChanged(m_value);
}

void DDeskPet_IntegerSlider::onTextEditingFinished()
{
    bool ok;
    int newValue = this->m_valueEdit->text().toInt(&ok);
    if (ok)
    {
        newValue = clamp(newValue);
        if (newValue != this->m_value)
        {
            setValue(newValue);
            emit lineeditValueChanged(this->m_value);
        }
        else
        {
            updateDisplay(); // 刷新显示
        }
    }
    else
    {
        updateDisplay(); // 无效输入，恢复原值
    }
    return;
}

void DDeskPet_IntegerSlider::updateDisplay()
{
    this->m_updating = true;
    this->m_valueEdit->setText(QString::number(this->m_value));
    this->m_valueEdit->adjustSize();
    this->m_updating = false;
    return;
}

void DDeskPet_IntegerSlider::updateSlider()
{
    this->m_updating = true;
    if (this->m_max != this->m_min)
    {
        double ratio = static_cast<double>(this->m_value - this->m_min) / (this->m_max - this->m_min);
        this->m_slider->setValue(static_cast<int>(std::round(ratio * this->m_tickCount)));
    }
    this->m_updating = false;
    return;
}
int DDeskPet_IntegerSlider::clamp(int value) const
{
    if (value < this->m_min)
    {
        return this->m_min;
    }

    if (value > this->m_max)
    {
        return this->m_max;
    }
    return value;
}
// ========== end ========== end ========== end =========== end =========== end =========== end ========== end =========== end ==========