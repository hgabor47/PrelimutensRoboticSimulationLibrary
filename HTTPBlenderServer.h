#ifndef __HTTP_BLENDER_SERVER_H
#define __HTTP_BLENDER_SERVER_H
#include <Arduino.h>
class ESP8266WebServer;
typedef std::function< String(uint8_t,ESP8266WebServer *)> 	TBLENDERFunction;
class HTTPBlenderServer
{
  public:
    HTTPBlenderServer(bool serial_debug=false);

    void setup(ESP8266WebServer *server)
    {
      setup(server, emptyString, emptyString);
    }

    void setup(ESP8266WebServer *server, const String& path)
    {
      setup(server, path, emptyString, emptyString);
    }

    void setup(ESP8266WebServer *server, const String& username, const String& password)
    {
      setup(server, "/", username, password);
    }

    void setup(ESP8266WebServer *server, const String& path, const String& username, const String& password);

    void updateCredentials(const String& username, const String& password)
    {
      _username = username;
      _password = password;
    }

    void setAnswerFunction(TBLENDERFunction answer){
      _answer = answer;
    }

  protected:
    void _setUpdaterError();

  private:
    bool _serial_output;
    ESP8266WebServer *_server;
    String _username;
    String _password;
    bool _authenticated;
    String _updaterError;
    TBLENDERFunction _answer;
};


#endif
