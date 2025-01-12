#ifndef IIPRESOLVER_H
#define IIPRESOLVER_H
#include <string>

namespace api_client
{
class IIPResolver{
public:
    virtual bool IsValidIP(const std::string& val) = 0;
    virtual std::string FetchIpFromHostname(const std::string& hostname) = 0;
};
}
#endif // IIPRESOLVER_H
