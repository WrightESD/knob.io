  
#include <SPI.h>

#define SDI    11 //Data
#define CLK    9 //Clock
#define LE     12  //Latch
#define OE     13

#define ENC_A  A4
#define ENC_B  A5
#define ENC_PORT PINF

int scaledCounter = 0;
int sequenceNumber = 5;
int bluPin = 6;  // Encoder's blue LED- D6 is PWM enabled


/*This is a 2 dimensional array with 3 LED sequences.  The outer array is the sequence; 
  the inner arrays are the values to output at each step of each sequence.  The output values
  are 16 bit hex values (hex math is actually easier here!).  An LED will be on if its 
  corresponding binary bit is a one, for example: 0x7 = 0000000000000111 and the first 3 LEDs 
  will be on.
  f
  The data type must be 'unsigned int' if the sequence uses the bottom LED since it's value is 0x8000 (out of range for signed int).
*/

unsigned int sequence[6][16] = {
                                {0x0,0x2,0x4,0x8,0x10,0x20,0x40,0x80,0x100,0x200,0x400,0x800,0x1000,0x2000,0x4000},
                                
                                {0x7fff,0x3fff,0x1fff,0xfff,0x7ff,0x3ff,0x1ff,0xff,0x7f,0x3f,0x1f,0xf,0x7,0x3,0x1,0x0},
                                {0x0,0x7fff,0x3ffe,0x1ffc,0xff8,0x7f0,0x3e0,0x1c0,0x80,0x1c0,0x3e0,0x7f0,0xff8,0x1ffC,0x3ffe,0x7fff},
                                {0x8000,0xC000,0xE000,0xF000,0xF800,0xFC00,0xFE00,0xFF00,0xFF80,0xFFC0,0xFFE0,0xFFF0,0xFFF8,0xFFFC,0xFFFE,0xFFFF},
                                {0x80,0x60,0x70,0x78,0x7C,0x7E,0x7F,0xC70F,0x207F,0x307F,0x387F,0x3C7F,0x3E7F,0x3F7F,0x3FFF,0x0},
                                {0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF,0x80FF,0xC0FF,0xE0FF,0xF0FF,0xF8FF,0xFCFF,0xFEFF,0xFFFF}
                               };
                               
                               
void setup()
{
    //Set SPI pins to output
    pinMode(SDI, OUTPUT);
    pinMode(CLK, OUTPUT);
    pinMode(LE, OUTPUT);
    pinMode(OE, OUTPUT);
    digitalWrite(OE, LOW);
    //Set encoder pins to input, turn internal pull-ups on
    pinMode(ENC_A, INPUT);
    digitalWrite(ENC_A, HIGH);
    pinMode(ENC_B, INPUT);
    digitalWrite(ENC_B, HIGH);
    pinMode(bluPin, OUTPUT);
    analogWrite(bluPin, 0);
    //Begin SPI
    SPI.begin();
    //Set rate to 115200
    Serial.begin(115200);
    Serial.setTimeout(50);
}

void loop()
{
  //Local Variables
  static uint8_t counter = 0;
  static int8_t tmpdata;
  static int value;
  
  tmpdata = read_encoder();
  
  //If the encoder has moved
  if(tmpdata)
    {
      //Send the counter over USB
      Serial.println(counter, DEC);
      //Set the new counter value of the encoder
      Serial.flush();
      counter += (tmpdata*4);
    }
     if(Serial.available())  //check the serial buffer
    {
      //Read the counter over USB
      counter=Serial.parseInt();
    }  
      //Scale the counter value for referencing the LED sequence
      scaledCounter = map(counter,0,255,0,15);      
      //Send the LED output to the shift register
      digitalWrite(LE, LOW);    
      shiftOut16(sequence[sequenceNumber][scaledCounter]);
      digitalWrite(LE, HIGH);
      Serial.flush();
    }
    
   
 
 
 int8_t read_encoder()
 {
    //int8_t enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
    int8_t enc_states[] = {0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0};
    static uint8_t old_AB = 0;
    
    old_AB <<= 2;      //remember previous state
    old_AB |= (ENC_PORT & 0x03); //add current state
    return ( enc_states[( old_AB & 0x0f )]);
 }
 
 
 void shiftOut16(uint16_t data)
{
  byte datamsb;
  byte datalsb;
  
  // Isolate the MSB and LSB
  datamsb = (data&0xFF00)>>8;  // mask out the MSB and shift it right 8 bits
  datalsb = data & 0xFF;  // Mask out the LSB
  
  // First shift out the MSB, MSB first.
  shiftOut(SDI, CLK, MSBFIRST, datalsb);
  // Then shift out the LSB
  shiftOut(SDI, CLK, MSBFIRST, datamsb);
}
  


