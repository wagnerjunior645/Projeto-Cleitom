
//Incluimos a livraria servo.h
#include <Servo.h>


int pinServo = 9;
int pinSensorMov = 7;

int pinLed3 = 3;
int pinLed4 = 4;
int pinLed5 = 5;
int pinLed6 = 6;
int pinLed7 = 7;

int ledArray[] = { pinLed3, pinLed4, pinLed5, pinLed6, pinLed7 };


//Criando o Servo
//Criando o Objeto Servo
Servo myservo;
int portaoFechado = 2; //Angulo Inicial
int portaoAberto = 80; //Angulo Final
int servoAtualPosicao = portaoFechado;
//Fim Criando Servo

//Criando Sensor Movimento
//Modelo PIR DYP-ME003

//Fim Sensor de Movimento

//Criando sensorTemperatura
int pinSensorTemperatura;
//Fim Sensor Temperatura

//Criando as leds

//Fim criando as leds


//Variaveis Geral
int isModified = 0;//Default = false no caso 0

void setup(){

    Serial.begin(9600);
    myservo.attach(pinServo);  // Selecionamos o pino 9 como o pino de controlo para o servo 
    pinMode(pinSensorMov, INPUT); //Setar o pino 7 para o sensor de movimento

    pinMode(pinLed3, OUTPUT);
    pinMode(pinLed4, OUTPUT);
    pinMode(pinLed5, OUTPUT);
    pinMode(pinLed6, OUTPUT);
    pinMode(pinLed7, OUTPUT);

}

void loop(){

}

//Essa funcao vai ser responsavel por passar os valores das variaveis para o Servidor para
//Parear todas os usuarios

int relatorioGeral(){

    String result = "";

    int resultado = digitalRead(3);

    return 0;
}

String integerToString(int value){

    if (value == 1) {
        return "1";
    } else {
        return "0";
    }

}
