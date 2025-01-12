#ifndef APIRESPONSE_H
#define APIRESPONSE_H

#include <iostream>
#include <string>
#include <vector>

#include "../rapidjson/document.h"
#include "../rapidjson/rapidjson.h"
#include "../rapidjson/schema.h"

using namespace rapidjson;

namespace models
{

class Language {
public:
    std::string code;
    std::string name;
    std::string native;

    static Language from_json(const Value& value) {
        Language lang;
        if (value.HasMember("code") && value["code"].IsString()) {
            lang.code = value["code"].GetString();
        }
        if (value.HasMember("name") && value["name"].IsString()) {
            lang.name = value["name"].GetString();
        }
        if (value.HasMember("native") && value["native"].IsString()) {
            lang.native = value["native"].GetString();
        }
        return lang;
    }

    static std::vector<Language> from_json_array(const Value& value) {
        std::vector<Language> languages;
        if (value.IsArray()) {
            for (SizeType i = 0; i < value.Size(); ++i) {
                languages.push_back(Language::from_json(value[i]));
            }
        }
        return languages;
    }
};

class Location {
public:
    int geoname_id;
    std::string capital;
    std::vector<Language> languages;
    std::string country_flag;
    std::string country_flag_emoji;
    std::string country_flag_emoji_unicode;
    std::string calling_code;
    bool is_eu;

    static Location from_json(const Value& value) {
        Location loc;
        if (value.HasMember("geoname_id") && value["geoname_id"].IsInt()) {
            loc.geoname_id = value["geoname_id"].GetInt();
        }
        if (value.HasMember("capital") && value["capital"].IsString()) {
            loc.capital = value["capital"].GetString();
        }
        if (value.HasMember("languages") && value["languages"].IsArray()) {
            loc.languages = Language::from_json_array(value["languages"]);
        }
        if (value.HasMember("country_flag") && value["country_flag"].IsString()) {
            loc.country_flag = value["country_flag"].GetString();
        }
        if (value.HasMember("country_flag_emoji") && value["country_flag_emoji"].IsString()) {
            loc.country_flag_emoji = value["country_flag_emoji"].GetString();
        }
        if (value.HasMember("country_flag_emoji_unicode") && value["country_flag_emoji_unicode"].IsString()) {
            loc.country_flag_emoji_unicode = value["country_flag_emoji_unicode"].GetString();
        }
        if (value.HasMember("calling_code") && value["calling_code"].IsString()) {
            loc.calling_code = value["calling_code"].GetString();
        }
        if (value.HasMember("is_eu") && value["is_eu"].IsBool()) {
            loc.is_eu = value["is_eu"].GetBool();
        }
        return loc;
    }
};

class IpInfo {
public:
    std::string ip;
    std::string type;
    std::string continent_code;
    std::string continent_name;
    std::string country_code;
    std::string country_name;
    std::string region_code;
    std::string region_name;
    std::string city;
    std::string zip;
    double latitude;
    double longitude;
    std::string ip_routing_type;
    std::string connection_type;
    Location location;

    static IpInfo from_json(const Value& value) {
        IpInfo info;
        if (value.HasMember("ip") && value["ip"].IsString()) {
            info.ip = value["ip"].GetString();
        }
        if (value.HasMember("type") && value["type"].IsString()) {
            info.type = value["type"].GetString();
        }
        if (value.HasMember("continent_code") && value["continent_code"].IsString()) {
            info.continent_code = value["continent_code"].GetString();
        }
        if (value.HasMember("continent_name") && value["continent_name"].IsString()) {
            info.continent_name = value["continent_name"].GetString();
        }
        if (value.HasMember("country_code") && value["country_code"].IsString()) {
            info.country_code = value["country_code"].GetString();
        }
        if (value.HasMember("country_name") && value["country_name"].IsString()) {
            info.country_name = value["country_name"].GetString();
        }
        if (value.HasMember("region_code") && value["region_code"].IsString()) {
            info.region_code = value["region_code"].GetString();
        }
        if (value.HasMember("region_name") && value["region_name"].IsString()) {
            info.region_name = value["region_name"].GetString();
        }
        if (value.HasMember("city") && value["city"].IsString()) {
            info.city = value["city"].GetString();
        }
        if (value.HasMember("zip") && value["zip"].IsString()) {
            info.zip = value["zip"].GetString();
        }
        if (value.HasMember("latitude") && value["latitude"].IsNumber()) {
            info.latitude = value["latitude"].GetDouble();
        }
        if (value.HasMember("longitude") && value["longitude"].IsNumber()) {
            info.longitude = value["longitude"].GetDouble();
        }
        if (value.HasMember("ip_routing_type") && value["ip_routing_type"].IsString()) {
            info.ip_routing_type = value["ip_routing_type"].GetString();
        }
        if (value.HasMember("connection_type") && value["connection_type"].IsString()) {
            info.connection_type = value["connection_type"].GetString();
        }
        if (value.HasMember("location") && value["location"].IsObject()) {
            info.location = Location::from_json(value["location"]);
        }
        return info;
    }
};
} // namespace models

#endif // APIRESPONSE_H
