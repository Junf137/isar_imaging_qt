#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Header.cuh"

#include <QDebug>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    /// Initiation //////////////////////////////
    ui->setupUi(this);
    dataBaseItfWidget = new DatabaseInterface;
    this->readSettings();

    this->init();
}

MainWindow::~MainWindow()
{
    delete dataBaseItfWidget;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    int exitOrNot = QMessageBox::question(this, tr("Warning"),
        QString(tr("Do you really want to Exit?")),
        QMessageBox::Yes | QMessageBox::No);

    if (exitOrNot == QMessageBox::No) {
        event->ignore();
    }else if (exitOrNot == QMessageBox::Yes) {
        if(dataBaseItfWidget->isVisible()){
            dataBaseItfWidget->close();
        }
        this->saveSettings();
        event->accept();
    }
}

void MainWindow::readSettings()
{
    settings = new QSettings("options.ini", QSettings::IniFormat);
    settings->setIniCodec("UTF-8");

    settings->beginGroup("MainWindow_GUI");
    const QByteArray geometry = settings->value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
    settings->endGroup();
}

void MainWindow::saveSettings()
{
    settings->beginGroup("MainWindow_GUI");
    settings->setValue("geometry", saveGeometry());
    settings->endGroup();

    settings->sync();
}

void MainWindow::init()
{
    /// Set Up Connection //////////////////////////////
    connect(ui->actionExit, &QAction::triggered, this, &QApplication::closeAllWindows);

    connect(ui->actionNext_Window, &QAction::triggered, ui->mdiArea, &QMdiArea::activateNextSubWindow);
    connect(ui->actionLast_Window, &QAction::triggered, ui->mdiArea, &QMdiArea::activatePreviousSubWindow);

    QActionGroup *grp = new QActionGroup(this);
    grp->setExclusive(true);
    grp->addAction(ui->actionTabView_Mode);
    grp->addAction(ui->actionSubWindow_Mode);
    connect(ui->actionTabView_Mode, &QAction::triggered, this, &MainWindow::tabViewMode);
    connect(ui->actionSubWindow_Mode, &QAction::triggered, this, &MainWindow::subWindowMode);

    connect(ui->actionTile_SubWindow, &QAction::triggered, ui->mdiArea, &QMdiArea::tileSubWindows);
    connect(ui->actionCascade_SubWindow, &QAction::triggered, ui->mdiArea, &QMdiArea::cascadeSubWindows);
    connect(ui->actionSwitch_Layout, &QAction::triggered, this, &MainWindow::switchLayoutDirection);

    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::aboutApplication);
    connect(ui->actionAbout_SYSU, &QAction::triggered, this, &MainWindow::aboutOrganization);

    connect(ui->btnOpenDatabase, &QPushButton::clicked, this->dataBaseItfWidget, &QWidget::show);
    connect(ui->qwtSlider_1, &QwtSlider::valueChanged, this,
            [=]{ui->editClutterRejection->setText(QString::number(ui->qwtSlider_1->value()));});
    connect(ui->qwtSlider_2, &QwtSlider::valueChanged, this,
            [=]{ui->editImageEnhance->setText(QString::number(ui->qwtSlider_2->value()));});

    connect(ui->mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::updateMenusStatus);

    connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::btnStartClicked);

    /// Set Value //////////////////////////////
    ui->qwtSlider_1->setGroove(true);
    ui->qwtSlider_1->setTrough(false);
    ui->qwtSlider_1->setScale(0.0, 0.1);
    ui->qwtSlider_1->setTotalSteps(1);
    ui->qwtSlider_1->setScaleStepSize(0.1);
    ui->qwtSlider_1->setValue(0);

    ui->qwtSlider_2->setGroove(true);
    ui->qwtSlider_2->setTrough(false);
    ui->qwtSlider_2->setScale(0, 2);
    ui->qwtSlider_2->setTotalSteps(2);
    ui->qwtSlider_2->setScaleStepSize(1);
    ui->qwtSlider_2->setValue(1);

    ui->qwtSlider_3->setOrientation(Qt::Horizontal);

    /// Setup Plot Region //////////////////////////////
    QLineSeries *series = new QLineSeries();

    for(int i=0; i<2000; i++){
        series->append(i*0.1, qSin(i*0.1));
    }

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Line Chart Example");

    ui->chartView_1->setChart(chart);
    ui->chartView_1->setRenderHint(QPainter::Antialiasing);
    ui->chartView_1->setRubberBand(QChartView::RectangleRubberBand);
}

