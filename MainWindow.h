#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <memory>

namespace Ui
{
    class MainWindow;
}

namespace nescore
{
    class NES;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void update();
    void loadRom();

private:
    std::shared_ptr<nescore::NES> _nes;
    QTimer* _updateTimer;

    Ui::MainWindow *_ui;
};

#endif // MAINWINDOW_H
