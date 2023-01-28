#include <Arduino.h>
#include <avr/wdt.h>
#include <SoftwareSerial.h>
SoftwareSerial btSerial(8, 9); // RX, TX

int pino_nivel_agua = 2; // nivel da agua
int pino_encher_agua = 3;
int pino_bomba_agua = 4;
int pino_agitacao = 5;
int pino_centrifugacao = 6;
int tempo_agitacao = 1;
int tempo_centrifugacao = 1;
int tempo_molho = 1 ;
int segundos_anterior = 0;


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
      int minutos = (millis() - inicio) / 1000 / 60 - tempo_agitacao;
      int segundos = 59 + ( minutos * 60 - ((millis() - inicio) / 1000 - 60 * tempo_agitacao));
      if(segundos != segundos_anterior){
        Serial.print(minutos + 1);
      Serial.print(" min ");
      Serial.print(segundos);
      Serial.print(" seg ");
      Serial.println("");
      segundos_anterior = segundos;
      }
      
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
    delay(1000);
    while (digitalRead(pino_nivel_agua) == LOW) { // verifica se ja esta no nivel de agua
      //loop e entrada de agua
    }
    digitalWrite(pino_encher_agua, LOW); // desliga entrada de agua
    agitacao();
  }
} 

void molho() {
  Serial.println("Modo de molhor, verificando nivel de agua.");
  int nivel = digitalRead(pino_nivel_agua);
  if (nivel) {
    Serial.println("Nivel de agua parece bom, iniciando contagem de tempo de molhor.");
    long inicio = millis();
    while (((millis() - inicio) / 1000) < 60 * tempo_molho) { 
      //loop de tempo de molho
      int minutos = (millis() - inicio) / 1000 / 60 - tempo_molho;
      int segundos = 59 + ( minutos * 60 - ((millis() - inicio) / 1000 - 60 * tempo_molho));
      if(segundos != segundos_anterior){
        Serial.print(minutos + 1);
      Serial.print(" min ");
      Serial.print(segundos);
      Serial.print(" seg ");
      Serial.println("");
      segundos_anterior = segundos;
      }
    }
    Serial.println("Tempo de molho finalizado.");
  }

  if (nivel == 0) {
    Serial.println("Nivel de agua parece baixo, liberando agua para encher.");
    digitalWrite(pino_encher_agua, HIGH);
    long inicio = millis();
    delay(1000);
    while (digitalRead(pino_nivel_agua) == LOW) {
      //loop ate encher
    }
    Serial.println("Nivel de agua atingido, desligando agua.");
    digitalWrite(pino_encher_agua, LOW);
    molho();
  }
}

void esvaziar() {
  Serial.println("Esvaziando...");
  int nivel = digitalRead(pino_nivel_agua);
  if (nivel == 0) {
    Serial.println("Ligando bomba dagua para esvaziar.");
    digitalWrite(pino_bomba_agua, HIGH);
    long inicio = millis();
    while (((millis() - inicio) / 1000) < 60 * 1) {
      //loop de tempo para esgotar agua
      int minutos = (millis() - inicio) / 1000 / 60 - 1;
      int segundos = 59 + ( minutos * 60 - ((millis() - inicio) / 1000 - 60 * 1));
      if(segundos != segundos_anterior){
        Serial.print(minutos + 1);
      Serial.print(" min ");
      Serial.print(segundos);
      Serial.print(" seg ");
      Serial.println("");
      segundos_anterior = segundos;
      }
    }
    Serial.println("Tempo de esvaziar finalizado.");
    digitalWrite(pino_bomba_agua, LOW);
  }
  if (nivel == 1) {
    Serial.println("Esvaziando... ate nivel mais baixo.");
    digitalWrite(pino_bomba_agua, HIGH);
    long inicio = millis();
    delay(1000);
    while (digitalRead(pino_nivel_agua) == 1) {
      //loop ate o nivel estiver abaixo do definido
    }
    esvaziar();
    Serial.println("Processo de esvaziar concluido");
  }

}

