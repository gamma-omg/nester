#include "ui_MainWindow.h"
#include "MainWindow.h"
#include <NES.h>
#include <chrono>
#include <QString>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _nes(new nescore::NES())
    , _updateTimer(new QTimer())
    , _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    connect(_updateTimer, SIGNAL(timeout()), this, SLOT(update()));
    _updateTimer->start(16);

    _nes->setRenderCallback(_ui->screen);

    setAnimated(false);
}

MainWindow::~MainWindow()
{
    delete _ui;
    delete _updateTimer;
}

void MainWindow::update()
{
    if (_nes->isRomLoaded())
    {
        _nes->update(std::chrono::seconds(1));
    }
}

void MainWindow::loadRom()
{
    QString romFileName = QFileDialog::getOpenFileName(this, tr("Open NES ROM"), "", tr("NES rom (*.nes)"));
    _nes->loadRom(romFileName.toStdString());
}
