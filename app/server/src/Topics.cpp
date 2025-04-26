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
    
    //topic>session>name>message 3xdelim
    while (std::getline(ss, part, '>')) {
        if(parts.size() < MaxArgs){
            parts.push_back(part);
        }else{
            if(ss.rdbuf()->in_avail() > 0){
                std::string remainder(ss.str(),ss.tellg());
                parts.push_back(move(remainder)); //should have auto move constructor but just to be sure
            }else{
                throw std::runtime_error("[VARVALUE] Invalid message appended");
            }
            break;
        }
    }

    auto trim_suffix = [](std::string& s) {
        if (!s.empty() && (s.back() == '?' || s.back() == '!')) {
            s.pop_back();
        }
    };

    size_t psize{parts.size()};

    if (key == "PREFIX") {
        if ( psize > 1 ) {
            std::string session = parts[MPART::SESSION];
            trim_suffix(session);
            return parts[MPART::TOPIC] + ">" + session + ">" + parts[MPART::NAME];
        } else {
            throw std::runtime_error("[VARVALUE] Invalid message prefix");    
        }
    }
    else if (key == "SESSION") {
        if ( psize > 1 ) {
            std::string session = parts[MPART::SESSION];
            trim_suffix(session);
            return session;
        } else {
            throw std::runtime_error("[VARVALUE] Invalid message prefix");    
        }
    }
    else if (key == "NAME") {
        if ( psize > 2 ) {
            return parts[MPART::NAME];
        } else {
            throw std::runtime_error("[VARVALUE] Invalid message appended");
        }
    }
    else if (key == "MESSAGE") {
        if ( psize > 3 ) {
            return parts[MPART::MESSAGE];
        } else {
            throw std::runtime_error("[VARVALUE] Invalid message appended");
        }
    }
    throw std::runtime_error("[PARAM] Invalid key");
}


void Topic_template::SetServiceFields(std::string input, const char& suffix) {
    std::vector<std::string> parts;
    std::stringstream ss(input);
    std::string part;
    while (std::getline(ss, part, '>')) {
        parts.push_back(part);
    }

    if (parts.size() < 4) {
        throw std::runtime_error("[STRUCTURE] Invalid input structure");
    }

    // Set prefix: topic>session
    std::string prefix = GetFromString("PREFIX", parts[MPART::TOPIC] + ">" + parts[MPART::SESSION]);
    Set_suffix(prefix,suffix);
    info["benternet"]["service"]["prefix"] = prefix;

    SetTopic(parts[MPART::TOPIC]);

    std::string session = parts[MPART::SESSION] + suffix;
    SetSession(session);

    SetSenderName(parts[MPART::NAME]);
    SetMessage(parts[MPART::MESSAGE]);
   
    info["benternet"]["service"]["status"] = "active";

    SetLastHeartbeat();
}

void
Topic_template::Set_suffix(string& prefix_structure, const char& suffix){
    std::vector<std::string> parts;
    std::stringstream ss(prefix_structure);
    std::string part;
    while (std::getline(ss, part, '>')) {
        if(parts.size() +1> MPART::SESSION){
            part += suffix;
            parts.push_back(part);
        }
    }
    prefix_structure.clear();
    prefix_structure = parts[MPART::TOPIC] + GetDelim() + 
                       parts[MPART::SESSION] + GetDelim() +
                       parts[MPART::NAME] + GetDelim() +
                       parts[MPART::MESSAGE];
}