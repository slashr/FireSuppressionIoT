//7set up of ports
const int firePinIn = A0; // Normally 800
const int rhSensPinIn = A2; // Normally 90
const int gasSensPinIn = A1; // Normally 60

const int alarmPinOut = 2; 
const int lockPinOut = 3; 
const int gasFanPinOutINA = 5;
//const int gasFanPinOutINB = 11;
const int exhaustFanPinOutINA = 6; 
//const int exhaustFanPinOutINB = 13;
const int freshFanPinOutINA = 9;
//const int freshFanPinOutINB = 8; 

//Scada
const int exhaustStatus = 10;
const int freshStatus = 11;
const int gasStatus = 12;
const int fireStatus = 13;

boolean fireExtinguished = false;
int i = 0;

void setup() {
  //analog in
  pinMode(firePinIn, INPUT);
  pinMode(rhSensPinIn, INPUT);
  pinMode(gasSensPinIn, INPUT);

  //digital out
  pinMode(alarmPinOut, OUTPUT);
  pinMode(lockPinOut, OUTPUT);
  pinMode(gasFanPinOutINA, OUTPUT);
  pinMode(exhaustFanPinOutINA, OUTPUT);
  pinMode(freshFanPinOutINA, OUTPUT);

  pinMode(exhaustStatus, OUTPUT);
  pinMode(freshStatus, OUTPUT);
  pinMode(gasStatus, OUTPUT);
  pinMode(fireStatus, OUTPUT);


  digitalWrite(exhaustStatus, LOW);
  digitalWrite(freshStatus, LOW);
  digitalWrite(gasStatus, LOW);
  digitalWrite(fireStatus, LOW);
  
  // setup values
  digitalWrite(alarmPinOut, LOW);
  digitalWrite(lockPinOut, HIGH);
  digitalWrite(gasFanPinOutINA, HIGH);
  digitalWrite(exhaustFanPinOutINA, HIGH);
  digitalWrite(freshFanPinOutINA, HIGH);
  Serial.begin(9600);
}

void loop() {
  
  setup();
  Serial.println("Setup Done.");  
  
   if (!fireExtinguished){
      Serial.println("RH Sensor:" + String(analogRead(rhSensPinIn)));
      Serial.println("Gas Sensor:" + String(analogRead(gasSensPinIn)));
      Serial.println("Fire Sensor:" + String(analogRead(firePinIn)));
  
      if (int(analogRead(firePinIn)) < 800){
        Serial.println("Fire Detected");
        Serial.println("Fire Sensor:" + String(analogRead(firePinIn)));
  
        delay(3000); // wait for 10 sec
  
        Serial.println("CO Sensor:" + String(analogRead(rhSensPinIn)));
  
        if (int(analogRead(firePinIn)) < 800) { 
  
          Serial.println("Fire Detection Confirmed!");  
          digitalWrite(alarmPinOut, HIGH);
          digitalWrite(lockPinOut, LOW);
          
          digitalWrite(fireStatus, HIGH);

          delay(3000);
          
          Serial.println("Waiting for room to be empty");
          delay(5000);
          
  
          Serial.println("Releasing Inergen Gas! Clear the room! Turn pump on");          
          digitalWrite(gasFanPinOutINA, LOW);

          //SCADA variables           
            
            digitalWrite(gasStatus, HIGH);
           
            
          //Wait for gas to enter room
          delay(5000); 
  
          //Stop gas pump
          digitalWrite(gasFanPinOutINA, HIGH);
          
          digitalWrite(gasStatus, LOW);
          
          Serial.println("Gas released. Suppresion complete. Turn pump off");
          delay(7000); 
          
          
          Serial.println("Turning on Exhaust and Fresh Air exhaust");
          digitalWrite(exhaustFanPinOutINA, LOW);

          digitalWrite(exhaustStatus, HIGH);
          
          digitalWrite(freshFanPinOutINA, LOW);

          digitalWrite(freshStatus, HIGH);
          
          delay(10000);
  
          Serial.println("Gas exhausted. Turning off exhaust and fresh air");
          delay(2000);
          digitalWrite(exhaustFanPinOutINA, HIGH);
          digitalWrite(freshFanPinOutINA, HIGH);


          digitalWrite(exhaustStatus, LOW);
          digitalWrite(freshStatus, LOW);


          int p = 0;
          delay(2000);
          Serial.println("Checking for any leftover gas");
          delay(2000);
          int gasLevel = analogRead(gasSensPinIn);
          while (gasLevel > 200){
            if(p > 5)
            break;
            delay(1000);
            Serial.println("Gas Detected. Turn on exhaust fan and fresh air fan");
            Serial.println("Gas Sensor:" + String(analogRead(rhSensPinIn)));
            Serial.println("RH Sensor:" + String(analogRead(gasSensPinIn)));
            digitalWrite(exhaustFanPinOutINA, LOW);
            digitalWrite(freshFanPinOutINA, LOW);  


            //Scada variables
            digitalWrite(exhaustStatus, HIGH);
            digitalWrite(freshStatus, HIGH);
            p++;
            delay(5000);
            gasLevel = analogRead(gasSensPinIn);
          }

          Serial.println("Gas disperesed. Turning off exhaust and fresh air fan");
          Serial.println("RH Sensor:" + String(analogRead(rhSensPinIn)));
          Serial.println("Gas Sensor:" + String(analogRead(gasSensPinIn)));
          digitalWrite(exhaustFanPinOutINA, HIGH);
          digitalWrite(freshFanPinOutINA, HIGH); 
          //Scada variables
          digitalWrite(exhaustStatus, LOW);
          digitalWrite(freshStatus, LOW);
          delay(2000);   
          digitalWrite(lockPinOut, LOW);
  
          Serial.println("Checking for fire inside");
          delay(10000);
          
          int i = 0;
          int flag = 0;
          while (analogRead(firePinIn) < 800) { // if fire still inside alarm cannot be turn off
  
            Serial.println("Fire still present");
            digitalWrite(alarmPinOut, HIGH);
            digitalWrite(lockPinOut, LOW);
            flag = 1;
            
            digitalWrite(fireStatus, HIGH);
            
            delay(2000);
  
            delay(500);
            i++;// wait 5sec fire should gone away
            if (i > 10 ) break;
          }

          if(flag == 1)
          {
              digitalWrite(alarmPinOut, HIGH);// fire inside, don't turn alarm off
              digitalWrite(lockPinOut, HIGH);
              fireExtinguished = true;
              
              digitalWrite(fireStatus, HIGH);
              delay(5000);
          }
          else if(flag == 0)
          {
            
              digitalWrite(alarmPinOut, LOW);// no fire inside we can turn alarm off
              digitalWrite(lockPinOut, HIGH);
              fireExtinguished = true;
              
              digitalWrite(fireStatus, LOW);
            
          }
          else
          {
            }
        }
      }  
    }
   delay(1000);  // wait for 10 sec
    
}

