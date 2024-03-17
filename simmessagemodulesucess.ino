#include <SoftwareSerial.h>
SoftwareSerial sim(10, 11);
int _timeout;
String _buffer;
String number = "+918114681984"; //-> change with your number 
String ipAddress = "103.4.220.252";
String SMS = "103.4.220.252";  
void setup() { 
  Serial.begin(9600);
  _buffer.reserve(50);
  Serial.println("System Started...");
  sim.begin(9600);
  delay(5000);
  sim.println("AT+CREG?");
  delay(2000);
  sim.println("AT+CSTT=\"portalnmms\"");
  delay(2000);
  sim.println("AT+CGDCONT=1,\"IP\",\"portalnmms\"");
  delay(2000);
  sendATCommand("AT+CIICR", 5000);
  delay(2000);
  // Retrieve IP address
  String ipAdd = sendATCommand("AT+CIFSR", 1000); 
  //delay(7000); //delay for 7 seconds to make sure the modules get the signal
  if (ipAdd.indexOf("ERROR") != -1 || ipAdd == "") {
    // Substring found
    Serial.println(ipAddress);
    SendMessage(); 
} else {
    // Substring not found
    Serial.print("IP Address: ");
    Serial.println(ipAdd);
    ipAddress = ipAdd;
    SendMessage();
}  
  Serial.println("Type s to send an SMS, r to receive an SMS, and c to make a call"); 
}

String sendATCommand(String command, int timeout) {
  String response = "";
  sim.println(command);
  long int time = millis();
  while ((time + timeout) > millis()) {
    while (sim.available()) {
      char c = sim.read();
      response += c;
    }
  }
  return response;
}
void loop() { 
  if (Serial.available() > 0)
    switch (Serial.read())
    {
      case 's':
        SendMessage(); 
        break;
      case 'r':
        RecieveMessage();
        break;
      case 'c':
        callNumber();
        break;
    }
  if (sim.available() > 0)
    Serial.write(sim.read());
} 
void SendMessage()
{
  //Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);
  // Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  String SMS = "Accident Detected!! - Please Help!! \n LOCATION: https://www.google.com/maps/place/Chennai+Institute+of+Technology/@12.9716113,80.0404974,17z/data=!3m1!4b1!4m6!3m5!1s0x3a52f4d07355bab5:0xbb6063169c4ed4d9!8m2!3d12.9716113!4d80.0430723!16s%2Fm%2F0ztgzpg?entry=ttu \n LIVE VIDEO: https://drive.google.com/drive/folders/1t6pqgR12_CcHkbjSkjC3fVLrRj57cgNc?usp=sharing \n PERSON: Kushal   \n"+ipAddress;
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  _buffer = _readSerial();
}
void RecieveMessage()
{
  Serial.println ("SIM800L Read an SMS");
  sim.println("AT+CMGF=1");
  delay (200);
  sim.println("AT+CNMI=1,2,0,0,0"); // AT Command to receive a live SMS
  delay(200);
  Serial.write ("Unread Message done");
}
String _readSerial() {
  _timeout = 0;
  while  (!sim.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
}
void callNumber() {
  sim.print (F("ATD"));
  sim.print (number);
  sim.print (F(";\r\n"));
  _buffer = _readSerial();
  Serial.println(_buffer);
}