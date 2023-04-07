#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include <QtCharts>

#include "databaseinterface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace QtCharts;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DatabaseInterface *dataBaseItfWidget;

    QSettings *settings;

    void init();
    void closeEvent(QCloseEvent *event);
    void readSettings();
    void saveSettings();

    void tabViewMode();

    void updateMenusStatus();
    bool activeMdiSubWindow();
    void subWindowMode();
    void switchLayoutDirection();
    void aboutApplication();
    void aboutOrganization();

    void btnStartClicked();


private slots:
};
#endif // MAINWINDOW_H
