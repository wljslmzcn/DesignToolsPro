#include "RgbColorTablePage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QButtonGroup>
#include <QClipboard>
#include <QApplication>
#include <QFrame>
#include <QEvent>
#include <QTimer>

RgbColorTablePage::RgbColorTablePage(QWidget *parent) : QWidget(parent) {
    initCategories();
    initColors();
    setupUI();
    filterByCategory(0);
}

void RgbColorTablePage::initCategories() {
    m_categories = {
        {"all", QString::fromUtf16(u"全部"), "🎨"},
        {"red", QString::fromUtf16(u"红色系"), "🔴"},
        {"orange", QString::fromUtf16(u"橙色系"), "🟠"},
        {"yellow", QString::fromUtf16(u"黄色系"), "🟡"},
        {"green", QString::fromUtf16(u"绿色系"), "🟢"},
        {"blue", QString::fromUtf16(u"蓝色系"), "🔵"},
        {"purple", QString::fromUtf16(u"紫色系"), "🟣"},
        {"pink", QString::fromUtf16(u"粉色系"), "🩷"},
        {"gray", QString::fromUtf16(u"灰色系"), "⚪"},
    };
}

void RgbColorTablePage::initColors() {
    m_allColors = {
        // 红色系
        {QString::fromUtf16(u"深红"), QString::fromUtf16(u"深沉的红色"), QColor("#8B0000"), "red"},
        {QString::fromUtf16(u"红色"), QString::fromUtf16(u"标准红色"), QColor("#FF0000"), "red"},
        {QString::fromUtf16(u"印度红"), QString::fromUtf16(u"印度传统红色"), QColor("#CD5C5C"), "red"},
        {QString::fromUtf16(u"火砖红"), QString::fromUtf16(u"像火砖的颜色"), QColor("#B22222"), "red"},
        {QString::fromUtf16(u"暗红"), QString::fromUtf16(u"暗淡的红色"), QColor("#A52A2A"), "red"},
        {QString::fromUtf16(u"珊瑚红"), QString::fromUtf16(u"珊瑚的颜色"), QColor("#FF7F50"), "red"},

        // 橙色系
        {QString::fromUtf16(u"橙色"), QString::fromUtf16(u"标准橙色"), QColor("#FF8C00"), "orange"},
        {QString::fromUtf16(u"橙红"), QString::fromUtf16(u"橙色偏红"), QColor("#FF4500"), "orange"},
        {QString::fromUtf16(u"番茄红"), QString::fromUtf16(u"番茄的颜色"), QColor("#FF6347"), "orange"},
        {QString::fromUtf16(u"南瓜橙"), QString::fromUtf16(u"南瓜的颜色"), QColor("#FF7518"), "orange"},
        {QString::fromUtf16(u"杏色"), QString::fromUtf16(u"杏子的颜色"), QColor("#FBCEB1"), "orange"},

        // 黄色系
        {QString::fromUtf16(u"黄色"), QString::fromUtf16(u"标准黄色"), QColor("#FFD700"), "yellow"},
        {QString::fromUtf16(u"金色"), QString::fromUtf16(u"黄金的颜色"), QColor("#FFD700"), "yellow"},
        {QString::fromUtf16(u"柠檬黄"), QString::fromUtf16(u"柠檬的颜色"), QColor("#FFF44F"), "yellow"},
        {QString::fromUtf16(u"卡其色"), QString::fromUtf16(u"大地的颜色"), QColor("#F0E68C"), "yellow"},
        {QString::fromUtf16(u"浅黄"), QString::fromUtf16(u"淡雅的黄色"), QColor("#FFFFE0"), "yellow"},

        // 绿色系
        {QString::fromUtf16(u"绿色"), QString::fromUtf16(u"标准绿色"), QColor("#008000"), "green"},
        {QString::fromUtf16(u"深绿"), QString::fromUtf16(u"深沉的绿色"), QColor("#006400"), "green"},
        {QString::fromUtf16(u"草绿"), QString::fromUtf16(u"草地的颜色"), QColor("#7CFC00"), "green"},
        {QString::fromUtf16(u"薄荷绿"), QString::fromUtf16(u"薄荷的颜色"), QColor("#98FF98"), "green"},
        {QString::fromUtf16(u"翠绿"), QString::fromUtf16(u"翡翠的颜色"), QColor("#50C878"), "green"},
        {QString::fromUtf16(u"橄榄绿"), QString::fromUtf16(u"橄榄的颜色"), QColor("#808000"), "green"},

        // 蓝色系
        {QString::fromUtf16(u"蓝色"), QString::fromUtf16(u"标准蓝色"), QColor("#0000FF"), "blue"},
        {QString::fromUtf16(u"深蓝"), QString::fromUtf16(u"深沉的蓝色"), QColor("#00008B"), "blue"},
        {QString::fromUtf16(u"天蓝"), QString::fromUtf16(u"天空的颜色"), QColor("#87CEEB"), "blue"},
        {QString::fromUtf16(u"湖蓝"), QString::fromUtf16(u"湖水的颜色"), QColor("#ADD8E6"), "blue"},
        {QString::fromUtf16(u"海军蓝"), QString::fromUtf16(u"海军的颜色"), QColor("#000080"), "blue"},
        {QString::fromUtf16(u"皇家蓝"), QString::fromUtf16(u"皇室的颜色"), QColor("#4169E1"), "blue"},

        // 紫色系
        {QString::fromUtf16(u"紫色"), QString::fromUtf16(u"标准紫色"), QColor("#800080"), "purple"},
        {QString::fromUtf16(u"深紫"), QString::fromUtf16(u"深沉的紫色"), QColor("#4B0082"), "purple"},
        {QString::fromUtf16(u"薰衣草"), QString::fromUtf16(u"薰衣草的颜色"), QColor("#E6E6FA"), "purple"},
        {QString::fromUtf16(u"丁香紫"), QString::fromUtf16(u"丁香花的颜色"), QColor("#C8A2C8"), "purple"},
        {QString::fromUtf16(u"靛蓝"), QString::fromUtf16(u"靛蓝染料"), QColor("#4B0082"), "purple"},

        // 粉色系
        {QString::fromUtf16(u"粉色"), QString::fromUtf16(u"标准粉色"), QColor("#FFC0CB"), "pink"},
        {QString::fromUtf16(u"热粉"), QString::fromUtf16(u"热情的粉色"), QColor("#FF69B4"), "pink"},
        {QString::fromUtf16(u"浅粉"), QString::fromUtf16(u"淡雅的粉色"), QColor("#FFB6C1"), "pink"},
        {QString::fromUtf16(u"玫瑰粉"), QString::fromUtf16(u"玫瑰的颜色"), QColor("#FF007F"), "pink"},
        {QString::fromUtf16(u"腮红粉"), QString::fromUtf16(u"腮红的颜色"), QColor("#FF91A4"), "pink"},

        // 灰色系
        {QString::fromUtf16(u"白色"), QString::fromUtf16(u"纯白色"), QColor("#FFFFFF"), "gray"},
        {QString::fromUtf16(u"银色"), QString::fromUtf16(u"金属银色"), QColor("#C0C0C0"), "gray"},
        {QString::fromUtf16(u"灰色"), QString::fromUtf16(u"标准灰色"), QColor("#808080"), "gray"},
        {QString::fromUtf16(u"暗灰"), QString::fromUtf16(u"较深的灰色"), QColor("#A9A9A9"), "gray"},
        {QString::fromUtf16(u"黑色"), QString::fromUtf16(u"纯黑色"), QColor("#000000"), "gray"},
    };
}

