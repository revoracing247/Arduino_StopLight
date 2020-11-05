/*
Stop light
By: Colby R.
Date: 1/23/2020

*/
#define R_LED1 2
#define Y_LED1 3
#define G_LED1 4

#define R_LED2 5
#define Y_LED2 6
#define G_LED2 7

#define R_LED3 8
#define Y_LED3 9
#define G_LED3 10
#define MOTION_IN 11
#define BUTTON_IN 12

#define LED_ON LOW
#define LED_OFF HIGH

#define LEDSTATE_OFF      0x00
#define LEDSTATE_NSGREEN  0x01
#define LEDSTATE_NSYELLOW 0x02
#define LEDSTATE_NSRED    0x03
#define LEDSTATE_EWGREEN  0x04
#define LEDSTATE_EWYELLOW 0x05
#define LEDSTATE_EWRED    0x06
#define LEDSTATE_NSFLASH  0x07
#define LEDSTATE_EWFLASH  0x08

#define APPSTATE_RADNOM_WITH_MOTION    0x00
#define APPSTATE_RANDOM_WITHOUT_MOTION 0x01
#define APPSTATE_FIXED_WITHOUT_MOTION  0x02
#define APPSTATE_FOUR_WAY_STOP         0x03
#define APPSTATE_TWO_WAY_STOP          0x04
#define APPSTATE_ONE_WAY_STOP          0x05
#define APPSTATE_FOUR_WAY_CAUTION      0x06
#define APPSTATE_NA07                  0x07
#define APPSTATE_NA08                  0x08
#define APPSTATE_NA09                  0x09

#define GREEN_LIGHT_TIME_MIN  2000 //ms
#define GREEN_LIGHT_TIME_MAX 25000 //ms
#define GREEN_LIGHT_TIME     10000 //ms
#define YELLOW_LIGHT_TIME     4000 //ms
#define RED_LIGHT_TIME        1000 //ms
#define LIGHT_FLASH_TIME      1000 //ms

#define BUTTON_DEBOUNCE_TIME    10 //ms
#define BUTTON_HOLD_TIME      3000 //ms

int LightState = LEDSTATE_NSGREEN;
int AppState = APPSTATE_RADNOM_WITH_MOTION;
int ChangeCountdown = 0;
int DebounceCountdown = 0;
int ButtonHeldTimer = 0;
bool ChangeTriggered = false;
bool ButtonDebouncing = false;
bool LastButtonState = LOW;

void SetLEDs(int LightOne, int LightTwo, int LightThree)
{
  if(LightOne & 0x04){ digitalWrite(R_LED1, LED_ON); }
  else               { digitalWrite(R_LED1, LED_OFF);}
  if(LightOne & 0x02){ digitalWrite(Y_LED1, LED_ON); }
  else               { digitalWrite(Y_LED1, LED_OFF);}
  if(LightOne & 0x01){ digitalWrite(G_LED1, LED_ON); }
  else               { digitalWrite(G_LED1, LED_OFF);}

  if(LightTwo & 0x04){ digitalWrite(R_LED2, LED_ON); }
  else               { digitalWrite(R_LED2, LED_OFF);}
  if(LightTwo & 0x02){ digitalWrite(Y_LED2, LED_ON); }
  else               { digitalWrite(Y_LED2, LED_OFF);}
  if(LightTwo & 0x01){ digitalWrite(G_LED2, LED_ON); }
  else               { digitalWrite(G_LED2, LED_OFF);}

  if(LightThree & 0x04){ digitalWrite(R_LED3, LED_ON); }
  else                 { digitalWrite(R_LED3, LED_OFF);}
  if(LightThree & 0x02){ digitalWrite(Y_LED3, LED_ON); }
  else                 { digitalWrite(Y_LED3, LED_OFF);}
  if(LightThree & 0x01){ digitalWrite(G_LED3, LED_ON); }
  else                 { digitalWrite(G_LED3, LED_OFF);}
}
// the setup function runs once when you press reset or power the board
void setup()
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(R_LED1, OUTPUT);
  pinMode(Y_LED1, OUTPUT);
  pinMode(G_LED1, OUTPUT);

  pinMode(R_LED2, OUTPUT);
  pinMode(Y_LED2, OUTPUT);
  pinMode(G_LED2, OUTPUT);

  pinMode(R_LED3, OUTPUT);
  pinMode(Y_LED3, OUTPUT);
  pinMode(G_LED3, OUTPUT);
  
  pinMode(MOTION_IN, INPUT);
  digitalWrite(MOTION_IN, HIGH);
  
  pinMode(BUTTON_IN, INPUT);
  digitalWrite(BUTTON_IN, HIGH);
  LastButtonState = digitalRead(BUTTON_IN);

  // pinMode(MOTION_IN, INPUT);

  LightState = LEDSTATE_NSGREEN;
  AppState = APPSTATE_RADNOM_WITH_MOTION;
  ChangeCountdown = 0;
  ChangeTriggered = true;

  SetLEDs(0b000,
          0b000,
          0b000); delay(150);
  
  SetLEDs(0b001,
          0b000,
          0b000); delay(150);
  
  SetLEDs(0b011,
          0b000,
          0b000); delay(150);
  
  SetLEDs(0b111,
          0b000,
          0b000); delay(150);
  
  SetLEDs(0b111,
          0b001,
          0b000); delay(150);
  
  SetLEDs(0b111,
          0b011,
          0b000); delay(150);
  
  SetLEDs(0b111,
          0b111,
          0b000); delay(150);
  
  SetLEDs(0b111,
          0b111,
          0b001); delay(150);
  
  SetLEDs(0b111,
          0b111,
          0b011); delay(150);
  
  SetLEDs(0b111,
          0b111,
          0b111); delay(500);
  
  SetLEDs(0b000,
          0b000,
          0b000); delay(500);
  
  SetLEDs(0b111,
          0b111,
          0b111); delay(500);
  
  SetLEDs(0b000,
          0b000,
          0b000); delay(1000);
}

