// CS 3505 – Assignment 8
// Pierce Bringhurst, Tyler Clark, Jakob Horvath, Bridger Peterson, Tri Tran, Royal Tippetts

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "controller.h"
#include "Canvas.h"
#include "previewCanvas.h"
#include "spritetools.h"

#include <QMainWindow>
#include <QDebug>
#include <QLayout>
#include <QColorDialog>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QGridLayout>
#include <QWidget>
#include <QTimer>
#include <QEventLoop>
#include <QString>


namespace Ui {

/**
 * The view for our sprite editor. Intended to act as a go between for our controller and tool classes,
 * this class instead became the central square for the project handling connections between classes
 * directly so far as visual tasks are concerned.
 */
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, Controller *controller = nullptr);
    ~MainWindow();
    Canvas *drawWindow;
    previewCanvas *previewWindow;
    SpriteTools *st;   
private:
    QEventLoop loop;
    QTimer * timer;
    QPushButton * decreasePlaybackSpeedButton;
    QPushButton * increasePlaybackSpeedButton;
    int playbackSpeed;
    Ui::MainWindow *ui;
    QColorDialog *colorDialog;
    QColor currentColor;
    bool eraserInactive;
    int dimensions;
    bool hasSavePath;
    QString savePath;

    int getCanvasSize();
    void closeEvent(QCloseEvent *event);

signals:
//Setup
    void setUp();
    void previewTimer();


//Toolbar
    void changeToolSelection(int toolNumber);


//Drawing in Canvas
    void drawPixel(); //pass coordinates of some sort. Controller will use current tool and color selections to make a change.


//changes to frame selection
    void addFrameSignal(int index); // call controller to add a frame in front of this index
    void loadFrame(); //Needs to specify which frame
    void deleteFrameSignal();


//Save, load, new, and export
    void save(QString filePath);
    void load(QString filePath);
    void newProject(int dimensions);
    void exportAsGIF(QString filePath);


public slots:
    void addFrameSlot(int index);
    void changeIndexNumber(int index, int size);
    void decreasePlaybackSpeed();
    void increasePlaybackSpeed();
    void colorSelected(const QColor & color);


//Toolbar
    void paletteButtonClicked(QPushButton *);
    void brushButtonClicked();
    void bucketButtonClicked();
    void eraserButtonClicked();
    void moveButtonClicked(QString);


//Timer
    void previewLoop();


private slots:
    //File Menu
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionLoad_triggered();
    void on_actionExport_as_GIF_triggered();
    void on_actionQuit_triggered();
};

#endif // MAINWINDOW_H
