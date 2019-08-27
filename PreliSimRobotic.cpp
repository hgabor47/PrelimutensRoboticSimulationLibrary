#include <Arduino.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
//#include "StreamString.h"
#include "PreliSimRobotic.h"
#include "blenderstruct.h"


PreliSimRobotic::PreliSimRobotic()
{
  _server = NULL;
  _username = emptyString;
  _password = emptyString;
  _authenticated = false;
  _answer=NULL;
  blenderinput = new BlenderStore();
  PSIMULATION=false;
}

void PreliSimRobotic::setup(ESP8266WebServer *server, const String& path, const String& username, const String& password)
{
    _server = server;
    _username = username;
    _password = password;
    String s1=path+"params";
    String s2=path+"values";

    // handler for the /update form page
    _server->on(s1.c_str(), HTTP_GET, [&](){
      if(_username != emptyString && _password != emptyString && !_server->authenticate(_username.c_str(), _password.c_str()))
        return _server->requestAuthentication();
      String json;
      if (_answer!=NULL){        
        json = _answer(0,_server);
      } else {
        json = "No answer function implemented!";
      }
      _server->send(200, "text/html", json);
    });
    _server->on(s2.c_str(), HTTP_GET, [&](){
      if(_username != emptyString && _password != emptyString && !_server->authenticate(_username.c_str(), _password.c_str()))
        return _server->requestAuthentication();
      String json;
      if (_answer!=NULL){
        json = _answer(1,_server);
      } else {
        json = "No answer function implemented!";
      }
      _server->send(200, "text/html", json);
    });
}

/*
String BlenderAnswer(uint8_t type,ESP8266WebServer * server){
  if (type==0) { //param 
    
    blenderinput->save( "head",(double)0);
    blenderinput->save("senFL",(uint16_t)0);
    blenderinput->save("senFR",(uint16_t)0);

    PSIMULATION = true;  //this will switch on the SIMULATION but at now fixed ON or OFF
    return "{ \"OmotL\" : \"float\",\"OmotR\" : \"float\",\"Ihead\" : \"float\",\"IsenFL\":\"float\", \"IsenFR\":\"float\" }";
  }
  if (type==1) {     //value    
    for(int i=0; i<server->args(); i++){
      String name = server->argName(i).substring(1);
      String value=server->arg(i);      

      blenderinput->save(  name.c_str(),value.c_str());            
    }
    doc.clear();
    doc["OmotL"] =0.4;
    doc["OmotR"] =0.4;
    
    String s;
    serializeJson(doc, s);
    return s;
    //return "{ \"V1\" : [\"1.56\",\"4.67\",\"9.84\"],\"Power\":\"6.6\", \"motL\":\"0.5\", \"motR\" : \"0.3\" }";
  }
}
*/
