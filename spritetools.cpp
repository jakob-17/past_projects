#include "spritetools.h"


SpriteTools::SpriteTools(QWidget *parent) : QWidget(parent)
{
    toolLayout = new QBoxLayout(QBoxLayout::TopToBottom, this); // initialize class variable
    toolLayout->setContentsMargins(0, 0, 0, 0); // toolbar will now touch widget's edges
    toolbar = new QToolBar(); // initialize class variable
    toolbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolbar->setStyleSheet("QToolBar{spacing: 0px;}"
                           "QPushButton::checked{color: white;} QPushButton::checked{background-color: green;}");
    toolbar->setOrientation(Qt::Vertical);
    toolLayout->addWidget(toolbar);
    BUTTON_MIN = QSize(20, 20);
    BUTTON_MAX = QSize(80, 80);
    populateToolBar();
}


void SpriteTools::populateToolBar() {
    // used to center the toolbar buttons
    auto bdummy = new QWidget(this);
    bdummy->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // establish all toolbar buttons
    paletteButton = new QPushButton(QString("Color"), toolbar);
    paletteButton->setStatusTip(tr("Choose paint color"));
    paletteButton->setMinimumSize(BUTTON_MIN);
    paletteButton->setMaximumSize(BUTTON_MAX);
    paletteButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(paletteButton, &QPushButton::pressed, this, &SpriteTools::paletteButtonClicked);
    brushButton = new QPushButton(QString("Brush"), toolbar);
    brushButton->setStatusTip(tr("Paint pixels"));
    brushButton->setMinimumSize(BUTTON_MIN);
    brushButton->setMaximumSize(BUTTON_MAX);
    brushButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    brushButton->setAutoExclusive(true);
    brushButton->setCheckable(true);
    brushButton->setChecked(true); // start checked
    connect(brushButton, &QPushButton::pressed, this, &SpriteTools::brushButtonSelected);
    bucketButton = new QPushButton(QString("Bucket"), toolbar);
    bucketButton->setStatusTip(tr("Fill empty pixels"));
    bucketButton->setMinimumSize(BUTTON_MIN);
    bucketButton->setMaximumSize(BUTTON_MAX);
    bucketButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    bucketButton->setAutoExclusive(true);
    bucketButton->setCheckable(true);
    connect(bucketButton, &QPushButton::pressed, this, &SpriteTools::bucketButtonSelected);
    eraserButton = new QPushButton(QString("Eraser"), toolbar);
    eraserButton->setStatusTip(tr("Erase pixels"));
    eraserButton->setMinimumSize(BUTTON_MIN);
    eraserButton->setMaximumSize(BUTTON_MAX);
    eraserButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    eraserButton->setAutoExclusive(true);
    eraserButton->setCheckable(true);
    connect(eraserButton, &QPushButton::pressed, this, &SpriteTools::eraserButtonSelected);
    onionButton = new QPushButton(QString("Onion"), toolbar);
    onionButton->setStatusTip(tr("Toggle onion skin layer"));
    onionButton->setMinimumSize(BUTTON_MIN);
    onionButton->setMaximumSize(BUTTON_MAX);
    onionButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(onionButton, &QPushButton::pressed, this, &SpriteTools::onionButtonSelected);

    // add the buttons to the toolbar
    toolbar->addWidget(paletteButton);
    toolbar->addSeparator();
    toolbar->addWidget(brushButton);
    toolbar->addSeparator();
    toolbar->addWidget(bucketButton);
    toolbar->addSeparator();
    toolbar->addWidget(eraserButton);
    toolbar->addSeparator();
    toolbar->addWidget(onionButton);
    toolbar->addSeparator();
    toolbar->addWidget(bdummy);
    addShiftButtons(); // create and add the buttons to shift (move) the pixel image around
    toolbar->addWidget(bdummy);
}


void SpriteTools::addShiftButtons() {
    // establish all shift/move buttons
    QPushButton *upButton = new QPushButton(QString("Shift Up"), toolbar);
    upButton->setMinimumSize(BUTTON_MIN);
    upButton->setMaximumSize(BUTTON_MAX);
    upButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(upButton, &QPushButton::pressed, this, &SpriteTools::shiftButtonClicked);
    QPushButton *downButton = new QPushButton(QString("Shift Down"), toolbar);
    downButton->setMinimumSize(BUTTON_MIN);
    downButton->setMaximumSize(BUTTON_MAX);
    downButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(downButton, &QPushButton::pressed, this, &SpriteTools::shiftButtonClicked);
    QPushButton *rightButton = new QPushButton(QString("Shift Right"), toolbar);
    rightButton->setMinimumSize(BUTTON_MIN);
    rightButton->setMaximumSize(BUTTON_MAX);
    rightButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(rightButton, &QPushButton::pressed, this, &SpriteTools::shiftButtonClicked);
    QPushButton *leftButton = new QPushButton(QString("Shift Left"), toolbar);
    leftButton->setMinimumSize(BUTTON_MIN);
    leftButton->setMaximumSize(BUTTON_MAX);
    leftButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(leftButton, &QPushButton::pressed, this, &SpriteTools::shiftButtonClicked);

    toolbar->addWidget(upButton);
    toolbar->addWidget(downButton);
    toolbar->addWidget(rightButton);
    toolbar->addWidget(leftButton);
}


void SpriteTools::paletteButtonClicked() {
    emit paletteButtonSig(paletteButton);
}
void SpriteTools::brushButtonSelected() {
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    emit brushButtonSig();
}
void SpriteTools::bucketButtonSelected() {
    QApplication::setOverrideCursor(Qt::PointingHandCursor);
    emit bucketButtonSig();
}
void SpriteTools::eraserButtonSelected() {
    QApplication::setOverrideCursor(Qt::CrossCursor);
    emit eraserButtonSig();
}
void SpriteTools::onionButtonSelected() {
    emit onionButtonSig();
}
void SpriteTools::shiftButtonClicked() {
    QPushButton *buttonSender = qobject_cast<QPushButton *>(sender());
    emit shiftButtonSig(buttonSender->text());
}
