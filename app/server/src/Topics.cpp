#include "Topics.h"
using namespace Benternet;

void 
BTopics::run(){
    CreateMainThread();
    BManager::instance().Run();
    BManager::instance().shutdown();
}

string 
Topic_template::GetCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

    std::tm now_tm = *std::localtime(&now_time_t);
    std::stringstream timestamp_stream;

    // Format the timestamp as "YYYY-MM-DDTHH:MM:SSZ"
    timestamp_stream << std::put_time(&now_tm, "%dT-%m-%Y%H:%M:%SZ");
    return timestamp_stream.str();
}
string 
Topic_template::GetFromString(const std::string& key, const std::string& haystack) {
    std::vector<std::string> parts;
    std::stringstream ss(haystack);
    std::string part;

    while (std::getline(ss, part, '>')) {
        parts.push_back(part);
    }

    auto trim_suffix = [](std::string& s) {
        if (!s.empty() && (s.back() == '?' || s.back() == '!')) {
            s.pop_back();
        }
    };

    if (key == "PREFIX") {
        if (parts.size() > 1) {
            std::string session = parts[1];
            trim_suffix(session);
            return (parts[0] + ">" + session );
        } else {
            throw std::runtime_error("GetFromString: not enough prefix arguments");
        }
    }
    else if (key == "SESSION") {
        if (parts.size() > 1) {
            std::string session = parts[1];
            trim_suffix(session);
            std::cout << "Session: " << session << std::endl;
            return session;
        } else {
            throw std::runtime_error("GetFromString: not enough prefix arguments");
        }
    }
    else if (key == "TOPIC") {
        if (!parts.empty()) {
            return parts[0];
        } else {
            throw std::runtime_error("GetFromString: not enough prefix arguments");
        }
    }
    else if (key == "MESSAGE") {
        if (parts.size() > 2) {
            return parts[2];
        } else {
            throw std::runtime_error("GetFromString: Invalid key or insufficient parts");
        }
    }
    else {
        throw std::runtime_error("GetFromString: Invalid key or insufficient parts");
    }
}


void Topic_template::SetServiceFields(std::string input) {

    std::vector<std::string> parts;
    std::stringstream ss(input);
    std::string part;
    while (std::getline(ss, part, '>')) {
        parts.push_back(part);
    }

    // Check the number of parts in input and set the fields accordingly
    
    // Set the "prefix" field: Uses topic and session if available
    if (parts.size() >= 2) {
        std::string prefix = GetFromString("PREFIX", parts[0] + ">" + parts[1]);
        info["benternet"]["service"]["prefix"] = prefix;
    } else {
        throw std::runtime_error("SetServiceFields: Invalid service prefix");
    }

    // Set the "topic" field: Use the first part of input or default if not present
    if (parts.size() > 0) {
        SetTopic(parts[0]);  // Setting topic from input
    } else {
        throw std::runtime_error("SetServiceFields: Invalid service prefix");
    }

    // Set the "session" field: Use the second part of input or default if not present
    if (parts.size() > 1) {
        SetSession(parts[1]);  // Setting session from input
    } else {
        throw std::runtime_error("SetServiceFields: Invalid service prefix");
    }

    // Set the "message" field: Use the third part of input or default if not present
    if (parts.size() > 2) {
        SetMessage(parts[2]);  // Setting message from input
    } else {
        throw std::runtime_error("GetFromString: Invalid key or insufficient parts");
    }

    // Set the "status" field: Defaults to "**NOVALUE**" if missing
    if (info["benternet"]["service"].contains("status")) {
        SetServiceStatus(info["benternet"]["service"]["status"].get<std::string>());
    } else {
        info["benternet"]["service"]["status"] = "**active**";
    }

    // Set the "last_heartbeat" field: Updates the timestamp if not found
    if (info["benternet"]["service"].contains("last_heartbeat")) {
        SetLastHeartbeat();
    } else {
        info["benternet"]["service"]["last_heartbeat"] = "**NOVALUE**";
        SetLastHeartbeat();
    }
}