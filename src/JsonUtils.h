//
// Created by bstil on 7/2/2024.
//

#ifndef DSS_IF_JSONUTILS_H
#define DSS_IF_JSONUTILS_H

#include "DssCmn.h"
#include "DssStatus.h"

#include "nlohmann/json.hpp"

namespace DssApi {

    namespace JsonUtils {


        typedef nlohmann::json JSon;

        //bool buildFromJson(const JSon &obj, DssStatus &status);
        //bool toJson(const DssStatus& dss,JSon& obj);

        //bool fromJson(const JSon &obj, ServerStatus &status);
        //void toJson(const ServerStatus& server,JSon& json);
        bool buildSeatsFromJson(const JSon &obj, DssStatus& status);

        //void fromJson(const JSon& json,SeatStatus& seat);
        //void toJson(const SeatStatus& seat,JSon& json);
        bool getBoolean(const JSon &jsonObj, const char *key);



        template <class T>
        std::string toJsonString(const T& v) {
            JSon  obj;
            to_json(obj,v);
            return to_string(obj);
        }

        template <class T>
        void fromJson(const std::string& str,T& tmp) {
            JSon obj = JSon::parse(str);

            fromJson(obj,tmp);

        }
    }
}

#endif //DSS_IF_JSONUTILS_H
