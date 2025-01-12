#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "models/ItemListModel.h"
#include <QString>
#include <QVariant>
#include <QtCore>
#include <QtGui>

#include "api_client/IGeolocationProvider.h"
#include "api_client/IIPResolver.h"
#include "db/IDataStorage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class Tests;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void InjectGeolocationProvider(std::shared_ptr<api_client::IGeolocationProvider> provider);
    void InjectIpResolver(std::shared_ptr<api_client::IIPResolver> resolver);
    void InjectAndSetupDB(std::shared_ptr<IDataStorage> provider);

public:
    Ui::MainWindow *ui;
private:
    std::shared_ptr<api_client::IGeolocationProvider> _geolocationProvider;
    std::shared_ptr<api_client::IIPResolver> _ipResolver;
    std::shared_ptr<IDataStorage> _dbProvider;

    void addItemToListWidget(const models::ItemListModel& item);
    QString formatItemDisplayText(const models::ItemListModel& item) const;
    bool isItemAlreadyInListWidget(const models::ItemListModel& newItem);
signals:
    void addLocationMarking(QVariant, QVariant, QVariant, QVariant);
    void removeLocationMarking(QVariant, QVariant, QVariant);
    void changeMapPosition(QVariant, QVariant);

private slots:
    void on_pushButton_clicked();
    void on_removeButton_clicked();
    void on_viewOnMapButton_clicked();
};
#endif // MAINWINDOW_H
