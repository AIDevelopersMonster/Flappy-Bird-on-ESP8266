/*
 * Flappy Bird on ESP8266
 * 
 * This is an adapted version of the popular arcade game Flappy Bird, developed for the ESP8266 platform. The game uses an SSD1306 display and an active buzzer for sound effects.
 * The bird is controlled with a single button. The player must avoid collisions with pipes while flying through them to score points.
 * 
 * Author: AIDevelopersMonster
 * The source code is available on GitHub:
 * https://github.com/AIDevelopersMonster/Flappy-Bird-on-ESP8266
 * 
 * Game demonstration on YouTube:
 * https://youtu.be/M-zgmhmOkPY
 * 
 * Changes:
 * - Added sound effect on pipe interaction.
 * - Configured control via button on pin D4.
 * - Added game graphical elements (bird and pipes).
 * - Introduced score counter displayed on the screen.
 * - Font setup for text using ArialMT_Plain_10.
 * 
 * Libraries used:
 * - SSD1306Wire (for SSD1306 display)
 * - Wire (for I2C interface)
 * 
 * Program structure:
 * - Pin and display initialization.
 * - Main game loop: pipe drawing, bird control, score updating.
 * - Sound signal via active buzzer.
 * - Restart the game upon pipe collision or going off-screen.
 * 
 * Version: 1.0
 * Date: 17.12.2024
 */
#include <Wire.h>
#include "SSD1306Wire.h"
#include "images.h"  // Подключаем файл с изображениями
#include "fontovi.h"  // Подключаем файл с шрифтами

// Инициализация дисплея SSD1306 с I2C адресом 0x3c и пинами для ESP (D1, D2)
SSD1306Wire display(0x3c, D1, D2);

// Константы игры
#define DEMO_DURATION 3000
typedef void (*Demo)(void);

// Константы для труб и птички
const int PIPE_WIDTH = 6;  // Ширина трубы
const int PIPE_GAP_WIDTH = 30;  // Ширина прохода между трубами
const int NUM_PIPES = 4;  // Количество труб

// Переменные для игры
float pipeXPositions[NUM_PIPES];        // Позиции труб по оси X
int pipeYPositions[NUM_PIPES];          // Позиции труб по оси Y
int gapPositions[NUM_PIPES];            // Позиции промежутков между трубами
int pipeSpacing = 32;                   // Расстояние между трубами

int score = 0;                          // Счет
int buttonState = 0;                     // Состояние кнопки (нажата/отпущена)
float birdX = 30.00;                    // Начальная позиция птички по оси X
float birdY = 22.00;                    // Начальная позиция птички по оси Y
int birdDirection = 0;                  // Направление движения птички (вверх/вниз)
unsigned long birdMovementTime = 0;     // Время для плавного движения птички
int gameState = 0;                      // Состояние игры (0 - начало, 1 - игра)
int frameCount = 0;                     // Номер кадра для анимации
int soundFlag = 0;                      // Флаг для проигрывания звука
unsigned long soundTime = 0;            // Время для звукового сигнала

void setup() {
  Serial.begin(9600);  // Инициализация серийного порта

  // Настройка пинов
  pinMode(D5, OUTPUT);  // Пин для индикатора состояния (например, для светодиода)
  digitalWrite(D5, LOW);  // Устанавливаем начальное состояние пина D5 в LOW (выключено)

  pinMode(D4, INPUT_PULLUP);  // Пин для кнопки управления

  // Инициализация дисплея
  display.init();
  display.flipScreenVertically();  // Поворот экрана для правильного отображения
  display.setFont(ArialMT_Plain_10);  // Установка шрифта для текста

  // Инициализация труб и их промежутков
  for (int i = 0; i < NUM_PIPES; i++) {
    pipeXPositions[i] = 128 + ((i + 1) * pipeSpacing);  // Установка начальной позиции труб
    gapPositions[i] = random(8, 32);  // Генерация случайного промежутка
  }
}

