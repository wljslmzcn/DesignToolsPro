#include "ColorPickerPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QClipboard>
#include <QApplication>
#include <QScreen>
#include <QKeyEvent>
#include <QSignalBlocker>
#include <QRandomGenerator>
#include <cmath>

ColorPickerPage::ColorPickerPage(QWidget *parent) : QWidget(parent) {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    // 主内容区域（左右分栏）
    auto *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(15);

    // 左侧：预览和颜色选择
    auto *leftPanel = new QWidget();
    auto *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(12);

    // 预览区域
    m_previewWidget = new QWidget();
    m_previewWidget->setFixedHeight(120);
    m_previewWidget->setStyleSheet("background-color: #4a90d9; border-radius: 8px;");
    leftLayout->addWidget(m_previewWidget);

    // 模式切换按钮
    auto *modeLayout = new QHBoxLayout();
    modeLayout->setSpacing(8);
    m_solidBtn = new QPushButton(QString::fromUtf16(u"纯色"));
    m_linearBtn = new QPushButton(QString::fromUtf16(u"线性渐变"));
    m_radialBtn = new QPushButton(QString::fromUtf16(u"径向渐变"));
    m_solidBtn->setCheckable(true);
    m_linearBtn->setCheckable(true);
    m_radialBtn->setCheckable(true);
    m_solidBtn->setChecked(true);
    modeLayout->addWidget(m_solidBtn);
    modeLayout->addWidget(m_linearBtn);
    modeLayout->addWidget(m_radialBtn);
    leftLayout->addLayout(modeLayout);

    connect(m_solidBtn, &QPushButton::clicked, this, &ColorPickerPage::onModeChanged);
    connect(m_linearBtn, &QPushButton::clicked, this, &ColorPickerPage::onModeChanged);
    connect(m_radialBtn, &QPushButton::clicked, this, &ColorPickerPage::onModeChanged);

    // 颜色选择器（滑块）
    auto *slidersGroup = new QGroupBox(QString::fromUtf16(u"颜色调整"));
    auto *slidersLayout = new QGridLayout(slidersGroup);
    slidersLayout->setSpacing(15);

    // 色相滑块
    slidersLayout->addWidget(new QLabel(QString::fromUtf16(u"色相")), 0, 0);
    m_hueSlider = new QSlider(Qt::Horizontal);
    m_hueSlider->setRange(0, 360);
    m_hueSlider->setValue(210);
    m_hueSlider->setPageStep(10);
    m_hueSlider->setSingleStep(1);
    m_hueValueLabel = new QLabel("210°");
    m_hueValueLabel->setFixedWidth(40);
    slidersLayout->addWidget(m_hueSlider, 0, 1);
    slidersLayout->addWidget(m_hueValueLabel, 0, 2);

    // 亮度滑块
    slidersLayout->addWidget(new QLabel(QString::fromUtf16(u"亮度")), 1, 0);
    m_lightnessSlider = new QSlider(Qt::Horizontal);
    m_lightnessSlider->setRange(0, 100);
    m_lightnessSlider->setValue(60);
    m_lightnessSlider->setPageStep(5);
    m_lightnessSlider->setSingleStep(1);
    m_lightnessValueLabel = new QLabel("60%");
    m_lightnessValueLabel->setFixedWidth(40);
    slidersLayout->addWidget(m_lightnessSlider, 1, 1);
    slidersLayout->addWidget(m_lightnessValueLabel, 1, 2);

    // 透明度滑块
    slidersLayout->addWidget(new QLabel(QString::fromUtf16(u"透明度")), 2, 0);
    m_alphaSlider = new QSlider(Qt::Horizontal);
    m_alphaSlider->setRange(0, 100);
    m_alphaSlider->setValue(100);
    m_alphaSlider->setPageStep(5);
    m_alphaSlider->setSingleStep(1);
    m_alphaValueLabel = new QLabel("100%");
    m_alphaValueLabel->setFixedWidth(40);
    slidersLayout->addWidget(m_alphaSlider, 2, 1);
    slidersLayout->addWidget(m_alphaValueLabel, 2, 2);

    leftLayout->addWidget(slidersGroup);

    // RGB输入
    auto *rgbGroup = new QGroupBox(QString::fromUtf16(u"RGB 值"));
    auto *rgbLayout = new QGridLayout(rgbGroup);
    rgbLayout->setSpacing(8);

    rgbLayout->addWidget(new QLabel("R:"), 0, 0);
    m_rSpin = new QSpinBox();
    m_rSpin->setRange(0, 255);
    m_rSpin->setValue(74);
    rgbLayout->addWidget(m_rSpin, 0, 1);

    rgbLayout->addWidget(new QLabel("G:"), 0, 2);
    m_gSpin = new QSpinBox();
    m_gSpin->setRange(0, 255);
    m_gSpin->setValue(144);
    rgbLayout->addWidget(m_gSpin, 0, 3);

    rgbLayout->addWidget(new QLabel("B:"), 0, 4);
    m_bSpin = new QSpinBox();
    m_bSpin->setRange(0, 255);
    m_bSpin->setValue(217);
    rgbLayout->addWidget(m_bSpin, 0, 5);

    rgbLayout->addWidget(new QLabel("HEX:"), 1, 0);
    m_hexEdit = new QLineEdit("#4A90D9");
    m_hexEdit->setPlaceholderText("#000000");
    rgbLayout->addWidget(m_hexEdit, 1, 1, 1, 3);

    m_pickColorBtn = new QPushButton(QString::fromUtf16(u"拾取颜色"));
    rgbLayout->addWidget(m_pickColorBtn, 1, 4, 1, 2);

    leftLayout->addWidget(rgbGroup);

    // 渐变参数（默认隐藏）
    m_gradientParamsCard = new QGroupBox(QString::fromUtf16(u"渐变参数"));
    auto *gradParamsLayout = new QGridLayout(m_gradientParamsCard);
    gradParamsLayout->setSpacing(8);

    gradParamsLayout->addWidget(new QLabel(QString::fromUtf16(u"角度")), 0, 0);
    m_angleSpin = new QSpinBox();
    m_angleSpin->setRange(0, 360);
    m_angleSpin->setValue(135);
    gradParamsLayout->addWidget(m_angleSpin, 0, 1);

    gradParamsLayout->addWidget(new QLabel(QString::fromUtf16(u"形状")), 0, 2);
    m_shapeCombo = new QComboBox();
    m_shapeCombo->addItems({QString::fromUtf16(u"椭圆"), QString::fromUtf16(u"圆形")});
    gradParamsLayout->addWidget(m_shapeCombo, 0, 3);

    m_gradientParamsCard->hide();
    leftLayout->addWidget(m_gradientParamsCard);

    leftLayout->addStretch();

    // 右侧：颜色停止点和代码
    auto *rightPanel = new QWidget();
    auto *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(12);

    // 颜色停止点（渐变模式）
    auto *stopsGroup = new QGroupBox(QString::fromUtf16(u"颜色停止点"));
    auto *stopsLayout = new QVBoxLayout(stopsGroup);

    auto *stopsHeader = new QHBoxLayout();
    m_removeBtn = new QPushButton(QString::fromUtf16(u"删除"));
    m_removeBtn->setEnabled(false);
    auto *addStopBtn = new QPushButton(QString::fromUtf16(u"添加"));
    stopsHeader->addStretch();
    stopsHeader->addWidget(m_removeBtn);
    stopsHeader->addWidget(addStopBtn);
    stopsLayout->addLayout(stopsHeader);

    m_stopsLayout = new QVBoxLayout();
    stopsLayout->addLayout(m_stopsLayout);

    stopsGroup->hide();
    rightLayout->addWidget(stopsGroup);

    // 生成的代码
    auto *codeGroup = new QGroupBox(QString::fromUtf16(u"生成的代码"));
    auto *codeLayout = new QVBoxLayout(codeGroup);

    m_codeEdit = new QPlainTextEdit();
    m_codeEdit->setReadOnly(true);
    m_codeEdit->setMaximumHeight(150);
    m_codeEdit->setStyleSheet("background-color: #2d2d2d; color: #d4d4d4; border: 1px solid #3c3c3c; border-radius: 4px; padding: 8px; font-family: Consolas, monospace;");
    codeLayout->addWidget(m_codeEdit);

    auto *codeButtons = new QHBoxLayout();
    m_copyBtn = new QPushButton(QString::fromUtf16(u"复制代码"));
    auto *copyHexBtn = new QPushButton(QString::fromUtf16(u"复制 HEX"));
    auto *copyRgbBtn = new QPushButton(QString::fromUtf16(u"复制 RGB"));
    codeButtons->addWidget(m_copyBtn);
    codeButtons->addWidget(copyHexBtn);
    codeButtons->addWidget(copyRgbBtn);
    codeButtons->addStretch();
    codeLayout->addLayout(codeButtons);

    rightLayout->addWidget(codeGroup);

    // 预设颜色
    auto *presetsGroup = new QGroupBox(QString::fromUtf16(u"预设颜色"));
    auto *presetsLayout = new QVBoxLayout(presetsGroup);

    auto *presetColorsLayout = new QHBoxLayout();
    setupPresetColors(presetColorsLayout);
    presetsLayout->addLayout(presetColorsLayout);

    auto *themeLabel = new QLabel(QString::fromUtf16(u"主题色"));
    presetsLayout->addWidget(themeLabel);
    auto *themeColorsLayout = new QHBoxLayout();
    setupThemeColors(themeColorsLayout);
    presetsLayout->addLayout(themeColorsLayout);

    rightLayout->addWidget(presetsGroup);
    rightLayout->addStretch();

    contentLayout->addWidget(leftPanel, 1);
    contentLayout->addWidget(rightPanel, 1);

    mainLayout->addLayout(contentLayout, 1);

    // 连接信号
    connect(m_hueSlider, &QSlider::valueChanged, this, &ColorPickerPage::onHueSliderChanged);
    connect(m_lightnessSlider, &QSlider::valueChanged, this, &ColorPickerPage::onLightnessSliderChanged);
    connect(m_alphaSlider, &QSlider::valueChanged, this, &ColorPickerPage::onAlphaSliderChanged);
    connect(m_hexEdit, &QLineEdit::textChanged, this, &ColorPickerPage::onHexChanged);
    connect(m_rSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &ColorPickerPage::onRgbChanged);
    connect(m_gSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &ColorPickerPage::onRgbChanged);
    connect(m_bSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &ColorPickerPage::onRgbChanged);
    connect(m_copyBtn, &QPushButton::clicked, this, &ColorPickerPage::onCopyCss);
    connect(m_removeBtn, &QPushButton::clicked, this, &ColorPickerPage::onRemoveStop);
    connect(addStopBtn, &QPushButton::clicked, this, &ColorPickerPage::onAddStop);
    connect(m_pickColorBtn, &QPushButton::clicked, this, &ColorPickerPage::startPickColor);
    connect(m_angleSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int) { updateGradient(); });

    // 初始化渐变停止点
    addStopRow("#4A90D9", 0);
    addStopRow("#E74C3C", 100);

    // 初始化预览
    updateFromColor(m_currentColor);
}

