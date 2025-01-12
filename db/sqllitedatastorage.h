#ifndef SQLLITEDATASTORAGE_H
#define SQLLITEDATASTORAGE_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include "IDataStorage.h"

class SqlLiteDataStorage : public IDataStorage
{
public:
    SqlLiteDataStorage();
    virtual void Connect() override;
    virtual void Store(models::ItemListModel val) override;
    virtual void Remove(models::ItemListModel val) override;
    virtual std::vector<models::ItemListModel> FetchAll() override;

private:
    bool SqlLiteDriverAvailable();
    QSqlError CreateItemsListTable();
    QSqlDatabase db;
};

#endif // SQLLITEDATASTORAGE_H
