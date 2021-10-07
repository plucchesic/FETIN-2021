#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Stepper.h>
// configurações do motor stepper
const int stepsPerRevolution = 65;
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11); //INICIALIZA O MOTOR UTILIZANDO OS PINOS DIGITAIS 8, 9, 10, 11
// configurações do teclado
const byte numRows = 4;
const byte numCols = 3;
char keymap[numRows][numCols] = {{'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[numRows] = {7, 6, 5, 4};
byte colPins[numCols] = {2, 1, 0};
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);
// declaração da variável de leitura da tecla digitada
char tecla2 = myKeypad.getKey();
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x3F for a 16 chars and 2 line display
int Aux = 1;
int Aux1; // variaveis auxiliares
int Aux2; // do tempo que a rosca gira
unsigned long int Aux3; // variavel auxiliar pro millis no case 4
unsigned long int Aux4; // variavel auxiliar pro millis no case 4
unsigned long int Aux5; // variavel auxiliar pra conttagem de segundos
int i = 0; // variavel de teste
int Thrs; // tempo em horas da alimentação
int Tmin; // tempo em minutos da alimentação
int Trosca; // tempo final da rosca
int Tdisplay; // tempo pra alimentar mostrado no LCD
int Tagr; // variavel utilizada no giro do motor ao depositar
int Tagr2; // variavel utilizada tb no giro do motor ao depositar
int auxmin = 0; // auxiliar da inserção de minutos
unsigned long int Tc; // tempo entre alimentações em ms
String testemin; // string que contara os minutos
// variaveis da contagem
uint16_t hrs; // horas
uint8_t min; // minutos
uint8_t sec; // segundos
//********************//
char tecla1;
String Cod = "";
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.init();
  lcd.backlight();
  // velocidade do motor
  myStepper.setSpeed(500);
  pinMode(3, OUTPUT);
}
void secondsToHMS( const uint32_t seconds, uint16_t &h, uint8_t &m, uint8_t &s )
{
  uint32_t t = seconds;

  s = t % 60;

  t = (t - s) / 60;
  m = t % 60;

  t = (t - m) / 60;
  h = t;
}
void loop() {
  char tecla2 = myKeypad.getKey();// tempo em horas a cada alimentação
  // variavel auxiliar que controla os cases
  // é digitado pelo usuário
  switch (Aux) {
    case 1: // case padrão onde ela começa
      lcd.setCursor(1, 0);
      lcd.print("Ola, e seja mt");
      lcd.setCursor(2, 1);
      lcd.print("bem vindo ao");
      delay(3500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Autoalimentador");
      lcd.setCursor(0, 1);
      lcd.print("Revopet da FETIN");
      delay(3500);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Para comecar,");
      lcd.setCursor(0, 1);
      lcd.print("Aperte qqr tecla");
      tecla2 = myKeypad.waitForKey();
      tecla1 = tecla2;
      Aux = 2;
      break;
    case 2:
      Tdisplay = 0;
      hrs = 0;
      min = 0;
      sec = 0;
      testemin = "";
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Vamos comecar!");
      delay(1500);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Defina o tempo");
      lcd.setCursor(1, 1);
      lcd.print("de dar comida");
      delay(2500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Digite, em horas");
      lcd.setCursor(6, 1);
      tecla2 = myKeypad.waitForKey();
      tecla1 = tecla2;
      if (tecla1 != NO_KEY) {
        lcd.setCursor(4, 1);
        lcd.print(tecla1);
        Thrs = tecla1 - '0';
        Tc = Thrs;
        Aux5 = Thrs * 3600;
        Tdisplay = Thrs * 3600;
        Tc = Tc * 3600000; // conversao pra ms
        lcd.setCursor(6, 1);
        lcd.print("horas");
        delay(2500);
      }
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("Ok!");
      delay(1500);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Digite, em min");
      lcd.setCursor(5, 1);
      tecla2 = myKeypad.waitForKey();
      tecla1 = tecla2;
      while (tecla1 != NO_KEY && auxmin != 2) {
        lcd.print(tecla1);
        testemin = testemin + tecla1;
        auxmin++;
        if (auxmin == 2) {
          lcd.setCursor(8, 1);
          lcd.print("min");
          break;
        }
        tecla2 = myKeypad.waitForKey();
        tecla1 = tecla2;
      }
      auxmin = 0;
      Tmin = testemin.toInt();
      Tdisplay = Tdisplay + 60 * Tmin;
      Aux5 = Tdisplay;
      delay(2000);
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("Ok!");
      delay(1500);
      lcd.clear();
      lcd.print("Agora, vamos");
      lcd.setCursor(0, 1);
      lcd.print("definir o tempo");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("que a rosca gira");
      lcd.setCursor(0, 1);
      lcd.print("e deposita racao");
      delay(2000);
      Aux = 3;
      break;
    case 3: // definição do tempo da rosca
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Verifique se ha");
      lcd.setCursor(0, 1);
      lcd.print("racao no reserv.");
      delay(2500);
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("antes de");
      lcd.setCursor(4, 1);
      lcd.print("comecar");
      delay(2500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Quando estiver");
      lcd.setCursor(0, 1);
      lcd.print("pronto, tecle 1");
      tecla2 = myKeypad.waitForKey();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Agora, deixe a");
      lcd.setCursor(0, 1);
      lcd.print("rosca girar por");
      delay(2500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("quanto tempo jul");
      lcd.setCursor(0, 1);
      lcd.print("gar necessario");
      delay(2500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("p/ comecar,");
      lcd.setCursor(0, 1);
      lcd.print("tecle qqr tecla");
      tecla2 = myKeypad.waitForKey();
      Aux1 = millis();
      tecla2 = NO_KEY;
      lcd.clear();
      while (tecla2 == NO_KEY) {
        lcd.print("Girando. Aperte");
        lcd.setCursor(0, 1);
        lcd.print("qqr p/ parar");
        while (tecla2 == NO_KEY) {
          myStepper.step(-stepsPerRevolution);
          tecla2 = myKeypad.getKey();
          if (tecla2 != NO_KEY) {
            break;
          }
        }
        break;
      }
      Aux2 = millis();
      Trosca = Aux2 - Aux1;
      lcd.clear();
      lcd.noDisplay();
      lcd.display();
      lcd.setCursor(6, 0);
      lcd.print("Ok!");
      lcd.setCursor(0, 1);
      lcd.print("Vamos confirmar:");
      delay(3000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Tempo:");
      lcd.setCursor(9, 0);
      lcd.print(Thrs);
      lcd.setCursor(10, 0);
      lcd.print("h");
      lcd.setCursor(11, 0);
      lcd.print(testemin);
      lcd.setCursor(13, 0);
      lcd.print("min");
      lcd.setCursor(0, 1);
      lcd.print("T-giro:");
      lcd.setCursor(8, 1);
      lcd.print(Trosca);
      lcd.setCursor(13, 1);
      lcd.print("ms");
      delay(3500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Se as info estao");
      lcd.setCursor(0, 1);
      lcd.print("corretas,tecle 1");
      tecla2 = myKeypad.waitForKey();
      tecla1 = tecla2;
      if (tecla1 == '1') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Ok! Agora, seu");
        lcd.setCursor(0, 1);
        lcd.print("alimentador esta");
        delay(2500);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("100% configurado");
        lcd.setCursor(2, 1);
        lcd.print("Aproveite-o!");
        delay(2500);
        lcd.clear();
        Aux3 = millis();
        Aux = 4;
      }
      else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Deseja retornar");
        lcd.setCursor(0, 1);
        lcd.print("e reconfigurar?");
        delay(3000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("tecle 1 se sim,");
        lcd.setCursor(0, 1);
        lcd.print("tecle 2 se nao");
        tecla2 = myKeypad.waitForKey();
        tecla1 = tecla2;
        if (tecla1 == '1') {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Ok!");
          Aux = 2;
        }
        else if (tecla1 == '2') {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Ok! Agora, seu");
          lcd.setCursor(0, 1);
          lcd.print("alimentador esta");
          delay(2500);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("100% configurado");
          lcd.setCursor(2, 1);
          lcd.print("Aproveite-o!");
          delay(2500);
          lcd.clear();
          Aux3 = millis();
          Aux = 4;
        }
        break;

      case 4: // standby
        Aux4 = millis();  //get the current time
        if (Aux4 - Aux3 >= 1000 && Tdisplay != 0) //test whether the period has elapsed
        {
          //IMPORTANT to save the start time
          lcd.setCursor(0, 0);
          lcd.print("Tempo p/ aliment");
          lcd.setCursor(0, 1);
          Tdisplay--;
          if (Tdisplay == 0) {
            Aux = 5;
          }
          secondsToHMS(Tdisplay, hrs, min, sec);
          lcd.setCursor(4, 1);
          lcd.print(hrs);
          lcd.setCursor(5, 1);
          lcd.print(":");
          lcd.setCursor(6, 1);
          lcd.print(min);
          lcd.setCursor(8, 1);
          lcd.print(":");
          lcd.setCursor(9, 1);
          lcd.print(sec);
        }
        delay(1000);
        if (tecla2 != NO_KEY) {
          tecla1 = tecla2;
          if (tecla1 == '#') {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Deseja voltar e");
            lcd.setCursor(3, 1);
            lcd.print("reconfigurar?");
            delay(2000);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("tecle 1 se sim");
            lcd.setCursor(0, 1);
            lcd.print("tecle outra se n");
            tecla2 = myKeypad.waitForKey();
            tecla1 = tecla2;
            if (tecla1 == '1') {
              Aux = 2;
            }
            else {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Ok! Retornando");
              lcd.setCursor(0, 1);
              lcd.print("ao temporizador.");
              delay(1500);
            }
          }
        }
        lcd.clear();
        Aux3 = Aux4;
      }
      break;
    case 5: // alimentando
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Depositando");
      lcd.setCursor(0, 1);
      lcd.print("comida pro pet..");
      Tagr = millis();
      Tagr2 = Tagr+1;
      while ((Tagr2-Tagr) < Trosca) {
        myStepper.step(stepsPerRevolution);
        Tagr2 = millis();
      }
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Concluido.");
      lcd.setCursor(3, 1);
      lcd.print("Retornando...");
      delay(1500);
      lcd.clear();
      Tdisplay = Aux5;
      Aux = 4;
  }
}
