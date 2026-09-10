#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollArea>
#include <QMap>
#include <QList>
#include <QColor>
#include <QPointer>

class QVBoxLayout;
class QGridLayout;
class QButtonGroup;
class QScrollArea;

class RgbColorTablePage : public QWidget {
    Q_OBJECT
public:
    explicit RgbColorTablePage(QWidget *parent = nullptr);
    void setDarkTheme(bool dark);

private slots:
    void onCategoryClicked(int id);
    void onColorClicked(const QColor &color, const QString &name);
    void onSearchChanged(const QString &text);
    void onCopyHex();
    void onCopyRgb();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    struct ColorEntry {
        QString name;
        QString desc;
        QColor color;
        QString category;
    };

    struct CategoryInfo {
        QString id;
        QString name;
        QString icon;
    };

    QLineEdit *m_searchEdit;
    QScrollArea *m_tabScroll;
    QPushButton *m_tabLeftBtn;
    QPushButton *m_tabRightBtn;
    QButtonGroup *m_categoryGroup;
    QWidget *m_gridContainer;
    QGridLayout *m_gridLayout;
    QScrollArea *m_gridScroll;
    QPointer<QWidget> m_selectedCard;

    QWidget *m_detailCard;
    QWidget *m_detailPreview;
    QLabel *m_detailNameLabel;
    QLabel *m_detailDescLabel;
    QLabel *m_detailHexLabel;
    QLabel *m_detailRgbLabel;
    QPushButton *m_copyHexBtn;
    QPushButton *m_copyRgbBtn;

    QList<ColorEntry> m_allColors;
    QList<CategoryInfo> m_categories;
    int m_currentCategory = 0;
    QColor m_selectedColor;
    QList<ColorEntry> m_currentColors;
    int m_cardWidth = 130;
    int m_cardHeight = 64;
    bool m_rebuilding = false;

    void initCategories();
    void initColors();
    void setupUI();
    void populateGrid(const QList<ColorEntry> &colors);
    void filterByCategory(int categoryId);
    void filterBySearch(const QString &text);
    void updateDetail(const QColor &color, const QString &name, const QString &desc);
    bool m_isDark = true;
};
