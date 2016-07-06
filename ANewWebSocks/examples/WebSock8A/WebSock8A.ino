//
//  WebSock8A.ino version 8A. July 2016. Open Source Firmware, NO LICENSE. 
//  See "NO LICENSE FOR WebSock8A.TXT". included
//  
//  Features:	A web server on port 80, serving the websocket page.
//		A web socket server on port 81, handling the traffic using tcp/ip
//		It uses a Static IP Address of 192.168.1.123 
//		A Speed tester to show how fast websocket are.
//		A LED Toggle button on pin 2. the Toggle button shows status and can be toggle by all connected devices.
//		Many devices can be connected and interact with each other.
//		A Scan for Wifi AP is integrated.
//
//		IMPORTANT --> Change the SSID  and Password before compiling.
//
#include <Hash.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

uint8_t ClientNum = 0; // used in loop()
// Useful linux commands samples.
// $ sudo picocom -b 115200 /dev/ttyUSB0
// $ sudo nmap -sP 192.168.1.0/24 | grep -B2 Espressif | grep scan

long previousMillis = 0; 
//  EspSpeed is used here to test the speed of websockets.    

// As of July 2016, using WebSocket, I measured frequencies (speed) is in excess of 475 Hertz.
const long EspSpeed =  1;  

static const char HomeRouterSSID[] = "HoneyImHome";
static const char HomeRouterPassword[] = "#$%aN!#$%45634234@#%^";

MDNSResponder mdns;
static void writeLED(bool);
// For ESP8266-01 GPIO pin 2 LED output set to LEDPIN = 2, 
const int LEDPIN = 2;
bool LEDStatus = true;
bool SPEEDENABLE = false;
	
// Prototyping
static void ToggleLED();
void Scan();
void scanHtml();
String scanUart();
void scanHtml(uint8_t num);

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("\n\n\n------------------------------\nSSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.println("MAC: " + WiFi.macAddress());
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm\n------------------------------\n");
}

void ConnectWiFi() {
  IPAddress ip(192, 168, 1, 123); // Static IP Address.
  IPAddress subnet(255, 255, 255, 0);
  IPAddress gt(192, 168, 1, 10);
  WiFi.mode(WIFI_STA);
  // Do not use "Wifi.config" is you want your IP given to you by your router DHCP.
  // Use "Wifi.config" to set a Staic IP address.
  WiFi.config(ip, gt, subnet); 
  WiFi.begin(HomeRouterSSID, HomeRouterPassword); 
  byte led_status = 0;
  digitalWrite(LEDPIN, LOW);
  // Blink LED while we wait for WiFi connection
  while ( WiFi.status() != WL_CONNECTED ) {
    digitalWrite(LEDPIN, led_status);
    led_status ^= 0x01;
    delay(50);
  }
    // Turn LED on when we are connected
  if (WiFi.status() == WL_CONNECTED)  {
	digitalWrite(LEDPIN, HIGH);
	printWifiStatus();
	// Leave LED ON long enough to show Wifi Connected.
	delay(2000); digitalWrite(LEDPIN, LOW);
  }
  if (mdns.begin("espWebSock", WiFi.localIP())) {
    Serial.println("MDNS responder started");
    mdns.addService("http", "tcp", 80);
    mdns.addService("ws", "tcp", 81);
  }
  else {
    Serial.println("MDNS.begin failed");
  }
  Serial.print("In Web Browser type,\n http://espWebSock.local or http://");
  Serial.println(WiFi.localIP());
}

ESP8266WebServer WebSvr(80);
WebSocketsServer webSocket = WebSocketsServer(81);

