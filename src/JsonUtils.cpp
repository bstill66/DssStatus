//
// Created by bstil on 7/2/2024.
//

#include <format>
#include "JsonUtils.h"


namespace DssApi {
    ParseException::ParseException(const std::string &msg)
            : std::runtime_error(msg) {}

    namespace JsonUtils {

        bool getBoolean(const JSon &jsonObj, const char *key) {
            bool result = false;

            if (jsonObj.contains(key)) {
                auto obj = jsonObj[key];

                if (obj.is_number() || obj.is_boolean()) {
                    auto valAsInt = obj.get<uint8_t>();
                    result = (valAsInt != 0) ? true : false;
                } else if (obj.is_string()) {
                    result = (obj.get<std::string>() != "0") ? true : false;
                } else {
                    auto ptr = to_string(obj);

                    std::string errStr = std::format("Invalid type for key: {}", key);
                    throw ParseException(errStr);
                }
            }

            return result;


        }


    }


}