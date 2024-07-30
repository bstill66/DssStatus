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

        bool toJson(const DssStatus::SeatCollection &c,JSon& obj) {
            for (auto s: c) {
                to_json(obj,s);
            }
            return true;
        }

        bool toJson(const DssStatus& s,JSon& obj) {
            // Server Status
            JSon  srvr;
            to_json(srvr,s.getServerStatus());
            obj["Server"] = srvr;

            JSon  seats;
            toJson(s.getSeatStatus(),seats);
            obj["Seats"] = seats;

            return true;
        }


        bool buildFromJson(const JSon &jsonObj, DssStatus &dssStatus) {
            bool ok = false;

            if (jsonObj.contains("Server")) {
                JSon dssJson = jsonObj["Server"];
                ServerStatus tmp;
                from_json(dssJson, tmp);
                dssStatus.set(tmp);
            }

            if (ok && jsonObj.contains("Seats")) {
                JSon seatInfo = jsonObj["Seats"];

                ok = buildSeatsFromJson(seatInfo, dssStatus);
            }
            return ok;
        }


        bool fromJson(const JSon &dssJson, ServerStatus &status) {

            from_json(dssJson,status);
            return true;

            status.clear();

            status.setAvailability(ServerStatus::INT_AVL, getBoolean(dssJson, "INT_AVL"));
            status.setAvailability(ServerStatus::PR_AVL, getBoolean(dssJson, "PR_AVL"));
            status.setAvailability(ServerStatus::TV_AVL, getBoolean(dssJson, "TV_AVL"));
            status.setAvailability(ServerStatus::FLT_OPN, getBoolean(dssJson,"FLT_OPN"));
            status.setAvailability(ServerStatus::LPNS_AVL, getBoolean(dssJson,"LPNS_AVL"));
            status.setUpTime(dssJson["UpTime"]);

            return true;
        }

        void toJson(const ServerStatus &status, JSon &json) {

            json  = status;
            return;
            json.clear();

            json["INT_AVL"] = status.getAvailability(ServerStatus::INT_AVL);
            json["PR_AVL"] = status.getAvailability(ServerStatus::PR_AVL);
            json["TV_AVL"] = status.getAvailability(ServerStatus::TV_AVL);
            json["LPNS_AVL"] = status.getAvailability(ServerStatus::LPNS_AVL);
            json["FLT_OPN"]  = status.getAvailability(ServerStatus::FLT_OPN);
            json["UpTime"] = status.getUpTime();
        }

        bool buildSeatsFromJson(const JSon &obj, DssStatus &results) {

            if (obj.is_array()) {
                for (auto &elem: obj) {
                    std::string id = elem["seat"].get<std::string>();
                    SeatStatus info(id);

                    info.setStatus(SeatStatus::DSS_COMM_LOSS, getBoolean(elem, "DSS_COMM_LOSS"));
                    info.setStatus(SeatStatus::TM_SYNC, getBoolean(elem, "TM_SYNC"));
                    info.setStatus(SeatStatus::TV_SVC_AVL, getBoolean(elem, "TV_SVC_AVL"));
                    info.setStatus(SeatStatus::VLS, getBoolean(elem, "VLS"));
                    info.setStatus(SeatStatus::PA,getBoolean(elem,"PA"));
                    info.setStatus(SeatStatus::PCTL_LOCK,getBoolean(elem,"PCTL_LOCK"));

                    info.setMode(SeatStatus::KID, getBoolean(elem, "KID"));
                    //results.push_back(info);

                }
            }
            return true;
        }

        void toJson(const SeatStatus &seat, JSon &json) {
            to_json(json,seat);
            return;
            json.clear();

            json["SeatId"] = seat.getSeatId();

            json["DSS_COMM_LOSS"] = seat.getStatus(SeatStatus::DSS_COMM_LOSS);
            json["TM_SYNC"]       = seat.getStatus(SeatStatus::TM_SYNC);
            json["TV_SVC_AVL"] = seat.getStatus(SeatStatus::TV_SVC_AVL);
            json["VLS"] = seat.getStatus(SeatStatus::VLS);
            json["PA"]  = seat.getStatus(SeatStatus::PA);
            json["PCTL_LOCK"] = seat.getStatus(SeatStatus::PCTL_LOCK);

            json["KID"] = seat.getMode(SeatStatus::KID);
            json["LOGIN_AVL"] = seat.getMode(SeatStatus::LOGIN_AVL);
            json["LOGD_IN"] = seat.getMode(SeatStatus::LOGD_IN);
            json["UI"] = seat.getUIState();
        }

        void fromJson(const JSon &json, SeatStatus &seat) {

            from_json(json,seat);
            return;
            std::string id = json["SeatId"].get<std::string>();
            SeatStatus info(id);

            info.setStatus(SeatStatus::DSS_COMM_LOSS, json["DSS_COMM_LOSS"]);
            info.setStatus(SeatStatus::TM_SYNC,json["TM_SYNC"]);
            info.setStatus(SeatStatus::TV_SVC_AVL, json["TV_SVC_AVL"]);
            info.setStatus(SeatStatus::VLS, json["VLS"]);
            info.setStatus(SeatStatus::PA,json["PA"]);
            info.setStatus(SeatStatus::PCTL_LOCK,json["PCTL_LOCK"]);

            info.setMode(SeatStatus::LOGIN_AVL,json["LOGIN_AVL"]);
            info.setMode(SeatStatus::LOGD_IN,json["LOGD_IN"]);
            info.setMode(SeatStatus::KID, json["KID"]);
            info.setUIState(static_cast<SeatStatus::UIState>(json["UI"]));

            seat = info;
        }
    }



}