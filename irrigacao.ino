#include <SoftwareSerial.h>

// Bluetooth RX/TX
SoftwareSerial BT(10, 11); // RX, TX

// Sensores
int sensores[4] = {A0, A1, A2, A3};

// Saída da bomba via relé
int releBomba = 8;

// Calibração da umidade
int secoMin = 600;  
int umidoMax = 400; 

// Converte a leitura do sensor para porcentagem
int converteParaUmidade(int leitura) {
  int umidade = map(leitura, secoMin, umidoMax, 0, 100);
  return constrain(umidade, 0, 100);
}

// Classificação da condição do solo
String classificaEstado(int umidade) {
  if (umidade < 30) return "SECO";
  if (umidade < 60) return "IDEAL";
  return "UMIDO";
}

void setup() {
  Serial.begin(9600);
  BT.begin(9600);

  pinMode(releBomba, OUTPUT);
  digitalWrite(releBomba, HIGH); // relé desligado (ativo em LOW)

  Serial.println("Sistema iniciado.");
  BT.println("Sistema iniciado.");
}

void ativaBomba(int vaso) {
  Serial.print("Bomba ativada no vaso ");
  Serial.println(vaso + 1);

  BT.print("Bomba ativada no vaso ");
  BT.println(vaso + 1);

  digitalWrite(releBomba, LOW);   // liga bomba
  delay(8000);                    // tempo de rega
  digitalWrite(releBomba, HIGH);  // desliga bomba

  Serial.println("Rega concluída.");
  BT.println("Rega concluída.");
}

void loop() {

  Serial.println("----- NOVA LEITURA -----");
  BT.println("----- NOVA LEITURA -----");

  for (int i = 0; i < 4; i++) {

    int leitura = analogRead(sensores[i]);
    int umidade = converteParaUmidade(leitura);
    String estado = classificaEstado(umidade);

    // Envio Serial
    Serial.print("VASO ");
    Serial.println(i + 1);

    Serial.print("Leitura: ");
    Serial.println(leitura);

    Serial.print("Umidade: ");
    Serial.print(umidade);
    Serial.println("%");

    Serial.print("Estado: ");
    Serial.println(estado);
    Serial.println();

    // Envio Bluetooth
    BT.print("VASO ");
    BT.println(i + 1);

    BT.print("Leitura: ");
    BT.println(leitura);

    BT.print("Umidade: ");
    BT.print(umidade);
    BT.println("%");

    BT.print("Estado: ");
    BT.println(estado);
    BT.println();

    if (estado == "SECO") {
      Serial.println("Ação: REGAR");
      BT.println("Ação: REGAR");
      ativaBomba(i);
    } else {
      Serial.println("Ação: Nenhuma");
      BT.println("Ação: Nenhuma");
    }

    delay(500);
  }

  Serial.println("Próxima leitura em 5 min...\n");
  BT.println("Próxima leitura em 5 min...\n");

  delay(5 * 60 * 1000);
}