void ColorPickerPage::setDarkTheme(bool dark) {
    m_isDark = dark;
    if (dark) {
        m_codeEdit->setStyleSheet("background-color: #2d2d2d; color: #d4d4d4; border: 1px solid #3c3c3c; border-radius: 4px; padding: 8px; font-family: Consolas, monospace;");
    } else {
        m_codeEdit->setStyleSheet("background-color: #ffffff; color: #333333; border: 1px solid #ccc; border-radius: 4px; padding: 8px; font-family: Consolas, monospace;");
    }
}

void ColorPickerPage::onModeChanged() {
    m_mode = m_solidBtn->isChecked() ? ModeSolid :
             m_linearBtn->isChecked() ? ModeLinear : ModeRadial;
    updateModeUI();
    updatePreview();
    updateGradient();
}

void ColorPickerPage::updateModeUI() {
    bool isGradient = (m_mode != ModeSolid);
    m_gradientParamsCard->setVisible(isGradient);

    // 更新停止点组的可见性
    QGroupBox *stopsGroup = qobject_cast<QGroupBox*>(m_stopsLayout->parentWidget()->parentWidget());
    if (stopsGroup) {
        stopsGroup->setVisible(isGradient);
    }
}

void ColorPickerPage::onHueSliderChanged(int value) {
    if (m_updating) return;
    m_hue = value;
    m_currentColor = QColor::fromHsl(value, 255, m_lightness * 255 / 100);
    m_currentColor.setAlpha(m_alpha * 255 / 100);
    m_hueValueLabel->setText(QString("%1°").arg(value));
    updatePreview();
    updateGradient();
}

