#ifndef ITEMLISTMODEL_H
#define ITEMLISTMODEL_H

#include "Geolocation.h"
#include <QVariant>

namespace models
{
class ItemListModel
{
public:
    Geolocation geolocation;
    std::string ip;
    std::string hostname;
};
} // namespace models


Q_DECLARE_METATYPE(models::ItemListModel)

#endif // ITEMLISTMODEL_H
