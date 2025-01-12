#ifndef MOCKDATASTORAGE_H
#define MOCKDATASTORAGE_H

#include <gmock/gmock.h>
#include "../../db/IDataStorage.h"

class MockDataStorage : public IDataStorage {
public:
    MOCK_METHOD(void, Connect, (), (override));
    MOCK_METHOD(void, Store, (models::ItemListModel), (override));
    MOCK_METHOD(void, Remove, (models::ItemListModel), (override));
    MOCK_METHOD(std::vector<models::ItemListModel>, FetchAll, (), (override));
};
#endif // MOCKDATASTORAGE_H
