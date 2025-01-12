#include "IpStackResponseParser.h"

#include <fstream>
#include <sstream>

#include "../models/ApiResponse.h"
#include "../rapidjson/schema.h"

namespace
{
std::string load_json_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return "";
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}
} // namespace


namespace api_client
{

IPStackResponseParser::IPStackResponseParser()
{
    Document schema_document;
    std::string schema_json = load_json_file(_schema_path);
    if(schema_document.Parse(schema_json.c_str()).HasParseError()){
        throw std::runtime_error("Invalid schema file");
    }

    _schemaDoc = std::make_unique<SchemaDocument>(schema_document);
    _validator = std::make_unique<SchemaValidator>(*_schemaDoc);
}

models::Geolocation IPStackResponseParser::ParseServerResponse(std::string &response)
{
    Document d;
    if (d.Parse(response.c_str()).HasParseError()) {
        std::string errorMessage = "Invalid response from server: " + std::string{GetParseError_En(d.GetParseError())};
        throw std::runtime_error(errorMessage);
    }

    ValidateSchema(d);
    auto parsedMessage = models::IpInfo::from_json(d);
    return {parsedMessage.latitude, parsedMessage.longitude};
}

void IPStackResponseParser::ValidateSchema(const Document& doc)
{
    if(not _validator)
    {
        throw std::runtime_error("Invalid schema validator");
    }

    if (not doc.Accept(*_validator)) {
        std::ostringstream errorMessage;

        rapidjson::StringBuffer sb;
        _validator->GetInvalidSchemaPointer().StringifyUriFragment(sb);

        errorMessage << "Invalid schema: " << sb.GetString();
        errorMessage << "\nInvalid keyword: " << _validator->GetInvalidSchemaKeyword();

        sb.Clear();
        _validator->GetInvalidDocumentPointer().StringifyUriFragment(sb);
        errorMessage << "\nInvalid document: " << sb.GetString();
    }
}

} // namespace api_client
