#include <IRremote.h>
#include <LiquidCrystal.h>
#include <DS3231.h>
#include <Keypad.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10,9); 

const int rs = 43, en = 45, d4 = 47, d5 = 49, d6 = 51, d7 = 53;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

DS3231  rtc(SDA, SCL);
Time  t;

const int RECV_PIN=2;
IRrecv irrecv(RECV_PIN);
decode_results results;

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {22,24,26,28}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {30,32,34,36}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
#define Light 42
#define TV 50
#define BULB 48
#define AC  46 
#define input1  A3
#define input2  A4
#define input3  A5
#define feedback1 A8
#define feedback2 A9



int i=0,j=0,k=0,n=0,state1=0,state2=0,state3=0,state4=0,count=0,count1=0 ;
int previous1,previous2,previous3,previous4;

String output;
String myno_call;
String myno_sms;
String ss;
String on_text="on";
String off_text="off";
String reset_text="reset";
String on_var;
String off_var;
String reset_var;
String det_sms;
String det_call;
bool motor_status=true;

char input;
String your="8329354665";
bool call_avlbl;
bool sms_avlbl;
String p;


void setup() {

      pinMode(AC, OUTPUT);
      pinMode(BULB, OUTPUT);
      pinMode(Light, OUTPUT);
      pinMode(TV, OUTPUT);
      pinMode(input1,INPUT);
      pinMode(input2,INPUT);
      pinMode(input3,INPUT);
      
      irrecv.enableIRIn(); // Start the receiver
      irrecv.blink13(true); 
       
      Serial.begin(9600); 
      mySerial.begin(9600);
      delay(100);
      mySerial.println("AT+CNMI=2,2,0,0,0\r" );
      delay(100);
      mySerial.println("AT+CMGF=1");
      delay(100);
      mySerial.println("AT+CMGF=2");
      delay(100);
      mySerial.println("AT+CMGF=3");
      delay(100);
      mySerial.println("AT+CMGR=1");
      delay(100);
      mySerial.println("AT+CMGR=2");
      delay(100);
      mySerial.println("AT+CMGR=3");
      delay(100);
      mySerial.println("AT+CMGD=1");
      delay(100);
      mySerial.println("AT+CMGD=2");
      delay(100);
      mySerial.println("AT+CMGD=3");
      delay(100);
      mySerial.write("AT+CMGF=1\r\n");
      delay(100);
      mySerial.println("AT+CLIP=1");
      delay(100);

      lcd.begin(16, 2);
      lcd.print("Remote Controlled");
      lcd.setCursor(0,1);
      lcd.print("Home Automation");
      delay(2000);
      lcd.clear();
      lcd.print("Circuit Ready");
      lcd.setCursor(0,1);
      delay(1000);
      lcd.print("System Ready...");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(" Light ");
      lcd.setCursor(0,1);
      lcd.print("   OFF  ");
     
      rtc.begin();

      Serial.println(analogRead(A2));

  
}

void loop() {
  Serial.println("Ready");
  if(count1%600==0){
      Serial.println (analogRead(feedback1));
  }
      count1++;
      previous1 = digitalRead(input1);
      previous2 = digitalRead(input2);
      previous3 = digitalRead(input3);
  
      t = rtc.getTime();
      char key = keypad.getKey(); 
  if(digitalRead(input1)!=previous1){
      tp1();
    }
  if(digitalRead(input2)!=previous2){    
      tp2();
    }
  if(digitalRead(input3)!=previous3){
      tp3();
    }
    
  if (irrecv.decode(&results)) 
   {
  if(results.value==0x38863BE0) //Key 1    
    {
      tp1();
    }  
  if(results.value==0x38863BD0) //Key 2  
    {
      tp2();
    }   
  if(results.value==0x38863BF0) //Key 3   
    {
      tp3();
    }
    
  if(results.value==0x38863BC8) //Key 4  
    {
      tp4();
    }
    irrecv.resume();
   }
 if (key!=NO_KEY)
   {
   if(key=='A'){
    tp1();
    }
    if(key=='B'){
    tp2();
    }
    if(key=='C'){
    tp3();
    }
    if(key=='D'){
    tp4();
    }
  }
 if(mySerial.available())                  // If data available on soft Serial,print it on hard Serial
 {
    output=mySerial.readString();
    Serial.println(output);

    det_sms=output.substring(3,6);
    det_call=output.substring(3,5);
       
    if(det_sms=="CMT")
          {
            sms_avlbl=true;
            Serial.println("sms recived");
            myno_sms=output.substring(12,22);
            on_var= output.substring(51,53);
            off_var= output.substring(51,54);
            reset_var= output.substring(51,56);
            
            if(myno_sms==your)
            {  
              Serial.println(myno_sms);
              if(on_var==on_text )
              {
                Serial.println("in a on condition");
                Serial.println(analogRead(feedback1));
                tp1();
              }
               if(off_var==off_text )
              {
            tp1();
              }
            }
        }
       
       if(det_call=="IN")
       {
        call_avlbl=true;
        Serial.println("call recevied");
        String strk = output;
        strk.remove(1, 17);
        strk.remove(14); 
        strk.remove(0,4);
        if (strk =="8329354665")
        {  
          tp1();
        }
      }
    p=output;
    Serial.println(output);  
    }
    
if(count%80==0)
 {
    lcd.setCursor(0,1);
    lcd.print(t.hour, DEC);
    lcd.print(":");
    lcd.print(t.min, DEC);
    lcd.print(":"); 
    lcd.print(t.sec, DEC);
 
 }
 count++;
if(count1%600==0)
{
 Serial.println (analogRead(feedback1));
  }
  count1++;
}


void tp1()
{
  if(analogRead(feedback1)>400)
   {
    state1=1;
    operate1();
    }
  else
   {
    state1=0;
    operate1();
    }
       
  }

void tp2()
{
  if(analogRead(feedback1)>400)
   {
    state2=1;
    operate2(); 
    }
  else
   {
    state2=0;
    operate2(); 
    }
      
  }
void tp3()
{
  if(analogRead(feedback1)>400)
    {
    state3=1;
    operate3(); 
    }
  else{
      state3=0;
      operate3(); 
      }
      
  }
void tp4()
{
  if(analogRead(feedback1)>400)
    {
    state4=1;
    operate4(); 
    }
  else
   {
    state4=0;
    operate4(); 
    }
      
  }

  
void operate1()
{
  
  if(state1==1)
   {
    digitalWrite(Light,0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" LIGHT ON  ");
    state1=0;
    }
  else if(state1==0)
   {
   digitalWrite(Light,1);
   state1=1;
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("LIGHT OFF ");
    }
  }
  
void operate2()
{
  
  if(state2==1)
    {
    digitalWrite(TV,0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" TV ON  ");
    }
  else if(state2==0)
    {
    digitalWrite(TV,1);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("TV OFF ");
    }
  }
  
void operate3(){
  
  if(state3==1)
   {
    digitalWrite(BULB,0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" BULB OFF  ");
    }
  else if(state3==0)
   {
    digitalWrite(BULB,1);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("BULB ON ");
    }
  }
  
void operate4()
{
  
  if(state4==1)
   {
    digitalWrite(AC,0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" AC OFF  ");
    }
  else if(state4==0)
   {
    digitalWrite(AC,1);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("LIGHT ON ");
    }
  }
