#pragma once

#include "chat_manager.h"

using namespace Benternet;

inline Socket_t& socket(const std::string& id, zmq::socket_type type = static_cast<zmq::socket_type>(-1), std::string topic = "dnd_session") {
    return chat_manager::instance().Socket(id, type, topic);
}

inline void poll() {
    chat_manager::instance().PollEvents();
}

inline void run() {
    chat_manager::instance().executeEvents();
}