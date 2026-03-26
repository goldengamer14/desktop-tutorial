// const int red_led = D1;
// const int green_led =D2;
// const int yellow_led = D3;

//      void setup()
//     {
     
//      pinMode(red_led, OUTPUT);
//      pinMode(yellow_led, OUTPUT);
//      pinMode(green_led, OUTPUT);
//     }


//     void loop()
//     {
    
//       digitalWrite(red_led,HIGH);
//       delay(2000);
//       digitalWrite(red_led, LOW);
    
//       digitalWrite(yellow_led,HIGH);
//       delay(1500);
//       digitalWrite(yellow_led, LOW);

//       digitalWrite(green_led,HIGH);
//       delay(1500);
//       digitalWrite(green_led, LOW);
      
//     }

const int RED_LED_PIN = D1,  GREEN_LED_PIN = D2, BLUE_LED_PIN = D3;
    

#define RED_LED_MODE (mode & 1)

#define GREEN_LED_MODE (mode & 2)

#define BLUE_LED_MODE (mode & 4)

uint8_t mode = 1;

void setup() {

// Define PINMODE of the LED to be OUTPUT

pinMode(RED_LED_PIN, OUTPUT);

pinMode(GREEN_LED_PIN, OUTPUT);

pinMode(BLUE_LED_PIN, OUTPUT);

}

void loop()
{

digitalWrite(RED_LED_PIN, RED_LED_MODE);

digitalWrite(GREEN_LED_PIN, GREEN_LED_MODE);

digitalWrite(BLUE_LED_PIN, BLUE_LED_MODE);

delay(500);

mode = (mode + 1) % 8;

}