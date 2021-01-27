#include <math.h>

#define LCD_DIG1 PD4
#define LCD_DIG2 PA3
#define LCD_DIG3 PA1

#define LCD_A PD5
#define LCD_B PA2
#define LCD_C PC7
#define LCD_D PD2
#define LCD_E PD3
#define LCD_F PD6
#define LCD_G PC6
#define LCD_DP PD1

#define SENSOR_PIN A0

volatile uint8_t mMemory[3];
volatile uint8_t mDot;
volatile uint8_t mCur;

void C7Segment3Show();
void Char2Bcd(const uint16_t _val, uint8_t * _buf);
void C7SegmentSet(uint16_t _val, uint8_t _dot);
uint16_t ReadThemperature();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LCD_DIG1, OUTPUT);
  pinMode(LCD_DIG2, OUTPUT);
  pinMode(LCD_DIG3, OUTPUT);
  pinMode(LCD_A, OUTPUT);
  pinMode(LCD_B, OUTPUT);
  pinMode(LCD_C, OUTPUT);
  pinMode(LCD_D, OUTPUT);
  pinMode(LCD_E, OUTPUT);
  pinMode(LCD_F, OUTPUT);
  pinMode(LCD_G, OUTPUT);
  pinMode(LCD_DP, OUTPUT);

  mMemory[0] = 0;
  mMemory[1] = 0;
  mMemory[2] = 0;
  
  mCur = 0;  
  mDot = 0;
}

uint32_t t1 = 0;
uint16_t mCount = 0;

void loop() {
  if(millis() - t1 >= 6)
  {
    t1 = millis();
    mCount++;
    C7Segment3Show(); 
  }
  if(mCount >= 10)
  {
    mCount = 0;
    ReadThemperature();
  }
}

/**
  _val 0..999
  _dot 1..3, or 0 - not dot
*/
void C7SegmentSet(uint16_t _val, uint8_t _dot)
{
  Char2Bcd(_val, mMemory);
  if(_dot < 4)
    mDot = _dot;
}

// 0..9
void C7Segment3SetDigit(uint8_t _dig)
{
  static const uint8_t table[10] = {0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe, 0xf6};
  if(_dig < 10)
  {
      uint8_t mask = table[_dig];
      digitalWrite(LCD_A, mask & (1<<7) ? LOW : HIGH);
      digitalWrite(LCD_B, mask & (1<<6) ? LOW : HIGH);
      digitalWrite(LCD_C, mask & (1<<5) ? LOW : HIGH);
      digitalWrite(LCD_D, mask & (1<<4) ? LOW : HIGH);
      digitalWrite(LCD_E, mask & (1<<3) ? LOW : HIGH);
      digitalWrite(LCD_F, mask & (1<<2) ? LOW : HIGH);
      digitalWrite(LCD_G, mask & (1<<1) ? LOW : HIGH);
  }
}

void C7SegmentSetDot(uint8_t _state)
{
  digitalWrite(LCD_DP, _state);
}

void C7Segment3Show()
{
  switch (mCur)
  {
    case 0:
      mCur=1;
      digitalWrite(LCD_DIG3, LOW);
      C7Segment3SetDigit(mMemory[0]);
      C7SegmentSetDot(mDot & (1<<0) ? LOW : HIGH);
      digitalWrite(LCD_DIG1, HIGH);
    break;
    case 1:
      mCur=2;
      digitalWrite(LCD_DIG1, LOW);
      C7Segment3SetDigit(mMemory[1]);
      C7SegmentSetDot(mDot & (1<<1) ? LOW : HIGH);
      digitalWrite(LCD_DIG2, HIGH);
    break;
    default: 
      mCur=0;
      digitalWrite(LCD_DIG2, LOW);
      C7Segment3SetDigit(mMemory[2]);
      C7SegmentSetDot(mDot & (1<<2) ? LOW : HIGH);
      digitalWrite(LCD_DIG3, HIGH);
    break;
  }
}

uint16_t div10(const uint16_t _val)
{
  static const uint32_t iDivisor = 0x19A; //load 1/10 * 2^12
  uint32_t ml = (iDivisor * _val);
  return (uint16_t) (ml >> 12);
}

void Char2Bcd(const uint16_t _val, uint8_t * _buf)
{
  uint16_t temp, temp2;
  temp = div10(_val);
  _buf[2] = (uint8_t)(_val - (temp * 10));
  temp2 = div10(temp);
  _buf[1] = (uint8_t)(temp - (temp2 * 10));
  temp = div10(temp2);
  _buf[0] = (uint8_t)(temp2 - (temp * 10));
}

uint16_t ReadThemperature()
{
  static uint8_t adc_count = 0;
  static uint32_t av = 0;
  av += analogRead(SENSOR_PIN);

  if(++adc_count >= 3)
  {
    adc_count = 0;
    av /= 4;
    float tmp = 1023.0f/(float)av - 1;
    float res = 9000.0f/tmp;
    uint8_t dt = 2;
  
    tmp = logf(10000.0f/res);
    tmp /= 4000; // coeff
    tmp += 1.0 / (25 + 273.15);
    tmp = 1.0 / tmp;
    tmp -= 273.15f;  // convert to celsius
    C7SegmentSet((uint16_t)(tmp * 10.0), dt); // show to LCD
  }
}
