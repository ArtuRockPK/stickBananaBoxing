/*
 * Fruit Ninja — StickS3 BLE IMU Controller v6
 * IMU: 200Hz, дисплей только статус подключения
 */

#include <M5Unified.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHAR_TX_UUID "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHAR_RX_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

const char*    BLE_NAME        = "FruitNinja";
const uint32_t IMU_INTERVAL_MS = 5;       // 200 Hz
const float    SWING_THRESHOLD = 250.0f;

BLECharacteristic* txChar        = nullptr;
volatile bool      connected     = false;
volatile bool      updateDisplay = true;

float gx=0,gy=0,gz=0,ax=0,ay=0,az=0;

// ── BLE Callbacks ─────────────────────────────────────────────────────────────
class ServerCB : public BLEServerCallbacks {
  void onConnect(BLEServer*) override {
    connected = true; updateDisplay = true;
    M5.Power.setLed(128);
    Serial.println("[BLE] Connected");
  }
  void onDisconnect(BLEServer*) override {
    connected = false; updateDisplay = true;
    M5.Power.setLed(0);
    BLEDevice::startAdvertising();
    Serial.println("[BLE] Disconnected");
  }
};

class RxCB : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* c) override {
    Serial.printf("[RX] %s\n", c->getValue().c_str());
  }
};

// ── Дисплей — только статус, не обновляется постоянно ──────────────────────
void drawStatus() {
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(1);

  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.setCursor(4, 2);
  M5.Lcd.print("Fruit Ninja");

  M5.Lcd.setCursor(4, 18);
  if (connected) {
    M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
    M5.Lcd.print("Connected!  ");
  } else {
    M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
    M5.Lcd.print("Waiting...  ");
  }

  // Подсказка по кнопке
  M5.Lcd.setTextColor(TFT_DARKGREY, TFT_BLACK);
  M5.Lcd.setCursor(4, 36);
  M5.Lcd.print("Btn A = center");
}

// ── Setup ─────────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  delay(200);

  auto cfg = M5.config();
  M5.begin(cfg);

  // Глушим динамик сразу
  M5.Speaker.end();
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);

  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(4, 4);
  M5.Lcd.print("Starting...");

  // IMU инициализируется внутри M5.begin()
  auto imuType = M5.Imu.getType();
  Serial.printf("[IMU] type: %d\n", (int)imuType);

  // BLE
  BLEDevice::init(BLE_NAME);
  BLEServer* server = BLEDevice::createServer();
  server->setCallbacks(new ServerCB());

  BLEService* service = server->createService(SERVICE_UUID);

  txChar = service->createCharacteristic(
    CHAR_TX_UUID, BLECharacteristic::PROPERTY_NOTIFY);
  txChar->addDescriptor(new BLE2902());

  BLECharacteristic* rxChar = service->createCharacteristic(
    CHAR_RX_UUID, BLECharacteristic::PROPERTY_WRITE);
  rxChar->setCallbacks(new RxCB());

  service->start();

  BLEAdvertising* adv = BLEDevice::getAdvertising();
  adv->addServiceUUID(SERVICE_UUID);
  adv->setScanResponse(true);
  adv->setMinPreferred(0x06);
  BLEDevice::startAdvertising();

  Serial.printf("[BLE] Advertising as \"%s\" @ 200Hz\n", BLE_NAME);
  drawStatus();
}

// ── Loop ──────────────────────────────────────────────────────────────────────
uint32_t lastSend = 0;

void loop() {
  M5.update();

  // ОБЯЗАТЕЛЬНО каждый тик
  M5.Imu.update();

  uint32_t now = millis();

  // Отправляем IMU данные
  if (now - lastSend >= IMU_INTERVAL_MS) {
    lastSend = now;

    auto d = M5.Imu.getImuData();
    gx = d.gyro.x;  gy = d.gyro.y;  gz = d.gyro.z;
    ax = d.accel.x; ay = d.accel.y; az = d.accel.z;

    float mag   = sqrtf(gx*gx + gy*gy + gz*gz);
    bool  swing = mag > SWING_THRESHOLD;

    if (connected && txChar) {
      char buf[92];
      snprintf(buf, sizeof(buf),
        "{\"gx\":%.2f,\"gy\":%.2f,\"gz\":%.2f,"
        "\"ax\":%.3f,\"ay\":%.3f,\"az\":%.3f,"
        "\"swing\":%s}",
        gx, gy, gz, ax, ay, az,
        swing ? "true" : "false"
      );
      txChar->setValue((uint8_t*)buf, strlen(buf));
      txChar->notify();
    }
  }

  // Дисплей — только когда статус меняется
  if (updateDisplay) {
    updateDisplay = false;
    drawStatus();
  }

  // Кнопка A — отправляем сигнал калибровки
  if (M5.BtnA.wasPressed()) {
    Serial.println("[BTN] A pressed — sending cal");
    if (connected && txChar) {
      char buf[92];
      snprintf(buf, sizeof(buf),
        "{\"gx\":%.2f,\"gy\":%.2f,\"gz\":%.2f,"
        "\"ax\":%.3f,\"ay\":%.3f,\"az\":%.3f,"
        "\"swing\":false,\"cal\":true}",
        gx, gy, gz, ax, ay, az
      );
      txChar->setValue((uint8_t*)buf, strlen(buf));
      txChar->notify();
    }
    // Визуальная обратная связь
    M5.Lcd.fillScreen(TFT_WHITE);
    delay(60);
    drawStatus();
  }
}
