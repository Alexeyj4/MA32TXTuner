#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>

//configuration variable:
const int i_pin=16; //I sensor I pin A2
const int buzzer_pin=11; //Buzzer pin
const int i_filter=64; //Filter for i measuring (average)
const int tx_thtreshold=170; //threshold to determine TX on
const int short_threshold=1000; //threshold to determine short cirquit


const int SCREEN_WIDTH=128; // OLED display width, in pixels
const int SCREEN_HEIGHT=64; // OLED display height, in pixels
const int first_string=12;  //first string on LCD
const int second_string=28;  //second string on LCD
const int third_string=44;  //third string on LCD
const int fourth_string=62;  //fourth string on LCD

int i=0;//average i
int i_min=1023; //minimum i measured
int i_delta=0;
int tone_iterator=0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

void setup() {
  // put your setup code here, to run once:
  analogRead(i_pin);//init for increase accurace
  
  pinMode(i_pin, INPUT);
//  analogReference(INTERNAL);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setFont(&FreeSerif9pt7b);
  display.setTextSize(1);             
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.display();
}

void i_measure(){    
  long i_sum=0;
  for(int i_iter=0;i_iter<i_filter;i_iter++){
    i_sum=i_sum+analogRead(i_pin);    
  }
  i=int(float(i_sum)/float(i_filter));
}

void loop() {
  // put your main code here, to run repeatedly:

  i_measure();

  //determain turn TX on:
  if( (i_min==1023) and (i>tx_thtreshold) ){
    display.fillRect(0, 0, SCREEN_WIDTH, first_string+2, BLACK);
    display.setCursor(0,first_string);
    display.print("wait...");
    display.display();    
    delay(3000);
    i_measure();
    display.fillRect(0, 0, SCREEN_WIDTH, first_string+2, BLACK);
    display.display();  
  } 

  display.fillRect(0, 0, SCREEN_WIDTH, first_string+2, BLACK);
  display.setCursor(0,first_string);
  display.print(i);
  display.display();

  if(i>short_threshold){
    display.fillRect(0, 0, SCREEN_WIDTH, first_string+2, BLACK);
    display.setCursor(0,first_string);
    display.print("OVERLOAD!");
    display.display();
    tone(buzzer_pin,1000,3000);
    delay(3000);   
    i=0;
    i_min=1023;
  }

  if(i<tx_thtreshold){
    i_min=1023;    
    display.fillRect(0, first_string+2, SCREEN_WIDTH, second_string, BLACK);
    display.setCursor(0,second_string);
    display.print("TX off");
    display.display();    
  } 
  
  if(i>tx_thtreshold){
    if(i<i_min){
      i_min=i;  
    }    
    display.fillRect(0, first_string+2, SCREEN_WIDTH, second_string, BLACK);
    display.setCursor(0,second_string);
    display.print(i_min);
    display.print(" min");
    display.fillRect(0, third_string, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
    display.fillRect(0, third_string, (i-i_min)*3, SCREEN_HEIGHT, WHITE);
    
    display.display();
    
  }

  i_delta=i-i_min;
  if(i_delta>0){
    if(tone_iterator>(10-i_delta)){
      tone(buzzer_pin,17000,1);
      tone_iterator=0;
    }
    tone_iterator++;
    
  }
  

}
