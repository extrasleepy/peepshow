// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

//test for internet connection
SYSTEM_MODE(SEMI_AUTOMATIC)
SYSTEM_THREAD(ENABLED)

const uint32_t msRetryDelay =   5*60000; // retry every 5min
const uint32_t msRetryTime  =   20000; // stop trying after 20sec

bool retryRunning = false;
Timer retryTimer(msRetryDelay, retryConnect);  // timer to retry connecting
Timer stopTimer(msRetryTime, stopConnect);     // timer to stop a long running try

//variables for eeprom
int address = 10;
int value;
int storedValue;
bool samePerson = FALSE;

void setup() {
  //EEPROM.put(address, 0); //reset value back to 0
  Serial.begin(115200);
  pinMode(D7,OUTPUT);
  
  Particle.connect();  //check for wifi
  if (!waitFor(Particle.connected, msRetryTime))
  WiFi.off();                // no luck, no need for WiFi
  
  EEPROM.get(address, storedValue);
  value = storedValue;
  //Particle.variable("value", value);

  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }

  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));
}


void loop() {
    
   if (!retryRunning && !Particle.connected())
  { // if we have not already scheduled a retry and are not connected
    Serial.println("schedule");
    stopTimer.start();         // set timeout for auto-retry by system
    retryRunning = true;
    retryTimer.start();        // schedula a retry
  }
   
  VL53L0X_RangingMeasurementData_t measure;

  Serial.print("Reading a measurement... ");
  //lox.rangingTest(&measure, true); // pass in 'true' to get debug data printout!
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
    samePerson = FALSE;
  }
  
  if (measure.RangeMilliMeter>40 && measure.RangeMilliMeter<1100) { 
    digitalWrite(D7,HIGH);
    
    if(samePerson==FALSE){
        value++;
        EEPROM.put(address, value);
        Particle.publish("interactions=", String(value));
        Serial.println(value);
        samePerson = TRUE;    
    }
  }else{
    digitalWrite(D7,LOW);
  }
  delay(200);
}

void retryConnect()
{
  if (!Particle.connected())   // if not connected to cloud
  {
    Serial.println("reconnect");
    stopTimer.start();         // set of the timout time
    WiFi.on();
    Particle.connect();        // start a reconnectino attempt
  }
  else                         // if already connected
  {
    Serial.println("connected");
    Particle.publish("interactions=", String(value));
    retryTimer.stop();         // no further attempts required
    retryRunning = false;
  }
}

void stopConnect()
{
    Serial.println("stopped");

    if (!Particle.connected()) // if after retryTime no connection
     WiFi.off();              // stop trying and swith off WiFi
     stopTimer.stop();
}
