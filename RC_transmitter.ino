#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define CE_PIN 9
#define CSN_PIN 10
#define switchPin 8
#define buttonPin 7
RF24 radio(CE_PIN, CSN_PIN);


struct Signal {
  byte xr;  // Sa? joystick sa?-sol hareketi
  byte yr;  // Sa? joystick yukar?-a?a?? hareketi
  byte xl;  // Sol joystick sa?-sol hareketi
  byte yl;  // Sol joystick yukar?-a?a?? hareketi
  byte aux; // Yard?mc? (potansiyometre) de?eri
  byte ssw; // Kayd?rma anahtar? durumu
  byte button; // D�?me durumu
};


Signal data;


byte mapToByte(int val) {
  return static_cast<byte>(map(val, 0, 1023, 0, 255));
}


void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(0xF0F0F0E1);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();


  pinMode(A7, INPUT); // xr (rightX)
  pinMode(A6, INPUT); // yr (rightY)
  pinMode(A4, INPUT); // xl (leftX)
  pinMode(A3, INPUT); // yl (leftY)
  pinMode(A2, INPUT); // aux (potansiyometre)
  pinMode(switchPin, INPUT); // ssw (slideswitch)
  pinMode(buttonPin, INPUT_PULLUP); // button (buton)
}


void loop() {
  data.xr = mapToByte(analogRead(A7));
  data.yr = mapToByte(analogRead(A6));
  data.xl = mapToByte(analogRead(A4));
  data.yl = mapToByte(analogRead(A3));
  data.aux = mapToByte(analogRead(A2));
  data.ssw = digitalRead(switchPin) == HIGH ? 1 : 0;
  data.button = digitalRead(buttonPin) == HIGH ? 1 : 0;
  radio.write(&data, sizeof(Signal));


  // HATA AYIKLAMA (DEBUG)
  Serial.print("sagy=");
  Serial.println(analogRead(A6));
  Serial.print("sagx=");
  Serial.println(analogRead(A7));
  Serial.print("soly=");
  Serial.println(analogRead(A3));
  Serial.print("solx=");
  Serial.println(analogRead(A4));
  Serial.print("aux=");
  Serial.println(analogRead(A2));
  Serial.print("ssw=");
  Serial.println(digitalRead(switchPin) == HIGH ? "ON" : "OFF");
  Serial.print("button=");
  Serial.println(digitalRead(buttonPin) == HIGH ? "Pressed" : "Released");
  radio.write("Bu bir test mesajidir", sizeof("Bu bir test mesajidir"));  // Test mesaj?n? g�nder
}