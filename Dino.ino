#include <Wire.h>
#include <LiquidCrystal_I2C.h>
/*
 *======================================== 
 *              НАСТРОЙКИ
 *=======================================     */
#define SPEED 300 //Скорость кактусов
#define JUMP_MAX_TIME 600 //Максимальное время в прыжке
#define SPEED_ADD 7 //Сколько добавлять скорость каждый кактус
/*=========================================   */
#define SOUND 9 //Порт пищалки
#define BTN 7 //Порт пищалки
/*========================================    */

LiquidCrystal_I2C lcd(0x3F, 16, 2);

byte dino_char[] = {
  B00000,
  B00110,
  B00111,
  B00111,
  B10010,
  B11110,
  B01110,
  B01010
};
byte cactus_char[] = {
  B00100,
  B10101,
  B10111,
  B11100,
  B00101,
  B00111,
  B00100,
  B00100
};

byte default_cactuses[] = {16, 6, 9}; //Массив с координатами кактусов
byte cactuses[] = {16, 6, 9}; // Второй массив

int score;
int speed = SPEED;


bool isJumping;//Состояние дино ( в прыжке)

unsigned long tmr; // Таймер на движения кактусов
unsigned long jump_time; // Таймер на время прыжка


void setup() {
  pinMode(SOUND,OUTPUT);
  pinMode(BTN, INPUT_PULLUP);
  


  lcd.init();
  lcd.backlight();
  lcd.createChar(0, dino_char);
  lcd.createChar(1, cactus_char);

  Serial.begin(9600);

  lcd.setCursor(0, 1);
  lcd.write(1);
}

void loop() {
  start_game();
  game();

}

void start_game(){
  lcd.clear();
  lcd.home();
  lcd.print("Press BUTTON ");
  lcd.setCursor(0,1);
  lcd.print("To start ");
  delay(500);
  while(true){
    if (digitalRead(BTN) == LOW){
      delay(500);

      for (int i = 0; i < sizeof(cactuses); i++) {
        cactuses[i] = default_cactuses[i];
      }
      score=0;
      speed = SPEED;
      
      break;
  }
  }
  
}

void game(){
  while(true){
  
  lcd.clear();

  //Отработка прыжка и рисовка дино
  if (digitalRead(BTN) == LOW) {

    if (millis() - jump_time >= JUMP_MAX_TIME && isJumping == true) {  // ищем разницу (500 мс)
      jump_time = millis();
      isJumping = false;
      lcd.setCursor(0, 1);
      lcd.write(0);

    }
    else {
      // выполнить действие
      //lcd.clear();
      isJumping = true;
      //jump_time = millis();
      lcd.home();
      lcd.write(0);
    }
  }
  else {
    jump_time = millis();
    //lcd.clear();
    isJumping = false;
    lcd.setCursor(0, 1);
    lcd.write(0);

  }

  //Рисовка кактусов
  for (int i = 0; i < sizeof(cactuses); i++) {
    // тело цикла
    lcd.setCursor(cactuses[i], 1);
    lcd.write(1);
  }

  //Отрисовка счета
  scorePrint();
  delay(50);



  //Отработка смерти и сдвиг кактусов в следующем ходу
  if (millis() - tmr >= speed) {
    tmr = millis();

    //lcd.scrollDisplayLeft();

    for (int i = 0; i < sizeof(cactuses); i++) {
      cactuses[i] -= 1;

      if (cactuses[i] == 0) {
        if (! isJumping) {
            lcd.clear();
  lcd.home();
  lcd.print("Game over!");
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.setCursor(7, 1);
  lcd.print(score);
  delay(1000);
  lcd.setCursor(15, 0);
  lcd.print(5);
  delay(1000);
  lcd.setCursor(15, 0);
  lcd.print(4);
  delay(1000);
  lcd.setCursor(15, 0);
  lcd.print(3);
  delay(1000);
  lcd.setCursor(15, 0);
  lcd.print(1);
  delay(1000);
  lcd.setCursor(15, 0);
  
  lcd.print(" ");
  while (true) {
    if (!digitalRead(BTN)) {
      return;
    }
  }
        }
        else {

          score++;
          speed -= SPEED_ADD;
          cactuses[i] = 17;
          Serial.println(score);
        }

      }

    }


  }
  }
  }


void scorePrint() {
  int x = 15;
  if (score >= 10) {
    x -= 1;
  }
  else if (score >= 10) {
    x -= 1;
  }
  else if (score >= 100) {
    x -= 2;
  }
  else if (score >= 1000) {
    x -= 3;
  }
  else if (score >= 10000) {
    x -= 4;
  }
  else if (score >= 100000) {
    x -= 5;
  }
  else if (score >= 1000000) {
    x -= 6;
  }
  else if (score >= 10000000) {
    x -= 7;
  }

  lcd.setCursor(x, 0);
  lcd.print(score);

}