void ColorPickerPage::onLightnessSliderChanged(int value) {
    if (m_updating) return;
    m_lightness = value;
    m_currentColor = QColor::fromHsl(m_hue, 255, value * 255 / 100);
    m_currentColor.setAlpha(m_alpha * 255 / 100);
    m_lightnessValueLabel->setText(QString("%1%").arg(value));
    updatePreview();
    updateGradient();
}

void ColorPickerPage::onAlphaSliderChanged(int value) {
    if (m_updating) return;
    m_alpha = value;
    m_currentColor.setAlpha(value * 255 / 100);
    m_alphaValueLabel->setText(QString("%1%").arg(value));
    updatePreview();
    updateGradient();
}

void ColorPickerPage::onHexChanged(const QString &text) {
    if (m_updating) return;
    QString hex = text;
    if (!hex.startsWith("#")) hex = "#" + hex;
    if (hex.length() == 7 || hex.length() == 9) {
        QColor color(hex);
        if (color.isValid()) {
            m_currentColor = color;
            m_hue = color.hue() >= 0 ? color.hue() : 0;
            m_lightness = color.lightness() * 100 / 255;
            m_alpha = color.alpha() * 100 / 255;
            m_updating = true;
            m_rSpin->setValue(color.red());
            m_gSpin->setValue(color.green());
            m_bSpin->setValue(color.blue());
            m_updating = false;
            updateSlidersFromColor();
            updatePreview();
            updateGradient();
        }
    }
}

