#pragma once

#include <mosquitto.h>

#include <functional>
#include <string>

class MqttClient {
public:
    MqttClient(const std::string& client_id,
               const std::string& host,
               int port);

    ~MqttClient();

    bool connect();
    void startLoop();
    void stopLoop();

    void subscribe(const std::string& topic);
    void publish(const std::string& topic, const std::string& payload);

    void setCommandCallback(std::function<void(const std::string&)> callback);

private:
    static void onConnect(struct mosquitto* mosq, void* userdata, int result);
    static void onMessage(struct mosquitto* mosq,
                          void* userdata,
                          const struct mosquitto_message* message);

    struct mosquitto* mosq_ = nullptr;

    std::string client_id_;
    std::string host_;
    int port_;

    std::function<void(const std::string&)> command_callback_;
};
