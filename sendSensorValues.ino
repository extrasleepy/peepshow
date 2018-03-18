
bool sensors[]={0,0,0,0,0,0,0,0,0,0} ;


void setup() {
  // start serial port at 9600 bps:
  Serial.begin(9600);

  for (int i=0;i<sizeof(sensors);i++){
    sensors[i]=random(1);
  }

}

void loop() {
  for (int i=0;i<sizeof(sensors);i++){
    sensors[i]=random(2);
    Serial.print(sensors[i]);
  }
    Serial.println();
    delay(5000);
}


