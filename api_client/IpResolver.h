#ifndef IPRESOLVER_H
#define IPRESOLVER_H

#include "IIPResolver.h"
namespace api_client
{
class IPResolver : public IIPResolver
{
public:
    IPResolver();
    virtual bool IsValidIP(const std::string& val) override;
    virtual std::string FetchIpFromHostname(const std::string& hostname) override;
};
}


#endif // IPRESOLVER_H
