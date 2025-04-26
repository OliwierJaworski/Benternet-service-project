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

std::string Topic_template::GetFromString(const std::string& key, const std::string& haystack) {
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
            return parts[0] + ">" + session + "!" + ">" + parts[2];
        } else {
            throw std::runtime_error("GetFromString: not enough prefix arguments");
        }
    }
    else if (key == "SESSION") {
        if (parts.size() > 1) {
            std::string session = parts[1];
            trim_suffix(session);
            return session;
        } else {
            throw std::runtime_error("GetFromString: not enough session parts");
        }
    }
    else if (key == "NAME") {
        if (parts.size() > 2) {
            return parts[2];
        } else {
            throw std::runtime_error("GetFromString: NAME not found in message");
        }
    }
    else if (key == "MESSAGE") {
        if (parts.size() > 3) {
            return parts[3];
        } else {
            throw std::runtime_error("GetFromString: MESSAGE not found in message");
        }
    }
    throw std::runtime_error("GetFromString: Invalid key");
}


void Topic_template::SetServiceFields(std::string input) {
    std::vector<std::string> parts;
    std::stringstream ss(input);
    std::string part;
    while (std::getline(ss, part, '>')) {
        parts.push_back(part);
    }

    if (parts.size() < 4) {
        throw std::runtime_error("SetServiceFields: Invalid input structure (need topic>session?>name>message)");
    }

    // Set prefix: topic>session
    std::string prefix = GetFromString("PREFIX", parts[0] + ">" + parts[1]);
    info["benternet"]["service"]["prefix"] = prefix;

    // Topic
    SetTopic(parts[0]);

    // Session: session?>name
    std::string session = parts[1] + ">" + parts[2];
    SetSession(session);

    // Name (user sending the message)
    SetSenderName(parts[3]);

    // Message
    if (parts.size() > 4) {
        SetMessage(parts[4]);
    } else {
        info["benternet"]["service"]["message"] = "**NOVALUE**";
    }

    // Service status
    if (info["benternet"]["service"].contains("status")) {
        SetServiceStatus(info["benternet"]["service"]["status"].get<std::string>());
    } else {
        info["benternet"]["service"]["status"] = "**active**";
    }

    // Heartbeat
    if (info["benternet"]["service"].contains("last_heartbeat")) {
        SetLastHeartbeat();
    } else {
        info["benternet"]["service"]["last_heartbeat"] = "**NOVALUE**";
        SetLastHeartbeat();
    }
}