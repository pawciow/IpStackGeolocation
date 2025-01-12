#ifndef IGEOLOCATIONPROVIDER_H
#define IGEOLOCATIONPROVIDER_H

#include <string>

#include "../models/Geolocation.h"

namespace api_client
{
class IGeolocationProvider
{
public:
    virtual models::Geolocation GetGeolocation(const std::string& ipOrHostname) = 0;
};
} // namespace api_client

#endif // IGEOLOCATIONPROVIDER_H
