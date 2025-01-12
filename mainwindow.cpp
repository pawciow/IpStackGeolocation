#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMainWindow>
#include <QtGui>
#include <QQuickItem>
#include <QQmlComponent>
#include <QMessageBox>

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mapWidget->setSource(QUrl(QStringLiteral("qrc:/QmlMap.qml")));
    ui->mapWidget->show();

    auto MapObject = ui->mapWidget->rootObject();
    connect(this, SIGNAL(addLocationMarking(QVariant,QVariant, QVariant, QVariant)),
            MapObject, SLOT(addLocationMarking(QVariant,QVariant, QVariant, QVariant)));

    connect(this, SIGNAL(removeLocationMarking(QVariant,QVariant, QVariant)),
            MapObject, SLOT(removeLocationMarking(QVariant,QVariant, QVariant)));

    connect(this, SIGNAL(changeMapPosition(QVariant, QVariant)),
            MapObject, SLOT(changeMapPosition(QVariant, QVariant)));
}

void MainWindow::addItemToListWidget(const models::ItemListModel& item)
{
    QString displayText = formatItemDisplayText(item);
    QListWidgetItem *listItem = new QListWidgetItem(displayText, ui->listWidget);

    listItem->setData(Qt::UserRole, QVariant::fromValue(item));
    ui->listWidget->addItem(listItem);
}

QString MainWindow::MainWindow::formatItemDisplayText(const models::ItemListModel &item) const
{
    std::string text = item.hostname.empty() ? item.ip : item.hostname;
    text += " (" + std::to_string(item.geolocation.latitude) + ","
            + std::to_string(item.geolocation.longitude) + ")";
    return QString::fromStdString(text);
}

bool MainWindow::isItemAlreadyInListWidget(const models::ItemListModel &newItem)
{
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem* item = ui->listWidget->item(i);
        QVariant itemData = item->data(Qt::UserRole);

        if (itemData.canConvert<models::ItemListModel>()) {
            models::ItemListModel existingItem = itemData.value<models::ItemListModel>();
            if (existingItem.ip == newItem.ip) {
                return true;
            }
        }
    }
    return false;
}

void MainWindow::InjectGeolocationProvider(std::shared_ptr<api_client::IGeolocationProvider> provider)
{
    this->_geolocationProvider = std::move(provider);
}

void MainWindow::InjectIpResolver(std::shared_ptr<api_client::IIPResolver> resolver)
{
    this->_ipResolver = std::move(resolver);
}

void MainWindow::InjectAndSetupDB(std::shared_ptr<IDataStorage> provider)
{
    try
    {
        _dbProvider = std::move(provider);
        _dbProvider->Connect();
        auto savedIPs = _dbProvider->FetchAll();
        for(const auto& widget: savedIPs)
        {
            addItemToListWidget(widget);
            emit addLocationMarking(widget.geolocation.latitude, widget.geolocation.longitude, QString::fromStdString(widget.ip), QString::fromStdString(widget.hostname));
        }
    }

    catch(std::exception& ex)
    {
        QMessageBox::critical(nullptr, "Database Error", QString::fromStdString(ex.what()));
        QApplication::quit();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(not _geolocationProvider or not _ipResolver or not _dbProvider)
    {
        QMessageBox::critical(this, "Error", "Internal application error.");
        QApplication::quit();
        return;
    }

    auto inuptBoxText = ui->inputTextEdit->toPlainText().toStdString();
    if(inuptBoxText.empty())
    {
        QMessageBox::critical(this, "Error", "Please write hostname or IP to add.");
        return;
    }

    models::ItemListModel itemToAdd;

    try
    {
        if(not _ipResolver->IsValidIP(inuptBoxText))
        {
            itemToAdd.hostname = inuptBoxText;
            itemToAdd.ip = _ipResolver->FetchIpFromHostname(itemToAdd.hostname);
        }
        else
        {
            itemToAdd.ip = inuptBoxText;
        }

        itemToAdd.geolocation  = _geolocationProvider->GetGeolocation(itemToAdd.ip);
        if(this->isItemAlreadyInListWidget(itemToAdd))
        {
            QMessageBox::critical(this, "Error", "IP already added");
            return;
        }

        _dbProvider->Store(itemToAdd);
        this->addItemToListWidget(itemToAdd);
        emit addLocationMarking(
            itemToAdd.geolocation.latitude,
            itemToAdd.geolocation.longitude,
            QString::fromStdString(itemToAdd.ip),
            QString::fromStdString(itemToAdd.hostname));
    }
    catch(std::exception& ex)
    {
        QMessageBox::critical(this, "Error", ex.what());
    }
}


void MainWindow::on_removeButton_clicked()
{
    if(not _dbProvider)
    {
        QMessageBox::critical(this, "Error", "Internal application error.");
        QApplication::quit();
        return;
    }

    QModelIndexList selectedIndexes = ui->listWidget->selectionModel()->selectedIndexes();
    if(selectedIndexes.isEmpty())
    {
        QMessageBox::critical(this, "Error", "Please select an item from the list to remove.");
        return;
    }

    for (const QModelIndex &index : selectedIndexes)
    {
        QVariant itemData = index.data(Qt::UserRole);
        if(not itemData.canConvert<models::ItemListModel>())
        {
            QMessageBox::critical(this, "Error", "Internal error - couldn't get the data from the selected item.");
        }

        models::ItemListModel selectedItem = itemData.value<models::ItemListModel>();
        try
        {
            _dbProvider->Remove(selectedItem);
        }
        catch(std::exception& ex)
        {
            QMessageBox::critical(this, "Error", ex.what());
        }

        QAbstractItemModel *model = ui->listWidget->model();
        if (not model)
        {
            QMessageBox::critical(this, "Error", "Internal error - couldn't get the model from the selected item.");
        }

        model->removeRow(index.row());
        emit removeLocationMarking(selectedItem.geolocation.latitude,
                                   selectedItem.geolocation.longitude,
                                   QString::fromStdString(selectedItem.ip));
    }
}


void MainWindow::on_viewOnMapButton_clicked()
{
    QModelIndexList selectedIndexes = ui->listWidget->selectionModel()->selectedIndexes();
    if(selectedIndexes.isEmpty())
    {
        QMessageBox::critical(this, "Error", "Please select an item from the list to check on map.");
        return;
    }

    for (const QModelIndex &index : selectedIndexes)
    {
        QVariant itemData = index.data(Qt::UserRole);
        if(not itemData.canConvert<models::ItemListModel>())
        {
            QMessageBox::critical(this, "Error", "Internal error - couldn't get the data from the selected item.");
        }

        models::ItemListModel selectedItem = itemData.value<models::ItemListModel>();

        emit changeMapPosition(selectedItem.geolocation.latitude,
                                   selectedItem.geolocation.longitude);
    }
}

