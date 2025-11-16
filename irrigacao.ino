int sensor1 = A0;
int sensor2 = A1;
int sensor3 = A2;
int sensor4 = A3;

int releBomba = 8;    // Pino do relé

// LIMITES DE UMIDADE
int limiteSeco = 550;   // acima disso = solo seco
int limiteUmido = 450;  // abaixo disso = solo úmido

void setup() {
  Serial.begin(9600);         
  pinMode(releBomba, OUTPUT);
  digitalWrite(releBomba, HIGH);  // Relé desligado (ativa com LOW)

  delay(2000);
  Serial.println("Sistema de Irrigacao Iniciado");
}


// Leitura dde sensores
void lerSensores(int &s1, int &s2, int &s3, int &s4) {
  s1 = analogRead(sensor1);
  s2 = analogRead(sensor2);
  s3 = analogRead(sensor3);
  s4 = analogRead(sensor4);
}

// função para analisar o solo
bool soloEstaSeco(int s1, int s2, int s3, int s4) {
  if (s1 > limiteSeco || s2 > limiteSeco || s3 > limiteSeco || s4 > limiteSeco) {
    return true;
  }
  return false;
}


// LOOP PRINCIPAL
void loop() {

  int s1, s2, s3, s4;
  lerSensores(s1, s2, s3, s4);

  if (soloEstaSeco(s1, s2, s3, s4)) {
    digitalWrite(releBomba, LOW);   // Liga bomba
  } else {
    digitalWrite(releBomba, HIGH);  // Desliga bomba
  }


  // comandos via bluetooth
  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');

    comando.trim();
    comando.toUpperCase();

    if (comando == "LIGA") {
      digitalWrite(releBomba, LOW);
      Serial.println("Bomba LIGADA pelo Bluetooth");
    }

    if (comando == "DESLIGA") {
      digitalWrite(releBomba, HIGH);
      Serial.println("Bomba DESLIGADA pelo Bluetooth");
    }

    if (comando == "STATUS") {
      Serial.print("Vaso 1: "); Serial.println(s1);
      Serial.print("Vaso 2: "); Serial.println(s2);
      Serial.print("Vaso 3: "); Serial.println(s3);
      Serial.print("Vaso 4: "); Serial.println(s4);
    }
  }

  delay(1000);
}
