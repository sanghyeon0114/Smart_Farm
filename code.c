#include <SoftwareSerial.h>
#include <DHT11.h>

#define RXD_PIN 2
#define TXD_PIN 3

SoftwareSerial bt(RXD_PIN, TXD_PIN);

int light = HIGH;

void readBluetooth() {
  char data;
  int count = 0;

  while(bt.available()) {
    data = bt.read(); 
    if(data == -1)
      break;
    if(data == 'L') {
      if(light) {
        Serial.println("Light OFF");
        light = LOW;
      } else {
        Serial.println("Light ON");
        light = HIGH;
      }
    }
  }

}

DHT11 dht11(A2);

int soil_pin0 = A0;
int soil_pin1 = A1;

int relay[] = {0, 13, 12, 11, 10, 9, 8};
int relayState[] = {0, LOW, LOW, LOW, LOW, LOW ,LOW};
int relayIndex = 1;

void setRelay(int n, int state) {
  digitalWrite(relay[n], state);
  relayState[n] = state;
}


int tick = 0;

void setup()
{ 
  Serial.begin(9600);
  bt.begin(9600);
}

void loop() { 
  int soil0 = analogRead(soil_pin0), soil1 = analogRead(soil_pin1);
  float temp, humi;
  int result;
  if(tick % 1000 == 0) {
    // 온습도센서는 1초보다 더 짧은 간격으로 측정 시, 에러 발생
    result = dht11.read(humi, temp);
  }
  // if(tick % 100 == 0) {
  //   // 릴레이 작동 확인
  //   relayIndex = relayIndex % 7 == 0 ? 1 : relayIndex+1;
  //   setRelay(relayIndex, HIGH);
  //   setRelay((relayIndex - 1) % 7, LOW);
  // }

  //////////////////////////CODE_START////////////////////////////////
  //온도 30보다 크면 FAN On.
  if(temp >= 30) {
    setRelay(2, HIGH);
  } else {
    setRelay(2, LOW);
  }

  // 습도 30보다 작으면 벨브1 On.
  if(humi >= 30) {
    setRelay(3, HIGH);
  } else {
    setRelay(3, LOW);
  }

  // 토양습도센서1 수치 800보다 크면 ( 물이 너무 없는 경우 ) 벨브2 On.
  if(soil0 >= 800) {
    setRelay(4, HIGH);
  } else {
    setRelay(4, LOW);
  }

  // 토양습도센서2 수치 400보다 크면 ( 물이 너무 없는 경우 ) 벨브3 On.
  if(soil1 >= 400) {
    setRelay(5, HIGH);
  } else {
    setRelay(5, LOW);
  }

  if(light) {
    int onTime = 1000*5;
    int offTime = 1000*1;
    int lightTime = tick % (onTime + offTime);

    if(lightTime == 0) {
      setRelay(6, HIGH);
    } else if(lightTime == onTime) {
      setRelay(6, LOW);
    }
  } else {
    setRelay(6, LOW);
  }

  //////////////////////////CODE_END////////////////////////////////

  //토양습도센서 2개, 온도 습도 측정 결과 출력
  if(tick % 1000 == 0) {
    if (result == 0) {
      bt.print(temp);
      bt.print(" \'C|");
      bt.print(humi);
      bt.print(" %|");
    } else {
      bt.print("Nane|Nane|");
    }
    bt.print(soil0);
    bt.print("|");
    bt.print(soil1);
    bt.print("|");
    bt.print(light == 1 ? "ON" : "OFF");
  }
  
  readBluetooth();

  // 24시간마다 tick을 0으로 초기화. ( int 오버플로우 방지. )
  if(tick >= 1000*60*60*24) {
    tick = 0;
  }
  tick++;
  delay(1);
}
