//TeleMetria - ISE BAJA versao 0.1 Beta
//Escrito por: Gabriel Rodrigues de Azeredo
//Data: 11/10/2016

#include <SPI.h>
#include <Ethernet.h>

//Mac Address do Ethernet Shield
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
//Endereco IP
IPAddress ip(10, 0, 0, 177);

//Porta de Servico
EthernetServer server(80);

//Canal Digital Onde Sera Ligado o Rele
//Responsavel por dar partida no motor
int rele_partida = 3;

//Variaveis do Sensor de Rotacao
volatile byte rpmcount;
unsigned int rpm;
unsigned long timeold;
float velocidadeLinear;
int raio = 30;

void rpm_fun() {
  rpmcount++;
}

void setup() {
  //Define a porta do rele de partida do motor como saida
  pinMode(rele_partida, OUTPUT);
  //Define um valor inicial ao rele como 0 (valor baixo)
  digitalWrite(rele_partida, LOW);
  digitalWrite(2, HIGH);
  
  attachInterrupt(0, rpm_fun, FALLING); //FALLING, CHANGE, RISING
  rpmcount = 0;
  rpm = 0;
  timeold = 0;
  
  // Abrir Comunicacao Serial
  Serial.begin(9600);
  Serial.println("*=======================*");
  Serial.println("|-=Telemetria ISE BAJA=-|");
  Serial.println("|    -=11/10/2016=-     |");
  Serial.println("*=======================*");
  Serial.print("\n");
  
  // Iniciar Conexao Ethernet:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("Meu IP e:");
  Serial.println(Ethernet.localIP());
}
int rpm_calc(){
  if (rpmcount >= 20 ){
     rpm = 60000/(millis() - timeold) * rpmcount; //Fator de conversao de Hz para RPM
     timeold = millis();
     Serial.println(rpmcount);
     rpmcount = 0;
     Serial.println(rpm);
  }
   else{
      rpm = 0;
      //timeold = millis();
      //rpmcount = 0;
      Serial.println(rpm);
    }
  return rpm;
}

//void tacometro(){
//  if (rpmcount >= 20) {
//    rpm = 60000/(millis() - timeold) * rpmcount; //Fator de conversao de Hz para RPM
//    timeold = millis();
//    rpmcount = 0;
//    velocidadeLinear = (rpm*raio*6)/1000;
//  }
//}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    //Serial.println("novo cliente");
    //uma requisicao http termina com um /n
    boolean currentLineIsBlank = true;
    String variaveis;
    int funcao = 0;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.print(c);
        variaveis.concat(c);

        if (variaveis.endsWith("/ligarmotor")) funcao = 1;
        else if(variaveis.endsWith("/desligarmotor")) funcao = 2;
        else if(variaveis.endsWith("/json")) funcao = 3;
        
        if (c == '\n' && currentLineIsBlank) {
          if (funcao == 1){
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");  // A conexao sera fechada apos receber as requisicoes
            //client.println("Refresh: 5");  // recarrega pagina a cada 5 segundos
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("*===================*<br/ >");
            client.println("  Telemetria ISE BAJA<br/ >");
            client.println("      11/10/2016     <br/ >");
            client.println("*===================*<br/ >");
            client.println("<br/ >");
            client.print("Ligando o Motor...");
            //Durante a requisicao ele vai mudar o status do pino onde o rele esta ligado
            digitalWrite(rele_partida, HIGH);
            client.println("<br />");
            client.println("</html>");
            client.print("\n");
            break;
          }
          if (funcao == 2){
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");  // A conexao sera fechada apos receber as requisicoes
            //client.println("Refresh: 5");  // recarrega pagina a cada 5 segundos
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("*===================*<br/ >");
            client.println("  Telemetria ISE BAJA<br/ >");
            client.println("      11/10/2016     <br/ >");
            client.println("*===================*<br/ >");
            client.println("<br/ >");
            client.print("Desligando o Motor...");
            //Durante a requisicao ele vai mudar o status do pino onde o rele esta desligado
            digitalWrite(rele_partida, LOW);
            client.println("<br />");
            client.println("</html>");
            client.print("\n");
            break;
          }
          else if (funcao == 3){
            client.println("HTTP/1.1 200 OK"); 
            client.println("Content-Type: application/json;charset=utf-8"); 
            client.println("Connnection: close");
            client.println(); 
            client.println("{"); 
            //client.print("\"Meu_ip\":\"");
            //client.print(Ethernet.localIP()); 
            //client.println("\",");
            client.print("\"Analogico0\":\"");
            client.print(analogRead(A0)); 
            client.println("\",");
            client.print("\"Analogico1\":\"");
            client.print(analogRead(A1)); 
            client.println("\",");
            client.print("\"Analogico2\":\"");
            client.print(analogRead(A2)); 
            client.println("\",");
            client.print("\"Analogico3\":\"");
            client.print(analogRead(A3)); 
            client.println("\",");
            client.print("\"Analogico4\":\"");
            client.print(analogRead(A4)); 
            client.println("\",");
            client.print("\"Analogico5\":\"");
            client.print(analogRead(A5)); 
            client.println("\",");
            client.print("\"Motor_Ligado\":\"");
            client.print(digitalRead(rele_partida)); 
            client.println("\",");
            client.print("\"RPM\":\"");
            client.print(rpm_calc()); 
            client.println("\""); 
            client.println("}"); 
            client.stop(); 
          }
          else{
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");  // A conexao sera fechada apos receber as requisicoes
            client.println("Refresh: 1");  // recarrega pagina a cada 5 segundos
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("*===================*<br/ >");
            client.println("  Telemetria ISE BAJA<br/ >");
            client.println("      11/10/2016     <br/ >");
            client.println("*===================*<br/ >");
            client.println("<br/ >");
            client.print("Motor Ligado - ");
            client.println(digitalRead(rele_partida));
            client.println("<br/ >");
            client.print("RPM - ");
            client.println(rpm_calc());
            client.println("<br/ >");
            for (int canalanalogico = 0; canalanalogico < 6; canalanalogico++) {
            int leituraSensor = analogRead(canalanalogico);
            client.print("Entrada Analogica ");
            client.print(canalanalogico);
            client.print(" e ");
            client.print(leituraSensor);
            client.println("<br />");
          }
            //client.println("<br />");
            //client.print("<a href='/analog0'>Analogico 0</a><br />");
            //client.print("<a href='/analog1'>Analogico 1</a><br />");
            //client.print("<a href='/analog2'>Analogico 2</a><br />");
            //client.print("<a href='/analog3'>Analogico 3</a><br />");
            //client.print("<a href='/analog4'>Analogico 4</a><br />");
            //client.print("<a href='/analog5'>Analogico 5</a><br />");
            //client.println("</html>");
            //client.print("\n");
            break;
          }
          
        }
        if (c == '\n') {
          // comecando a proxima linha
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // pegou um caracter na linha atual
          currentLineIsBlank = false;
        }
      }
    }
    // da ao navegador o tempo para receber o arquivo
    delay(1);
    // fecha conexao:
    client.stop();
    //Serial.println("cliente disconectado");
  }
}
