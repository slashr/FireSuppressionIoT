// set up of ports
const int firePinIn = A0; // Normally 800
const int rhSensPinIn = A1; // Normally 90
const int gasSensPinIn = A2; // Normally 60
const int movementPinInTrig = A3; //
const int movementPinInEcho = A4; //


const int alarmPinOut = 0; //
const int lockPinOut = 1; //
const int safetyPinOut = 2; //
const int gasFanPinOutINA = 10; //
const int gasFanPinOutINB = 11; //
const int exhaustFanPinOutINA = 12; //
const int exhaustFanPinOutINB = 13; //
const int freshFanPinOutINA = 9; //
const int freshFanPinOutINB = 8;  //


boolean fireExtinguished = false;
int i = 0;

void setup() {
  // put your setup code here, to run once:
  //analog in
  pinMode(firePinIn, INPUT);
  pinMode(rhSensPinIn, INPUT);
  pinMode(gasSensPinIn, INPUT);
  pinMode(movementPinInTrig, INPUT);

  //digital out
  pinMode(alarmPinOut, OUTPUT);
  pinMode(lockPinOut, OUTPUT);
  pinMode(safetyPinOut, OUTPUT);
  pinMode(gasFanPinOutINA, OUTPUT);
  pinMode(gasFanPinOutINB, OUTPUT);
  pinMode(exhaustFanPinOutINA, OUTPUT);
  pinMode(exhaustFanPinOutINB, OUTPUT);
  pinMode(freshFanPinOutINA, OUTPUT);
  pinMode(freshFanPinOutINB, OUTPUT);
  
  // setup values
  digitalWrite(alarmPinOut, LOW);
  digitalWrite(lockPinOut, HIGH);
  digitalWrite(safetyPinOut, LOW);
  digitalWrite(gasFanPinOutINA, LOW);
  digitalWrite(gasFanPinOutINB, LOW);
  digitalWrite(exhaustFanPinOutINA, LOW);
  digitalWrite(exhaustFanPinOutINB, LOW);
  digitalWrite(freshFanPinOutINB, LOW);
  digitalWrite(freshFanPinOutINA, LOW);
}

void loop() {
  
  setup();
  Serial.println("Setup Done.");  
  
   if (!fireExtinguished){
      Serial.println("Inside First If");
      Serial.println("CO Sensor:" + String(analogRead(rhSensPinIn)));
      Serial.println("Gas Sensor:" + String(analogRead(gasSensPinIn)));
      Serial.println("Fire Sensor:" + String(analogRead(firePinIn)));
  
      if (int(analogRead(firePinIn)) < 800){
        Serial.println("Fire Detected");
        Serial.println("Fire Sens:" + String(analogRead(firePinIn)));
  
        delay(3000); // wait for 10 sec
  
        Serial.println("CO Sensor:" + String(analogRead(rhSensPinIn)));
  
        if (int(analogRead(firePinIn)) < 800){ 
  
          Serial.println("Fire and Smoke Detected");  
          digitalWrite(alarmPinOut, HIGH);
          digitalWrite(lockPinOut, LOW);
          digitalWrite(safetyPinOut, LOW);
          delay(2000);
  
          Serial.println("Detecting movement");

          int timeChecker = 0;
          while (timeChecker < 5) { // while there is motion inside we should wait
            if (analogRead(movementPinInTrig) > 50) {
              timeChecker = 100;
            }
            timeChecker++;
            delay(2000);
          }
  
          Serial.println("No movement detected. Releasing Inergen gas. Turn pump on");          
          digitalWrite(gasFanPinOutINB, HIGH); // release gas inside
          digitalWrite(gasFanPinOutINA, LOW);
          delay(15000); // wait 10 sec while gas occupy all room
  
  
          digitalWrite(gasFanPinOutINB, LOW); // stop releasing Gas
          digitalWrite(gasFanPinOutINA, LOW);
  
          Serial.println("Gas released. Fire suppressed. Turn pump off.");
  
          delay(15000); // wait when fire stops
          Serial.println("Turning on Exhaust");
  
  
          digitalWrite(exhaustFanPinOutINB, LOW); // exhaust gas out of room
          digitalWrite(exhaustFanPinOutINA, HIGH);
          
          digitalWrite(freshFanPinOutINB, LOW); // put fresh air
          digitalWrite(freshFanPinOutINA, HIGH);
  
          delay(15000);

          int p = 0;
          while (analogRead(gasSensPinIn) > 200 || p == 15){
            delay(2000);
            p++;
          }
  
          Serial.println("Gas exhausted. Turning off exhaust");
          delay(2000);
  
          digitalWrite(exhaustFanPinOutINB, LOW); // turn off motors for exhausting
          digitalWrite(exhaustFanPinOutINA, LOW);
  
          digitalWrite(freshFanPinOutINB, LOW); // put fresh air off
          digitalWrite(freshFanPinOutINA, LOW);
  
        
          digitalWrite(lockPinOut, LOW);// anyway should be opened due to CO gone away
  
          Serial.println("Checking for flame inside");
          delay(2000);
          
          int i = 0;
          while (analogRead(firePinIn) < 800) { // if fire still inside alarm cannot be turn off
  
            Serial.println("Fire still present");
            digitalWrite(alarmPinOut, HIGH);
            digitalWrite(lockPinOut, HIGH);
            delay(2000);
  
            delay(500);
            i++;// wait 5sec fire should gone away
            if (i > 10 ) break;
          }
          digitalWrite(alarmPinOut, LOW);// no fire inside we can turn alarm off
          digitalWrite(lockPinOut, HIGH);
          fireExtinguished = true;
        }
      }  
    }
   delay(1000);  // wait for 10 sec
   
    
}
