#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

int ldr_sensor = A0;
unsigned int sensor_duration = 0;
int sensor_value = 0;
int mute_button = 2;
int alarm = 7;
int button_state = 1;
int bits[4];

int get_bit(int val){
  if(val < 500){
    return 1;
  }else{
    return 0;
  }
}

void setup()
{
  pinMode(7, OUTPUT);
  pinMode(4, INPUT);
  pinMode(2,INPUT_PULLUP);
  digitalWrite(7, LOW);
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.print("Welcome");
  Serial.begin(9600);
}

void loop()
{
  button_state = digitalRead(2);
  if(!button_state){
    delay(5000);
    digitalWrite(7, LOW);
    lcd.clear();
  }
  sensor_value = analogRead(ldr_sensor);
  bits[0] = get_bit(sensor_value);
  if(bits[0]){
    delay(250);
    sensor_value = analogRead(ldr_sensor);
    bits[1] = get_bit(sensor_value);
    delay(250);
    sensor_value = analogRead(ldr_sensor);
    bits[2] = get_bit(sensor_value);
    delay(250);
    sensor_value = analogRead(ldr_sensor);
    bits[3] = get_bit(sensor_value);

    if(bits[3]){
      //Magnetic
      Serial.println("Magnetic");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Door is Opened");
      digitalWrite(7, HIGH);

    }else if(bits[2]){
      //Fume
      Serial.println("Fume");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Fire");
      digitalWrite(7, HIGH);
    }else if(bits[0]){
      //Ultrasonic
      Serial.println("Ultrasonic");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("An intruder");
      lcd.setCursor(0, 1);
      lcd.print("is detected");
      digitalWrite(7, HIGH);
    }
  }
  delay(250);
}
