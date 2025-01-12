#ifndef IDATASTORAGE_H
#define IDATASTORAGE_H

#include <vector>

#include "../models/ItemListModel.h"

class IDataStorage
{
public:
    virtual void Connect() = 0;
    virtual void Store(models::ItemListModel) = 0;
    virtual void Remove(models::ItemListModel) = 0;
    virtual std::vector<models::ItemListModel> FetchAll() = 0;
};

#endif // IDATASTORAGE_H