void loop() {
  display.clear();  // Очистка экрана перед новым кадром

  if (gameState == 0) {  // Если игра не началась
    display.setFont(ArialMT_Plain_16);  // Установка большего шрифта для титульного экрана
    display.drawString(0, 4, "Flappy ");  // Название игры
    display.drawXbm(0, 0, 128, 64, pozadina);  // Фоновое изображение
    display.drawXbm(20, 32, 14, 9, ptica);  // Изображение птички

    display.setFont(ArialMT_Plain_10);  // Установка стандартного шрифта
    display.drawString(0, 44, "Press to start");  // Сообщение о начале игры

    if (digitalRead(D4) == LOW)  // Если кнопка нажата
      gameState = 1;  // Начать игру
  }

  if (gameState == 1) {  // Если игра началась
    display.setFont(ArialMT_Plain_10);  // Установка шрифта для отображения счета
    display.drawString(3, 0, String(score));  // Отображение текущего счета

    if (digitalRead(D4) == LOW) {  // Если кнопка нажата
      if (buttonState == 0) {  // Если кнопка еще не была нажата
        birdMovementTime = millis();  // Засекаем время
        birdDirection = 1;  // Направление вверх
        soundFlag = 1;  // Включаем звук
        buttonState = 1;  // Устанавливаем флаг, что кнопка была нажата
        soundTime = millis();  // Засекаем время для звука
      }
    } else {
      buttonState = 0;  // Если кнопка отпущена, сбрасываем флаг
    }

    // Отображение труб
    for (int j = 0; j < NUM_PIPES; j++) {
      display.setColor(WHITE);
      display.fillRect(pipeXPositions[j], 0, PIPE_WIDTH, 64);  // Отображение труб
      display.setColor(BLACK);
      display.fillRect(pipeXPositions[j], gapPositions[j], PIPE_WIDTH, PIPE_GAP_WIDTH);  // Отображение пустого промежутка
    }

    display.setColor(WHITE);
    display.drawXbm(birdX, birdY, 14, 9, ptica);  // Отображение птички

    // Обновление позиции труб
    for (int j = 0; j < NUM_PIPES; j++) {
      pipeXPositions[j] -= 0.01;  // Двигаем трубы влево
      if (pipeXPositions[j] < -PIPE_WIDTH) {  // Если труба вышла за экран
        score++;  // Увеличиваем счет
        digitalWrite(D5, HIGH);  // Включаем индикатор (например, светодиод)
        gapPositions[j] = random(8, 32);  // Генерация нового промежутка
        pipeXPositions[j] = 128;  // Перемещаем трубу в начало экрана
      }
    }

    // Управление движением птички
    if ((birdMovementTime + 185) < millis())  // Если время прошло, сбрасываем движение птички
      birdDirection = 0;

    if ((soundTime + 40) < millis())  // Если время для звука прошло, выключаем его
      soundFlag = 0;

    if (birdDirection == 0)
      birdY += 0.01;  // Если не нажата кнопка, птичка падает
    else
      birdY -= 0.03;  // Если нажата кнопка, птичка поднимается

    // Управление индикатором (например, светодиодом)
    if (soundFlag == 1)
      digitalWrite(D5, HIGH);  // Включаем индикатор
    else
      digitalWrite(D5, LOW);  // Выключаем индикатор

    // Проверка, не вышла ли птичка за пределы экрана
    if (birdY > 63 || birdY < 0) {
      gameState = 0;  // Завершаем игру
      birdY = 22;     // Сбрасываем позицию птички
      score = 0;      // Обнуляем счет
      digitalWrite(D5, HIGH);  // Включаем индикатор (например, светодиод)
      delay(500);  // Задержка
      digitalWrite(D5, LOW);  // Выключаем индикатор
      for (int i = 0; i < NUM_PIPES; i++) {
        pipeXPositions[i] = 128 + ((i + 1) * pipeSpacing);  // Сброс труб
        gapPositions[i] = random(4, 30);  // Генерация новых промежутков
      }
    }

    // Проверка на столкновение с трубами
    for (int m = 0; m < NUM_PIPES; m++) {
      if (pipeXPositions[m] <= birdX + 7 && birdX + 7 <= pipeXPositions[m] + PIPE_WIDTH) {  // Если птичка пересекает трубу
        if (birdY < gapPositions[m] || birdY + 8 > gapPositions[m] + PIPE_GAP_WIDTH) {  // Если птичка не в промежутке
          gameState = 0;  // Завершаем игру
          birdY = 22;     // Сбрасываем позицию птички
          score = 0;      // Обнуляем счет
          digitalWrite(D5, HIGH);  // Включаем индикатор (например, светодиод)
          delay(500);  // Задержка
          digitalWrite(D5, LOW);  // Выключаем индикатор
          for (int i = 0; i < NUM_PIPES; i++) {
            pipeXPositions[i] = 128 + ((i + 1) * pipeSpacing);  // Сброс труб
            gapPositions[i] = random(8, 32);  // Генерация новых промежутков
          }
        }
      }
    }

    display.drawRect(0, 0, 128, 64);  // Отображение рамки вокруг экрана
  }

  display.display();
}
