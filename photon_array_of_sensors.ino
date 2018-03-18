char sensorArray[] = {"0000000000"};
int inByte;
byte i=0;

void setup(){
  // read from port 1, send to port 0:  
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop(){

    if (Serial1.available()){
        inByte = Serial1.read();
        //Serial.println(i);
    
        if (inByte=='\n'){
            Serial.print(sensorArray);
            Serial.println();
            Particle.publish("array",sensorArray);
            strcpy(sensorArray,""); 
            
            i=0;
        }else{
            sensorArray[i]=inByte;
            i++;
        }
    }
}