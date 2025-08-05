#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include "product_data.h"

// RFID Module Pins
#define RST_PIN         9
#define SS_PIN          10

// LCD Configuration (I2C)
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// Buzzer Pin
#define BUZZER_PIN      8

// Anti-theft gate pins
#define GATE_ENTRY_PIN  6
#define GATE_EXIT_PIN   7

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

// Shopping cart variables
float totalAmount = 0.0;
int itemCount = 0;
String cartItems[MAX_PRODUCTS];
float cartPrices[MAX_PRODUCTS];
int cartQuantities[MAX_PRODUCTS] = {0};

void setup() {
  Serial.begin(9600);
  SPI.begin();          // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // Initialize buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Initialize anti-theft gate pins
  pinMode(GATE_ENTRY_PIN, INPUT_PULLUP);
  pinMode(GATE_EXIT_PIN, INPUT_PULLUP);
  
  // Display welcome message
  lcd.setCursor(0, 0);
  lcd.print(" Smart Shopping  ");
  lcd.setCursor(0, 1);
  lcd.print("   Trolley v1.0  ");
  delay(2000);
  
  updateDisplay();
}

void loop() {
  // Check for new RFID cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String tagID = getRFIDTag();
    Serial.println("Scanned Tag: " + tagID);
    
    // Find product in database
    int productIndex = findProductByTag(tagID);
    
    if (productIndex != -1) {
      addToCart(productIndex);
      beep(1); // Positive feedback beep
    } else {
      lcd.clear();
      lcd.print("Product Not Found");
      beep(3); // Error beep
      delay(2000);
      updateDisplay();
    }
    
    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1(); // Stop encryption on PCD
  }
  
  // Anti-theft detection
  checkAntiTheft();
  
  // Check for cart reset (for demo purposes)
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'r') {
      resetCart();
    }
  }
}

String getRFIDTag() {
  String tagID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    tagID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  tagID.toUpperCase();
  return tagID;
}

int findProductByTag(String tagID) {
  for (int i = 0; i < MAX_PRODUCTS; i++) {
    if (productDatabase[i].rfidTag == tagID) {
      return i;
    }
  }
  return -1; // Not found
}

void addToCart(int productIndex) {
  // Check if item already in cart
  bool itemExists = false;
  for (int i = 0; i < itemCount; i++) {
    if (cartItems[i] == productDatabase[productIndex].name) {
      cartQuantities[i]++;
      itemExists = true;
      break;
    }
  }
  
  // If new item, add to cart
  if (!itemExists && itemCount < MAX_PRODUCTS) {
    cartItems[itemCount] = productDatabase[productIndex].name;
    cartPrices[itemCount] = productDatabase[productIndex].price;
    cartQuantities[itemCount] = 1;
    itemCount++;
  }
  
  // Update total amount
  totalAmount += productDatabase[productIndex].price;
  
  // Show added item on LCD
  lcd.clear();
  lcd.print("Added: " + productDatabase[productIndex].name.substring(0, 9));
  lcd.setCursor(0, 1);
  lcd.print("Price: $" + String(productDatabase[productIndex].price));
  delay(1500);
  
  updateDisplay();
}

void updateDisplay() {
  lcd.clear();
  
  if (itemCount == 0) {
    lcd.print("Scan Products");
    lcd.setCursor(0, 1);
    lcd.print("to begin");
    return;
  }
  
  // Show last item and total
  lcd.print("Items: " + String(itemCount));
  lcd.setCursor(0, 1);
  lcd.print("Total: $" + String(totalAmount));
}

void checkAntiTheft() {
  static bool inStore = true;
  
  // Check if trolley is passing through exit gate
  if (digitalRead(GATE_EXIT_PIN) == LOW) {
    if (itemCount > 0) {
      // Trigger alarm if items in cart when exiting
      lcd.clear();
      lcd.print("ALERT: Unpaid Items!");
      lcd.setCursor(0, 1);
      lcd.print("Please checkout");
      activateAlarm();
    }
    inStore = false;
  } else if (digitalRead(GATE_ENTRY_PIN) == LOW) {
    inStore = true;
  }
}

void activateAlarm() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);
  }
}

void beep(int count) {
  for (int i = 0; i < count; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    if (i < count-1) delay(100);
  }
}

void resetCart() {
  totalAmount = 0.0;
  itemCount = 0;
  for (int i = 0; i < MAX_PRODUCTS; i++) {
    cartQuantities[i] = 0;
  }
  updateDisplay();
  lcd.clear();
  lcd.print("Cart Cleared");
  delay(1000);
  updateDisplay();
}