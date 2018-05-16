
//Incluimos a livraria servo.h
#include <Servo.h>

//Sensor temperatura
#include <dht.h>
#define dht_dpin A1 //Pino DATA do Sensor ligado na porta Analogica A1
dht DHT; //Inicializa o sensor

int pinServo = 9;
int pinSensorMov = 7;

int pinLed3 = 3;
int pinLed4 = 4;
int pinLed5 = 5;
int pinLed6 = 6;
int pinLed7 = 7;

//Array de String em que o primeiro
//linha 0 = nome das leds
//linha 1 = porta digital da led
//linha 2 = estado da led ligada ou desligada
String ledArray[5][3] = {
    {"l3","3","0"},
    {"l4","4","0"},
    {"l5","5","0"},
    {"l6","6","0"},
    {"l7","7","0"}
};


//Criando o Servo
//Criando o Objeto Servo
Servo myservo;
int portaoFechado = 5; //Angulo Inicial
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
int estadoEmergenciaIncendio = 0;//Estado de emergencia Incendio desligado
int estadoEmergenciaInvasao = 0;//Estado de emergencia Invasao desligado
float temperatura = 0;//Varial para temperatura

String text = "";

void setup(){

    Serial.begin(9600);
    myservo.attach(pinServo);  // Selecionamos o pino 9 como o pino de controlo para o servo 
    pinMode(pinSensorMov, INPUT); //Setar o pino 7 para o sensor de movimento

    //Zerando todas as leds
    pinMode(pinLed3, OUTPUT);
    pinMode(pinLed4, OUTPUT);
    pinMode(pinLed5, OUTPUT);
    pinMode(pinLed6, OUTPUT);
    pinMode(pinLed7, OUTPUT);

}

void loop(){

    if(isModified == 1){
        relatorioGeral();
        isModified = 0;
    }

    text = "";
    //Se for passado algum coomando para o arduino
    if(Serial.available() > 0){
        char c = Serial.read();
        while(1){
            if(c == '$'){
                break;
            }else{
                text = text + c;
            }
        }
    }

    //Verificar se algum comando foi passado parao arduino
    if(text != ""){
        executarAcao(text);
    }

}

//funcao para leitura de temperatura
float lerTemperatura(){

    DHT.read11(dht_dpin); //Lê as informações do sensor
    float f = DHT.temperature;

    if(f > 30){
        estadoEmergenciaIncendio = 1;
    }

    return f;

}

//Funcao responsavel por manter as leds ligadas void
//quando as leds recebem sinal auto ele sempre fica auto
//nao precisa executar sempre
void execucaoConstante(){
    //ledArray
    int x = 5;
    int y = 3;
    int i;
    int j;

    for(i = 0; i < x; i++){
        int pino = stringToInteger(ledArray[i][1]);
        int saida = stringToInteger(ledArray[i][2]);
        digitalWrite(pino, saida);
    }

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
    }else if (value == 2) {
        return "2";
    }else if (value == 3) {
        return "3";
    }else if (value == 4) {
        return "4";
    }else if (value == 5) {
        return "5";
    }else if (value == 6) {
        return "6";
    }else if (value == 7) {
        return "7";
    }else {
        return "0";
    }

}

int stringToInteger(String value){

    if(value == "0"){
        return 0;
    }else if (value == "1") {
        return 1;
    }else if (value == "2") {
        return 2;
    }else if (value == "3") {
        return 3;
    }else if (value == "4") {
        return 4;
    }else if (value == "5") {
        return 5;
    }else if (value == "6") {
        return 6;
    }else if (value == "7") {
        return 7;
    }else{
        return -1;
    }

}

//Funcao que irá executar a acao baseado nas strings passadas
void executarAcao(String acao){

    isModified = 1;

    if(acao == "ce1"){

    }else if (acao == "ce2") {

    }

    if(acao == "sm0"){

    }else if (acao == "sm1") {

    }

    //split string
    String tempChar0 = "";
    tempChar0 = tempChar0 + acao.charAt(0);

    if(tempChar0 == 'l'){

        String tempChar1 = "";
        String tempChar2 = "";

        tempChar1 = tempChar1 + acao.charAt(1);
        tempChar2 = tempChar2 + acao.charAt(2);

        int pino = stringToInteger(tempChar1);
        int saida = stringToInteger(tempChar2);

        digitalWrite(pino, saida);

        int x = 5;
        int i;

        for(i = 0; i < x; i++){
            if(ledArray[i][1] == tempChar1){
                ledArray[i][2] = tempChar2;
            }
        }

    }

    

}

//LEGENDAS PARA AS ACOES

//l30 = LED 3 Desligar = 0
//l31 = LED 3 Ligar = 1

//---------- ce CANCELAR ESTADO DE EMERGENCIA ----------
//ce1 = estado de emergencia temperatura
//ce2 = estado de emergencia sinal de movimento

//---------- sm = SERVOMOTOR ----------
//sm1 = servo motor portao abrir
//sm0 = servo motor portao fechar

//pi = ping para retornar a funcao relatorioGeral()



//----------- Anotacoes -----------
//sizeof(a) / sizeof(int); <--> para ver o tamanho do array

