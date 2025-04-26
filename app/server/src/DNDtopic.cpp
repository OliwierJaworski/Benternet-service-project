#include "Topics.h"
namespace Benternet{

json MainTopic_info = {
    { "benternet", {
        { "service", {
            { "prefix", "dnd_session>start" },
            { "topic", "dnd_session" },
            { "session", "start" },
            { "message", "" },
            { "delim", ">" },
            { "name", ""},
            // { "id", "service_001" },
            { "status", "active" },
            { "last_heartbeat", "" }
        }},
        { "commands", {
            { "!DontUseFirstEntry!", "due to bug the first command will not send" },
            { "!about", "hello from about!" },
            { "!help", "[ CORRECT USAGE: ], topic>session>!**command** : for commands: !commands" },
            { "!time", "**FUNCTYPE**"},
            { "!play!", "**FUNCTYPE**" },
            { "!delim", ">" }, //wont be allowed yet to change delim
            { "!ActiveUsers", 0 },
            { "!Sessions", json::array({
                { {"sessionID", ""}, {"Players", json::array({""})} }
            }) },
            { "!id", "DND-lobby" },
            { "!status", "active" },
            { "!last_heartbeat", "**FUNCTYPE**" }
        }},
        { "configuration", {
            { "max_connections", 100 },
            { "timeout", 5000 },
            { "auto_restart", true },
            { "logging_enabled", true },
            { "log_level", "INFO" }
        }},
        { "monitoring", {
            { "uptime", "" },
            { "error_count", 0 },
            { "request_count", 0 },
            { "health_check_interval", 60 }
        }},
        { "dependencies", {
            { "**NOVALUE**", {
                { "topic", "" },
                { "session", "" },
                { "argument", "" },
                { "Vartype", "" }
            }}
        }}
    }}
};   

};