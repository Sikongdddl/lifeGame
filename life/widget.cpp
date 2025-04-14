#include "widget.h"
#include <vector>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QVBoxLayout>
#include <QMouseEvent>

#include <QIntValidator>
Widget::Widget(QWidget *parent)
    : QWidget(parent), rows(10), cols(10),grid(rows, std::vector<int>(cols, 0)), isRunning(false)
{
    navBarHeight = 30;

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Widget::updateGrid);

    toggleButton = new QPushButton("Start", this);
    toggleButton->setGeometry(10,0,50,navBarHeight);


    clearButton = new QPushButton("Clear",this);
    clearButton->setGeometry(350,0,70,navBarHeight);
    connect(clearButton, &QPushButton::clicked, this, [this]() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                grid[i][j] = 0;
            }
        }
        update();  // 触发重绘
    });

    stepButton = new QPushButton("Step",this);
    stepButton->setGeometry(70,0,50,navBarHeight);

    connect(stepButton, &QPushButton::clicked, this, [this]() {
        updateGrid();  // 只运行一轮
    });

    connect(toggleButton, &QPushButton::clicked, this, [this]() {
        if (isRunning) {
            // 停止定时器
            timer->stop();
            isRunning = false;
            toggleButton->setText("Start");  // 改变按钮文字为“Start”
        } else {
            // 启动定时器
            timer->start(1000);  // 每1秒更新一次
            isRunning = true;
            toggleButton->setText("Pause");  // 改变按钮文字为“Pause”
        }
    });

    rowsLabel = new QLabel("Rows:", this);
    rowsLabel->setGeometry(130, 0, 40, navBarHeight);  // 设置标签位置和大小
    rowsInput = new QLineEdit(this);
    rowsInput->setGeometry(180, 0, 50, navBarHeight);  // 设置输入框位置和大小
    rowsInput->setValidator(new QIntValidator(1, 100, this));  // 限制输入为数字
    rowsInput->setText(QString::number(rows));

    // 创建列数标签和输入框
    colsLabel = new QLabel("Cols:", this);
    colsLabel->setGeometry(240, 0, 40, navBarHeight);  // 设置标签位置和大小
    colsInput = new QLineEdit(this);
    colsInput->setGeometry(290, 0, 50, navBarHeight);  // 设置输入框位置和大小
    colsInput->setValidator(new QIntValidator(1, 100, this));  // 限制输入为数字
    colsInput->setText(QString::number(cols));

    // 创建应用按钮
    applyButton = new QPushButton("Apply", this);
    applyButton->setGeometry(430, 0, 70, navBarHeight);  // 设置按钮位置和大小
    connect(applyButton, &QPushButton::clicked, this, &Widget::applyGridSize);

}

void Widget::applyGridSize() {
    // 获取输入的行列数并更新
    rows = rowsInput->text().toInt();
    cols = colsInput->text().toInt();

    // 更新grid并重绘
    grid.resize(rows);
    for (auto &row : grid) {
        row.resize(cols, 0);
    }
    update();  // 更新界面
}

void Widget::paintEvent(QPaintEvent *) {

    QPainter painter(this);

    int gridWidth = width() / cols;  // 每个格子的宽度
    int gridHeight = (height() - navBarHeight) / rows; // 每个格子的高度，减去按钮的高度

    // 绘制网格区域
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            QRect rect(col * gridWidth, row * gridHeight + navBarHeight, gridWidth, gridHeight);
            if (grid[row][col] == 1) {
                painter.setBrush(Qt::black);  // 值为 1 用黑色填充
            } else {
                painter.setBrush(Qt::white);  // 值为 0 用白色填充
            }
            painter.drawRect(rect);  // 绘制矩形
        }
    }

    // 绘制网格线
    painter.setPen(Qt::gray);
    for (int row = 0; row <= rows; ++row) {
        painter.drawLine(0, row * gridHeight + navBarHeight, width(), row * gridHeight + navBarHeight); // 水平线
    }
    for (int col = 0; col <= cols; ++col) {
        painter.drawLine(col * gridWidth, navBarHeight, col * gridWidth, height()); // 垂直线
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    // 获取点击的坐标
    int x = event->position().x();
    int y = event->position().y();

    // 确保点击的位置不在按钮区域上
    if (y < navBarHeight) {
        return;  // 点击在按钮区域，忽略
    }

    // 计算点击位置对应的网格行列
    int gridWidth = width() / cols;
    int gridHeight = (height() - navBarHeight) / rows;

    int col = x / gridWidth;
    int row = (y - navBarHeight) / gridHeight;

    // 检查计算出的行列是否有效
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        // 改变对应位置的grid值（反转0和1）
        grid[row][col] = (grid[row][col] + 1) % 2;
        isDrawing = true;
        lastRow = row;
        lastCol = col;
        update();  // 更新界面
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (isDrawing) {  // 如果正在绘制
        int x = event->position().x();
        int y = event->position().y();

        int gridWidth = width() / cols;
        int gridHeight = (height() - navBarHeight) / rows;

        int col = x / gridWidth;
        int row = (y - navBarHeight) / gridHeight;

        if (row >= 0 && row < rows && col >= 0 && col < cols && (row != lastRow || col != lastCol)) {
            grid[row][col] = (grid[row][col] + 1) % 2;  // 切换状态
            lastRow = row;
            lastCol = col;
            update();  // 更新界面
        }
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    isDrawing = false;  // 停止绘制
}


bool Widget::nextStatus(std::vector<int>& neighbors, bool status){
    int ans = 0;
    for(int i = 0; i < neighbors.size(); ++i){
        if(neighbors[i] == 1){
            ans += 1;
        }
    }
    if(status == true){
        switch (ans){
            case 0:
            case 1:
                return false;
            case 2:
            case 3:
                return true;
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
                return false;
        }
    }
    if(status == false){
        if(ans == 3){
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

void Widget::updateGrid(){
    qDebug() << "update!";
    std::vector<int> neighbors;
    std::vector<std::vector<int>> buffer2;
    std::vector<int> buffer1;

    for(int i = 0; i < this->grid.size(); ++i){
        for(int j = 0; j < this->grid[0].size(); ++j){
            neighbors.clear();
            if(i != 0){
                neighbors.push_back(this->grid[i-1][j]);
            }
            if(j != 0){
                neighbors.push_back(this->grid[i][j-1]);
            }
            if(i != 0 && j != 0){
                neighbors.push_back(this->grid[i-1][j-1]);
            }
            if(i != this->grid.size()-1){
                neighbors.push_back(this->grid[i+1][j]);
            }
            if(j != this->grid[0].size()-1){
                neighbors.push_back(this->grid[i][j+1]);
            }
            if(i != this->grid.size()-1 && j != this->grid[0].size()-1){
                neighbors.push_back(this->grid[i+1][j+1]);
            }
            if(i != 0 && j != this->grid[0].size()-1){
                neighbors.push_back(this->grid[i-1][j+1]);
            }
            if(i != this->grid.size()-1 && j != 0){
                neighbors.push_back(this->grid[i+1][j-1]);
            }
            bool status = nextStatus(neighbors,this->grid[i][j]);
            buffer1.push_back(status);
        }
        buffer2.push_back(buffer1);
        buffer1.clear();
    }
    for(int i = 0; i < buffer2.size(); ++i){
        for(int j = 0; j < buffer2[0].size(); ++j){
            this->grid[i][j] = buffer2[i][j];
        }
    }
    update();
}
Widget::~Widget()
{

}
