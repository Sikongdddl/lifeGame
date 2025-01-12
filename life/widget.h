#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <vector>
#include <QPushButton>

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
private:
    Ui::Widget *ui;
    int rows;  // 行数
    int cols;  // 列数
    QTimer *timer;
    bool nextStatus(std::vector<int>& neighbors, bool status);
    void updateGrid();
    std::vector<std::vector<int>> grid;  // 存储格子数据

    bool isRunning;  // 标记是否正在运行
    QPushButton *toggleButton;  // 按钮

};
#endif // WIDGET_H
