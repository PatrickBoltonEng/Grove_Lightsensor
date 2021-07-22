/*
 * Project:           Grove_Lightsensor
 * Description:       Grove Light Sensor v1.2 with LS06-MØ5 
 * Author:            Patrick Bolton 
 * Date:              05/28/21
 */

#include "Particle.h"
#include "math.h"

SYSTEM_THREAD(ENABLED);

#define dbSerial Serial

SerialLogHandler logHandler;

#define UPDATE_INTERVAL 5000  //1 sec = 1000 millis
//Grove Light Sensor v1.2 with LS06-MØ5
const int LS06_pin= A2;               //Connect the LED Grove A2 on Boron
const int LS_threshold=10;            //The threshold for which the LED should turn on. 
int LS_sensorV;                       //sensor analog read
float LS_ave;                         //average sensor reading
float LS_lux;                         //Resistance in K, 

unsigned long UpdateInterval;
int min_last, min_time;

void setup() 
{
  dbSerial.begin(9600);

  pinMode(LS06_pin, INPUT);

  UpdateInterval = millis();
  min_last=-1;
}

void loop()
{
  Time.zone(-7);
  if(Particle.disconnected()){return;}
  
  if ((millis() - UpdateInterval) > UPDATE_INTERVAL)
  {
    int LS_sum = 0;
    for(int i=0; i<512; i++)
    {
      int LS_sensorV = analogRead(A2); 
      LS_sum = LS_sum  + LS_sensorV;
      float LS_ave = LS_sum / (i+1);
    }
    
    LS_lux=(LS_ave)*(3.3/4095.0)*(10.0/5.0);
    Log.info("Sensor Value: %d", LS_sensorV);
    Log.info("Sensor Lux: %f", LS_lux);

    min_time=Time.minute();
    if((min_time!=min_last)&&(min_time==0||min_time==15||min_time==30||min_time==45))
    {
      //createEventPayload1();
      min_last = min_time;    
      Log.info("Last Update: %d", min_last);
      Log.info(Time.timeStr());
    }
    
    Log.info(Time.timeStr());
    UpdateInterval = millis();
  }
}