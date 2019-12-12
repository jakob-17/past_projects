// CS 3505 – Assignment 8
// Pierce Bringhurst, Tyler Clark, Jakob Horvath, Bridger Peterson, Tri Tran, Royal Tippetts

#ifndef SPRITETOOLS_H
#define SPRITETOOLS_H

#include <QWidget>
#include <QBoxLayout>
#include <QToolBar>
#include <QApplication>
#include <QMenu>
#include <QPushButton>


/**
 * @brief The SpriteTools class contains all tools/buttons which directly interact with the editor window (canvas).
 */
class SpriteTools : public QWidget
{
    Q_OBJECT
public:
    /**
     * Generates a QBoxLayout and a QToolbar. The toolLayout holds the toolbar.
     *
     * (param1) parent - SpriteTools is a member of the QWidget class
     */
    explicit SpriteTools(QWidget *parent = nullptr);

signals:
    void paletteButtonSig(QPushButton *);
    void brushButtonSig();
    void bucketButtonSig();
    void eraserButtonSig();
    void onionButtonSig();
    void shiftButtonSig(QString);

// Recieves the button signals from the toolbar. Emits further signals to connect to the mainwindow.
public slots:
    void paletteButtonClicked();
    void brushButtonSelected();
    void bucketButtonSelected();
    void eraserButtonSelected();
    void onionButtonSelected();
    void shiftButtonClicked();

private:
    QBoxLayout *toolLayout;
    QToolBar *toolbar;
    void populateToolBar(); // Adds four buttons that are responsible for shifting (moving; dragging) the entire pixel image.
    void addShiftButtons(); // Add buttons and their signals.

    QPushButton *paletteButton;
    QPushButton *brushButton;
    QPushButton *bucketButton;
    QPushButton *eraserButton;
    QPushButton *onionButton;
    QSize BUTTON_MIN;
    QSize BUTTON_MAX;
};

#endif // SPRITETOOLS_H
