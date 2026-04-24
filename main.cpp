#include <Arduino.h>
#include <Bounce2.h>

enum STATES {
  STATE_INVALID = 0,
  STATE_IDLE = 1,
  STATE_SELECTED = 2,
  STATE_MOTOR_ACTIVE = 3
};

const int BTN_PROD_01 = 4;
const int BTN_PROD_02 = 3;
const int BTN_MONEY = 2;
const int LED_GRN = 13;
const int LED_YLW = 12;
const int LED_BLU = 11;
const int MOTOR_RUNNING_TIME = 3000;
const int WAIT_FOR_COIN_INSERTION_TIME = 4000;
const int MotorA1 = 10;
const int MotorA2 = 9;
const int MotorB1 = 8;
const int MotorB2 = 7;

unsigned long time1;

enum STATES current_state = STATES::STATE_IDLE;

Bounce2::Button button_prod_1 = Bounce2::Button();
Bounce2::Button button_prod_2 = Bounce2::Button();
Bounce2::Button button_money = Bounce2::Button();

int selected_product = 0;

void HandleStateIdle();
void HandleStateSelected();
void HandleStateSMotorActive();

void setup() {
  Serial.begin(9600);
  button_prod_1.attach(BTN_PROD_01, INPUT_PULLUP);
  button_prod_1.interval(10);
  button_prod_1.setPressedState(LOW);

  button_prod_2.attach(BTN_PROD_02, INPUT_PULLUP);
  button_prod_2.interval(10);
  button_prod_2.setPressedState(LOW);

  button_money.attach(BTN_MONEY, INPUT_PULLUP);
  button_money.interval(10);
  button_money.setPressedState(LOW);

  pinMode(LED_GRN, OUTPUT);
  pinMode(LED_YLW, OUTPUT);
  pinMode(LED_BLU, OUTPUT);
  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);

  digitalWrite(LED_GRN, HIGH);
  current_state = STATES::STATE_IDLE;

  time1 = millis();
 }


void loop() {
  button_prod_1.update();
  button_prod_2.update();
  button_money.update();

  switch (current_state) {
    case STATES::STATE_IDLE:
      HandleStateIdle();
      break;
    case STATES::STATE_SELECTED:
      HandleStateSelected();
      break;
    case STATES::STATE_MOTOR_ACTIVE:
      HandleStateSMotorActive();
      break;
    default:
      Serial.println("Fataler Logikfehler");
      Serial.println("Zustand ist ungültig");
  }

 }



void HandleStateIdle()
{
  bool btn_prod_1_pressed = button_prod_1.pressed();
  if(btn_prod_1_pressed)
  {
    current_state = STATES::STATE_SELECTED;
    digitalWrite(LED_GRN, LOW);
    digitalWrite(LED_YLW, HIGH);
    selected_product = 1;
    time1 = millis();
  }
  bool btn_prod_2_pressed = button_prod_2.pressed();
  if(btn_prod_2_pressed)
  {
    current_state = STATES::STATE_SELECTED;
    digitalWrite(LED_GRN, LOW);
    digitalWrite(LED_YLW, HIGH);
    selected_product = 2;
    time1 = millis();
  }
  digitalWrite(MotorA1, LOW);
  digitalWrite(MotorB1, LOW);
  digitalWrite(MotorA2, LOW);
  digitalWrite(MotorB2, LOW);
}
void HandleStateSelected()
{
  bool btn_money_pressed = button_money.pressed();
  if (millis() - time1 >= WAIT_FOR_COIN_INSERTION_TIME)
  {
    time1 = millis();
    digitalWrite(LED_GRN, HIGH);
    digitalWrite(LED_YLW, LOW);
    current_state = STATES::STATE_IDLE;
  }
  if(btn_money_pressed)
  {
    current_state = STATES::STATE_MOTOR_ACTIVE;
    digitalWrite(LED_YLW, LOW);
    digitalWrite(LED_BLU, HIGH);
    time1 = millis();
  }

}
void HandleStateSMotorActive()
{
  if (millis() - time1 >= MOTOR_RUNNING_TIME)
  {
    time1 = millis();
    digitalWrite(LED_GRN, HIGH);
    digitalWrite(LED_BLU, LOW);
    current_state = STATES::STATE_IDLE;
    
  }
  if (selected_product == 1)
  {
    digitalWrite(MotorA1, HIGH);
    digitalWrite(MotorA2, LOW);
  }
  if (selected_product == 2)
  {
    digitalWrite(MotorB1, HIGH);
    digitalWrite(MotorB2, LOW);
  }

}
