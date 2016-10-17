int rpmcount = 0;
float rpm = 0;
float tempo = 0;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(13, OUTPUT);
}
void loop() {
  int sensorVal = digitalRead(2);
  if (!sensorVal) {
    rpmcount++;
    if (rpmcount >= 5) {
      rpm = 60000 / (millis() - tempo) * 5;
      Serial.println(millis() - tempo);
      Serial.println(rpm);
      rpmcount = 0;
      tempo = millis();
      rpm = 0;
    }
  }
  else {Serial.println(rpm);}
  delay(500); 
}

