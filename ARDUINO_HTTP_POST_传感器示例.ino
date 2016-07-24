/*
  Power By Coolpy
  Site:http://icoolpy.com
 
  This example code is in the public domain.
 */

#include <SPI.h>
#include <Ethernet.h>

byte mac[6];
//服务器域名或IP地址
char server[] = "192.168.1.105";
//服务器端口号
int port = 6543;
//UserKey用户密钥(必改项)
char ukey[] = "51dd40af-135f-4733-89ff-a07615282812";
//Hub ID(必改项)
char hub[]="5";
//Node ID(必改项)
char node[]="8";

EthernetClient client;

void setup() {
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
  
  char buffer[25];
  //修改此处代码修改发送的内容
  //{\"TimeStamp\":\"2012-03-15T16:13:14\",\"Value\":200}
  String json = "";
  //post has key data point
  //json += String("{\"TimeStamp\":\"2012-03-15T16:13:14\",\"Value\":") + dtostrf(12.5, 5, 2, buffer) + String("}");
  //post no key data point
  json += String("{\"Value\":") + dtostrf(16.4, 5, 2, buffer) + String("}");
  Post(json);
}

String inStr="";
boolean isOnData=false;

void loop()
{
  if (client.available()) {
    char c = client.read();
    if(c == '{'){
     inStr = "";
     isOnData = true;
     inStr += c;
    }else if(c == '}'){
      isOnData = false;
      inStr += c;
      
      //显示请求返回内容
      Serial.println("");
      Serial.println(inStr); 
      
    }else if(isOnData){
      inStr += c;
    }
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while(true);
  }
}

void Post(String PostData)
{
// if you get a connection, report back via serial:
  if (client.connect(server, port)) {
    // Make a HTTP request:
    client.print("POST /api/hub/");
    client.print(hub);
    client.print("/node/");
    client.print(node);
    client.println("/datapoints HTTP/1.1");
    client.print("Host:");
    client.println(server);
    client.println("Content-Type:application/json");
    client.print("U-ApiKey:");
    client.println(ukey);
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(PostData.length()+1);
    client.println();
    client.println(PostData);
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}