static const char PROGMEM INDEX[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width">
<title>ESPWebSocket7A</title>
<style>
*{margin:2px;padding:2px}
body{font-family:"Lucida Sans Unicode", "Lucida Grande", sans-serif;}
.button{background-color: #4d4dff;color: #FedFFF;padding: 10px;border-radius: 10px;margin: 6px;}
</style>
<script>
var websock;
var MyCounter = 0;
var d = new Date();
var B4Time = d.getTime();
var Avg = 0;
var AvgCount = 0;
var AvgHz = 0;
var EspSpeed = 1;
function Timex(){
        // Returns the elapsed time when it was last called. 
	d = new Date();
	var NowTime = d.getTime();
        var ElapsedTime = NowTime - B4Time
        Avg = Avg + ElapsedTime;
	AvgCount = AvgCount + 1;
	if (AvgCount == 1000) { AvgHz = (Avg / 1000); AvgHz = (1000/AvgHz); Avg = 0; AvgCount = 0;} 
	B4Time = NowTime;
	return ElapsedTime;
} 
function WebSockClientEvent() {
	var Coders = "";
	websock = new WebSocket("ws://" + window.location.hostname + ":81/");
	websock.onopen = function(evt) { console.log('websock open'); };
	websock.onclose = function(evt) { console.log('websock close'); };
	websock.onerror = function(evt) { console.log(evt); };
	websock.onmessage = function(evt) {
	console.log(evt);

	Code = (evt.data[0] +  evt.data[1] +  evt.data[2]);
	switch (Code) {  

		case "</b":
			document.getElementById("W").innerHTML = evt.data;
		break;
		case "TL0":
			var T = document.getElementById("TLX").style.color = "black";
		break;
		case "TL1":
			var T = document.getElementById("TLX").style.color = "red"; 
		break;
		case "SP0":
			document.getElementById("H").innerHTML = "";
		break;
		case "Esp":
			var Tick = Timex();
			var H = document.getElementById("H");
			H.innerHTML = "<h2>Speed: " + AvgHz.toFixed(2) + " Hz</h2>";
		break;
		default:
			console.log('Unknown Event');
			console.log(evt.data);
	}
	};
}
function TogLed(a) {websock.send(a.id);}
function GetSpeed(b) {websock.send(b.id);}
function ScanIt(c) {websock.send(c.id);}

</script>
</head>
<body onload="javascript:WebSockClientEvent();">
<h2>ESPWebSocket v. 8A</h2>
<div><button class="button" style="color:black" id="TLX" onclick="TogLed(this);"><h1> LED </h1></button></div>
</br><div><button class="button" id="SN1" onclick="ScanIt(this);"><h1>SCAN</h1></button></div>
</br><div><button class="button" id="SP0" onclick="GetSpeed(this);"><h1>SPEED</h1></button></div>
<div style="color:blue" id="H"></div>
</br><div id="W"></div>
</body>
</html>
)rawliteral";

void ESPSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  Serial.printf("\nESPSocketEvent(%d, %d, ...)\r\n", num, type);
  String Code = String((char *) &payload[0]);
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\r\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
	if (LEDStatus)  webSocket.broadcastTXT("TL0",3); 
	else webSocket.broadcastTXT("TL1",3); 	
      }
      break;
    case WStype_TEXT:
	ClientNum = num;
      	Serial.printf("[%u] get Text: %s\r\n", num, payload);

      	if (Code == "TLX"){
		ToggleLED(); 
		// broadcastTXT. Toggle the Black.Red color for all connected client webpage.
		// sendTXT. Toggle the Black.Red color for only the "num" client webpage.
		if (LEDStatus)  webSocket.broadcastTXT("TL0",3); 
		else webSocket.broadcastTXT("TL1",3);     
		// webSocket.sendTXT(num, "TL1", 3);  
	}

      	if (Code == "SP0"){
		if (SPEEDENABLE == false) SPEEDENABLE = true;
		else { 
			SPEEDENABLE = false;	
			webSocket.sendTXT(num, "SP0", 3); // clear speed div
		}
      	}

      	if (Code == "SN1"){
		//Scan(); 
		Serial.print(scanUart()); 
		scanHtml(num);
      	}
      	

      	// send data to all connected clients
      	//webSocket.broadcastTXT(payload, length);
      	break;


    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\r\n", num, length);
      hexdump(payload, length);
      webSocket.sendBIN(num, payload, length);
      break;

    default:
	Serial.printf("Invalid WStype [%d]\n", type);
	Serial.printf("Unknown command. Payload -->[%u] get Text: %s\r\n", num, payload);
	break;
  }
}



