#include <mosquitto.h>

#include <cstring>
#include <iostream>

static void on_connect(struct mosquitto *mosq, void *userdata, int rc)
{
    (void)userdata;

    if(rc != 0) {
        std::cerr << "Connect failed: " << mosquitto_strerror(rc) << "\n";
        return;
    }

    std::cout << "Connected\n";
    mosquitto_subscribe(mosq, nullptr, "home/+/telemetry", 0);
}

static void on_message(struct mosquitto *mosq, void *userdata, const mosquitto_message *msg)
{
    (void)mosq;
    (void)userdata;

    std::cout << "Topic: " << msg->topic << "\n";
    std::cout << "Payload: ";
    std::cout.write(static_cast<const char *>(msg->payload), msg->payloadlen);
    std::cout << "\n";
}

int main()
{
    mosquitto_lib_init();

    mosquitto *mosq = mosquitto_new("cpp-client", true, nullptr);
    if(!mosq) {
        std::cerr << "Failed to create client\n";
        mosquitto_lib_cleanup();
        return 1;
    }

    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);

    int rc = mosquitto_connect(mosq, "localhost", 1883, 60);
    if(rc != MOSQ_ERR_SUCCESS) {
        std::cerr << "Connect error: " << mosquitto_strerror(rc) << "\n";
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return 1;
    }

    rc = mosquitto_loop_forever(mosq, -1, 1);

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return rc == MOSQ_ERR_SUCCESS ? 0 : 1;
}
