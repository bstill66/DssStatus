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

        bool buildFromJson(const JSon &jsonObj, DssStatus &dssStatus) {
            bool ok = false;

            if (jsonObj.contains("DssStatus")) {
                JSon dssJson = jsonObj["DssStatus"];

                ok = buildFromJson(dssJson, dssStatus);
            }

            if (ok && jsonObj.contains("SeatStatus")) {
                JSon seatInfo = jsonObj["SeatStatus"];

                ok = buildSeatsFromJson(seatInfo, dssStatus);
            }
            return ok;
        }


        bool fromJson(const JSon &dssJson, ServerStatus &status) {
            status.clear();



            status.setAvailability(ServerStatus::INT_AVL, getBoolean(dssJson, "INT_AVL"));
            status.setAvailability(ServerStatus::PR_AVL, getBoolean(dssJson, "PR_AVL"));
            status.setAvailability(ServerStatus::TV_AVL, getBoolean(dssJson, "TV_AVL"));

            status.setUpTime(dssJson["UpTime"]);
            status.setPairingCount(dssJson["PairingCount"]);

            return true;
        }

        void toJson(const ServerStatus &status, JSon &json) {
            json.clear();

            json["INT_AVL"] = status.getAvailability(ServerStatus::INT_AVL);
            json["PR_AVL"] = status.getAvailability(ServerStatus::PR_AVL);
            json["TV_AVL"] = status.getAvailability(ServerStatus::TV_AVL);
            json["UpTime"] = status.getUpTime();
            json["PairingCount"] = status.getPairingCount();
        }

        bool buildSeatsFromJson(const JSon &obj, DssStatus &results) {

            if (obj.is_array()) {
                for (auto &elem: obj) {
                    std::string id = elem["seat"].get<std::string>();
                    SeatStatus info(id);

                    info.setStatus(SeatStatus::DSS, getBoolean(elem, "DSS"));
                    info.setStatus(SeatStatus::PR_AVL, getBoolean(elem, "PR_AVL"));
                    info.setStatus(SeatStatus::TV_SVC, getBoolean(elem, "TV_SVC"));
                    info.setStatus(SeatStatus::VLS, getBoolean(elem, "VLS"));

                    info.setMode(SeatStatus::KID, getBoolean(elem, "KID"));
                    //results.push_back(info);

                }
            }
            return true;
        }

        void toJson(const SeatStatus &seat, JSon &json) {
            json.clear();

            json["seat"] = seat.getSeatId();

            json["DSS"] = seat.getStatus(SeatStatus::DSS);
            json["PR_AVL"] = seat.getStatus(SeatStatus::PR_AVL);
            json["TV_SVC"] = seat.getStatus(SeatStatus::TV_SVC);
            json["VLS"] = seat.getStatus(SeatStatus::VLS);

            json["KID"] = seat.getMode(SeatStatus::KID);
            json["UI"] = seat.getUIState();
        }

        void fromJson(const JSon &json, SeatStatus &seat) {
            std::string id = json["seat"].get<std::string>();
            SeatStatus info(id);

            info.setStatus(SeatStatus::DSS, json["DSS"]);
            info.setStatus(SeatStatus::PR_AVL, json["PR_AVL"]);
            info.setStatus(SeatStatus::TV_SVC, json["TV_SVC"]);
            info.setStatus(SeatStatus::VLS, json["VLS"]);

            info.setMode(SeatStatus::KID, json["KID"]);
            info.setUIState(static_cast<SeatStatus::UIState>(json["UI"]));

            seat = info;
        }
    }
}