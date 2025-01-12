#ifndef IPSTACKRESPONSEPARSER_H
#define IPSTACKRESPONSEPARSER_H

#include "../models/ApiResponse.h"
#include "../models/Geolocation.h"

#include <memory>

namespace api_client
{
class IPStackResponseParser
{
public:
    IPStackResponseParser();
    models::Geolocation ParseServerResponse(std::string& response);
private:
    void ValidateSchema(const Document& doc);

    std::unique_ptr<SchemaDocument> _schemaDoc;
    std::unique_ptr<SchemaValidator> _validator;
    std::string _schema_path{"./models/schema.json"};
};

}

#endif // IPSTACKRESPONSEPARSER_H
