// set up of ports
const int tempSensPinIn = 1; //41 - analog in
const int coSensePinIn = 2; //40 - analog in
const int gasSensPinIn = 3; // 39 - analog in
const int moveSensPinIn = 4; // 25 - digital in


const int alarmRedPinOut = 23; // 8 - digital out
const int alarmGrPinOut = 24; // 9 - digital out
const int lockPinOut = 25; // 10 - digital out
const int gasReleasePinOut = 26; // 11 - digital out
const int gasPumpPinOut = 27; // 18 - digital out
const int buttonLed = 22; // Just for Testing


// set up variables
boolean sensTempIn = false;
boolean sensCOIn = false;
boolean moveInRoom = false;
boolean alarmOn = false;
boolean lockDoor = false;


void setup() {
  // put your setup code here, to run once:
  pinMode(moveSensPinIn, INPUT);
  pinMode(alarmRedPinOut, OUTPUT);
  pinMode(alarmGrPinOut, OUTPUT);
  pinMode(lockPinOut, OUTPUT);
  pinMode(gasReleasePinOut, OUTPUT);
  pinMode(gasPumpPinOut, OUTPUT);
}

void loop() {
  // set up
  pinMode(buttonLed, OUTPUT); //just for testing
  digitalWrite(buttonLed, HIGH);
  
  setup();

  digitalWrite(buttonLed, LOW);

  if(digitalRead(tempSensPinIn)) // get temperature  //should be analogRead(tempSensPinIn)
  {
    delay(10000); // wait for 10 sec
    if(digitalRead(tempSensPinIn)){ //should be analogRead(tempSensPinIn)
      digitalWrite(alarmRedPinOut, HIGH);
      digitalWrite(lockPinOut, HIGH);
      delay(10000); // wait for 10 sec

      while(digitalRead(moveSensPinIn)){// while there is motion inside we should wait
		  delay(10000);
	  } 
      digitalWrite(gasReleasePinOut, HIGH); // release gas inside
	  delay(10000); // wait 10 sec while gas occupy all room
	  digitalWrite(gasReleasePinOut, LOW); // stop releasing Gas
	  
	  delay(42000000); // wait when fire stops
	
	  digitalWrite(gasPumpPinOut, HIGH); // exhaust gas out of room
	
	  while(digitalRead(coSensePinIn)){ //  should be analogRead(coSensePinIn)
		delay(5000); // wait for 5 sec 
	  }  
	  digitalWrite(gasPumpPinOut, LOW); // turn off motors for exhausting
	
	  digitalWrite(lockPinOut, LOW);// anyway should be opened due to CO gone away
			while(digitalRead(tempSensPinIn)){ // if fire still inside alarm cannot be turn off  // should be  analogRead(tempSensPinIn)
			  delay(5000); // wait 5sec fire should gone away
			}                
	  digitalWrite(alarmRedPinOut, LOW); // no fire inside we can turn alarm off      
    }
  }
  delay(10000); // wait for 10 sec
}
