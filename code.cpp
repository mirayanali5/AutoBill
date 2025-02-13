#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

#define RST_PIN 9     
#define SS_PIN 10     
#define BUTTON_PIN 4  
#define BUZZER_PIN 6  

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(SS_PIN, RST_PIN);   

#define PRODUCT1_UID "C3FC6632" 
#define PRODUCT2_UID "514E EECF" 

int product1Price = 100; 
int product2Price = 200;
int total = 0;           

void setup() {
    Serial.begin(9600);    
    SPI.begin();           
    mfrc522.PCD_Init();    
    lcd.init();            
    lcd.backlight();       

    pinMode(BUTTON_PIN, INPUT_PULLUP); 
    pinMode(BUZZER_PIN, OUTPUT);       
    pinMode(SS_PIN, OUTPUT);           

    Serial.println("Try AutoBill!!");
    lcd.setCursor(0, 0);
    lcd.print(" Try using ");
    lcd.setCursor(0, 1);
    lcd.print(" AutoBill!! ");
}

void loop() {
    static bool buttonPressed = false;

    if (digitalRead(BUTTON_PIN) == LOW) {
        if (!buttonPressed) {
            buttonPressed = true;
            total = 0; 
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(" Cart Cleared ");
            lcd.setCursor(0, 1);
            lcd.print(" Successfully ");
            delay(1500);
            lcd.clear();
            lcd.print("Scan Items Again");
            delay(1500);
            lcd.clear();
            lcd.print(" Try using ");
            lcd.setCursor(0, 1);
            lcd.print(" AutoBill!! ");
        }
    } else {
        buttonPressed = false;
    }

    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        Serial.print("UID tag: ");
        String content = "";
        
        for (byte i = 0; i < mfrc522.uid.size; i++) {
            content += String(mfrc522.uid.uidByte[i], HEX); 
        
        content.toUpperCase(); 
        Serial.println(content);
        lcd.clear();

        if (content == PRODUCT1_UID) {
            Serial.println("Rice added to cart");
            lcd.print("1kg Rice added");
            lcd.setCursor(0, 1);
            lcd.print(" to your cart ");
            tone(BUZZER_PIN, 1000, 100);
            total += product1Price;
        } else if (content == PRODUCT2_UID) {
            Serial.println("1L Milk added to cart");
            lcd.print("1L Milk added");
            lcd.setCursor(0, 1);
            lcd.print(" to your cart ");
            tone(BUZZER_PIN, 1500, 100);
            total += product2Price;
        } else {
            Serial.println("Unknown product");
            lcd.print("Unknown Item!");
        }

        delay(1500);
        noTone(BUZZER_PIN);

        lcd.clear();
        lcd.print("Item Price: ");
        if (content == PRODUCT1_UID) lcd.print(product1Price);
        else if (content == PRODUCT2_UID) lcd.print(product2Price);
        delay(1500);

        lcd.clear();
        lcd.print("Grand total: ");
        lcd.print(total);
        delay(1500);

        lcd.clear();
        lcd.print(" Thank You :) ");
        lcd.setCursor(0, 1);
        lcd.print(" for shopping ");
        delay(1500);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" Add More ");
        lcd.setCursor(0, 1);
        lcd.print(" Items ");
        delay(1500);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" Try using ");
        lcd.setCursor(0, 1);
        lcd.print(" AutoBill!! ");

        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
        delay(500);
    }
}
