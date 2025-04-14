#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <vector>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    Ui::Widget *ui;
    int rows;  // 行数
    int cols;  // 列数
    int lastRow; // 维护最后绘制位置
    int lastCol; // 不重复绘制表格
    int navBarHeight; //状态栏高度
    QTimer *timer;
    bool nextStatus(std::vector<int>& neighbors, bool status);
    void updateGrid();
    std::vector<std::vector<int>> grid;  // 存储格子数据

    bool isRunning;  // 标记是否正在运行
    bool isDrawing;  // 标记拖动绘制
    QPushButton *toggleButton;  // 按钮
    QPushButton *applyButton;   // 应用按钮
    QLineEdit *rowsInput;       // 行数输入框
    QLineEdit *colsInput;       // 列数输入框
    QLabel *rowsLabel;          // 行数标签
    QLabel *colsLabel;          // 列数标签

    void applyGridSize();       // 应用新的表格尺寸

};
#endif // WIDGET_H
