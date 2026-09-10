#include <QApplication>
#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QIcon>
#include <QSettings>
#include <QLabel>
#include <QPushButton>
#include <QMap>
#include <QPair>
#include <QDialog>
#include <QPixmap>
#include <QImageReader>
#include <QDebug>
#include <QCoreApplication>
#include "pages/ColorPickerPage.h"
#include "pages/RgbColorTablePage.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle(QString::fromUtf16(u"DesignTools Pro v1.0.0"));
        resize(1000, 700);
        setMinimumSize(800, 500);
        setWindowIcon(QIcon(":/app.ico"));

        auto *centralWidget = new QWidget();
        setCentralWidget(centralWidget);
        auto *mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);

        // 标题栏
        m_titleBar = new QWidget();
        m_titleBar->setFixedHeight(50);
        m_titleBar->setStyleSheet("background-color: #2b2b2b; border-bottom: 1px solid #3c3c3c;");
        auto *titleLayout = new QHBoxLayout(m_titleBar);
        titleLayout->setContentsMargins(15, 0, 15, 0);

        m_titleLabel = new QLabel(QString::fromUtf16(u"🎨 DesignTools Pro"));
        m_titleLabel->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");
        titleLayout->addWidget(m_titleLabel);
        titleLayout->addStretch();

        // 导航按钮
        auto *navWidget = new QWidget();
        auto *navLayout = new QHBoxLayout(navWidget);
        navLayout->setContentsMargins(10, 5, 10, 5);
        navLayout->setSpacing(10);

        m_colorBtn = createNavButton(QString::fromUtf16(u"🎨 颜色选择器"));
        m_rgbBtn = createNavButton(QString::fromUtf16(u"🌈 RGB 颜色表"));
        m_themeBtn = createNavButton(QString::fromUtf16(u"🌙 夜间模式"));
        m_wechatBtn = createNavButton(QString::fromUtf16(u"💬 加入微信交流群"));

        navLayout->addWidget(m_colorBtn);
        navLayout->addWidget(m_rgbBtn);
        navLayout->addWidget(m_themeBtn);
        navLayout->addWidget(m_wechatBtn);
        navLayout->addStretch();

        titleLayout->addWidget(navWidget);

        mainLayout->addWidget(m_titleBar);

        // 内容区域
        m_stackedWidget = new QStackedWidget();
        m_stackedWidget->setObjectName("contentArea");

        m_colorPage = new ColorPickerPage();
        m_rgbPage = new RgbColorTablePage();

        m_stackedWidget->addWidget(m_colorPage);
        m_stackedWidget->addWidget(m_rgbPage);

        m_pageMap["colorpicker"] = 0;
        m_pageMap["rgbtable"] = 1;

        mainLayout->addWidget(m_stackedWidget, 1);

        // 连接信号
        connect(m_colorBtn, &QPushButton::clicked, this, [this]() {
            m_stackedWidget->setCurrentIndex(m_pageMap["colorpicker"]);
            updateNavButtons("colorpicker");
        });
        connect(m_rgbBtn, &QPushButton::clicked, this, [this]() {
            m_stackedWidget->setCurrentIndex(m_pageMap["rgbtable"]);
            updateNavButtons("rgbtable");
        });
        connect(m_themeBtn, &QPushButton::clicked, this, [this]() {
            m_isDark = !m_isDark;
            m_colorPage->setDarkTheme(m_isDark);
            m_rgbPage->setDarkTheme(m_isDark);
            applyTheme();
        });

        connect(m_wechatBtn, &QPushButton::clicked, this, [this]() {
            showWechatDialog();
        });

        // 默认选中颜色选择器
        m_stackedWidget->setCurrentIndex(m_pageMap["colorpicker"]);
        updateNavButtons("colorpicker");

        // 应用主题样式
        applyTheme();
    }

