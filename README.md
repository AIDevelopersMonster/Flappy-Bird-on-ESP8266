
# Flappy Bird on ESP8266

**Flappy Bird** — это классическая аркадная игра, в которой игрок управляет птичкой, пытающейся пролететь через трубы, избегая столкновений. В этой версии игры используется микроконтроллер **ESP8266**, дисплей **SSD1306** и активный динамик для звуковых эффектов.

## Описание

Это простая реализация популярной игры **Flappy Bird**, адаптированная для работы на платформе **ESP8266** с использованием дисплея **SSD1306** через интерфейс I2C. Игра позволяет управлять птичкой с помощью кнопки, при этом генерируются случайные позиции труб, и игрок должен избегать столкновений. Когда птичка сталкивается с трубой или выходит за пределы экрана, игра завершается и начинается заново.

Особенности:
- **Дисплей SSD1306** с разрешением 128x64.
- Управление с помощью одной кнопки.
- Звуковые эффекты с использованием активного динамика.
- Поддержка простого интерфейса на экране (показ счета и состояния игры).

## Демонстрация игры

Посмотрите демонстрацию игры на YouTube:  
[Flappy Bird on ESP8266 - Демонстрация](https://youtu.be/M-zgmhmOkPY)

## Установка

1. Подключите дисплей **SSD1306** к ESP8266 через интерфейс I2C.
2. Подключите кнопку управления (напр. на пин D4).
3. Подключите активный динамик к пину D5.
4. Скачайте или клонируйте репозиторий на свою машину.

```bash
git clone https://github.com/AIDevelopersMonster/Flappy-Bird-on-ESP8266.git
```

5. Откройте проект в **Arduino IDE**.
6. Убедитесь, что у вас установлены библиотеки для работы с **SSD1306**:
    - **SSD1306Wire** для дисплея.
    - **Wire** для работы с интерфейсом I2C.

7. Выберите плату **ESP8266** в Arduino IDE и загрузите скетч на устройство.

## Как играть

- Нажимайте кнопку для того, чтобы птичка поднималась вверх.
- Избегайте столкновений с трубами и не выходите за пределы экрана.
- Каждый успешный проход через трубы увеличивает ваш счет.
- Игра заканчивается при столкновении с трубой или при выходе за экран.

## Управление

- **Кнопка (D4)** — управляет движением птички (подъем вверх).

## Структура программы

Программа состоит из следующих частей:
- **Инициализация дисплея** и настройка пинов для управления кнопкой и динамиком.
- **Основной цикл игры**: управление трубами, птичкой, отображение счетов и анимация.
- **Звуковые эффекты**: при взаимодействии с трубами или в начале игры.
- **Загрузка графики и данных** с внешней памяти (если требуется).

## Используемые библиотеки

- **SSD1306Wire** — для работы с дисплеем SSD1306.
- **Wire** — для работы с I2C.

## Ссылки

- **GitHub репозиторий**: [Flappy Bird на ESP8266](https://github.com/AIDevelopersMonster/Flappy-Bird-on-ESP8266)
- **Демонстрация игры на YouTube**: [Flappy Bird - демонстрация](https://youtu.be/M-zgmhmOkPY)

---
