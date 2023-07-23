#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
/* Thiết lập Wifi AP mode
const char* apSSID = "ESP32-AP";
const char* apPassword = "";*/
// Thiết lập Wifi Station mode
const char* ssid = "Tang 1";
const char* pass = "0372727797";

#define LED 2

WebServer server(80);
//=========Biến chứa mã HTML========//
const char Mainpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <title>Digital Control</title>
    <style> body {
        text-align: center;
        }

    h1 {
        color: #fc068d;
    }

    a {
        text-decoration: none; color: #b3e464;
    }
    .bt_on {
        height: 50px;
        width: 100px;
        margin: 15px 0;
        background-color: #df4d4d; border-radius: 20px;
    }
    .bt_off {
    height: 50px;
    width: 100px;
    margin: 15px 0; background-color: #20db7e;
    border-radius: 20px;
    }
    </style>
<meta charset="UTF-8">
</head>
<body>
    <h1>ESP32 Web Server - AJAX</h1>
    <h1>QThanh</h1>
    <div>LED Status: <b><pan id="trangthai_LED"><pan></b></div>
    <div>
        <button class="bt_on" onclick="getdata('onLED')">ON</button>
        <button class="bt_off" onclick="getdata('offLED')">OFF</button>
    </div>
    <div id="reponsetext"></div>
    <script>
        //--------Tạo đối tượng request-----------
        function create_obj() {
            td = navigator.appName;
            if (td == "Microsoft Internet Explorer") {
                obj = new ActiveXObject("Microsoft.XMLHTTP");
            } else {
                obj = new XMLHttpRequest();
            }
            return obj;
        }
        var xhttp = create_obj();
        //-----Thiết Lập dữ liệu và gửi request------
        function getdata(url) {
            xhttp.open("GET", "/" + url, true);
            xhttp.onreadystatechange = process; //nhận reponse
            xhttp.send();
        }
        //-----Kiếm tra response-----
        function process() {
            if (xhttp.readyState = 4 && xhttp.status == 200) {
        //–––––-Updat data sử dụng javascript-
        ketqua = xhttp.responseText;
        document.getElementById("reponsetext").innerHTML = ketqua;
            }
        }
        //-----Kiểm tra trạng thái chân LED------
        function getstatusD1D2() {
            xhttp.open("GET", "getstatus_LED", true);
            xhttp.onreadystatechange = process_json;
            xhttp.send();
        }
        //--Kiểm tra response-
        function process_json() {
        if (xhttp.readyState == 4 && xhttp.status == 200) {
        //------Update data sử dụng javascript
        var trangthaiD1D2_JSON = xhttp.responseText;
        var Obj = JSON.parse(trangthaiD1D2_JSON);
        document.getElementById("trangthai_LED").innerHTML = Obj.LED;
        }
    }
    //---Ham update du lieu tu dong---
    setInterval(function() {
        getstatusD1D2();
    }, 1000);
    </script>
</body>
</html>
)=====";
// Chương trình con
void mainpage() {
  String s = FPSTR(Mainpage);
  server.send(200, "application/json",s);
}
void onLED() {
  digitalWrite(LED, 1);
  server.send(200,"text/html","Bật Led");
}
void offLED() {
  digitalWrite(LED, 0);
  server.send(200,"text/html","Tắt Led");
}
void getstatus_LED() {
  String Led;
  if(digitalRead(LED)==1){
    Led = "ON";
    }else{
    Led = "OFF";
    }
  String s ="{\"LED\": \""+ Led + "\"}";
  server.send(200, "application/json",s);
}
void setup() {
  pinMode(LED, 0);
  WiFi.begin(ssid,pass);
  Serial.begin(115200);
  Serial.print("Đang kết nối...");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("...");
  }
  Serial.println(WiFi.localIP());

  server.on("/", mainpage);
  server.on("onLED", onLED);
  server.on("offLED", offLED);
  server.on("/getstatus_LED", getstatus_LED);
}

void loop() {
  server.handleClient();
}