void centrifugar() {
  Serial.println("Iniciando centrifugação");
  int nivel = digitalRead(pino_nivel_agua);
  if (nivel == 1)
    Serial.println("Nivel de agua parece alto.");
    esvaziar();
  if (nivel == 0) {
    Serial.println("Iniciando centrifugação.");
    digitalWrite(pino_agitacao, LOW);
    digitalWrite(pino_centrifugacao, HIGH);
    long inicio = millis();
    while (((millis() - inicio) / 1000) < (60 * tempo_centrifugacao / 2)) {
      //loop de tempo para centrifugação
      int minutos = (millis() - inicio) / 1000 / (60 - tempo_centrifugacao /2);
      int segundos = 59 + ( minutos * 60 - ((millis() - inicio) / 1000 - 60 * (tempo_centrifugacao / 2)));
      if(segundos != segundos_anterior){
        Serial.print(minutos + 1);
      Serial.print(" min ");
      Serial.print(segundos);
      Serial.print(" seg ");
      Serial.println("");
      segundos_anterior = segundos;
      }
    }
    Serial.println("Ligando bomba para retirar resto de agua.");
    digitalWrite(pino_bomba_agua, HIGH);
    while (((millis() - inicio) / 1000) < (60 * tempo_centrifugacao)) {
      //loop de tempo  centrifugação
      int minutos = (millis() - inicio) / 1000 / 60 - tempo_centrifugacao;
      int segundos = 59 + ( minutos * 60 - ((millis() - inicio) / 1000 - 60 * tempo_centrifugacao));
      if(segundos != segundos_anterior){
        Serial.print(minutos + 1);
      Serial.print(" min ");
      Serial.print(segundos);
      Serial.print(" seg ");
      Serial.println("");
      segundos_anterior = segundos;
      }
    }
    digitalWrite(pino_bomba_agua, LOW);
    Serial.println("Centrifugação finalizado.");
  }
}


void principal(String comando_recebido) {
  Serial.println(comando_recebido);
  if (comando_recebido == "1"){
    Serial.println("Brancos");
    agitacao();
    molho();
    agitacao();
    esvaziar();
    centrifugar();
    agitacao();
    esvaziar();
    centrifugar();
    Serial.println("Modo 1 Brancos finalizado");

  }
  if (comando_recebido == "2"){
    Serial.println("Coloridos resistentes");
    agitacao();
    molho();
    agitacao();
    agitacao();
    esvaziar();
    centrifugar();
    agitacao();
    esvaziar();
    centrifugar();
    Serial.println("Modo 2 Coloridos resistentes finalizado");

  }
  if (comando_recebido == "3"){
    Serial.println("Coloridos delicados");
    agitacao();
    molho();
    esvaziar();
    centrifugar();
    agitacao();
    esvaziar();
    centrifugar();
    Serial.println("Modo 3 Coloridos delicados finalizado");

  }
  if (comando_recebido == "4"){
    Serial.println("Enxague");
    esvaziar();
    agitacao();
    centrifugar();
    Serial.println("Modo 4 Enxague finalizado");

  }
  if (comando_recebido == "5"){
    Serial.println("Centrifugação");
    esvaziar();
    centrifugar();
    Serial.println("Modo 5 Centrifugação finalizado");

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
  //Serial.println("Recebendo dados");
  while (btSerial.available()) {
     char comandoRec = btSerial.read();
     btComando += char( comandoRec );
     //Serial.print(comandoRec);
     if (comandoRec) {
        if (btComando == "1" or btComando == "2" or btComando == "3" or btComando == "4" or btComando == "5" or btComando == "6" or btComando == "0") {
            Serial.println(comandoRec);
            Serial.println("----------------");
            delay(1000);
            principal(btComando);
            
        }
        btComando = ""; 
     }
  }
   delay(300);  
}