void RgbColorTablePage::setupUI() {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    // 搜索栏
    auto *searchLayout = new QHBoxLayout();
    searchLayout->setSpacing(10);

    m_searchEdit = new QLineEdit();
    m_searchEdit->setPlaceholderText(QString::fromUtf16(u"🔍 搜索颜色..."));
    m_searchEdit->setFixedHeight(35);
    searchLayout->addWidget(m_searchEdit);

    mainLayout->addLayout(searchLayout);

    // 分类标签
    auto *tabLayout = new QHBoxLayout();
    tabLayout->setSpacing(5);

    m_tabLeftBtn = new QPushButton("◀");
    m_tabLeftBtn->setFixedSize(30, 30);
    m_tabLeftBtn->hide();

    m_tabScroll = new QScrollArea();
    m_tabScroll->setWidgetResizable(true);
    m_tabScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tabScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tabScroll->setFixedHeight(40);

    auto *tabWidget = new QWidget();
    m_categoryGroup = new QButtonGroup(this);
    m_categoryGroup->setExclusive(true);

    auto *tabGridLayout = new QHBoxLayout(tabWidget);
    tabGridLayout->setContentsMargins(0, 0, 0, 0);
    tabGridLayout->setSpacing(5);

    for (int i = 0; i < m_categories.size(); ++i) {
        auto *btn = new QPushButton(QString("%1 %2").arg(m_categories[i].icon, m_categories[i].name));
        btn->setCheckable(true);
        btn->setFixedHeight(30);
        btn->setMinimumWidth(80);
        m_categoryGroup->addButton(btn, i);
        tabGridLayout->addWidget(btn);
    }
    tabGridLayout->addStretch();

    m_tabScroll->setWidget(tabWidget);
    tabLayout->addWidget(m_tabScroll);

    m_tabRightBtn = new QPushButton("▶");
    m_tabRightBtn->setFixedSize(30, 30);
    m_tabRightBtn->hide();

    mainLayout->addLayout(tabLayout);

    // 主内容区域
    auto *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(15);

    // 左侧颜色网格
    auto *gridPanel = new QWidget();
    auto *gridPanelLayout = new QVBoxLayout(gridPanel);
    gridPanelLayout->setContentsMargins(0, 0, 0, 0);

    m_gridScroll = new QScrollArea();
    m_gridScroll->setWidgetResizable(true);
    m_gridScroll->setFrameShape(QFrame::NoFrame);

    m_gridContainer = new QWidget();
    m_gridLayout = new QGridLayout(m_gridContainer);
    m_gridLayout->setSpacing(8);
    m_gridLayout->setContentsMargins(5, 5, 5, 5);

    m_gridScroll->setWidget(m_gridContainer);
    gridPanelLayout->addWidget(m_gridScroll);

    contentLayout->addWidget(gridPanel, 2);

    // 右侧详情面板
    auto *detailPanel = new QWidget();
    detailPanel->setFixedWidth(250);
    auto *detailLayout = new QVBoxLayout(detailPanel);
    detailLayout->setContentsMargins(0, 0, 0, 0);
    detailLayout->setSpacing(10);

    m_detailCard = new QWidget();
    m_detailCard->setStyleSheet("background-color: #2d2d2d; border-radius: 8px; padding: 15px;");
    auto *detailCardLayout = new QVBoxLayout(m_detailCard);
    detailCardLayout->setSpacing(10);

    m_detailPreview = new QWidget();
    m_detailPreview->setFixedHeight(100);
    m_detailPreview->setStyleSheet("background-color: #4A90D9; border-radius: 8px;");
    detailCardLayout->addWidget(m_detailPreview);

    m_detailNameLabel = new QLabel(QString::fromUtf16(u"选择一个颜色"));
    m_detailNameLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #ffffff;");
    detailCardLayout->addWidget(m_detailNameLabel);

    m_detailDescLabel = new QLabel(QString::fromUtf16(u"点击颜色卡片查看详情"));
    m_detailDescLabel->setWordWrap(true);
    detailCardLayout->addWidget(m_detailDescLabel);

    m_detailHexLabel = new QLabel("HEX: #4A90D9");
    m_detailHexLabel->setStyleSheet("font-family: Consolas, monospace; color: #4FC3F7;");
    detailCardLayout->addWidget(m_detailHexLabel);

    m_detailRgbLabel = new QLabel("RGB: 74, 144, 217");
    m_detailRgbLabel->setStyleSheet("font-family: Consolas, monospace; color: #4FC3F7;");
    detailCardLayout->addWidget(m_detailRgbLabel);

    auto *copyLayout = new QHBoxLayout();
    m_copyHexBtn = new QPushButton(QString::fromUtf16(u"复制 HEX"));
    m_copyRgbBtn = new QPushButton(QString::fromUtf16(u"复制 RGB"));
    copyLayout->addWidget(m_copyHexBtn);
    copyLayout->addWidget(m_copyRgbBtn);
    detailCardLayout->addLayout(copyLayout);

    detailLayout->addWidget(m_detailCard);
    detailLayout->addStretch();

    contentLayout->addWidget(detailPanel, 1);

    mainLayout->addLayout(contentLayout, 1);

    // 连接信号
    connect(m_searchEdit, &QLineEdit::textChanged, this, &RgbColorTablePage::onSearchChanged);
    connect(m_categoryGroup, QOverload<int>::of(&QButtonGroup::idClicked), this, &RgbColorTablePage::onCategoryClicked);
    connect(m_copyHexBtn, &QPushButton::clicked, this, &RgbColorTablePage::onCopyHex);
    connect(m_copyRgbBtn, &QPushButton::clicked, this, &RgbColorTablePage::onCopyRgb);

    // 默认选中"全部"
    m_categoryGroup->button(0)->setChecked(true);
}

