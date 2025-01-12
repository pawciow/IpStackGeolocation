#ifndef IPSTACKAPICLIENT_H
#define IPSTACKAPICLIENT_H
#include <string>
#include <QString>

#include "../include/httplib/httplib.h"

#include "IGeolocationProvider.h"
#include "IpStackResponseParser.h"

namespace api_client
{
class IPStackApiClient : public IGeolocationProvider
{
public:
    IPStackApiClient();
    virtual models::Geolocation GetGeolocation(const std::string& ipOrHostname) override;
private:
    IPStackResponseParser parser;
    std::string FetchResponse(const QString &val);

    const std::map<httplib::Error, std::string> _httplibErrorMap = {
        {httplib::Error::Success, "Success: The request was successful."},
        {httplib::Error::Unknown, "Unknown error occurred."},
        {httplib::Error::Connection, "Connection error: Failed to connect to the server."},
        {httplib::Error::BindIPAddress, "Bind IP address error: Failed to bind to the specified IP address."},
        {httplib::Error::Read, "Read error: Failed to read the response."},
        {httplib::Error::Write, "Write error: Failed to write the request."},
        {httplib::Error::ExceedRedirectCount, "Exceeded redirect count: Too many redirects occurred."},
        {httplib::Error::Canceled, "Request canceled by the user."},
        {httplib::Error::SSLConnection, "SSL connection error: SSL handshake or connection failure."},
        {httplib::Error::SSLLoadingCerts, "SSL loading certificates error: Failed to load SSL certificates."},
        {httplib::Error::SSLServerVerification, "SSL server verification error: Failed to verify the SSL server certificate."},
        {httplib::Error::UnsupportedMultipartBoundaryChars, "Unsupported multipart boundary characters error."},
        {httplib::Error::Compression, "Compression error: Failed to compress or decompress the data."},
        {httplib::Error::ConnectionTimeout, "Connection timeout error: The connection attempt timed out."}
    };

};
} // namespace api_client

#endif // IPSTACKAPICLIENT_H
