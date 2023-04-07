#include "databaseinterface.h"
#include "ui_databaseinterface.h"

DatabaseInterface::DatabaseInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatabaseInterface)
{
    ui->setupUi(this);
    this->readSettings();

    // set table attribute
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStyleSheet("::section{Background-color:rgb(190,190,190);}");
    ui->tableWidget->setStyleSheet("selection-background-color:lightblue;");
    ui->tableWidget->setMinimumSize(1100, 200);
}

DatabaseInterface::~DatabaseInterface()
{
    delete ui;
}

void DatabaseInterface::closeEvent(QCloseEvent * event)
{
    this->saveSettings();
    event->accept();
}

void DatabaseInterface::readSettings()
{
    settings = new QSettings("options.ini", QSettings::IniFormat);
    settings->setIniCodec("UTF-8");

    settings->beginGroup("DataBase_GUI");
    const QByteArray geometry = settings->value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        // setting default size
    } else {
        restoreGeometry(geometry);
    }
    settings->endGroup();
}

void DatabaseInterface::saveSettings()
{
    settings->beginGroup("DataBase_GUI");
    settings->setValue("geometry", saveGeometry());
    settings->endGroup();

    settings->sync();
}
