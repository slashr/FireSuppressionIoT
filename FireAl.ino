// set up of ports
const int flameSensPinIn = A0; //
const int coSensePinIn = A1; // in normal 90
const int gasSensPinIn = A2; // in normal 60
const int moveSensPinInTrig = A3; //
const int moveSensPinInEcho = A4; // 


const int alarmRedPinOut = 0; //
const int alarmGrPinOut = 1; // 
const int lockPinOut = 2; // 
const int gasReleasePinOutINB = 10; //
const int gasReleasePinOutINA = 11;
const int gasPumpPinOutINA = 8; // 
const int gasPumpPinOutINB = 9;

// set up variables
boolean sensTempIn = false;
boolean sensCOIn = false;
boolean moveInRoom = false;
boolean alarmOn = false;
boolean lockDoor = false;
boolean isWorked = false;
int i = 0;

void setup() {  
  // put your setup code here, to run once:
  //analog in
  pinMode(flameSensPinIn, INPUT);
  pinMode(coSensePinIn, INPUT);
  pinMode(gasSensPinIn, INPUT);
  pinMode(moveSensPinInTrig, INPUT);
  
  //digital out  
  pinMode(alarmRedPinOut, OUTPUT);
  pinMode(alarmGrPinOut, OUTPUT);
  pinMode(lockPinOut, OUTPUT);
  pinMode(gasReleasePinOutINB, OUTPUT);
  pinMode(gasReleasePinOutINA, OUTPUT);
  pinMode(gasPumpPinOutINA, OUTPUT);
  pinMode(gasPumpPinOutINB, OUTPUT);

  digitalWrite(alarmRedPinOut, LOW);
  digitalWrite(alarmGrPinOut, LOW);
  digitalWrite(lockPinOut, HIGH);
  digitalWrite(gasReleasePinOutINB, LOW);
  digitalWrite(gasReleasePinOutINA, LOW);
  digitalWrite(gasPumpPinOutINA, LOW);
  digitalWrite(gasPumpPinOutINB, LOW);
  
}

void loop() {

  
  setup();
  Serial.println("Setuped!");
 if(!isWorked)
 {
        Serial.println("Work in first if");

        int tmp = analogRead(coSensePinIn);
        Serial.println(tmp);
    
        if(int(analogRead(flameSensPinIn)) < 800) // if flame
        {
          Serial.println("Senseced the flame");
          
          delay(1000); // wait for 10 sec
          tmp = analogRead(coSensePinIn);
          Serial.println(tmp);
          if(int(analogRead(flameSensPinIn)) < 800 && int(analogRead(coSensePinIn)) > 110){
            isWorked = true;
            Serial.println("Stiil the flame turning on the Alarm");
                       
            digitalWrite(alarmRedPinOut, HIGH);
            digitalWrite(lockPinOut, LOW);
            delay(1000); 

            int timeChecker = 0;

            Serial.println("Searching for the move inside");
                  
            while(timeChecker < 10){// while there is motion inside we should wait
              if(analogRead(moveSensPinInTrig) > 50){
                    
                //Serial.println("Yes, there is the move inside. Wait for 20 seconds");
                //delay(1000);
                timeChecker = 100;
              }
              timeChecker++;
      		    delay(1000);
      	    } 

            Serial.println("Reliasing the gas! Turn on fan");
            
            digitalWrite(gasReleasePinOutINB, HIGH); // release gas inside
            digitalWrite(gasReleasePinOutINA, LOW);
      	    delay(10000); // wait 10 sec while gas occupy all room
            
            Serial.println("Gas is reliased, turn off fan");
                        
      	    digitalWrite(gasReleasePinOutINB, LOW); // stop releasing Gas
            digitalWrite(gasReleasePinOutINA, LOW);
      	  
      	    delay(5000); // wait when fire stops
            
            Serial.println("Exhaust the gas! Turn on the fans");
            
            
      	    digitalWrite(gasPumpPinOutINB, HIGH); // exhaust gas out of room
            digitalWrite(gasPumpPinOutINA, LOW); 

             delay(10000); 
      	    while(analogRead(gasSensPinIn) > 200){ //  should be analogRead(coSensePinIn)
      		    delay(1000); // wait for 5 sec 
      	    }
            
            Serial.println("Gas is exhausted, turn off fan");
            delay(2000);

            digitalWrite(gasPumpPinOutINB, LOW); // // turn off motors for exhausting
            digitalWrite(gasPumpPinOutINA, LOW); 
      	     
      	    int i = 0;
      	    digitalWrite(lockPinOut, HIGH);// anyway should be opened due to CO gone away

            Serial.println("Checking for flame inside");
            delay(2000);
                        
      			while(analogRead(flameSensPinIn) < 800){ // if fire still inside alarm cannot be turn off  // should be  analogRead(tempSensPinIn)
      			  
              Serial.println("Is still ");
              delay(2000);
              
      			  delay(500);
      			  i++;// wait 5sec fire should gone away
              if(i > 10 ) break;
      			}                
      	    digitalWrite(alarmRedPinOut, LOW); // no fire inside we can turn alarm off      
          }
        }
        
  }
  delay(1000); // wait for 10 sec
}
