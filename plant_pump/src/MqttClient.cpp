#include "MqttClient.hpp"

#include <iostream>

MqttClient::MqttClient(const std::string& client_id,
                       const std::string& host,
                       int port)
    : client_id_(client_id),
      host_(host),
      port_(port) {
    mosquitto_lib_init();

    mosq_ = mosquitto_new(client_id_.c_str(), true, this);

    if (!mosq_) {
        std::cerr << "Failed to create Mosquitto client" << std::endl;
        return;
    }

    mosquitto_connect_callback_set(mosq_, MqttClient::onConnect);
    mosquitto_message_callback_set(mosq_, MqttClient::onMessage);
}

MqttClient::~MqttClient() {
    if (mosq_) {
        mosquitto_loop_stop(mosq_, true);
        mosquitto_destroy(mosq_);
    }

    mosquitto_lib_cleanup();
}

bool MqttClient::connect() {
    if (!mosq_) {
        return false;
    }

    int rc = mosquitto_connect(mosq_, host_.c_str(), port_, 60);

    if (rc != MOSQ_ERR_SUCCESS) {
        std::cerr << "Could not connect to broker: "
                  << mosquitto_strerror(rc)
                  << std::endl;
        return false;
    }

    return true;
}

void MqttClient::startLoop() {
    mosquitto_loop_start(mosq_);
}

void MqttClient::stopLoop() {
    mosquitto_loop_stop(mosq_, true);
}

void MqttClient::subscribe(const std::string& topic) {
    mosquitto_subscribe(mosq_, nullptr, topic.c_str(), 0);
}

void MqttClient::publish(const std::string& topic, const std::string& payload) {
    mosquitto_publish(
        mosq_,
        nullptr,
        topic.c_str(),
        static_cast<int>(payload.size()),
        payload.c_str(),
        0,
        false
    );
}

void MqttClient::setCommandCallback(
    std::function<void(const std::string&)> callback) {
    command_callback_ = callback;
}

void MqttClient::onConnect(struct mosquitto*,
                           void*,
                           int result) {
    if (result == 0) {
        std::cout << "Connected to MQTT broker" << std::endl;
    } else {
        std::cerr << "Failed to connect. Code: " << result << std::endl;
    }
}

void MqttClient::onMessage(struct mosquitto*,
                           void* userdata,
                           const struct mosquitto_message* message) {
    auto* client = static_cast<MqttClient*>(userdata);

    if (!client || message->payloadlen == 0) {
        return;
    }

    std::string topic = message->topic;
    std::string payload(
        static_cast<char*>(message->payload),
        message->payloadlen
    );

    std::cout << "Received message on " << topic << ": " << payload << std::endl;

    if (topic == "plant/commands" && client->command_callback_) {
        client->command_callback_(payload);
    }
}
