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

std::string 
Topic_template::GetFromString(const std::string& key, const std::string& haystack) {
    std::vector<std::string> parts;
    std::stringstream ss(haystack);
    std::string part;
    
    while (std::getline(ss, part, '>')) {
        if(parts.size() < MaxArgs){
            std::cout << "current part left:" << part <<"\n";
            parts.push_back(part);
            
        }else{
            std::cout << "current part:" << part <<"\n";
            if(part.size() > 0){
                parts.push_back(part); 
            }else{
                throw std::runtime_error("[VARVALUE] Invalid message appended");
            }
            break;
        }
    }

    if(parts.size() < 3)
        throw std::runtime_error("[VARVALUE] Invalid message prefix");   
    

    if (key == "PREFIX") {
        std::string session = parts[MPART::SESSION];
        return parts[MPART::TOPIC] + GetDelim() + session + GetDelim() + parts[MPART::NAME];
    }

    else if (key == "SESSION") {
        std::string session = parts[MPART::SESSION];
        return session;
    }

    else if (key == "NAME") {
        return parts[MPART::NAME];
    }
    else if (key == "MESSAGE") {
        return parts[MPART::MESSAGE];
    }
    throw std::runtime_error("[PARAM] Invalid key");
}


void Topic_template::SetServiceFields(std::string& input, const char& suffix) {

    std::vector<std::string> parts;
    std::stringstream ss(input);
    std::string part;

    Set_suffix(input,suffix);
    while (std::getline(ss, part, '>')) {
        parts.push_back(part);
    }

    if (parts.size() < 3) //no prefix provided = cant recover from that
        throw std::runtime_error("[STRUCTURE] Invalid input structure");
    
    // Set prefix: topic>session
    std::string prefix = GetFromString("PREFIX", input);
    
    info["benternet"]["service"]["prefix"] = prefix;

    SetTopic(parts[MPART::TOPIC]);

    std::string session = parts[MPART::SESSION]; 
    session.pop_back();
    session += suffix;
    SetSession(session);

    SetSenderName(parts[MPART::NAME]);

    info["benternet"]["service"]["status"] = "active";

    SetLastHeartbeat();

    if(parts.size() < 4) //no prefix provided = can recover
        throw std::runtime_error("[VARVALUE] Invalid message prefix");

    SetMessage(parts[MPART::MESSAGE]);
}

void
Topic_template::Set_suffix(string& prefix_structure, const char& suffix){
    std::vector<std::string> parts;
    std::stringstream ss(prefix_structure);
    std::string part;
    while (std::getline(ss, part, '>')) {
        if(parts.size() == MPART::SESSION){
            if (part.back() == '?' || part.back() == '!') {
                part.pop_back();
                part += suffix;
            }
        }
        parts.push_back(part);
    }

    if (parts.size() < 3) 
        throw std::runtime_error("[STRUCTURE] Invalid input structure");

    prefix_structure.clear();
    prefix_structure += parts[MPART::TOPIC] + GetDelim() + 
                        parts[MPART::SESSION] + GetDelim() +
                        parts[MPART::NAME] + GetDelim();

    if (parts.size() < 4) 
        throw std::runtime_error("[VARVALUE] Invalid message prefix");

    prefix_structure += parts[MPART::MESSAGE]+ GetDelim();

    std::cout << "-------------------suffixed string result:" << prefix_structure << "\n";
}