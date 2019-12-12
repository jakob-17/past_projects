#include "Canvas.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QRgba64>
#include <QDebug>

Canvas::Canvas(int dimensions, QWidget *parent) : QWidget(parent) {
    width = 512;
    height = 512;
    userDimensions = dimensions;
    pixelSize = width / userDimensions;
    drawColor = QColor(Qt::black);
    resize(width, height);
    clearCanvas(canvasImage, width, height);
    lastColor = QColor(Qt::black);
}

void Canvas::loadCleanCanvas(QImage *&Canvas, int dimensions) {
    if(*Canvas == QImage()) {
        Canvas = new QImage(width, height, QImage::Format_RGBA8888);
        Canvas->fill(QColor(255, 255, 255, 0));
    }
    canvasImage = *Canvas;
    pixelSize = width / dimensions;
    //Have QImage* we are putting in point to canvasImage so we get the edits
    Canvas = &canvasImage;
    repaint();
    update();
}

void Canvas::clearCanvas(QImage &Canvas, int width, int height) {
    Canvas = QImage(width,height,QImage::Format_RGBA8888);
    Canvas.fill(QColor(255, 255, 255, 0));
}

void Canvas::changeFrame(QImage *&Canvas, QImage *&previousCanvas) {
    if(*Canvas == QImage()) {
        Canvas = new QImage(width, height, QImage::Format_RGBA8888);
        Canvas->fill(QColor(255, 255, 255, 0));
    }
    if(*previousCanvas == QImage()) {
        previousCanvas = new QImage(width, height, QImage::Format_RGBA8888);
        previousCanvas->fill(QColor(255, 255, 255, 0));
    }
    //make the QImage* we are switching out point to a copy of current canvasImage
    previousCanvas = new QImage(canvasImage);
    //make canvasImage look like QImage* we are switching in
    canvasImage = *Canvas;
    //Have QImage* we are putting in point to canvasImage so we get the edits
    Canvas = &canvasImage;
    setOnionLayer(previousCanvas);
    resize(parentWidget()->width(), parentWidget()->height());
    update();
}

void Canvas::setOnionLayer(QImage *&previousCanvas) {
    onionImage = *previousCanvas;
}

void Canvas::toggleOnionSkinLayer() {
    onionLayerToggle = !onionLayerToggle;
    update();
}

void Canvas::setDrawColor(QColor color) {
    drawColor = color;
}

void Canvas::clear() {
    clearCanvas(canvasImage, width, height);
    update();
}


/**
 * Given an int direction, will shift the current image up, down, left, or right by one unit. Pixels will wrap to the opposite side if necessary.
 *
 * @param direction - 3, 4, 5, or 6 corresponding to up, down, right, or left
 * @param canvas - The current image being edited
 */
