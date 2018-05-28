
//Incluimos a livraria servo.h
#include <Servo.h>

//Sensor temperatura
#include "DHT.h"
#define DHTPIN A1 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);

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
int estadoAlarme = 0;//Estado de alarme para detectar movimento pela casa inicio desligado

String text = "";

void setup(){

    Serial.begin(9600);
    
    myservo.attach(pinServo);  // Selecionamos o pino 9 como o pino de controlo para o servo 
    pinMode(pinSensorMov, INPUT); //Setar o pino 7 para o sensor de movimento

    //Zerando todas as leds e as setando
    pinMode(pinLed3, OUTPUT);
    pinMode(pinLed4, OUTPUT);
    pinMode(pinLed5, OUTPUT);
    pinMode(pinLed6, OUTPUT);
    pinMode(pinLed7, OUTPUT);

}

void loop(){

    //if(isModified == 1){
    //    relatorioGeral();
    //    isModified = 0;
    //}

    text = "";
    //Se for passado algum coomando para o arduino
    if(Serial.available() > 0){
        char c = 'a';
        while(1){

            c = Serial.read();

            if(c == '$'){
                break;
            }else{
                text = text + c;
            }

        }
    }

    //Depois acrescentar temperatura
    //Que irá entrar em emergencia

    //float temperatura = lerTemperatura();

    //Verificar se algum comando foi passado para o arduino
    if(text != ""){
        executarAcao(text);
    }

    if(estadoEmergenciaIncendio == 1 || estadoEmergenciaInvasao == 1){
        estadoDeEmergenciaLigar();
    }

    //Enviar para o servidor as insformacoes
    relatorioGeral();

    delay(1500);

}

void moverServo(int position){

    servoAtualPosicao = position;
    myservo.write(position);

}

//funcao para leitura de temperatura
float lerTemperatura(){

    //DHT.read11(dht_dpin); //Lê as informações do sensor
    float f = dht.readTemperature();

    if(isnan(f)){
      f = 0;
    }

    if(f > 30){
        estadoEmergenciaIncendio = 1;
    }

    return f;

}

float lerHumidade(){

    //DHT.read11(dht_dpin); //Lê as informações do sensor
    float h = dht.readHumidity();

    if(isnan(h)){
      h = 1000;
    }

    return h;

}

//Funcao usada para ligas e desligar todas as leds no array
void ledOnOff(int value){

    int x = 5;
    int i;

    for(i = 0; i < x; i++){
        int pino = stringToInteger(ledArray[i][1]);
        int saida = stringToInteger(ledArray[i][2]);
        digitalWrite(pino, value);
        ledArray[i][2] = value;
    }

}

void estadoDeEmergenciaLigar(){

    int i;

    for(i = 0; i<3 ;i++){
        ledOnOff(1);
        delay(300);
        ledOnOff(0);
        delay(300);
    }

    ledOnOff(1);
    delay(1000);
    ledOnOff(0);

}

//Cancelar Estado de Emergencia apagas todas as leds
void desligarEstadoDeEmergencia(){
    ledOnOff(0);
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

void relatorioGeral(){

    String result = "";
    String final = "";

    if(servoAtualPosicao == portaoAberto){
        result = "sm1";
    }else{
        result = "sm0";
    }

    final = final + result + ";";

    //forcar passar float to int
    int temperatura = (int)lerTemperatura();

    //numero float com 0 casas decimais
    result = "tp" + String(temperatura);

    final = final + result + ";" ;

    //relatorio estados de emergencia
    final = final + "ct" +String(estadoEmergenciaIncendio) + ";";
    final = final + "cm" +String(estadoEmergenciaInvasao) + ";";

    //relatorios das leds
    int i;
    int x = 5;

    for(i = 0; i < x; i++){
        result = ledArray[i][0];
        result = result + ledArray[i][2];
        result = result + ";";
        final = final + result;
    }

    Serial.println(final);

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

    //em teoria so podera vir 0 para desligar
    if(acao == "ct0"){
        estadoEmergenciaIncendio = 0;
        desligarEstadoDeEmergencia();
        return;
    }

    //em teoria so podera vir 0 para desligar
    if(acao == "cm0"){
        estadoEmergenciaInvasao = 0;
        desligarEstadoDeEmergencia();
        return;
    }

    //Travar todas as acoes que  nao seja desligar estados de emergencia
    //caso algumas delas estejam ativadas ou ligadas
    if(estadoEmergenciaIncendio == 1 || estadoEmergenciaInvasao == 1){
        return;
    }

    //0 baixar portao
    //1 levantar portao
    if(acao == "sm0"){
        moverServo(portaoFechado);
        return;
    }else if (acao == "sm1") {
        moverServo(portaoAberto);
        return;
    }

    //split string
    String tempChar0 = "";
    tempChar0 = tempChar0 + acao.charAt(0);

    if(tempChar0 == "l"){

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
//ct0 = estado de emergencia temperatura/incendio Desligado
//cm0 = estado de emergencia sinal de movimento Desligadok

//Ligar alarme para que o sinal de movimento possa comecar a funcionar
//al0 = alarme desligado
//al1 = alarme ligado

//---------- sm = SERVOMOTOR ----------
//sm1 = servo motor portao abrir
//sm0 = servo motor portao fechar

//pi = ping para retornar a funcao relatorioGeral()

//tp[float temparature] == variavel que vai representar a temperatura


//----------- Anotacoes -----------
//sizeof(a) / sizeof(int); <--> para ver o tamanho do array


//jkdjflsjkldf
//ccc
//xD