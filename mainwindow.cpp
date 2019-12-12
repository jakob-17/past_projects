#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, Controller *controller) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    dimensions = getCanvasSize();
    ui->setupUi(this);
    controller = new Controller();
    st = new SpriteTools();

    //declare Edit Window and Preview Animation Cycle Window
    drawWindow = new Canvas(dimensions, this);
    previewWindow = new previewCanvas(new QWidget());

    colorDialog = new QColorDialog(this);
    colorDialog->setOptions(QColorDialog::ShowAlphaChannel | QColorDialog::NoButtons | QColorDialog::DontUseNativeDialog);

    ui->spriteToolsLayout->addWidget(st /*, 3, 0, 3, 0, Qt::AlignLeft*/);
    ui->editLayout->addWidget(drawWindow);
    ui->previewLayout->addWidget(previewWindow);

    // toolbar connections to mainwindow
    connect(st, &SpriteTools::paletteButtonSig, this, &MainWindow::paletteButtonClicked);
    connect(st, &SpriteTools::brushButtonSig, this, &MainWindow::brushButtonClicked);
    connect(st, &SpriteTools::bucketButtonSig, this, &MainWindow::bucketButtonClicked);
    connect(st, &SpriteTools::eraserButtonSig, this, &MainWindow::eraserButtonClicked);
    connect(st, &SpriteTools::shiftButtonSig, this, &MainWindow::moveButtonClicked);
    connect(st, &SpriteTools::onionButtonSig, drawWindow, &Canvas::toggleOnionSkinLayer);

    // mainwindow connection to controller (toolbar)
    connect(this, &MainWindow::newProject, controller, &Controller::createNewProject);
    connect(this, &MainWindow::save, controller, &Controller::save);
    connect(this, &MainWindow::load, controller, &Controller::load);
    connect(this, &MainWindow::exportAsGIF, controller, &Controller::exportAsGif);

    // mainwindow connection to canvas (toolbar)
    connect(this, &MainWindow::changeToolSelection, drawWindow, &Canvas::toolChanged);

    //Make connection between previewCanvas and controller
    connect (this, &MainWindow::previewTimer, controller, &Controller::changePreviewFrame);
    connect(controller, &Controller::sendFrameToPreview, previewWindow, &previewCanvas::changeFrame);
    connect(controller, &Controller::sendIndexNumber, this, &MainWindow::changeIndexNumber);

    //Make connections between Controller and Canvas to make Frame edits
    connect (ui->nextFrame, &QPushButton::clicked, controller, &Controller::changeToFrame);
    connect(controller, &Controller::sendFrameToCanvas, drawWindow, &Canvas::changeFrame);

    //Make connections for previous Frame button
    connect(ui->previousFrame, &QPushButton::clicked, controller, &Controller::changeToPreviousFrame);

    //Make connection for decreasePlaybackSpeed button
    connect(ui->decreasePlaybackSpeed, &QPushButton::clicked, this, &MainWindow::decreasePlaybackSpeed);
    connect(ui->increasePlaybackSpeed, &QPushButton::clicked, this, &MainWindow::increasePlaybackSpeed);

    //Make connection to add frame
    connect(this, &MainWindow::addFrameSignal, controller, &Controller::addNewFrame);
    connect(ui->addFrameButton, &QPushButton::clicked, this, &MainWindow::addFrameSlot);

    //Make connection to delete frame
    connect(ui->deleteFrameButton, &QPushButton::clicked, controller, &Controller::deleteFrame);
    connect(controller, &Controller::loadCleanCanvas, drawWindow, &Canvas::loadCleanCanvas);

    //set up timer for the preview
    playbackSpeed = 500;
    timer = new QTimer(this);
    timer->setInterval(playbackSpeed);
    connect(timer, SIGNAL(timeout()), SLOT(previewLoop()));
    timer->start();
}


MainWindow::~MainWindow()
{
    delete ui;
}


int MainWindow::getCanvasSize(){
    bool ok;
    return QInputDialog::getInt(this, tr("Canvas Size Selection"), tr("Please input a canvs size in pixels:"), 32, 8, 256, 8, &ok);
}


//Slots//

void MainWindow::changeIndexNumber(int index, int size) {
    ui->indexLabel->setText("(" + QString::number(index + 1) + ", " + QString::number(size) + ")");

}


void MainWindow::addFrameSlot(int index) {
    addFrameSignal(index);
}


