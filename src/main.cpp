#include <Arduino.h>
#include <avr/wdt.h>
#include <SoftwareSerial.h>
SoftwareSerial btSerial(8, 9); // RX, TX

int pino_nivel_agua = 2; // nivel da agua
int pino_encher_agua = 13;
int pino_bomba_agua = 13;
int pino_agitacao = 13;
int pino_centrifugacao = 13;
int tempo_agitacao = 1;
int tempo_centrifugacao = 1;
int tempo_molho = 1 ;


void agitacao() {
  Serial.println("Iniciando modo agitação");
  int nivel = digitalRead(pino_nivel_agua); // vazio 0 | cheio 1
  if (nivel) {
    Serial.println("Nivel de agua atingido");
    Serial.println(nivel);
    digitalWrite(pino_centrifugacao, LOW);  //desativa rele de centrifugacao 
    digitalWrite(pino_agitacao, HIGH); //aciona rele para agitacao
    Serial.println("Ativando rele de agitação");
    long inicio = millis(); //tempo atual
    while (((millis() - inicio) / 1000) < (60 * tempo_agitacao)) { 
      //loop ate finalizar o tempo de agitacao
    }
    Serial.println("Tempo de agitação finalizado");
    digitalWrite(pino_centrifugacao, LOW);  //desativa rele de centrifugacao 
    digitalWrite(pino_agitacao, LOW); //aciona rele para agitacao
    Serial.println("Motor desligado");
    
  }

  if (nivel == 0) {
    Serial.println("Nivel de agua baixo, enchendo...");
    Serial.println(nivel);
    digitalWrite(pino_encher_agua, HIGH); // liga entrada de agua
    long inicio = millis();
    while (digitalRead(pino_nivel_agua) == LOW) { // verifica se ja esta no nivel de agua
      //loop e entrada de agua
    }
    digitalWrite(pino_encher_agua, LOW); // desliga entrada de agua
    Serial.println("Nivel de agua atingido");
    agitacao();
  }
} 

void molho() {
  int nivel = digitalRead(pino_nivel_agua);
  if (nivel) {
    long inicio = millis();
    while (((millis() - inicio) / 1000) < 60 * tempo_molho) { 
      //loop de tempo de molho
    }
  }

  if (nivel == 0) {
    digitalWrite(pino_encher_agua, HIGH);
    long inicio = millis();
    while (digitalRead(pino_nivel_agua) == LOW) {
      //loop ate encher
    }
    digitalWrite(pino_encher_agua, LOW);
    molho();
  }
}

void esvaziar() {
  int nivel = digitalRead(pino_nivel_agua);
  if (nivel == 0) {
    digitalWrite(pino_bomba_agua, HIGH);
    long inicio = millis();
    while (((millis() - inicio) / 1000) < 60 * 1) {
      //loop de tempo para esgotar agua
    }
    digitalWrite(pino_bomba_agua, LOW);
  }
  if (nivel == 1) {
    digitalWrite(pino_bomba_agua, HIGH);
    long inicio = millis();
    while (digitalRead(pino_nivel_agua) == 1) {
      //loop ate o nivel estiver abaixo do definido
    }
    esvaziar();
  }

}

void centrifugar() {
  int nivel = digitalRead(pino_nivel_agua);
  if (nivel == 1)
    esvaziar();
  if (nivel == 0) {
    digitalWrite(pino_agitacao, LOW);
    digitalWrite(pino_centrifugacao, HIGH);
    long inicio = millis();
    while (((millis() - inicio) / 1000) < (60 * tempo_centrifugacao / 2)) {
      //loop de tempo para centrifugação
    }
    digitalWrite(pino_bomba_agua, LOW);
    while (((millis() - inicio) / 1000) < (60 * tempo_centrifugacao)) {
      //loop de tempo  centrifugação      
    }
  }
}


void principal(String comando_recebido) {
  if (comando_recebido == "1"){
    Serial.println("Brancos");
    agitacao();
    molho();
    esvaziar();
    centrifugar();
  }
  if (comando_recebido == "2"){
    Serial.println("Coloridos resistentes");
    agitacao();
    molho();
    agitacao();
    esvaziar();
    centrifugar();
  }
  if (comando_recebido == "3"){
    Serial.println("Coloridos delicados");
    agitacao();
    molho();
    agitacao();
    esvaziar();
    centrifugar();
  }
  if (comando_recebido == "4"){
    Serial.println("Enxague");
    esvaziar();
  }
  if (comando_recebido == "5"){
    Serial.println("Centrifugação");
    esvaziar();
    centrifugar();
  }
  if (comando_recebido == "6"){
    Serial.println("Personalizado");
    //coloque aqui modo personalizado 
  }
  if (comando_recebido == "0"){
    Serial.println("Cancelar");
    //cancelar desliga tudo
  }

}


void setup() {
  wdt_disable(); // monitoramento
  btSerial.begin(115200);//bluetooth
  Serial.begin(9600);//porta serial
  pinMode(pino_nivel_agua,INPUT);
  pinMode(pino_bomba_agua,OUTPUT);
  pinMode(pino_agitacao,OUTPUT);
  pinMode(pino_centrifugacao,OUTPUT);
  pinMode(pino_encher_agua,OUTPUT);
  // desliga motor 
  digitalWrite(pino_agitacao, LOW);
  digitalWrite(pino_centrifugacao, LOW);
  delay(3000); 
}

void loop() {
  static String btComando;
  Serial.println("Recebendo dados");
  while (btSerial.available()) {
     char comandoRec = btSerial.read();
     btComando += char( comandoRec );
     //Serial.print(comandoRec);
     if (comandoRec) {
        if (btComando == "1" or btComando == "2") {
            principal(btComando);
            Serial.println(comandoRec);
            Serial.print("----------------");
            delay(5000);
        }
        btComando = ""; 
     }
  }
     
}
