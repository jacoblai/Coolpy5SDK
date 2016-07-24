#include <SPI.h>
#include <Ethernet.h>
//https://github.com/knolleary/pubsubclient
//v1.9.1
#include <PubSubClient.h>

byte mac[6];
//服务器域名或IP地址
char server[] = "192.168.1.105";
//服务器端口号
int port = 1883;
//UserKey用户密钥(必改项)
char ukey[] = "51dd40af-135f-4733-89ff-a07615282812";
//Hub ID(必改项)
char hub[]="5";
//Node ID(必改项)
char node[]="5";
//byte server[] = { 192, 168, 1, 179 };

String instr;
String outstr;

//led light
int led = 7;

void callback(char* topic, byte* payload, unsigned int length) {
  //display all message by serial port
  Serial.print("topic:");
  Serial.println(topic);
  instr = String();
  for(int i=0;i<length;i++){
    char c = payload[i];
    instr += c;
  }
  //显示所有数据
  Serial.println(instr);
  
        //根据内容判断LED的开或关
        if(instr.startsWith("{\"HubId\":5,\"NodeId\":5,\"Svalue\":2}")){
          digitalWrite(led, HIGH);
        }else if(instr.startsWith("{\"HubId\":5,\"NodeId\":5,\"Svalue\":1}")){
          digitalWrite(led, LOW);  
        }
        //end
}

EthernetClient ethClient;
PubSubClient client(server, port, callback, ethClient);

void setup()
{
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
  
  Serial.begin(9600);
  mac[0]=random(1, 254);
  mac[1]=random(1, 254);
  mac[2]=random(1, 254);
  mac[3]=random(1, 254);
  mac[4]=random(1, 254);
  mac[5]=random(1, 254);
  if (Ethernet.begin(mac) == 0) {
    for(;;)
      ;
  }
  
  // give the Ethernet shield a second to initialize:
  delay(1000);
  String mqttid = String("arduino:");
  mqttid += random(1,99999);
  char charBuf[mqttid.length()+1];
  mqttid.toCharArray(charBuf, mqttid.length()+1);
  if (client.connect(charBuf)) {
    //client.publish("outTopic","hello world");
    outstr = String(ukey);
    outstr += ":";
    outstr += hub;
    outstr += ":";
    outstr += node;
    char charBuf1[outstr.length()+1];
    outstr.toCharArray(charBuf1, outstr.length()+1);
    client.subscribe(charBuf1);
    
    Serial.println("connect ok");
  }
}

void loop()
{
  client.loop();
}
