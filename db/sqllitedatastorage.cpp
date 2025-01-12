#include "SqlLiteDataStorage.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QDebug>

SqlLiteDataStorage::SqlLiteDataStorage() {}

void SqlLiteDataStorage::Connect()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("IPStack.db");
    if(not SqlLiteDriverAvailable())
    {
        throw std::runtime_error("Sql lite driver not available!");
    }

    if (not db.open()) {
        throw std::runtime_error("Failed to open database: " + db.lastError().text().toStdString());
    }

    QSqlError error = CreateItemsListTable();
    if (error.isValid())
    {
        throw std::runtime_error(error.text().toStdString());
    }
}

void SqlLiteDataStorage::Store(models::ItemListModel val)
{
    QSqlQuery query;
    query.prepare("INSERT INTO item_list (ip, latitude, longitude, hostname) "
                  "VALUES (:ip, :latitude, :longitude, :hostname)");

    query.bindValue(":ip", QString::fromStdString(val.ip));
    query.bindValue(":latitude", val.geolocation.latitude);
    query.bindValue(":longitude", val.geolocation.longitude);
    val.hostname.empty()
        ? query.bindValue(":hostname", QVariant()) : query.bindValue(":hostname", QString::fromStdString(val.hostname));

    if (not query.exec())
    {
        throw std::runtime_error("Error inserting data into database: " + query.lastError().text().toStdString());
    }
}

void SqlLiteDataStorage::Remove(models::ItemListModel val)
{
    QSqlQuery query;
    query.prepare("DELETE FROM item_list WHERE ip = :ip");
    query.bindValue(":ip", QString::fromStdString(val.ip));
    if (!query.exec())
    {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    if (query.numRowsAffected() == 0)
    {
        throw std::runtime_error("No record found with the specified IP.");
    }
}

std::vector<models::ItemListModel> SqlLiteDataStorage::FetchAll()
{
    std::vector<models::ItemListModel> itemList;
    QSqlQuery query;
    query.prepare("SELECT ip, latitude, longitude, hostname FROM item_list");
    if (!query.exec())
        throw std::runtime_error(query.lastError().text().toStdString());

    while (query.next()) {
        models::ItemListModel item;

        item.geolocation.latitude = query.value("latitude").toDouble();
        item.geolocation.longitude = query.value("longitude").toDouble();
        item.ip = query.value("ip").toString().toStdString();
        item.hostname = query.value("hostname").toString().toStdString();

        itemList.push_back(item);
    }

    return itemList;
}

QSqlError SqlLiteDataStorage::CreateItemsListTable()
{
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS item_list ("
               "ip TEXT PRIMARY KEY, "
               "latitude REAL, "
               "longitude REAL, "
               "hostname TEXT)");

    return query.lastError();
}

bool SqlLiteDataStorage::SqlLiteDriverAvailable()
{
    return QSqlDatabase::isDriverAvailable("QSQLITE");
}
