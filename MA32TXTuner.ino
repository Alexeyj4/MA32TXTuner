#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>

const int i_pin=16; //I sensor I pin A2
const int buzzer_pin=11; //Buzzer pin
const int i_filter=100; //Filter for i measuring (average)


const int SCREEN_WIDTH=128; // OLED display width, in pixels
const int SCREEN_HEIGHT=64; // OLED display height, in pixels
const int first_string=12;  //first string on LCD
const int second_string=28;  //second string on LCD
const int third_string=44;  //third string on LCD
const int fourth_string=62;  //fourth string on LCD

int i_iter=0;
long i_sum=0; 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

void setup() {
  // put your setup code here, to run once:
  
  pinMode(i_pin, INPUT);
  analogReference(INTERNAL);


  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setFont(&FreeSerif9pt7b);
  display.setTextSize(1);             
  display.setTextColor(WHITE);  

  display.clearDisplay();
  display.setCursor(0,first_string);
  display.print("i=");
  display.display(); 

  //tone(buzzer_pin,4000);

}



void loop() {
  // put your main code here, to run repeatedly:
  display.clearDisplay();
  display.setCursor(0,first_string);
  display.print(analogRead(i_pin));
  display.display();

}
