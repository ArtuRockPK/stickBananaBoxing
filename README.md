#  Stick Banana Boxing

<img width="1194" height="896" alt="Banana Boxing" src="https://github.com/user-attachments/assets/87fb8913-bae4-4381-a8a0-e972ea0dc18e" />

**RU** | [EN](#-banana-boxing--en) | [中文](#chinese)

---

## 🍌 Banana Boxing — RU

Браузерная 3D-игра, в которой вы боксируете с бананом с помощью реального физического контроллера M5Stack. Контроллер передаёт данные гироскопа и акселерометра по Bluetooth, нейросеть на TensorFlow.js распознаёт тип удара и анимирует его в игре.

### Что входит в проект

| Файл / Папка | Описание |
|---|---|
| `gesture_trainer_tf.html` | Инструмент для записи ударов и обучения модели |
| `banana_boxing.html` | Игра — боксируй с бананом |
| `FruitNinja_StickCPlus2/` | Прошивка для M5Stack StickC Plus2 |
| `FruitNinja_StickS3/` | Прошивка для M5Stack StickS3 |

### Требования

- **Браузер:** Google Chrome (обязательно — нужен Web Bluetooth)
- **Контроллер:** M5Stack StickC Plus2 или M5Stack StickS3 с прошивкой FruitNinja
- **Интернет:** нужен для загрузки TensorFlow.js и Three.js

---

### Шаг 1 — Обучение модели (`gesture_trainer_tf.html`)

> Если у вас уже есть обученная модель (`gesture_model_tf.json`), пропустите этот шаг.

1. **Откройте** `gesture_trainer_tf.html` в Chrome.
2. **Установите прошивку на StickCPlus2 или StickS3 в M5Burner. Что бы найти прошивку в списке в поиске пишите Banana Boxing**
3. **После прошивки перезагрузите стик** Если на экране появилась надпись FruitNinja то всё ок.
4. **Нажмите «Подключить»** — появится окно поиска Bluetooth. Выберите устройство **FruitNinja**.
5. **Подождите 2–3 секунды** после подключения — контроллер автоматически калибруется (держите его неподвижно).
6. **Выберите тип удара** из четырёх карточек:
   - 👊➡️ **Джеб правой**
   - ⬅️👊 **Джеб левой**
   - 🪝➡️ **Хук правой**
   - ⬅️🪝 **Хук левой**
7. **Выполняйте удар** — система автоматически фиксирует движение при превышении порога. Счётчик на карточке увеличивается.
   - Нужно **минимум 5 примеров на каждый тип** (рекомендуется 15–20).
   - Удары должны быть чёткими и резкими — слабые движения игнорируются.
8. **Повторите** для остальных трёх типов удара.
9. **Нажмите «Обучить»** — начнётся обучение (100 эпох, ~1–2 минуты). Прогресс виден на экране.
10. После завершения можно **протестировать** — система начнёт распознавать удары в реальном времени.
11. **Нажмите «Экспорт модели»** — скачается файл `gesture_model_tf.json`.

**Советы по записи:**
- Держите контроллер в той же ориентации при каждом ударе.
- Делайте паузу 1–2 секунды между ударами.
- Если счётчик не увеличивается — удар слишком слабый; бейте резче.

---

### Шаг 2 — Загрузка модели в игру (`banana_boxing.html`)

1. **Откройте** `banana_boxing.html` в Chrome.
2. **Нажмите «🧠 Загрузить TF модель»** и выберите файл `gesture_model_tf.json`.
   - Кнопка станет фиолетовой с текстом **«✓ Модель загружена»**.
3. **Нажмите «🎮 Подключить контроллер»** — выберите **FruitNinja** в окне Bluetooth.
4. **Подождите калибровку** (~2–3 сек после подключения, держите контроллер неподвижно).
5. **Бейте!** Игра распознаёт удары и анимирует их на экране.

### Геймплей

| Удар | Урон |
|---|---|
| Джеб (правой / левой) | 20 HP |
| Хук (правой / левой) | 35 HP |
| **Критический удар** (2 джеба + 1 хук за 7 сек) | **+50 HP бонус** |

- Банан имеет **1000 HP** — уничтожьте его!
- Следите за **комбо-счётчиком** — чем больше ударов подряд, тем ярче цвет.

### О проекте

Это **демонстрационный проект**, показывающий, что M5Stack StickC Plus2 / StickS3 можно использовать не только как IoT-устройство, но и как **полноценный моушн-контроллер** — по аналогии с Nintendo Wii Remote. Встроенный IMU (гироскоп + акселерометр) в связке с Web Bluetooth и машинным обучением прямо в браузере открывает широкие возможности: от игр до жестового управления реальными устройствами.

Проект намеренно сделан простым и самодостаточным — всё работает через два HTML-файла без серверной части. Берите за основу, экспериментируйте, делайте своё!

### Лицензия

Проект полностью свободный — **Public Domain / Unlicense**. Используйте код в любых целях без ограничений: копируйте, модифицируйте, распространяйте, используйте в коммерческих проектах. Никаких условий, никаких уведомлений.

---
---

## 🍌 Banana Boxing — EN

A browser-based 3D boxing game where you fight a banana character using a real M5Stack motion controller. The controller streams gyroscope and accelerometer data over Bluetooth; a TensorFlow.js neural network recognizes punch types and animates them in the game.

### Project Structure

| File / Folder | Description |
|---|---|
| `gesture_trainer_tf.html` | Tool for recording punches and training the model |
| `banana_boxing.html` | The game — box the banana |
| `FruitNinja_StickCPlus2/` | Firmware for M5Stack StickC Plus2 |
| `FruitNinja_StickS3/` | Firmware for M5Stack StickS3 |

### Requirements

- **Browser:** Google Chrome (required — Web Bluetooth)
- **Controller:** M5Stack StickC Plus2 or M5Stack StickS3 with FruitNinja firmware
- **Internet:** required to load TensorFlow.js and Three.js

---

### Step 1 — Train the model (`gesture_trainer_tf.html`)

> Skip this step if you already have a trained model (`gesture_model_tf.json`).

1. **Open** `gesture_trainer_tf.html` in Chrome.
2. **Click "Connect Controller"** — a Bluetooth picker will appear. Select **FruitNinja**.
3. **Wait 2–3 seconds** after connecting — the controller auto-calibrates (keep it still).
4. **Select a punch type** from the four cards:
   - 👊➡️ **Jab Right**
   - ⬅️👊 **Jab Left**
   - 🪝➡️ **Hook Right**
   - ⬅️🪝 **Hook Left**
5. **Perform the punch** — the system automatically captures motion above the threshold. The counter on the card increases.
   - You need **at least 5 samples per class** (15–20 recommended).
   - Punches must be sharp and fast — weak movements are ignored.
6. **Repeat** for all other punch types.
7. **Click "Train Model"** — training starts (100 epochs, ~1–2 minutes). Progress is shown on screen.
8. After training you can **test** — the system will recognize punches in real time.
9. **Click "Export Model"** — `gesture_model_tf.json` will be downloaded.

**Recording tips:**
- Hold the controller in the same orientation for every punch.
- Pause 1–2 seconds between punches.
- If the counter doesn't increase — the punch is too weak; hit faster and harder.

---

### Step 2 — Load the model into the game (`banana_boxing.html`)

1. **Open** `banana_boxing.html` in Chrome.
2. **Click "🧠 Load TF Model"** and select `gesture_model_tf.json`.
   - The button turns purple and shows **"✓ Model Loaded"**.
3. **Click "🎮 Connect Controller"** — select **FruitNinja** from the Bluetooth picker.
4. **Wait for calibration** (~2–3 sec after connecting, keep the controller still).
5. **Start punching!** The game recognizes your hits and animates them.

### Gameplay

| Punch | Damage |
|---|---|
| Jab (right / left) | 20 HP |
| Hook (right / left) | 35 HP |
| **Critical Hit** (2 jabs + 1 hook within 7 sec) | **+50 HP bonus** |

- The banana has **1000 HP** — destroy it!
- Watch the **combo counter** — the more consecutive hits, the brighter it glows.

### About this project

This is a **proof-of-concept demo** showing that the M5Stack StickC Plus2 / StickS3 can be used not just as an IoT device but as a **full motion controller** — similar to a Nintendo Wii Remote. The built-in IMU (gyroscope + accelerometer) combined with Web Bluetooth and in-browser machine learning opens up a wide range of possibilities: from games to gesture-driven control of real hardware.

The project is intentionally kept simple and self-contained — everything runs from two HTML files with no backend. Use it as a starting point, experiment freely, and build your own!

### License

This project is completely free — **Public Domain / Unlicense**. Use the code for any purpose without restriction: copy, modify, distribute, use in commercial projects. No conditions, no attribution required.

---
---

<a name="chinese"></a>
## 🍌 Banana Boxing — 中文

这是一款基于浏览器的3D拳击游戏，使用真实的M5Stack运动控制器与香蕉角色对战。控制器通过蓝牙传输陀螺仪和加速度计数据，TensorFlow.js神经网络识别出击拳类型并在游戏中进行动画展示。

### 项目结构

| 文件 / 文件夹 | 说明 |
|---|---|
| `gesture_trainer_tf.html` | 录制击拳动作并训练模型的工具 |
| `banana_boxing.html` | 游戏 — 与香蕉对战 |
| `FruitNinja_StickCPlus2/` | M5Stack StickC Plus2 固件 |
| `FruitNinja_StickS3/` | M5Stack StickS3 固件 |

### 环境要求

- **浏览器：** Google Chrome（必须 — 需要 Web Bluetooth）
- **控制器：** M5Stack StickC Plus2 或 M5Stack StickS3（已刷入 FruitNinja 固件）
- **网络：** 需要联网加载 TensorFlow.js 和 Three.js

---

### 第一步 — 训练模型（`gesture_trainer_tf.html`）

> 如果已有训练好的模型文件（`gesture_model_tf.json`），可跳过此步骤。

1. **用 Chrome 打开** `gesture_trainer_tf.html`。
2. **点击「连接」** — 弹出蓝牙设备选择窗口，选择 **FruitNinja**。
3. **连接后等待 2–3 秒** — 控制器自动校准（保持静止不动）。
4. **选择击拳类型**，共四种：
   - 👊➡️ **右直拳**
   - ⬅️👊 **左直拳**
   - 🪝➡️ **右勾拳**
   - ⬅️🪝 **左勾拳**
5. **执行击拳动作** — 系统在检测到动作超过阈值后自动记录，对应卡片上的计数器增加。
   - 每种类型**至少需要 5 个样本**（建议 15–20 个）。
   - 击拳动作必须快速有力 — 过弱的动作会被忽略。
6. **对其余三种击拳类型重复**以上步骤。
7. **点击「训练」** — 开始训练（100 轮，约 1–2 分钟），进度在屏幕上实时显示。
8. 训练完成后可**实时测试** — 系统将识别你的击拳。
9. **点击「导出模型」** — 下载 `gesture_model_tf.json` 文件。

**录制技巧：**
- 每次击拳时保持控制器方向一致。
- 击拳之间停顿 1–2 秒。
- 若计数器未增加，说明击拳力度不足，请更快更有力地出拳。

---

### 第二步 — 将模型加载到游戏（`banana_boxing.html`）

1. **用 Chrome 打开** `banana_boxing.html`。
2. **点击「🧠 加载TF模型」**，选择 `gesture_model_tf.json` 文件。
   - 按钮变为紫色，显示 **「✓ 模型已加载」**。
3. **点击「🎮 连接控制器」** — 在蓝牙选择窗口中选择 **FruitNinja**。
4. **等待校准**（连接后约 2–3 秒，保持控制器静止）。
5. **开始出拳！** 游戏识别你的击打并展示动画。

### 游戏玩法

| 击拳类型 | 伤害 |
|---|---|
| 直拳（左 / 右） | 20 HP |
| 勾拳（左 / 右） | 35 HP |
| **暴击**（7秒内 2次直拳 + 1次勾拳） | **+50 HP 额外伤害** |

- 香蕉有 **1000 HP** — 将它击倒！
- 注意右上角的**连击计数器** — 连续击中越多，颜色越亮。

### 关于本项目

这是一个**概念验证演示项目**，旨在展示 M5Stack StickC Plus2 / StickS3 不仅可以作为物联网设备使用，还能作为**完整的体感控制器**——类似于任天堂 Wii Remote。内置 IMU（陀螺仪 + 加速度计）结合 Web Bluetooth 和浏览器内机器学习，开辟了广泛的应用可能：从游戏到手势控制真实硬件设备。

本项目刻意保持简单独立——仅通过两个 HTML 文件运行，无需后端。欢迎以此为基础自由实验，开发属于你自己的项目！

### 许可证

本项目完全自由开放 — **Public Domain / Unlicense**。可将代码用于任何目的，不受任何限制：复制、修改、分发、用于商业项目均可。无需任何条件，无需注明出处。
