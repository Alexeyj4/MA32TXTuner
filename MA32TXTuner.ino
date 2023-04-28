#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>

const int i_pin=16; //I sensor I pin A2
const int buzzer_pin=11; //Buzzer pin
const int i_filter=4; //Filter for i measuring (average)
const int tx_thtreshold=100; //threshold to determine TX on


const int SCREEN_WIDTH=128; // OLED display width, in pixels
const int SCREEN_HEIGHT=64; // OLED display height, in pixels
const int first_string=12;  //first string on LCD
const int second_string=28;  //second string on LCD
const int third_string=44;  //third string on LCD
const int fourth_string=62;  //fourth string on LCD

int i_iter=0;
long i_sum=0;
int i=0;//average i
int i_min=1023; //minimum i measured
int beep_duration;

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
  display.display();
}



void loop() {
  // put your main code here, to run repeatedly:
  i_sum+=analogRead(i_pin);
  i_iter+=1;
  if(i_iter>=i_filter){
    i=int(float(i_sum)/float(i_filter));
    i_sum=0;
    i_iter=0;    
  }
  
  display.fillRect(0, 0, SCREEN_WIDTH, first_string+2, BLACK);
  display.setCursor(0,first_string);
  display.print(i);
  display.display();

  if(i<i_min){
    i_min=i;
    display.fillRect(0, first_string+2, SCREEN_WIDTH, second_string, BLACK);
    display.setCursor(0,second_string);
    display.print(i_min);
    display.display();
  }
  if(i<tx_thtreshold){
    i_min=1023;
    delay(800);
  }
  

  beep_duration=i-i_min;
  if(beep_duration>50)beep_duration=50;
  if(i>=(i_min+1))tone(buzzer_pin,10000,beep_duration);
  

}