void ColorPickerPage::onRgbChanged() {
    if (m_updating) return;
    m_currentColor.setRgb(m_rSpin->value(), m_gSpin->value(), m_bSpin->value());
    m_currentColor.setAlpha(m_alpha * 255 / 100);
    m_hue = m_currentColor.hue() >= 0 ? m_currentColor.hue() : 0;
    m_lightness = m_currentColor.lightness() * 100 / 255;
    m_updating = true;
    m_hexEdit->setText(m_currentColor.name().toUpper());
    m_updating = false;
    updateSlidersFromColor();
    updatePreview();
    updateGradient();
}

void ColorPickerPage::updateFromColor(const QColor &color) {
    m_updating = true;
    m_rSpin->setValue(color.red());
    m_gSpin->setValue(color.green());
    m_bSpin->setValue(color.blue());
    m_hexEdit->setText(color.name().toUpper());
    m_updating = false;
    updatePreview();
    updateGradient();
}

void ColorPickerPage::updateSlidersFromColor() {
    QSignalBlocker hueBlocker(m_hueSlider);
    QSignalBlocker lightBlocker(m_lightnessSlider);
    QSignalBlocker alphaBlocker(m_alphaSlider);

    m_hueSlider->setValue(m_hue);
    m_lightnessSlider->setValue(m_lightness);
    m_alphaSlider->setValue(m_alpha);

    m_hueValueLabel->setText(QString("%1°").arg(m_hue));
    m_lightnessValueLabel->setText(QString("%1%").arg(m_lightness));
    m_alphaValueLabel->setText(QString("%1%").arg(m_alpha));
}