void MainWindow::updateMenusStatus()
{
    bool hasMdiSubWindow = activeMdiSubWindow();

    bool isSubWindowViewMode = true;
    if (ui->actionTabView_Mode->isChecked())
    {
        isSubWindowViewMode = false;
    }

    ui->actionTile_SubWindow->setEnabled(hasMdiSubWindow && isSubWindowViewMode);
    ui->actionCascade_SubWindow->setEnabled(hasMdiSubWindow && isSubWindowViewMode);
    ui->actionNext_Window->setEnabled(hasMdiSubWindow);
    ui->actionLast_Window->setEnabled(hasMdiSubWindow);
}

bool MainWindow::activeMdiSubWindow()
{
    if (ui->mdiArea->activeSubWindow())
        return true;
    return false;
}

void MainWindow::tabViewMode()
{
    ui->mdiArea->setViewMode(QMdiArea::TabbedView);
    ui->mdiArea->setTabsClosable(false);

    ui->actionTile_SubWindow->setEnabled(false);
    ui->actionCascade_SubWindow->setEnabled(false);

    qDebug() << ui->chartView_1->geometry().width();
    qDebug() << ui->chartView_1->geometry().height();
}

void MainWindow::subWindowMode()
{
    ui->mdiArea->setViewMode(QMdiArea::SubWindowView);

    bool hasMdiSubWindow = activeMdiSubWindow();
    ui->actionTile_SubWindow->setEnabled(true && hasMdiSubWindow);
    ui->actionCascade_SubWindow->setEnabled(true && hasMdiSubWindow);
    ui->mdiArea->cascadeSubWindows();
}

void MainWindow::switchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        QGuiApplication::setLayoutDirection(Qt::RightToLeft);
    else
        QGuiApplication::setLayoutDirection(Qt::LeftToRight);
}

void MainWindow::aboutApplication()
{
   QMessageBox::about(this, tr("About ") + QCoreApplication::applicationName(),
                      tr("Radar DispControl<br>") +
                      tr("Display target trajectory information in spatial-domain Plot.<br>") +
                      tr("Display recieved signal in time-domain Plot.<br>"));
}

void MainWindow::aboutOrganization()
{
   QMessageBox::about(this, tr("About ") + QCoreApplication::organizationName(),
                      tr("SCHOOL OF ELECTRONICS AND COMMUNICATION ENGINEERING(SECE).<br>") +
                      tr("    Description of SECE.<br>") +
                      tr("    <a href=\"https://sece.sysu.edu.cn/\">Click</a> to see more information.<br><br>") +
                      tr("SUN YAT-SEN UNIVERSITY(SYSU).<br>") +
                      tr("    <a href=\"https://www.sysu.edu.cn/index.htm\">Click</a> to see more information.<br>"));
}


void MainWindow::btnStartClicked()
{
//    cuda test
    const unsigned int arraySize = 5;
    const int a[arraySize] = { 10, 20, 30, 40, 50 };
    const int b[arraySize] = { 10, 20, 30, 40, 50 };
    int c[arraySize] = { 0 };
    arrayAdd(c, a, b, arraySize);
    qDebug() << QString("%1, %2, %3, %4, %5").arg(c[0]).arg(c[1]).arg(c[2]).arg(c[3]).arg(c[4]);
}
