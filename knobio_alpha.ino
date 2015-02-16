#include <SPI.h>

#define SDI    11 //Data
#define CLK    13 //Clock
#define LE     2  //Latch

#define ENC_A  8
#define ENC_B  9
#define ENC_PORT PINB

int scaledCounter = 0;
int sequenceNumber = 0;

/*This is a 2 dimensional array with 3 LED sequences.  The outer array is the sequence; 
  the inner arrays are the values to output at each step of each sequence.  The output values
  are 16 bit hex values (hex math is actually easier here!).  An LED will be on if its 
  corresponding binary bit is a one, for example: 0x7 = 0000000000000111 and the first 3 LEDs 
  will be on.
  f
  The data type must be 'unsigned int' if the sequence uses the bottom LED since it's value is 0x8000 (out of range for signed int).
*/

unsigned int sequence[3][16] = {{0x0,0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80,0x100,0x200,0x400,0x800,0x1000,0x2000,0x4000},
                                {0x0,0x1,0x3,0x7,0xf,0x1f,0x3f,0x7f,0xff,0x1ff,0x3ff,0x7ff,0xfff,0x1fff,0x3fff,0x7fff},
                                {0x0,0x7fff,0x3ffe,0x1ffc,0xff8,0x7f0,0x3e0,0x1c0,0x80,0x1c0,0x3e0,0x7f0,0xff8,0x1ffC,0x3ffe,0x7fff},
                               };
                               
void setup()
{
    //Set SPI pins to output
    pinMode(SDI, OUTPUT);
    pinMode(CLK, OUTPUT);
    pinMode(LE, OUTPUT);
    //Set encoder pins to input, turn internal pull-ups on
    pinMode(ENC_A, INPUT);
    digitalWrite(ENC_A, HIGH);
    pinMode(ENC_B, INPUT);
    digitalWrite(ENC_B, HIGH);
    //Begin SPI
    SPI.begin();
    //Set rate to 115200
    Serial.begin(115200);
}

void loop()
{
  //Local Variables
  static uint8_t counter = 0;
  static int8_t tmpdata;
  static int value;
  
  tmpdata = read_encoder();
  
  //If the encoer has moved
  if(tmpdata)
    {
      //Send the counter over USB
      Serial.println(counter, DEC);
      //Set the new counter value of the encoder
      Serial.flush();
      //multiply the counter by 4 for more increments per click
      counter += (tmpdata*4);
    }
     if(Serial.available())  //check the serial buffer
    {
      //Read the counter over USB
      counter=Serial.parseInt();
    }  
      //Scale the counter value for referencing the LED sequence
      scaledCounter = map(counter,0,255,16,31);
      
      //Send the LED output to the shift register
      digitalWrite(LE, LOW);
      SPI.transfer((sequence[sequenceNumber][scaledCounter] >> 8 ));
      SPI.transfer(sequence[sequenceNumber][scaledCounter]);
      digitalWrite(LE, HIGH);
      Serial.flush();
 
    }
 
 int8_t read_encoder()
 {
    int8_t enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
    static uint8_t old_AB = 0;
    
    old_AB <<= 2;      //remember previous state
    old_AB |= (ENC_PORT & 0x03); //add current state
    return ( enc_states[( old_AB & 0x0f )]);
 }
 
 
    
