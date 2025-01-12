#ifndef MOCKGEOLOCATIONPROVIDER_H
#define MOCKGEOLOCATIONPROVIDER_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../api_client/IGeolocationProvider.h"
namespace api_client {
class MockGeolocationProvider : public IGeolocationProvider {
public:
    MOCK_METHOD(models::Geolocation, GetGeolocation, (const std::string& ipOrHostname), (override));
};
} // namespace api_client
#endif // MOCKGEOLOCATIONPROVIDER_H