void RgbColorTablePage::onCategoryClicked(int id) {
    m_currentCategory = id;
    filterByCategory(id);
}

void RgbColorTablePage::onColorClicked(const QColor &color, const QString &name) {
    QString desc;
    for (const auto &entry : m_allColors) {
        if (entry.name == name) {
            desc = entry.desc;
            break;
        }
    }
    updateDetail(color, name, desc);
}

void RgbColorTablePage::onSearchChanged(const QString &text) {
    filterBySearch(text);
}

void RgbColorTablePage::onCopyHex() {
    QString hex = m_selectedColor.name().toUpper();
    QApplication::clipboard()->setText(hex);
    m_copyHexBtn->setText(QString::fromUtf16(u"已复制!"));
    QTimer::singleShot(1500, m_copyHexBtn, [this]() {
        m_copyHexBtn->setText(QString::fromUtf16(u"复制 HEX"));
    });
}

void RgbColorTablePage::onCopyRgb() {
    QString rgb = QString("rgb(%1, %2, %3)")
        .arg(m_selectedColor.red())
        .arg(m_selectedColor.green())
        .arg(m_selectedColor.blue());
    QApplication::clipboard()->setText(rgb);
    m_copyRgbBtn->setText(QString::fromUtf16(u"已复制!"));
    QTimer::singleShot(1500, m_copyRgbBtn, [this]() {
        m_copyRgbBtn->setText(QString::fromUtf16(u"复制 RGB"));
    });
}