void MainWindow::increasePlaybackSpeed() {
    playbackSpeed = playbackSpeed - 20;
    timer->setInterval(playbackSpeed);
    if(playbackSpeed == 20) {
        ui->increasePlaybackSpeed->setDisabled(true);
    }
    if(playbackSpeed < 2000) {
        ui->decreasePlaybackSpeed->setEnabled(true);
    }

}


void MainWindow::decreasePlaybackSpeed() {
    playbackSpeed = playbackSpeed + 20;
    timer->setInterval(playbackSpeed);
    if(playbackSpeed == 2000) {
        ui->decreasePlaybackSpeed->setDisabled(true);
    }

    if(playbackSpeed > 20) {
        ui->increasePlaybackSpeed->setEnabled(true);
    }

}


    //slot that sends previewTimer signal to controller to change previewCanvas
    void MainWindow::previewLoop() {
        previewTimer();
    }


    void MainWindow::colorSelected(const QColor & color) {
        drawWindow->setDrawColor(color);
    }


//Toolbar//
    // Toolbar slots emit signals that communicate with the canvas class.
    void MainWindow::paletteButtonClicked(QPushButton *paletteButton) {
        if (eraserInactive) {
            currentColor = colorDialog->getColor();
            colorSelected(currentColor);
            // changes the background of the palette button to reflect the chosen color, makes the text a complement
            paletteButton->setStyleSheet(QString("background: rgb(%1,%2,%3); color: rgb(%4,%5,%6);").arg(currentColor.red()).arg(currentColor.green()).arg(currentColor.blue())
                                         .arg(255-currentColor.red()).arg(255-currentColor.green()).arg(255-currentColor.blue()));
        }
    }


    void MainWindow::brushButtonClicked() {
        emit changeToolSelection(0);
        eraserInactive = true;
    }


    void MainWindow::bucketButtonClicked() {
        emit changeToolSelection(1);
        eraserInactive = true;
    }


    void MainWindow::eraserButtonClicked() {
        emit changeToolSelection(2);
        eraserInactive = false;
    }


    void MainWindow::moveButtonClicked(QString buttonName) {
        if (buttonName == "Shift Up") {
            emit changeToolSelection(3);
        }
        else if (buttonName == "Shift Down") {
            emit changeToolSelection(4);
        }
        else if (buttonName == "Shift Right") {
            emit changeToolSelection(5);
        }
        else { // Shift Left
            emit changeToolSelection(6);
        }
    }


    void MainWindow::closeEvent(QCloseEvent *event){
        QMessageBox::StandardButton confirmClose;
        confirmClose = QMessageBox::question(this, tr("Close Program"), tr("Are you sure you want to quit? Any unsaved work will be lost."), QMessageBox::Yes|QMessageBox::No);

        if (confirmClose == QMessageBox::Yes)
        {
            event->accept();
        }
        else {
            event->ignore();
        }
    }


//File Menu//
void MainWindow::on_actionNew_triggered()
{
    QMessageBox::StandardButton confirmNew;
    confirmNew = QMessageBox::question(this, tr("Open New Project"), tr("Are you sure you want to start a new project? Any unsaved work will be lost."), QMessageBox::Yes|QMessageBox::Cancel);

    if (confirmNew == QMessageBox::Yes)
    {
        dimensions = getCanvasSize();
        hasSavePath = false;
        emit newProject(dimensions);
    }
}


void MainWindow::on_actionSave_triggered()
{
    if (hasSavePath) {
        emit save(savePath);
    }
    else {
        on_actionSave_As_triggered();
    }
}


void MainWindow::on_actionSave_As_triggered()
{
    savePath = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::currentPath(), tr("Sprite Sheet Project (*.ssp)"));
    emit save(savePath);
}

void MainWindow::on_actionLoad_triggered()
{
    QMessageBox::StandardButton confirmLoad;
    confirmLoad = QMessageBox::question(this, tr("Load Project"), tr("Are you sure you want to load a different project? Any unsaved work will be lost."), QMessageBox::Yes|QMessageBox::Cancel);

    if (confirmLoad == QMessageBox::Yes)
    {
        emit load(QFileDialog::getOpenFileName(this, tr("Load"), QDir::currentPath(), tr("Sprite Sheet Project (*.ssp)")));
    }
}


void MainWindow::on_actionExport_as_GIF_triggered()
{
    emit exportAsGIF(QFileDialog::getSaveFileName(this, tr("Export"), QDir::currentPath(), tr("GIF (*.gif)")));
}


void MainWindow::on_actionQuit_triggered()
{
    close();
}
