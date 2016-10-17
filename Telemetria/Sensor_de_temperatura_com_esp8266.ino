#include <SoftwareSerial.h>

//RX pino 2, TX pino 3
SoftwareSerial esp8266(2, 3);

#define DEBUG true

const int LM35 = A0; // Define o pino que lera a saída do LM35
float temperatura; // Variável que armazenará a temperatura medida

void setup()
{
  Serial.begin(9600);
  esp8266.begin(19200);

  sendData("AT+RST\r\n", 2000, DEBUG); // rst
  sendData("AT+CWLAP\r\n", 1000, DEBUG);
  // Conecta a rede wireless
  sendData("AT+CWJAP=\"El Lord del Universo\",\"12345678\"\r\n", 2000, DEBUG);
  delay(3000);
  
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG);
  // Mostra o endereco IP
  sendData("AT+CIFSR\r\n", 1000, DEBUG);
  // Configura para multiplas conexoes
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG);
  // Inicia o web server na porta 80
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG);
}

void loop()
{
  // Verifica se o ESP8266 esta enviando dados
  if (esp8266.available())
  {
    if (esp8266.find("+IPD,"))
    {
      delay(300);
      int connectionId = esp8266.read() - 48;

      String webpage = "<head><meta http-equiv=""refresh"" content=""3"">";
      webpage += "</head><h1><u>Sensor de temperatura</u></h1><h2>";
      webpage += "Temperatura: ";
      temperatura = (float(analogRead(LM35))*5/(1023))/0.01;
      Serial.print("Temperatura LM35: ");
      Serial.println(temperatura);
      int a = temperatura;
      webpage += a;
      webpage += "ºC";

      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += webpage.length();
      cipSend += "\r\n";

      sendData(cipSend, 1000, DEBUG);
      sendData(webpage, 1000, DEBUG);

      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId; // append connection id
      closeCommand += "\r\n";

      sendData(closeCommand, 3000, DEBUG);
    }
  }
}

String sendData(String command, const int timeout, boolean debug)
{
  // Envio dos comandos AT para o modulo
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