void WebMainPage(){WebSvr.send(200, "text/html", INDEX);}
//void WebMainPage(){WebSvr.send(200, "text/plain", INDEX);}
void WebHandleErrors()
{
  String msg = "File Not Found\n\n";
  msg += "URI: ";
  msg += WebSvr.uri();
  msg += "\nMethod: ";
  msg += (WebSvr.method() == HTTP_GET)?"GET":"POST";
  msg += "\nArguments: ";
  msg += WebSvr.args();
  msg += "\n";
  for (uint8_t i=0; i<WebSvr.args(); i++){msg += " " + WebSvr.argName(i) + ": " + WebSvr.arg(i) + "\n";}
  WebSvr.send(404, "text/plain", msg);
}

void setup()
{
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);
  Serial.begin(115200);
  ConnectWiFi();
  WebSvr.on("/", WebMainPage);
  WebSvr.onNotFound(WebHandleErrors);
  WebSvr.begin();
  webSocket.begin();
  webSocket.onEvent(ESPSocketEvent);
}

void loop()
{
	webSocket.loop();
	WebSvr.handleClient();
	unsigned long currentMillis = millis();
	if (SPEEDENABLE == true){
		if(currentMillis - previousMillis > EspSpeed) {
			previousMillis = currentMillis; 	    
			webSocket.sendTXT(ClientNum, "Esp",3);
			//webSocket.broadcastTXT("Esp",3); 
		}
	}
	//yield();
}


// You may have to reverse the Status for some devices.
// It is set for The ESP8266-01
// The ESP8266-12 is reverse.
static void ToggleLED(){
  if (LEDStatus) {
    digitalWrite(LEDPIN, HIGH);
    LEDStatus = false;
  }
  else {
    digitalWrite(LEDPIN, LOW);
    LEDStatus = true;
  }
}

String scanUart() {
  int numSsid = WiFi.scanNetworks();
  String s = "Scan Networks\n\n";
  if (numSsid == 0) s += " No Network Found\n"; 
  else {
	  s += numSsid; 
	  s += " Nework Found\n";
	  for (int n = 0; n<numSsid; n++) {   
		  s += n+1;     
		  s += ": ";     
		  s += WiFi.SSID(n);     
		  s += " (";     
		  s += WiFi.RSSI(n);
		  s += " dbm) "; 
		  s += (WiFi.encryptionType(n) == ENC_TYPE_NONE)?" Open":" Secure";
		  s += "\n";         
	  }
  }
  return s;
}

void scanHtml(uint8_t num) { 
	// Use "</b" as a Code name.
	String myString = "</br>";
        int numSsid = WiFi.scanNetworks();
	delay(20);
	  if (numSsid == 0){
		myString += "No networks found.";
		Serial.printf("myString: %s\n", myString.c_str());
                webSocket.sendTXT(num, myString.c_str(), myString.length());
	  }
	  else
	  {
		myString += numSsid;
		myString += " Networks found.</br></br>";
		for (int n = 0; n<numSsid; n++) {
		  	myString += n + 1;
			myString += ": ";
			myString += WiFi.SSID(n);
			myString += " (";
			myString += WiFi.RSSI(n);
 			myString += " dbm)";
			myString += (WiFi.encryptionType(n) == ENC_TYPE_NONE)?" Open":" Secure";
			myString += "</br>";
		}	
	 	Serial.printf("myString: %sn", myString.c_str());
		webSocket.sendTXT(num, myString.c_str(), myString.length());
	}
}


void Scan(){
  int n = WiFi.scanNetworks();
  Serial.println("\n\nScan Completed.");
  if (n == 0)
    Serial.println("No networks found.");
  else
  {
    Serial.print(n);
    Serial.println(" networks found.");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(" dbm)");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" Open":" Secure");
      delay(10);
    }
  }
  Serial.printf("\n\n");
}