private:
    QPushButton* createNavButton(const QString &text) {
        auto *btn = new QPushButton(text);
        btn->setCheckable(true);
        btn->setFixedHeight(35);
        btn->setMinimumWidth(120);
        return btn;
    }

    void updateNavButtons(const QString &activePage) {
        m_colorBtn->setChecked(activePage == "colorpicker");
        m_rgbBtn->setChecked(activePage == "rgbtable");
    }

    void showWechatDialog() {
        QDialog dialog(this);
        dialog.setWindowTitle(QString::fromUtf16(u"加入微信交流群"));
        dialog.setFixedSize(360, 480);
        dialog.setModal(true);

        auto *layout = new QVBoxLayout(&dialog);
        layout->setContentsMargins(20, 20, 20, 20);
        layout->setSpacing(12);

        // 标题
        auto *titleLabel = new QLabel(QString::fromUtf16(u"扫码关注公众号"));
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
        layout->addWidget(titleLabel);

        // 二维码图片 - 从exe同级目录的img文件夹加载
        auto *qrLabel = new QLabel();
        QString imgPath = QCoreApplication::applicationDirPath() + "/img/wechat.jpg";
        QPixmap pix(imgPath);
        if (!pix.isNull()) {
            qrLabel->setPixmap(pix);
        } else {
            qrLabel->setText(QString::fromUtf16(u"二维码加载失败"));
            qrLabel->setMinimumSize(260, 260);
        }
        qrLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(qrLabel);

        // 提示文字
        auto *hintLabel = new QLabel(QString::fromUtf16(u"关注后回复「加群」即可加入交流群"));
        hintLabel->setAlignment(Qt::AlignCenter);
        hintLabel->setWordWrap(true);
        hintLabel->setStyleSheet("color: #888; font-size: 13px;");
        layout->addWidget(hintLabel);

        // 关闭按钮
        auto *closeBtn = new QPushButton(QString::fromUtf16(u"关闭"));
        closeBtn->setFixedHeight(32);
        closeBtn->setMinimumWidth(100);
        connect(closeBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
        layout->addWidget(closeBtn, 0, Qt::AlignCenter);

        dialog.exec();
    }

    void applyTheme() {
        QString btnStyle = m_isDark
            ? "color: #d4d4d4;"
            : "color: #333333;";
        m_colorBtn->setStyleSheet(btnStyle);
        m_rgbBtn->setStyleSheet(btnStyle);
        m_themeBtn->setStyleSheet(btnStyle);
        m_wechatBtn->setStyleSheet(btnStyle);

        if (m_isDark) {
            m_themeBtn->setText(QString::fromUtf16(u"🌙 夜间模式"));
            m_titleBar->setStyleSheet("background-color: #2b2b2b; border-bottom: 1px solid #3c3c3c;");
            m_titleLabel->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");
            qApp->setStyleSheet(R"(
                * {
                    font-family: "Microsoft YaHei", "Segoe UI", sans-serif;
                }
                QMainWindow {
                    background-color: #1e1e1e;
                }
                QWidget {
                    background-color: #1e1e1e;
                    color: #d4d4d4;
                }
                QPushButton {
                    background-color: #3c3c3c;
                    color: #d4d4d4;
                    border: 1px solid #555;
                    border-radius: 4px;
                    padding: 6px 12px;
                }
                QPushButton:hover {
                    background-color: #4a4a4a;
                }
                QPushButton:checked {
                    background-color: #0078d4;
                    color: white;
                }
                QLineEdit, QSpinBox, QComboBox {
                    background-color: #3c3c3c;
                    color: #d4d4d4;
                    border: 1px solid #555;
                    border-radius: 4px;
                    padding: 5px;
                }
                QLabel {
                    color: #d4d4d4;
                }
                QScrollArea {
                    border: none;
                }
                QScrollBar:vertical {
                    border: none;
                    background-color: #1e1e1e;
                    width: 10px;
                }
                QScrollBar::handle:vertical {
                    background-color: #555;
                    min-height: 30px;
                    border-radius: 5px;
                }
                QScrollBar::handle:vertical:hover {
                    background-color: #666;
                }
                QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                    height: 0px;
                }
                QSlider::groove:horizontal {
                    height: 6px;
                    background: #555;
                    border-radius: 3px;
                }
                QSlider::handle:horizontal {
                    background: #0078d4;
                    width: 14px;
                    height: 14px;
                    margin: -4px 0;
                    border-radius: 7px;
                }
                QSlider::handle:horizontal:hover {
                    background: #1a8ae8;
                }
            )");
        } else {
            m_themeBtn->setText(QString::fromUtf16(u"☀️ 日间模式"));
            m_titleBar->setStyleSheet("background-color: #f0f0f0; border-bottom: 1px solid #ccc;");
            m_titleLabel->setStyleSheet("color: #333; font-size: 18px; font-weight: bold;");
            qApp->setStyleSheet(R"(
                * {
                    font-family: "Microsoft YaHei", "Segoe UI", sans-serif;
                }
                QMainWindow {
                    background-color: #ffffff;
                }
                QWidget {
                    background-color: #ffffff;
                    color: #333333;
                }
                QPushButton {
                    background-color: #e0e0e0;
                    color: #333333;
                    border: 1px solid #ccc;
                    border-radius: 4px;
                    padding: 6px 12px;
                }
                QPushButton:hover {
                    background-color: #d0d0d0;
                }
                QPushButton:checked {
                    background-color: #0078d4;
                    color: white;
                }
                QLineEdit, QSpinBox, QComboBox {
                    background-color: #ffffff;
                    color: #333333;
                    border: 1px solid #ccc;
                    border-radius: 4px;
                    padding: 5px;
                }
                QLabel {
                    color: #333333;
                }
                QScrollArea {
                    border: none;
                }
                QScrollBar:vertical {
                    border: none;
                    background-color: #f0f0f0;
                    width: 10px;
                }
                QScrollBar::handle:vertical {
                    background-color: #ccc;
                    min-height: 30px;
                    border-radius: 5px;
                }
                QScrollBar::handle:vertical:hover {
                    background-color: #aaa;
                }
                QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                    height: 0px;
                }
                QSlider::groove:horizontal {
                    height: 6px;
                    background: #ccc;
                    border-radius: 3px;
                }
                QSlider::handle:horizontal {
                    background: #0078d4;
                    width: 14px;
                    height: 14px;
                    margin: -4px 0;
                    border-radius: 7px;
                }
                QSlider::handle:horizontal:hover {
                    background: #1a8ae8;
                }
            )");
        }
    }

    QStackedWidget *m_stackedWidget = nullptr;
    ColorPickerPage *m_colorPage = nullptr;
    RgbColorTablePage *m_rgbPage = nullptr;
    QPushButton *m_colorBtn = nullptr;
    QPushButton *m_rgbBtn = nullptr;
    QPushButton *m_themeBtn = nullptr;
    QPushButton *m_wechatBtn = nullptr;
    QLabel *m_titleLabel = nullptr;
    QWidget *m_titleBar = nullptr;
    bool m_isDark = true;
    QMap<QString, int> m_pageMap;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("DesignTools Pro");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("DesignTools");

    MainWindow window;
    window.show();

    return app.exec();
}

#include "main.moc"
