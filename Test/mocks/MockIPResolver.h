#ifndef MOCKIPRESOLVER_H
#define MOCKIPRESOLVER_H

#include <gmock/gmock.h>

#include <string>
#include "../../api_client/IIPResolver.h"
namespace api_client
{
class MockIpResolver : public IIPResolver {
public:

    MOCK_METHOD(bool, IsValidIP, (const std::string& val), (override));
    MOCK_METHOD(std::string, FetchIpFromHostname, (const std::string& hostname), (override));
};

} // namespace api_resolver

#endif // MOCKIPRESOLVER_H