// the loop function runs over and over again forever
void loop()
{
  // if(digitalRead(MOTION_IN))
  // {
  //   ChangeTriggered = true;
  // }
  
  // +--------------------------------------------------------------+
  // |                     Standard With Motion                     |
  // +--------------------------------------------------------------+
  if(AppState == APPSTATE_RADNOM_WITH_MOTION)
  {
    // sanitize appstate and countdown
    if(LightState != LEDSTATE_NSGREEN  &&
       LightState != LEDSTATE_NSYELLOW &&
       LightState != LEDSTATE_NSRED    &&
       LightState != LEDSTATE_EWGREEN  &&
       LightState != LEDSTATE_EWYELLOW &&
       LightState != LEDSTATE_EWRED     ){ LightState = LEDSTATE_NSGREEN; ChangeCountdown = 0; }
    
    // actual running stuff
    if(LightState == LEDSTATE_NSGREEN && ChangeCountdown == 0) // RIGHT and LEFT GREEN
    {
      SetLEDs(0b100, 0b001, 0b100); // set LEDS
      LightState = LEDSTATE_NSYELLOW; // change LED state
      ChangeCountdown = random(GREEN_LIGHT_TIME_MIN,GREEN_LIGHT_TIME_MAX); // set countdown
      ChangeTriggered = false;
    }
    else if(LightState == LEDSTATE_NSYELLOW && ChangeCountdown == 0  && digitalRead(MOTION_IN) == HIGH)
    {
      SetLEDs(0b100, 0b010, 0b100); // set LEDS
      LightState = LEDSTATE_NSRED; // change LED state
      ChangeCountdown = YELLOW_LIGHT_TIME; // set countdown
    }
    else if(LightState == LEDSTATE_NSRED && ChangeCountdown == 0)
    {
      SetLEDs(0b100, 0b100, 0b100); // set LEDS
      LightState = LEDSTATE_EWGREEN; // change LED state
      ChangeCountdown = RED_LIGHT_TIME; // set countdown
    }

    else if(LightState == LEDSTATE_EWGREEN && ChangeCountdown == 0) // center GREEN
    {
      SetLEDs(0b001, 0b100, 0b001); // set LEDS
      LightState = LEDSTATE_EWYELLOW; // change LED state
      ChangeCountdown = random(GREEN_LIGHT_TIME_MIN,GREEN_LIGHT_TIME_MAX); // set countdown
      ChangeTriggered = false;
    }
    else if(LightState == LEDSTATE_EWYELLOW && ChangeCountdown == 0  && digitalRead(MOTION_IN) == HIGH)
    {
      SetLEDs(0b010, 0b100, 0b010); // set LEDS
      LightState = LEDSTATE_EWRED; // change LED state
      ChangeCountdown = YELLOW_LIGHT_TIME; // set countdown
    }
    else if(LightState == LEDSTATE_EWRED && ChangeCountdown == 0)
    {
      SetLEDs(0b100, 0b100, 0b100); // set LEDS
      LightState = LEDSTATE_NSGREEN; // change LED state
      ChangeCountdown = RED_LIGHT_TIME; // set countdown
    }
  }
  
  // +--------------------------------------------------------------+
  // |                   Standard Without Motion                    |
  // +--------------------------------------------------------------+
  else if(AppState == APPSTATE_RANDOM_WITHOUT_MOTION)
  {
    // sanitize appstate and countdown
    if(LightState != LEDSTATE_NSGREEN  &&
       LightState != LEDSTATE_NSYELLOW &&
       LightState != LEDSTATE_NSRED    &&
       LightState != LEDSTATE_EWGREEN  &&
       LightState != LEDSTATE_EWYELLOW &&
       LightState != LEDSTATE_EWRED     ){ LightState = LEDSTATE_NSGREEN; ChangeCountdown = 0; }
    
    // actual running stuff
    if(LightState == LEDSTATE_NSGREEN && ChangeCountdown == 0) // RIGHT and LEFT GREEN
    {
      SetLEDs(0b100, 0b001, 0b100); // set LEDS
      LightState = LEDSTATE_NSYELLOW; // change LED state
      ChangeCountdown = random(GREEN_LIGHT_TIME_MIN,GREEN_LIGHT_TIME_MAX); // set countdown
    }
    else if(LightState == LEDSTATE_NSYELLOW && ChangeCountdown == 0)
    {
      SetLEDs(0b100, 0b010, 0b100); // set LEDS
      LightState = LEDSTATE_NSRED; // change LED state
      ChangeCountdown = YELLOW_LIGHT_TIME; // set countdown
    }
    else if(LightState == LEDSTATE_NSRED && ChangeCountdown == 0)
    {
      SetLEDs(0b100, 0b100, 0b100); // set LEDS
      LightState = LEDSTATE_EWGREEN; // change LED state
      ChangeCountdown = RED_LIGHT_TIME; // set countdown
    }

    else if(LightState == LEDSTATE_EWGREEN && ChangeCountdown == 0) // center GREEN
    {
      SetLEDs(0b001, 0b100, 0b001); // set LEDS
      LightState = LEDSTATE_EWYELLOW; // change LED state
      ChangeCountdown = random(GREEN_LIGHT_TIME_MIN,GREEN_LIGHT_TIME_MAX); // set countdown
    }
    else if(LightState == LEDSTATE_EWYELLOW && ChangeCountdown == 0)
    {
      SetLEDs(0b010, 0b100, 0b010); // set LEDS
      LightState = LEDSTATE_EWRED; // change LED state
      ChangeCountdown = YELLOW_LIGHT_TIME; // set countdown
    }
    else if(LightState == LEDSTATE_EWRED && ChangeCountdown == 0)
    {
      SetLEDs(0b100, 0b100, 0b100); // set LEDS
      LightState = LEDSTATE_NSGREEN; // change LED state
      ChangeCountdown = RED_LIGHT_TIME; // set countdown
    }
  }
  
  // +--------------------------------------------------------------+
  // |                     Fixed Without Motion                     |
  // +--------------------------------------------------------------+
  else if(AppState == APPSTATE_FIXED_WITHOUT_MOTION)
  {
    // sanitize appstate and countdown
    if(LightState != LEDSTATE_NSGREEN  &&
       LightState != LEDSTATE_NSYELLOW &&
       LightState != LEDSTATE_NSRED    &&
       LightState != LEDSTATE_EWGREEN  &&
       LightState != LEDSTATE_EWYELLOW &&
       LightState != LEDSTATE_EWRED     ){ LightState = LEDSTATE_NSGREEN; ChangeCountdown = 0; }
    
    // actual running stuff
    if(LightState == LEDSTATE_NSGREEN && ChangeCountdown == 0) // RIGHT and LEFT GREEN
    {
      SetLEDs(0b100, 0b001, 0b100); // set LEDS
      LightState = LEDSTATE_NSYELLOW; // change LED state
      ChangeCountdown = GREEN_LIGHT_TIME; // set countdown
    }
    else if(LightState == LEDSTATE_NSYELLOW && ChangeCountdown == 0)
    {
      SetLEDs(0b100, 0b010, 0b100); // set LEDS
      LightState = LEDSTATE_NSRED; // change LED state
      ChangeCountdown = YELLOW_LIGHT_TIME; // set countdown
    }
    else if(LightState == LEDSTATE_NSRED && ChangeCountdown == 0)
    {
      SetLEDs(0b100, 0b100, 0b100); // set LEDS
      LightState = LEDSTATE_EWGREEN; // change LED state
      ChangeCountdown = RED_LIGHT_TIME; // set countdown
    }

    else if(LightState == LEDSTATE_EWGREEN && ChangeCountdown == 0) // center GREEN
    {
      SetLEDs(0b001, 0b100, 0b001); // set LEDS
      LightState = LEDSTATE_EWYELLOW; // change LED state
      ChangeCountdown = GREEN_LIGHT_TIME; // set countdown
    }
    else if(LightState == LEDSTATE_EWYELLOW && ChangeCountdown == 0)
    {
      SetLEDs(0b010, 0b100, 0b010); // set LEDS
      LightState = LEDSTATE_EWRED; // change LED state
      ChangeCountdown = YELLOW_LIGHT_TIME; // set countdown
    }
    else if(LightState == LEDSTATE_EWRED && ChangeCountdown == 0)
    {
      SetLEDs(0b100, 0b100, 0b100); // set LEDS
      LightState = LEDSTATE_NSGREEN; // change LED state
      ChangeCountdown = RED_LIGHT_TIME; // set countdown
    }
  }
  
  // +--------------------------------------------------------------+
  // |                        Four Way Stop                         |
  // +--------------------------------------------------------------+
  else if(AppState == APPSTATE_FOUR_WAY_STOP)
  {
    // sanitize appstate and countdown
    if(LightState != LEDSTATE_NSFLASH && LightState != LEDSTATE_EWFLASH){ LightState = LEDSTATE_NSFLASH;  ChangeCountdown = 0; }
    
    // Actual running stuff
    if(LightState == LEDSTATE_NSFLASH && ChangeCountdown == 0) // RIGHT and LEFT GREEN
    {
      SetLEDs(0b100, 0b000, 0b100); // set LEDS
      LightState = LEDSTATE_EWFLASH; // change LED state
      ChangeCountdown = LIGHT_FLASH_TIME; // set countdown
    }
    else if(LightState == LEDSTATE_EWFLASH && ChangeCountdown == 0)
    {
      SetLEDs(0b000, 0b100, 0b000); // set LEDS
      LightState = LEDSTATE_NSFLASH; // change LED state
      ChangeCountdown = LIGHT_FLASH_TIME; // set countdown
    }
  }
  
  // +--------------------------------------------------------------+
  // |                         Two Way Stop                         |
  // +--------------------------------------------------------------+
  else if(AppState == APPSTATE_TWO_WAY_STOP)
  {
    // sanitize appstate and countdown
    if(LightState != LEDSTATE_NSFLASH && LightState != LEDSTATE_EWFLASH){ LightState = LEDSTATE_NSFLASH;  ChangeCountdown = 0; }
    
    // Actual running stuff
    if(LightState == LEDSTATE_NSFLASH && ChangeCountdown == 0) // RIGHT and LEFT GREEN
    {
      SetLEDs(0b100, 0b000, 0b100); // set LEDS
      LightState = LEDSTATE_EWFLASH; // change LED state
      ChangeCountdown = LIGHT_FLASH_TIME; // set countdown
    }
    else if(LightState == LEDSTATE_EWFLASH && ChangeCountdown == 0)
    {
      SetLEDs(0b000, 0b010, 0b000); // set LEDS
      LightState = LEDSTATE_NSFLASH; // change LED state
      ChangeCountdown = LIGHT_FLASH_TIME; // set countdown
    }
  }
  
  // +--------------------------------------------------------------+
  // |                         One Way Stop                         |
  // +--------------------------------------------------------------+
  else if(AppState == APPSTATE_ONE_WAY_STOP)
  {
    // sanitize appstate and countdown
    if(LightState != LEDSTATE_NSFLASH && LightState != LEDSTATE_EWFLASH){ LightState = LEDSTATE_NSFLASH;  ChangeCountdown = 0; }
    
    // Actual running stuff
    if(LightState == LEDSTATE_NSFLASH && ChangeCountdown == 0) // RIGHT and LEFT GREEN
    {
      SetLEDs(0b010, 0b000, 0b010); // set LEDS
      LightState = LEDSTATE_EWFLASH; // change LED state
      ChangeCountdown = LIGHT_FLASH_TIME; // set countdown
    }
    else if(LightState == LEDSTATE_EWFLASH && ChangeCountdown == 0)
    {
      SetLEDs(0b000, 0b100, 0b000); // set LEDS
      LightState = LEDSTATE_NSFLASH; // change LED state
      ChangeCountdown = LIGHT_FLASH_TIME; // set countdown
    }
  }
  
  // +--------------------------------------------------------------+
  // |                       Four Way Caution                       |
  // +--------------------------------------------------------------+
  else if(AppState == APPSTATE_FOUR_WAY_CAUTION)
  {
    // sanitize appstate and countdown
    if(LightState != LEDSTATE_NSFLASH && LightState != LEDSTATE_EWFLASH){ LightState = LEDSTATE_NSFLASH;  ChangeCountdown = 0; }
    
    // Actual running stuff
    if(LightState == LEDSTATE_NSFLASH && ChangeCountdown == 0) // RIGHT and LEFT GREEN
    {
      SetLEDs(0b010, 0b000, 0b010); // set LEDS
      LightState = LEDSTATE_EWFLASH; // change LED state
      ChangeCountdown = LIGHT_FLASH_TIME; // set countdown
    }
    else if(LightState == LEDSTATE_EWFLASH && ChangeCountdown == 0)
    {
      SetLEDs(0b000, 0b010, 0b000); // set LEDS
      LightState = LEDSTATE_NSFLASH; // change LED state
      ChangeCountdown = LIGHT_FLASH_TIME; // set countdown
    }
  }
  
  // +--------------------------------------------------------------+
  // |                       Button Debounce                        |
  // +--------------------------------------------------------------+
  if(digitalRead(BUTTON_IN) != LastButtonState && !ButtonDebouncing)
  {
    ButtonDebouncing = true;
    DebounceCountdown = BUTTON_DEBOUNCE_TIME;
  }
  if(digitalRead(BUTTON_IN) != LastButtonState && ButtonDebouncing && DebounceCountdown == 0)
  {
    ButtonDebouncing = false;
    LastButtonState = digitalRead(BUTTON_IN);
  }
  
  // +--------------------------------------------------------------+
  // |                     Button State Change                      |
  // +--------------------------------------------------------------+
  // if(LastButtonState == LOW && ButtonHeldTimer >= BUTTON_HOLD_TIME) // BUTTON HELD!
  // {
    // 
  // }
  // else if(LastButtonState == HIGH && ButtonHeldTimer > 0) // BUTTON PRESSED (then released)
  if(LastButtonState == HIGH && ButtonHeldTimer > 0) // BUTTON PRESSED (then released)
  {
    AppState ++; // increment our AppState
    if(AppState > APPSTATE_FOUR_WAY_CAUTION) { AppState = APPSTATE_RADNOM_WITH_MOTION; } // except roll it back over if we hit the end of the list
    LightState = LEDSTATE_OFF;
    SetLEDs((AppState & 0x07),
            (AppState & 0x28) >> 3,
            (AppState & 0xC0) >> 6); delay(500);    
    // then flash LEDS to indicate change
    SetLEDs(0b111,
            0b111,
            0b111); delay(200);
  
    SetLEDs(0b000,
            0b000,
            0b000); delay(200);
    
    SetLEDs(0b111,
            0b111,
            0b111); delay(200);
    
    SetLEDs(0b000,
            0b000,
            0b000); delay(200);
  }
  
  // +--------------------------------------------------------------+
  // |                         Dirty Timer                          |
  // +--------------------------------------------------------------+
  if(ChangeCountdown > 0){ ChangeCountdown --; }
  if(DebounceCountdown > 0){ DebounceCountdown --; }
  if(LastButtonState == LOW){ ButtonHeldTimer ++; } else{ ButtonHeldTimer = 0; }
  delay(1);
}