void ColorPickerPage::updatePreview() {
    if (m_mode == ModeSolid) {
        int alpha = m_alpha;
        QString style;
        if (alpha < 100) {
            style = QString("background-color: rgba(%1, %2, %3, %4); border-radius: 8px;")
                .arg(m_currentColor.red()).arg(m_currentColor.green())
                .arg(m_currentColor.blue()).arg(alpha / 100.0);
        } else {
            style = QString("background-color: %1; border-radius: 8px;").arg(m_currentColor.name());
        }
        m_previewWidget->setStyleSheet(style);
    }
}

void ColorPickerPage::updateGradient() {
    if (m_mode == ModeSolid) return;

    QString css = generateCss();
    QString style = QString("background: %1; border-radius: 8px;").arg(css);
    m_previewWidget->setStyleSheet(style);

    // 更新代码编辑框
    m_codeEdit->setPlainText(css);
}

QString ColorPickerPage::generateCss() const {
    if (m_mode == ModeSolid) {
        if (m_currentColor.alpha() < 255) {
            return QString("rgba(%1, %2, %3, %4)")
                .arg(m_currentColor.red()).arg(m_currentColor.green())
                .arg(m_currentColor.blue()).arg(m_currentColor.alpha() / 255.0, 0, 'f', 2);
        }
        return m_currentColor.name();
    }

    QString type = (m_mode == ModeLinear) ? "linear-gradient" : "radial-gradient";
    int angle = m_angleSpin->value();

    QString stops;
    for (const auto &row : m_stopRows) {
        if (!stops.isEmpty()) stops += ", ";
        stops += QString("%1 %2%").arg(row.colorEdit->text()).arg(row.posSpin->value());
    }

    if (m_mode == ModeLinear) {
        return QString("%1(%2deg, %3)").arg(type).arg(angle).arg(stops);
    } else {
        QString shape = m_shapeCombo->currentIndex() == 0 ? "ellipse" : "circle";
        return QString("%1(%2, %3)").arg(type).arg(shape).arg(stops);
    }
}

QString ColorPickerPage::generateQtGradient() const {
    if (m_mode == ModeSolid) {
        return QString("QColor(%1, %2, %3, %4)")
            .arg(m_currentColor.red()).arg(m_currentColor.green())
            .arg(m_currentColor.blue()).arg(m_currentColor.alpha());
    }

    QString type = (m_mode == ModeLinear) ? "QLinearGradient" : "QRadialGradient";
    // 简化的 Qt 代码生成
    return QString("// Qt gradient code would be generated here\n// Use: %1").arg(type);
}

void ColorPickerPage::onAddStop() {
    int pos = m_stopRows.isEmpty() ? 0 : 100;
    addStopRow("#FFFFFF", pos);
    updateGradient();
}

void ColorPickerPage::onRemoveStop() {
    removeStopRow();
    updateGradient();
}

void ColorPickerPage::addStopRow(const QString &color, int position) {
    StopRow row;

    auto *rowWidget = new QWidget();
    auto *rowLayout = new QHBoxLayout(rowWidget);
    rowLayout->setContentsMargins(0, 0, 0, 0);
    rowLayout->setSpacing(8);

    row.colorBtn = new QPushButton();
    row.colorBtn->setFixedSize(30, 30);
    row.colorBtn->setStyleSheet(QString("background-color: %1; border: 1px solid #555; border-radius: 4px;").arg(color));
    rowLayout->addWidget(row.colorBtn);

    row.colorEdit = new QLineEdit(color);
    row.colorEdit->setFixedWidth(80);
    rowLayout->addWidget(row.colorEdit);

    row.posSpin = new QSpinBox();
    row.posSpin->setRange(0, 100);
    row.posSpin->setValue(position);
    row.posSpin->setSuffix("%");
    rowLayout->addWidget(row.posSpin);

    row.colorBtn->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(row.colorBtn, &QPushButton::customContextMenuRequested, this, [this, row](const QPoint &) {
        m_stopRows.removeOne(row);
        row.colorBtn->parentWidget()->deleteLater();
        updateGradient();
    });

    connect(row.colorBtn, &QPushButton::clicked, this, [this, row]() {
        // 这里可以添加颜色选择对话框
        // 暂时使用简单的方式
    });

    connect(row.colorEdit, &QLineEdit::textChanged, this, [this, row](const QString &text) {
        row.colorBtn->setStyleSheet(QString("background-color: %1; border: 1px solid #555; border-radius: 4px;").arg(text));
        updateGradient();
    });

    connect(row.posSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int) {
        updateGradient();
    });

    m_stopsLayout->addWidget(rowWidget);
    m_stopRows.append(row);

    m_removeBtn->setEnabled(m_stopRows.size() > 2);
}

