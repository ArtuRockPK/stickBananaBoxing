/*
 * Fruit Ninja — StickC Plus 2 BLE IMU Controller
 * IMU: 200Hz, дисплей только статус подключения
 * 
 * Board: M5StickC Plus 2 (ESP32-PICO-V3-02)
 * IMU: BMI270 (6-axis)
 * Display: 135x240 ST7789V2
 */

#include <M5Unified.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHAR_TX_UUID "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHAR_RX_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

// LED на StickC Plus 2
#define LED_PIN 19

const char*    BLE_NAME        = "FruitNinja";
const uint32_t IMU_INTERVAL_MS = 5;       // 200 Hz
const float    SWING_THRESHOLD = 250.0f;

BLECharacteristic* txChar        = nullptr;
volatile bool      connected     = false;
volatile bool      updateDisplay = true;

float gx=0, gy=0, gz=0, ax=0, ay=0, az=0;

// ── BLE Callbacks ─────────────────────────────────────────────────────────────
class ServerCB : public BLEServerCallbacks {
  void onConnect(BLEServer*) override {
    connected = true; 
    updateDisplay = true;
    digitalWrite(LED_PIN, LOW);  // LED ON (active low)
    Serial.println("[BLE] Connected");
  }
  void onDisconnect(BLEServer*) override {
    connected = false; 
    updateDisplay = true;
    digitalWrite(LED_PIN, HIGH); // LED OFF
    BLEDevice::startAdvertising();
    Serial.println("[BLE] Disconnected");
  }
};

class RxCB : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* c) override {
    Serial.printf("[RX] %s\n", c->getValue().c_str());
  }
};

// ── Дисплей — только статус ──────────────────────────────────────────────────
void drawStatus() {
  M5.Lcd.fillScreen(TFT_BLACK);
  
  // Заголовок
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.print("Fruit Ninja");
  
  // Статус подключения
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(10, 50);
  if (connected) {
    M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
    M5.Lcd.print("Connected!");
  } else {
    M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
    M5.Lcd.print("Waiting...");
  }
  
  // IMU тип
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(TFT_DARKGREY, TFT_BLACK);
  M5.Lcd.setCursor(10, 90);
  auto imuType = M5.Imu.getType();
  M5.Lcd.printf("IMU: %s", 
    imuType == m5::imu_t::imu_bmi270 ? "BMI270" :
    imuType == m5::imu_t::imu_mpu6886 ? "MPU6886" :
    imuType == m5::imu_t::imu_mpu6050 ? "MPU6050" : "Unknown");
  
  // Подсказка
  M5.Lcd.setCursor(10, 110);
  M5.Lcd.print("Btn M5 = calibrate");
}

// ── Setup ─────────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  delay(200);

  // M5Unified конфиг
  auto cfg = M5.config();
  cfg.internal_imu = true;  // Включить внутренний IMU
  M5.begin(cfg);

  // LED setup
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // OFF

  // Выключаем динамик чтобы не жрал батарею
  M5.Speaker.end();

  // Экран
  M5.Lcd.setRotation(1);  // Горизонтальная ориентация
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.print("Starting...");

  // Проверка IMU
  auto imuType = M5.Imu.getType();
  Serial.printf("[IMU] type: %d\n", (int)imuType);
  if (imuType == m5::imu_t::imu_none) {
    M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
    M5.Lcd.setCursor(10, 40);
    M5.Lcd.print("IMU ERROR!");
    Serial.println("[IMU] Not found!");
  }

  // BLE инициализация
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
  
  // Обновляем IMU каждый тик
  M5.Imu.update();

  uint32_t now = millis();

  // Отправляем IMU данные с частотой 200Hz
  if (now - lastSend >= IMU_INTERVAL_MS) {
    lastSend = now;

    auto d = M5.Imu.getImuData();
    gx = d.gyro.x;  gy = d.gyro.y;  gz = d.gyro.z;
    ax = d.accel.x; ay = d.accel.y; az = d.accel.z;

    float mag   = sqrtf(gx*gx + gy*gy + gz*gz);
    bool  swing = mag > SWING_THRESHOLD;

    if (connected && txChar) {
      char buf[96];
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

  // Обновляем дисплей только при изменении статуса
  if (updateDisplay) {
    updateDisplay = false;
    drawStatus();
  }

  // Кнопка M5 (BtnA) — калибровка
  if (M5.BtnA.wasPressed()) {
    Serial.println("[BTN] M5 pressed — sending cal");
    if (connected && txChar) {
      char buf[96];
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
  
  // Кнопка сбоку (BtnB) — можно добавить функционал
  if (M5.BtnB.wasPressed()) {
    Serial.println("[BTN] Side button pressed");
    // Например, показать батарею
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(TFT_CYAN, TFT_BLACK);
    M5.Lcd.setCursor(10, 50);
    int batLevel = M5.Power.getBatteryLevel();
    M5.Lcd.printf("Battery: %d%%", batLevel);
    delay(1500);
    drawStatus();
  }
}
