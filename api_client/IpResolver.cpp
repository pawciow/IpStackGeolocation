#include "IpResolver.h"

#include <QHostInfo>
namespace api_client
{

IPResolver::IPResolver() {}

bool IPResolver::IsValidIP(const std::string& val)
{
    QHostAddress hostAddress(QString::fromStdString(val));
    return (hostAddress.protocol() == QAbstractSocket::IPv4Protocol
            or hostAddress.protocol() == QAbstractSocket::IPv6Protocol);
}

std::string IPResolver::FetchIpFromHostname(const std::string &hostname)
{
    QHostInfo info = QHostInfo::fromName(QString::fromStdString(hostname));
    if (info.error() != QHostInfo::NoError)
        std::runtime_error("Invalid Hostname provided - Couldn't resolve IP!");


    for (const QHostAddress &address : info.addresses())
    {
        return address.toString().toStdString();
    }

    std::runtime_error("Invalid Hostname provided - Empty IP!");
    return {};
}
}