void Canvas::shiftPixels(int direction, QImage &canvas) {
    std::deque<QColor> wrapPixels;
    std::deque<QColor> currentPixels;
    int pixelLength = pixelSize; // define the length of a pixel segment
    if (direction == 3) { // shift up
        // get pixels that will wrap around to the bottom
        for (int h = 0; h < pixelLength; h++) {
            for (int w = 0; w < canvas.width(); w++) {
                wrapPixels.push_back(canvas.pixelColor(w, h));
            }
        }
        // shift pixels up
        for (int h = pixelLength; h < canvas.height(); h++) {
            for (int w = 0; w < canvas.width(); w++) {
                currentPixels.push_back(canvas.pixelColor(w, h));
            }
        }
        for (int h = 0; h < canvas.height() - pixelLength; h++) {
            for (int w = 0; w < canvas.width(); w++) {
                canvas.setPixelColor(w, h, currentPixels.front());
                currentPixels.pop_front();
            }
        }
        // assign saved wrap pixels to the bottom segment
        for (int h = canvas.height() - pixelLength; h < canvas.height(); h++) {
            for (int w = 0; w < canvas.width(); w++) {
                canvas.setPixelColor(w, h, wrapPixels.front());
                wrapPixels.pop_front();
            }
        }
    }
    else if (direction == 4) { // shift down
        // get pixels that will wrap around to the top
        for (int h = canvas.height() - pixelLength; h < canvas.height(); h++) {
            for (int w = 0; w < canvas.width(); w++) {
                wrapPixels.push_back(canvas.pixelColor(w, h));
            }
        }
        // shift pixels down
        for (int h = 0; h < canvas.height() - pixelLength; h++) {
            for (int w = 0; w < canvas.width(); w++) {
                currentPixels.push_back(canvas.pixelColor(w, h));
            }
        }
        for (int h = pixelLength; h < canvas.height(); h++) {
            for (int w = 0; w < canvas.width(); w++) {
                canvas.setPixelColor(w, h, currentPixels.front());
                currentPixels.pop_front();
            }
        }
        // assign saved wrap pixels to the top segment
        for (int h = 0; h < pixelLength; h++) {
            for (int w = 0; w < canvas.width(); w++) {
                canvas.setPixelColor(w, h, wrapPixels.front());
                wrapPixels.pop_front();
            }
        }
    }
    else if (direction == 5) { // shift right
        // get pixels that will wrap around to the left
        for (int w = canvas.width() - pixelLength; w < canvas.width(); w++) {
            for (int h = 0; h < canvas.height(); h++) {
                wrapPixels.push_back(canvas.pixelColor(w, h));
            }
        }
        // shift pixels right
        for (int w = 0; w < canvas.width() - pixelLength; w++) {
            for (int h = 0; h < canvas.height(); h++) {
                currentPixels.push_back(canvas.pixelColor(w, h));
            }
        }
        for (int w = pixelLength; w < canvas.width(); w++) {
            for (int h = 0; h < canvas.height(); h++) {
                canvas.setPixelColor(w, h, currentPixels.front());
                currentPixels.pop_front();
            }
        }
        // assign saved wrap pixels to the leftmost segment
        for (int w = 0; w < pixelLength; w++) {
            for (int h = 0; h < canvas.height(); h++) {
                canvas.setPixelColor(w, h, wrapPixels.front());
                wrapPixels.pop_front();
            }
        }
    }
    else { // shift left
        // get pixels that will wrap around to the right
        for (int w = 0; w < pixelLength; w++) {
            for (int h = 0; h < canvas.height(); h++) {
                wrapPixels.push_back(canvas.pixelColor(w, h));
            }
        }
        // shift pixels left
        for (int w = pixelLength; w < canvas.width(); w++) {
            for (int h = 0; h < canvas.height(); h++) {
                currentPixels.push_back(canvas.pixelColor(w, h));
            }
        }
        for (int w = 0; w < canvas.width() - pixelLength; w++) {
            for (int h = 0; h < canvas.height(); h++) {
                canvas.setPixelColor(w, h, currentPixels.front());
                currentPixels.pop_front();
            }
        }
        // assign saved wrap pixels to the rightmost segment
        for (int w = canvas.width() - pixelLength; w < canvas.width(); w++) {
            for (int h = 0; h < canvas.height(); h++) {
                canvas.setPixelColor(w, h, wrapPixels.front());
                wrapPixels.pop_front();
            }
        }
    }
}


/**
 * Given the color of the selected pixel, fills all pixels of the that color with the current color within a possible border.
 *
 * @param pt - Location of the mouse click
 * @param canvas - The current image being edited
 */
