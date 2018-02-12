
#ifndef WEBRTC_EXAMPLES_PEERCONNECTION_CLIENT_DEFAULTS_H_
#define WEBRTC_EXAMPLES_PEERCONNECTION_CLIENT_DEFAULTS_H_
#pragma once

#include <string>

#include <string>

extern const char kAudioLabel[];
extern const char kVideoLabel[];
extern const char kStreamLabel[];
extern const uint16_t kDefaultServerPort;

std::string GetEnvVarOrDefault(const char *env_var_name,
                               const char *default_value);
std::string GetPeerConnectionString();
std::string GetDefaultServerName();
std::string GetPeerName();

#endif // WEBRTC_EXAMPLES_PEERCONNECTION_CLIENT_DEFAULTS_H_
