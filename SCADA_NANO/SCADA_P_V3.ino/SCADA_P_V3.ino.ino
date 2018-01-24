
     // @C "andresarmento" // I have used open source libraries of  https://github.com/andresarmento/modbus-arduino project, used with keeplive function
     // I would like to thank a lot to Andresarmento for such a nice development.


#include <EtherCard.h>
#include <Modbus.h>
#include <ModbusIP_ENC28J60.h>

//Pins for MODBUS TCP DI Acquisition

const int   lockLedPin        = 6;      //  access control on scada -> get form NANO-1 -->  Pin no 6  using direct connection from board
const int   exhaustMPin       = 9;      //  M2 exhaust motor        -> get form NANO-1 -->  Pin no 9 (INB) using direct connection from board
const int   gasReleaseMPin    = 7;      //  M3 inergen gas motor    -> get form NANO-1 -->  Pin no 7 (INA) using direct connection from board
const int   freshMPin         = 8;      //  M1 Fresh Air  motor     -> get form NANO-1 -->  Pin no 8 (INA) using direct connection from board
const int   movementPin       = 4;      //  occupancy sensor        
const int   firePin           = 2;      //  Fire sensor             -> get form NANO-1 -->  Pin no 2 using direct connection from board
const int   emerSwt           = 3;      //  Emergency switch


//Pin used for MODBUS TCP AI Acquisition

const int     rhPin             = A0;           // get RH index
const int     gasIndexPin       = A2;           // get gas index

//Modbus Registers Offsets (0-9999)
//const int DI_gas             =  102;
const int DI_lockLed         =  104;
const int DI_MFresh          =  105;
const int DI_MExhaust        =  106;
const int DI_MgasRel         =  107;
const int DI_Movement        =  108;
const int DI_Fire            =  109;
const int DI_Emr             =  110;


//Modbus Holding Reg

const int     AI_RH     = 301;
const int     AI_Gas    = 302;
const int     AI_HIndex = 303;


//ModbusIP object

ModbusIP mb;        // memory allocation for data storage

void setup() {
  // The media access control (ethernet hardware) address for the shield
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  // The IP address for the shield
  byte ip[] = { 192, 168, 1, 120 };
  //Config Modbus IP
  mb.config(mac, ip);           


  //Set DI PIN mode

  pinMode   (lockLedPin, INPUT);
  pinMode   (exhaustMPin, INPUT);
  pinMode   (gasReleaseMPin, INPUT);
  pinMode   (freshMPin, INPUT);
  pinMode   (movementPin, INPUT);
  pinMode   (firePin, INPUT);
  pinMode   (emerSwt, INPUT);
 

  // Set AI PIN mode    

  pinMode (rhPin, INPUT);
  pinMode (AI_Gas, INPUT);

  // Add DI register to funcode 01 - Use Coil for digital data

    
  mb.addCoil(DI_lockLed);
  mb.addCoil(DI_MFresh);
  mb.addCoil(DI_MExhaust);
  mb.addCoil(DI_MgasRel);
  mb.addCoil(DI_Movement);
  mb.addCoil(DI_Fire);
  mb.addCoil(DI_Emr);


  // Add AI register to function code 03 - use Holding register for Analog data

  mb.addHreg(AI_RH);
  mb.addHreg(AI_Gas);
  mb.addHreg(AI_HIndex);
}

void loop() {

  mb.task();


  // Save values in Coil Function Code 1

  mb.Coil(DI_lockLed, digitalRead(lockLedPin));
  mb.Coil(DI_MFresh, digitalRead(freshMPin));
  mb.Coil(DI_MExhaust, digitalRead(exhaustMPin));
  mb.Coil(DI_MFresh, digitalRead (freshMPin));
  mb.Coil(DI_MgasRel, digitalRead(gasReleaseMPin));
  mb.Coil(DI_Movement, digitalRead(movementPin));
  mb.Coil(DI_Fire, digitalRead(firePin));
  mb.Coil(DI_Emr, digitalRead(emerSwt));

  // calculate Heat index
  
  float hIndex;
  
  int rh;
  const int rTemp = 25;

  rh = (analogRead (rhPin));
  
  int factor = (rh / 10);     // for Scaling the AI value to percentage
  
      // If  RH value is less than 80% uncomment following formula
      
      hIndex = (0.5 * ((rTemp + 61) + ((rTemp - 68) * 1.2) + (factor * 0.094)));


    // If  RH value is greater than 80% uncomment following formula
    
  /*    hIndex = ((-42.379) + (2.04901523*rTemp) + (10.14333127*factor) - (0.22475541*rTemp*factor) - (.00683783*rTemp*rTemp)
      - (0.05481717*factor*factor) + (0.00122874*rTemp*rTemp*factor) + (0.00085282*rTemp*factor*factor) - (0.00000199*rTemp*rTemp*factor*factor));

  */

  // Save values in Holding reg Function Code 3

  mb.Hreg(AI_RH, factor);
  mb.Hreg(AI_Gas, analogRead (AI_Gas));
  mb.Hreg(AI_HIndex, hIndex);

}
