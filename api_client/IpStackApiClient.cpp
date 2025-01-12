#include "IpStackApiClient.h"

#include <QHostInfo>


namespace api_client
{
IPStackApiClient::IPStackApiClient() {}

models::Geolocation IPStackApiClient::GetGeolocation(const std::string& ipOrHostname)
{
    auto resp = FetchResponse(QString::fromStdString(ipOrHostname));
    auto parsedResponse = parser.ParseServerResponse(resp);
    return parsedResponse;
}

std::string IPStackApiClient::FetchResponse(const QString &val)
{
    std::string ip;

    QHostAddress hostAddress(val);
    if (hostAddress.protocol() == QAbstractSocket::IPv4Protocol
        or hostAddress.protocol() == QAbstractSocket::IPv6Protocol)
    {
        ip = hostAddress.toString().toStdString();
    }
    else
    {
        throw std::runtime_error("Invalid IP provided to api client!");
    }

    const char* api_key = std::getenv("IPSTACK_API_KEY");
    if (!api_key) {
        throw std::runtime_error("API key not found in environment variables!");
    }

    httplib::Client cli("http://api.ipstack.com");
    std::string url = "/" + ip + "?access_key=" + std::string(api_key);
    auto res = cli.Get(url.c_str());

    if (not res) {
        auto mappedError = _httplibErrorMap.find(res.error());
        if(mappedError == _httplibErrorMap.end())
        {
            throw std::runtime_error("Request failed due to unknown reason");
        }
        throw std::runtime_error("Request failed: " + mappedError->second);
    }

    if (res->status != 200) {
        throw std::runtime_error("Invalid response from server. HTTP Status Code: " + std::to_string(res->status));
    }

    return res->body;
}

}// namespace api_client

