#include <LiquidCrystal_I2C.h> 
#include <Servo.h> 
#include <Wire.h> 
 
// LCD and Servo setup 
LiquidCrystal_I2C lcd(0x27, 16, 2); 
Servo myservo; 
 
#define ir_enter 2  // IR sensor for entry 
#define ir_back 4   // IR sensor for exit 
 
#define ir_car1 5 
#define ir_car2 6 
#define ir_car3 7 
#define ir_car4 8 
#define ir_car5 9 
#define ir_car6 10 
 
int S1 = 0, S2 = 0, S3 = 0, S4 = 0, S5 = 0, S6 = 0; 
int flag1 = 0;  // Flag to check if a car is inside 
int flag2 = 0;  // Flag to check if the gate is already opened 
int slot = 6; 
 
void setup()  
{ 
    Serial.begin(9600); 
    pinMode(ir_car1, INPUT); 
    pinMode(ir_car2, INPUT); 
    pinMode(ir_car3, INPUT); 
    pinMode(ir_car4, INPUT); 
    pinMode(ir_car5, INPUT); 
    pinMode(ir_car6, INPUT); 
    pinMode(ir_enter, INPUT); 
    pinMode(ir_back, INPUT); 
 
    myservo.attach(3); 
    myservo.write(90); // Start with the gate closed 
    lcd.backlight(); 
    lcd.begin(16, 2); 
     
    lcd.setCursor(0, 0); 
    lcd.print("   Car Parking   "); 
    lcd.setCursor(0, 1); 
    lcd.print("    System      "); 
    delay(1000); 
    lcd.clear(); 
    Read_Sensor(); 

    int total = S1 + S2 + S3 + S4 + S5 + S6; 
    slot = slot - total; 
 
    lcd.setCursor(0, 0); 
    lcd.print("  Have Slot: "); 
    lcd.print(slot); 
    lcd.print("   "); 
     
    displaySlots(); 
} 
 
void loop() { 
    // Continuously read sensor data and update the display 
    Read_Sensor(); 
    displaySlots(); 
 
    // Check for car entry 
    if (digitalRead(ir_back) == LOW || flag2 == 0) { // When IR sensor detects 
a car 
        flag2 = 1; // Set flag to indicate the gate is open 
        if (flag1 == 0) { 
            myservo.write(180); // Open the gate 
            slot++; // Increase slot count 
            Serial.println("Car entered"); // Log entry 
            delay(2000); // Delay to keep gate open 
            myservo.write(90); // Close the gate 
            flag1 = 1; // Set flag to indicate a car is now in 
        } 
    } 
 
    // Check for car exit 
    if (digitalRead(ir_enter) == LOW && flag1 == 1) { // When IR sensor 
detects a car exiting 
        flag1 = 0; // Reset the flag for car exit 
        myservo.write(180); // Open the gate 
        slot--; // Decrease slot count 
        Serial.println("Car exited"); // Log exit 
        delay(1000); // Delay to keep gate open 
        myservo.write(90); // Close the gate 
        flag2 = 0; // Reset flag to indicate the gate is closed 
    } 
 
    // Servo control based on IR sensor detection (optional) 
    // You can add any additional control here if needed. 
} 
 
void Read_Sensor() { 
    S1 = S2 = S3 = S4 = S5 = S6 = 0; 
 
    if (digitalRead(ir_car1) == 0) { S1 = 1; } 
    if (digitalRead(ir_car2) == 0) { S2 = 1; } 
    if (digitalRead(ir_car3) == 0) { S3 = 1; } 
    if (digitalRead(ir_car4) == 0) { S4 = 1; } 
    if (digitalRead(ir_car5) == 0) { S5 = 1; } 
    if (digitalRead(ir_car6) == 0) { S6 = 1; } 
} 
 
void displaySlots() { 
    lcd.setCursor(0, 1); 
    lcd.print("S1:"); lcd.print(S1 == 1 ? "Fill " : "Empty "); 
    lcd.setCursor(10, 1); 
    lcd.print("S2:"); lcd.print(S2 == 1 ? "Fill " : "Empty "); 
    delay(2000); 
 
    lcd.setCursor(0, 3); 
    lcd.print("S3:"); lcd.print(S3 == 1 ? "Fill " : "Empty "); 
    lcd.setCursor(10, 3); 
    lcd.print("S4:"); lcd.print(S4 == 1 ? "Fill " : "Empty "); 
    delay(2000); 
 
    lcd.setCursor(0, 3); 
    lcd.print("S5:"); lcd.print(S5 == 1 ? "Fill " : "Empty "); 
    lcd.setCursor(10, 3); 
    lcd.print("S6:"); lcd.print(S6 == 1 ? "Fill " : "Empty "); 
    delay(2000); 
} 