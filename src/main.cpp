#include <Arduino.h>

enum TimerProgramValues{
  Check_20_Sec = 20000,
  Check_One_Minute = 60000,
  Check_FiveMinute = 300000
};

struct CommonDataWeather {
  float temperature;
  int humidity;
};

bool executeSendMessage(TimerProgramValues timerValue);
void sendMessageTemperature(TimerProgramValues timerValue, bool canSend, unsigned long previousTime);
void getSensorInfo(CommonDataWeather& model);
bool checkCanActivateTimer(int value, unsigned long& previousTime,  unsigned long currentTime);
void sendFreeHeapSize(bool controlSendIsActive, TimerProgramValues enumValue);


void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("System start!");
}

void loop() { 
 executeSendMessage(TimerProgramValues::Check_20_Sec);
 executeSendMessage(TimerProgramValues::Check_One_Minute);
 executeSendMessage(TimerProgramValues::Check_FiveMinute); 
}

bool executeSendMessage(TimerProgramValues timerValue)
{
  bool flag = false;
  int millisecEnumParam = static_cast<int>(timerValue);
  unsigned long current_time = millis();
  switch (timerValue)
  { 
    case Check_20_Sec:
    {
        static unsigned long previousTimeTwentySec;
        flag = checkCanActivateTimer(millisecEnumParam, previousTimeTwentySec, current_time);
        sendMessageTemperature(TimerProgramValues::Check_20_Sec, flag, previousTimeTwentySec);
        break;
    }
    case Check_One_Minute:
    {
        static unsigned long previousTimeOneMinute;
        flag = checkCanActivateTimer(millisecEnumParam, previousTimeOneMinute, current_time);
        sendFreeHeapSize(flag, timerValue);
        break;
    }
    case Check_FiveMinute:
    {
        static unsigned long previousTimeFive;
        flag = checkCanActivateTimer(millisecEnumParam, previousTimeFive, current_time);
        sendFreeHeapSize(flag, timerValue);
        break;
    }
  
  default:
    break;
  }
  
  return flag;
}

bool checkCanActivateTimer(int value, unsigned long& previousTime,  unsigned long currentTime)
{
    bool result = false;

    if(currentTime - previousTime >= value)
    {
     previousTime = currentTime;
     result = true;
    }

    return result;
}

void sendMessageTemperature(TimerProgramValues timerValue, bool canSend, unsigned long timestamp)
{
    if(canSend)
    {
      CommonDataWeather model;
      getSensorInfo(model);
      Serial.printf("Temperature %.2f, Humidity: %d, timestamp: %u\r\n", model.temperature, model.humidity, timestamp);
    }       
}

void getSensorInfo(CommonDataWeather& model)
{
    model.temperature = random(1500, 3100) / 100.0;
    model.humidity = random(30, 66);
}

void sendFreeHeapSize(bool controlSendIsActive, TimerProgramValues enumValue)
{
  if(controlSendIsActive)
  {
   if(enumValue == TimerProgramValues::Check_One_Minute)
   {
    uint32_t freeHeap = ESP.getFreeHeap();

    Serial.printf("Free heap: %u\r\n", freeHeap);
   }
   else if(enumValue == TimerProgramValues::Check_FiveMinute)
   {
      Serial.println("Значення стабільне");
   }
  }
}