void ColorPickerPage::removeStopRow() {
    if (m_stopRows.size() <= 2) return;

    auto lastRow = m_stopRows.last();
    m_stopRows.removeLast();
    delete lastRow.colorBtn->parentWidget();

    m_removeBtn->setEnabled(m_stopRows.size() > 2);
}

void ColorPickerPage::onCopyCss() {
    QString css = generateCss();
    QApplication::clipboard()->setText(css);
    m_copyBtn->setText(QString::fromUtf16(u"已复制!"));
    QTimer::singleShot(1500, m_copyBtn, [this]() {
        m_copyBtn->setText(QString::fromUtf16(u"复制代码"));
    });
}

void ColorPickerPage::copyText(const QString &text, QPushButton *btn) {
    QApplication::clipboard()->setText(text);
    QString originalText = btn->text();
    btn->setText(QString::fromUtf16(u"已复制!"));
    QTimer::singleShot(1500, btn, [btn, originalText]() {
        btn->setText(originalText);
    });
}

void ColorPickerPage::startPickColor() {
    // 颜色拾取功能的简化实现
    m_canPick = true;
    grabKeyboard();
}

void ColorPickerPage::stopPickColor(bool confirm) {
    m_canPick = false;
    releaseKeyboard();
    if (m_magnifier) {
        m_magnifier->hide();
        m_magnifier->deleteLater();
        m_magnifier = nullptr;
    }
}

void ColorPickerPage::onPickTick() {
    // 颜色拾取的定时器回调
}

void ColorPickerPage::onAngleQuick(int delta) {
    int value = m_angleSpin->value() + delta;
    if (value < 0) value += 360;
    if (value >= 360) value -= 360;
    m_angleSpin->setValue(value);
}

void ColorPickerPage::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape && m_canPick) {
        stopPickColor(false);
    }
    QWidget::keyPressEvent(event);
}

void ColorPickerPage::setupPresetColors(QHBoxLayout *layout) {
    QStringList presetColors = {
        "#FF6B6B", "#4ECDC4", "#45B7D1", "#96CEB4",
        "#FFEAA7", "#DDA0DD", "#98D8C8", "#F7DC6F",
        "#BB8FCE", "#85C1E9", "#F8C471", "#82E0AA"
    };

    for (const QString &color : presetColors) {
        auto *btn = new QPushButton();
        btn->setFixedSize(32, 32);
        btn->setStyleSheet(QString("background-color: %1; border: 2px solid #3c3c3c; border-radius: 4px;").arg(color));
        connect(btn, &QPushButton::clicked, this, [this, color]() {
            m_currentColor = QColor(color);
            updateFromColor(m_currentColor);
        });
        layout->addWidget(btn);
    }
}

void ColorPickerPage::setupThemeColors(QHBoxLayout *layout) {
    QStringList themeColors = {
        "#0078D4", "#107C10", "#D83B01", "#B4009E",
        "#008272", "#4997D0", "#767676", "#FF8C00"
    };

    for (const QString &color : themeColors) {
        auto *btn = new QPushButton();
        btn->setFixedSize(32, 32);
        btn->setStyleSheet(QString("background-color: %1; border: 2px solid #3c3c3c; border-radius: 4px;").arg(color));
        connect(btn, &QPushButton::clicked, this, [this, color]() {
            m_currentColor = QColor(color);
            updateFromColor(m_currentColor);
        });
        layout->addWidget(btn);
    }
}

void ColorPickerPage::onRandomGradient() {
    // TODO: 实现随机渐变生成
}