void RgbColorTablePage::filterByCategory(int categoryId) {
    m_currentColors.clear();
    QString category = m_categories[categoryId].id;

    for (const auto &color : m_allColors) {
        if (category == "all" || color.category == category) {
            m_currentColors.append(color);
        }
    }

    populateGrid(m_currentColors);
}

void RgbColorTablePage::filterBySearch(const QString &text) {
    if (text.isEmpty()) {
        filterByCategory(m_currentCategory);
        return;
    }

    m_currentColors.clear();
    for (const auto &color : m_allColors) {
        if (color.name.contains(text, Qt::CaseInsensitive) ||
            color.desc.contains(text, Qt::CaseInsensitive) ||
            color.color.name().contains(text, Qt::CaseInsensitive)) {
            m_currentColors.append(color);
        }
    }

    populateGrid(m_currentColors);
}

void RgbColorTablePage::populateGrid(const QList<ColorEntry> &colors) {
    // 清空现有内容
    QLayoutItem *item;
    while ((item = m_gridLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    int row = 0;
    int col = 0;
    int maxCols = 4;

    for (const auto &colorEntry : colors) {
        auto *card = new QWidget();
        card->setFixedSize(m_cardWidth, m_cardHeight);
        card->setStyleSheet(QString(
            "background-color: %1; border-radius: 6px; border: 2px solid transparent;"
        ).arg(colorEntry.color.name()));
        card->setCursor(Qt::PointingHandCursor);
        card->installEventFilter(this);

        auto *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(8, 8, 8, 8);

        auto *nameLabel = new QLabel(colorEntry.name);
        nameLabel->setStyleSheet(QString("color: %1; font-weight: bold; font-size: 12px;")
            .arg(colorEntry.color.lightness() > 128 ? "#000000" : "#FFFFFF"));
        cardLayout->addWidget(nameLabel);

        auto *hexLabel = new QLabel(colorEntry.color.name().toUpper());
        hexLabel->setStyleSheet(QString("color: %1; font-family: Consolas, monospace; font-size: 10px;")
            .arg(colorEntry.color.lightness() > 128 ? "#333333" : "#EEEEEE"));
        cardLayout->addWidget(hexLabel);

        card->setProperty("color", colorEntry.color);
        card->setProperty("name", colorEntry.name);

        connect(card, &QWidget::customContextMenuRequested, this, [this, colorEntry](const QPoint &) {
            QApplication::clipboard()->setText(colorEntry.color.name().toUpper());
        });

        m_gridLayout->addWidget(card, row, col);

        col++;
        if (col >= maxCols) {
            col = 0;
            row++;
        }
    }
}

bool RgbColorTablePage::eventFilter(QObject *obj, QEvent *event) {
    QWidget *widget = qobject_cast<QWidget*>(obj);
    if (!widget) return false;

    if (event->type() == QEvent::MouseButtonPress) {
        QColor color = widget->property("color").value<QColor>();
        QString name = widget->property("name").toString();
        if (color.isValid()) {
            onColorClicked(color, name);
            return true;
        }
    }

    return QWidget::eventFilter(obj, event);
}

void RgbColorTablePage::updateDetail(const QColor &color, const QString &name, const QString &desc) {
    m_selectedColor = color;
    m_detailPreview->setStyleSheet(QString("background-color: %1; border-radius: 8px;").arg(color.name()));
    m_detailNameLabel->setText(name);
    m_detailDescLabel->setText(desc);
    m_detailHexLabel->setText(QString("HEX: %1").arg(color.name().toUpper()));
    m_detailRgbLabel->setText(QString("RGB: %1, %2, %3").arg(color.red()).arg(color.green()).arg(color.blue()));
}

void RgbColorTablePage::setDarkTheme(bool dark) {
    m_isDark = dark;
    if (dark) {
        m_detailCard->setStyleSheet("background-color: #2d2d2d; border-radius: 8px; padding: 15px;");
        m_detailNameLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #ffffff;");
        m_detailHexLabel->setStyleSheet("font-family: Consolas, monospace; color: #4FC3F7;");
        m_detailRgbLabel->setStyleSheet("font-family: Consolas, monospace; color: #4FC3F7;");
    } else {
        m_detailCard->setStyleSheet("background-color: #f5f5f5; border-radius: 8px; padding: 15px;");
        m_detailNameLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #333333;");
        m_detailHexLabel->setStyleSheet("font-family: Consolas, monospace; color: #0078d4;");
        m_detailRgbLabel->setStyleSheet("font-family: Consolas, monospace; color: #0078d4;");
    }
}
