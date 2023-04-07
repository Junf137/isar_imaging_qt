#ifndef DATABASEINTERFACE_H
#define DATABASEINTERFACE_H

#include <QWidget>
#include <QSettings>
#include <QCloseEvent>

#include <QDebug>

namespace Ui {
class DatabaseInterface;
}

class DatabaseInterface : public QWidget
{
    Q_OBJECT

public:
    explicit DatabaseInterface(QWidget *parent = nullptr);
    ~DatabaseInterface();

private:
    Ui::DatabaseInterface *ui;

    QSettings *settings;

    void closeEvent(QCloseEvent *event);
    void readSettings();
    void saveSettings();
};

#endif // DATABASEINTERFACE_H
