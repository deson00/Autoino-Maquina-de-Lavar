#include <Arduino.h>
#include <avr/wdt.h>

String entrada ;

int pino_nivel_agua = 2; // nivel da agua
int pino_encher_agua = 3;
int pino_bomba_agua = 4;
int pino_agitacao = 5;
int pino_centrifugacao = 6;
int tempo_agitacao = 5;
int tempo_centrifugacao = 2;
int tempo_molho = 5 ;

void setup() {
  wdt_disable(); // monitoramento
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
  principal(); // Função de tarefaz
}

void principal() {
  int modo = 1;
  if (modo == "1"){
    agitacao();
    molho();
    esvaziar();
    centrifugar();
  }
}

void agitacao() {
  nivel = digitalRead(pino_nivel_agua); // vazio 0 | cheio 1
  if (nivel) {
    digitalWrite(pino_centrifugacao LOW);  //desativa rele de centrifugacao 
    digitalWrite(pino_agitacao, HIGH); //aciona rele para agitacao
    long inicio = millis(); //tempo atual
    while (((millis() - inicio) / 1000) < 60 * tempo_agitacao) { 
      //loop ate finalizar o tempo de agitacao
      int minutos = (millis() - inicio) / 1000 / 60 - tempo_agitacao;
      int segundos = 59 + ( minutos * 60 - ((millis() - inicio) / 1000 - 60 * tempo_agitacao));
    }
    digitalWrite(pino_centrifugacao LOW);  //desativa rele de centrifugacao 
    digitalWrite(pino_agitacao, LOW); //aciona rele para agitacao
    
  }

  if (nivel == 0) {
    digitalWrite(pino_encher_agua, HIGH); // liga entrada de agua
    long inicio = millis();
    while (digitalRead(pino_nivel_agua) == LOW) { // verifica se ja esta no nivel de agua
      //loop e entrada de agua
    }
    digitalWrite(pino_encher_agua, LOW); // desliga entrada de agua
    agitacao();
  }
} 

void molho() {
  nivel = digitalRead(pino_nivel_agua);
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
  nivel = digitalRead(pino_nivel_agua);
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
  nivel = digitalRead(pino_nivel_agua);
  if (nivel == 1)
    esvaziar();
  if (nivel == 0) {
    digitalWrite(pino_agitacao, LOW);
    digitalWrite(pino_centrifugacao, HIGH);
    long inicio = millis();
    while (((millis() - inicio) / 1000) < 60 * tempo_centrifugacao / 2) {
      //loop de tempo para centrifugação
    }
    digitalWrite(pino_bomba_agua, LOW);
    while (((millis() - inicio) / 1000) < 60 * tempo_centrifugacao) {
      //loop de tempo  centrifugação      
    }
  }
}
