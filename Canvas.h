// CS 3505 – Assignment 8
// Pierce Bringhurst, Tyler Clark, Jakob Horvath, Bridger Peterson, Tri Tran, Royal Tippetts

#ifndef Canvas_H
#define Canvas_H

#include <QWidget>
#include <QObject>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QRgb>
#include <deque>

/**
 * @brief The Canvas class constructs and displays the editor window. It also handles drawing and tool use.
 */
class Canvas : public QWidget
{
    Q_OBJECT

public:
    Canvas(QWidget *parent = nullptr);
    Canvas(int dimensions, QWidget *parent = nullptr);

private:
    QColor drawColor;
    QColor lastColor;
    QImage canvasImage;
    QImage onionImage;
    int width;
    int height;
    int brushSize = 10;
    int currentTool = 0;
    int pixelSize;
    int userDimensions;
    bool onionLayerToggle = 1;
    void fillGrid(const QPoint &point);
    void shiftPixels(int, QImage &);
    void initializeSize(int dimensions);

public slots:
    void clear();
    void loadCleanCanvas(QImage *&Canvas, int dimensions);
    void setDrawColor(QColor color);
    void changeFrame(QImage *&Canvas, QImage *&previousCanvas);
    void clearCanvas(QImage &Canvas, int width, int height);
    void setOnionLayer(QImage *&previousCanvas);
    void toggleOnionSkinLayer();
    void toolChanged(int);
    void fillPixels(QPoint clickedPoint, QImage &canvas);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // Canvas_H
