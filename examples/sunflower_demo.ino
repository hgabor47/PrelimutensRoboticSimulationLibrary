#include <blenderstruct.h>
#include <PreliSimRobotic.h>    
#include <Arduino.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>

ESP8266WebServer httpServer(81);
ESP8266HTTPUpdateServer httpUpdater;
ESP8266WebServer BlenderServer(82);
PreliSimRobotic BlenderService;
    
#define STASSID0 "hgplsoft"
#define STAPSK0  "7102237203210"
#define LIMIT 5.0
    
const char* otssid0 = STASSID0;
const char* otpassword0 = STAPSK0;

BlenderStore * blenderinput;
StaticJsonDocument<200> doc;
bool SIMULATION=false;
int i=0;
double r=0;

void BlenderAnswerInit(){
    blenderinput->save( "magx" ,(double)0);
    blenderinput->save( "magy",(double)0);
    blenderinput->save( "magz",(double)0);
    blenderinput->save("senBL",(uint8_t)0);
    blenderinput->save("senBR",(uint8_t)0);
    blenderinput->save("IsenFL",(uint16_t)0);
    blenderinput->save("Ihead",(double)0);
    //webprint(scyinfo.blenderinput->listAll());
}

String BlenderAnswer(uint8_t type,ESP8266WebServer * server){
  
  if (type==0) { //param 
    BlenderAnswerInit();    
    SIMULATION = true;  //this will switch on the SIMULATION but at now fixed ON or OFF
    return "{ \"OmotL\" : \"float\",\"OmotR\" : \"float\",\"Ihead\":\"float\" }";
  }
  if (type==1) {     //value
    
    for(int i=0; i<server->args(); i++){
      String name = server->argName(i);
      String value=server->arg(i);      
      
      blenderinput->save(  name.c_str(),value.c_str());      
    }


    double f = blenderinput->doubleload("Ihead");
    Serial.print("Ihead");
    Serial.println(f);

    doc.clear();
    doc["OmotL"] =0; 
    doc["OmotR"] =0; 
    if (f>LIMIT){
      doc["OmotL"] =1; 
    }
    if (f<-LIMIT){
      doc["OmotR"] =1; 
    }

    String s;
    serializeJson(doc, s);
    return s;
    //return "{ \"V1\" : [\"1.56\",\"4.67\",\"9.84\"],\"Power\":\"6.6\", \"motL\":\"0.5\", \"motR\" : \"0.3\" }";
  }
}


void setup() {
      blenderinput = new BlenderStore();
      WiFi.persistent(false);
      WiFi.mode(WIFI_STA);
      WiFi.enableInsecureWEP();
      WiFi.begin(otssid0, otpassword0);
      
      httpUpdater.setup(&httpServer);
      httpServer.begin();
      BlenderService.setup(&BlenderServer);
      BlenderServer.begin();
      BlenderService.setAnswerFunction(BlenderAnswer);

      Serial.begin(9600);
}

void loop() {
    httpServer.handleClient();
    BlenderServer.handleClient();
}