void Canvas::fillPixels(QPoint pt, QImage &canvas) {
    QColor oldColor = canvas.pixelColor(pt);
    QColor newColor = drawColor;

    // mark every pixel as not visited
    std::vector<bool> visited(static_cast<unsigned long>(canvas.width() * canvas.height()));
    for (unsigned long i = 0; i < visited.size(); i++) {
        visited[i] = false;
    }

    std::list<long> queue; // create queue for BFS
    // set initial pixel to visited and add to the queue
    visited.at(static_cast<unsigned long>(pt.x() + (canvas.width() * pt.y()))) = true;
    queue.push_back(pt.x() + (canvas.width() * pt.y()));

    long adjIndex;
    long currentPixel;
    while (!queue.empty()) {
        // assign front pixel to current and dequeue
        currentPixel = queue.front();
        queue.pop_front();
        // get all adjacent pixels to current, if not visited then mark visited, if the same color as oldColor then update color and enqueue it
        if (currentPixel - 1 >= 0 && !visited.at(static_cast<unsigned long>(currentPixel - 1))) { // pixel to the left
            adjIndex = currentPixel - 1;
            if (canvas.pixelColor(adjIndex % canvas.width(), static_cast<int>(adjIndex / canvas.width())) == oldColor) {
                canvas.setPixelColor(adjIndex % canvas.width(), static_cast<int>(adjIndex / canvas.width()), newColor);
                queue.push_back(adjIndex);
            }
            visited.at(static_cast<unsigned long>(adjIndex)) = true;
        }
        if (static_cast<unsigned long>(currentPixel + 1) < visited.size() && !visited.at(static_cast<unsigned long>(currentPixel + 1))) { // pixel to the right
            adjIndex = currentPixel + 1;
            if (canvas.pixelColor(adjIndex % canvas.width(), static_cast<int>(adjIndex / canvas.width())) == oldColor) {
                canvas.setPixelColor(adjIndex % canvas.width(), static_cast<int>(adjIndex / canvas.width()), newColor);
                queue.push_back(adjIndex);
            }
            visited.at(static_cast<unsigned long>(adjIndex)) = true;
        }
        if (currentPixel - canvas.width() >= 0 && !visited.at(static_cast<unsigned long>(currentPixel - canvas.width()))) { // pixel above
            adjIndex = currentPixel - canvas.width();
            if (canvas.pixelColor(adjIndex % canvas.width(), static_cast<int>(adjIndex / canvas.width())) == oldColor) {
                canvas.setPixelColor(adjIndex % canvas.width(), static_cast<int>(adjIndex / canvas.width()), newColor);
                queue.push_back(adjIndex);
            }
            visited.at(static_cast<unsigned long>(adjIndex)) = true;
        }
        if (static_cast<unsigned long>(currentPixel + canvas.width()) < visited.size() && !visited.at(static_cast<unsigned long>(currentPixel + canvas.width()))) { // pixel below
            adjIndex = currentPixel + canvas.width();
            if (canvas.pixelColor(adjIndex % canvas.width(), static_cast<int>(adjIndex / canvas.width())) == oldColor) {
                canvas.setPixelColor(adjIndex % canvas.width(), static_cast<int>(adjIndex / canvas.width()), newColor);
                queue.push_back(adjIndex);
            }
            visited.at(static_cast<unsigned long>(adjIndex)) = true;
        }
    }
    canvas.setPixelColor(pt.x(), pt.y(), newColor); // update the pixel that was clicked on
}


/**
 * Decides what actions the canvas should take depending on the type of tool/button selected.
 *
 * @param toolNum - The tool that was selected.
 */
void Canvas::toolChanged(int toolNum) {
    // shift button clicked
    if (toolNum == 3 || toolNum == 4 || toolNum == 5 || toolNum == 6) {
        shiftPixels(toolNum, canvasImage);
        repaint();
        update();
    }
    // brush, bucket, or eraser
    else {
         // Change to brush or fill
        if (currentTool == 2  && (toolNum == 0 || toolNum == 1)) {
            drawColor = lastColor;
        }
        // Change to eraser
        if (toolNum == 2) {
            lastColor = drawColor;
            drawColor = QColor(255, 255, 255, 0);
        }
        currentTool = toolNum;
    }
}

void Canvas::fillGrid(const QPoint &point)
{
    for (int i = 0; i < pixelSize; i++) {
        for (int j = 0; j < pixelSize; j++) {
            // Add pixels by a box of pixels
            int x = (point.x() / pixelSize) * pixelSize + i;
            int y = (point.y() / pixelSize) * pixelSize + j;
            canvasImage.setPixelColor(x, y, drawColor);
        }
    }
    update();
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton){
        if (currentTool == 0 || currentTool == 2) {
            // regular brush or eraser
            fillGrid(event->pos());
        }
        else {
            // fill brush
            fillPixels(event->pos(), canvasImage);
        }
        repaint();
        update();
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton){
        fillGrid(event->pos());
        repaint();
        update();
    }
}

void Canvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawRect(0, 0, width, height);
    QRect drawRectangle = event->rect();
    painter.drawImage(drawRectangle, canvasImage, drawRectangle);
    if(onionLayerToggle) {
        QPixmap onionLayer = QPixmap::fromImage(onionImage);
        painter.setOpacity(0.50);
        painter.drawPixmap(0,0,onionLayer);
    }
}
