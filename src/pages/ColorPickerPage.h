#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QColor>
#include <QTimer>
#include <QPixmap>
#include <QList>

class QHBoxLayout;
class QVBoxLayout;

class ColorPickerPage : public QWidget {
    Q_OBJECT
public:
    explicit ColorPickerPage(QWidget *parent = nullptr);
    void setDarkTheme(bool dark);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onModeChanged();
    void onHueSliderChanged(int value);
    void onLightnessSliderChanged(int value);
    void onAlphaSliderChanged(int value);
    void onHexChanged(const QString &text);
    void onRgbChanged();
    void onAddStop();
    void onRemoveStop();
    void onAngleQuick(int delta);
    void onCopyCss();
    void onRandomGradient();

private:
    enum ColorMode { ModeSolid = 0, ModeLinear = 1, ModeRadial = 2 };
    ColorMode m_mode = ModeSolid;

    QWidget *m_previewWidget;
    QPushButton *m_solidBtn;
    QPushButton *m_linearBtn;
    QPushButton *m_radialBtn;

    QSlider *m_hueSlider;
    QSlider *m_lightnessSlider;
    QSlider *m_alphaSlider;
    QLabel *m_hueValueLabel;
    QLabel *m_lightnessValueLabel;
    QLabel *m_alphaValueLabel;
    QWidget *m_hueSliderBg;
    QWidget *m_alphaSliderBg;

    QLineEdit *m_hexEdit;
    QSpinBox *m_rSpin;
    QSpinBox *m_gSpin;
    QSpinBox *m_bSpin;
    QPushButton *m_pickColorBtn;

    QSpinBox *m_angleSpin;
    QComboBox *m_shapeCombo;
    QWidget *m_gradientParamsCard;
    QVBoxLayout *m_stopsLayout;
    QPushButton *m_removeBtn;

    QPlainTextEdit *m_codeEdit;
    QPushButton *m_copyBtn;

    QColor m_currentColor = QColor(74, 144, 217);
    int m_hue = 210;
    int m_saturation = 100;
    int m_lightness = 60;
    int m_alpha = 100;
    bool m_updating = false;

    bool m_pickingColor = false;
    bool m_canPick = false;
    QTimer *m_pickTimer = nullptr;
    QWidget *m_magnifier = nullptr;
    QLabel *m_magImageLabel = nullptr;
    QLabel *m_magColorLabel = nullptr;
    QPixmap m_screenGrab;

    struct StopRow {
        QPushButton *colorBtn;
        QLineEdit *colorEdit;
        QSpinBox *posSpin;
        bool operator==(const StopRow &other) const {
            return colorBtn == other.colorBtn && colorEdit == other.colorEdit && posSpin == other.posSpin;
        }
    };
    QList<StopRow> m_stopRows;

    void updateFromColor(const QColor &color);
    void updatePreview();
    void updateSlidersFromColor();
    void updateHueSliderBg();
    void updateAlphaSliderBg();
    void updateModeUI();
    void copyText(const QString &text, QPushButton *btn);
    void startPickColor();
    void stopPickColor(bool confirm);
    void onPickTick();
    void addStopRow(const QString &color, int position);
    void removeStopRow();
    QString generateCss() const;
    QString generateQtGradient() const;
    void updateGradient();
    void setupPresetColors(QHBoxLayout *layout);
    void setupThemeColors(QHBoxLayout *layout);
    bool m_isDark = true;
};
