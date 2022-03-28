// Based on the Paho C code example from www.eclipse.org/paho/ #include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include "MQTTClient.h"
#define CPU_TEMP "/sys/class/thermal/thermal_zone0/temp" using namespace std;
//Please replace the following address with the address of your server
#define ADDRESS #define CLIENTID #define AUTHMETHOD #define AUTHTOKEN #define TOPIC #define QOS #define TIMEOUT
"tcp://192.168.1.19:1883" "rpi1"
"molloyd"
"password" "ee513/CPUTemp"
1 10000L
fstream fs;
fs.open(CPU_TEMP, fstream::in); // read from the file fs >> cpuTemp;
fs.close();
return (((float)cpuTemp)/1000);
}
int main(int argc, char* argv[]) {
char str_payload[100];
MQTTClient client;
MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer; MQTTClient_message pubmsg = MQTTClient_message_initializer; MQTTClient_deliveryToken token;
MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL); opts.keepAliveInterval = 20;
opts.cleansession = 1;
opts.username = AUTHMETHOD;
opts.password = AUTHTOKEN;
int rc;
if ((rc = MQTTClient_connect(client, &opts)) != MQTTCLIENT_SUCCESS) {
cout << "Failed to connect, return code " << rc << endl;
return -1; }
sprintf(str_payload, "{\"d\":{\"CPUTemp\": %f }}", getCPUTemperature()); pubmsg.payload = str_payload;
pubmsg.payloadlen = strlen(str_payload);
pubmsg.qos = QOS;
pubmsg.retained = 0;
MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
cout
<< "Waiting for up to " << (int)(TIMEOUT/1000) <<
" seconds for publication of " << str_payload <<
" \non topic " << TOPIC << " for ClientID: " << CLIENTID << endl; MQTTClient_waitForCompletion(client, token, TIMEOUT);
<< "Message with token " << (int)token << " delivered." << endl;
rc =
cout
MQTTClient_disconnect(client, 10000); MQTTClient_destroy(&client);
return rc;
}
