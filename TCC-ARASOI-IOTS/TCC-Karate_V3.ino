//Libraries
#include "BluetoothSerial.h" //Biblioteca do Bluetooth

BluetoothSerial SerialBT; //Comunicação do Bluetooth
bool isConnected = false;

void setup() {
  Serial.begin(115200); //Define a frequencia a ser usada na placa
  SerialBT.begin("ESP32_BT"); //começa o Bluetooth com o nome ESP32_BT - Brabo
  Serial.println("Bluetooth iniciando. Insira o endereço MAC do dispositivo: "); //Exibe a mensagem no Serial, pedindo o MAC do dispositivo
}

void loop() {
  /*Deus nos ajude pq essa porra não tá funcionando. Verificar o pq ele não tá lendo e nem identificando quando alguém tenta conectar nele*/

   //vai ler o MAC do usuario
  if (!isConnected){
    if (Serial.available()) {
      String macAddress = Serial.readStringUntil('\n');
      macAddress.trim();

    //vai ver se tiver conectado direito
      if (connectToDevice(macAddress)) {
        Serial.println("Conexão bem-sucedida"); //caso conecte
        isConnected = true;
      }else{
        Serial.println("Falha, em conectar LOL."); //caso de erro
      }
    }
  }
}

//enquanto conecta
bool connectToDevice(String macAddress) {
  Serial.print("Tentando conectar ao dispositivo: ");
  Serial.println(macAddress);


  if (SerialBT.connect(macAddress)) {
    Serial.println("Dispositivo conectado");//quando conectado
    return true;
  }else{
    Serial.println("Não foi possivel conectar ao dispositivo");//quando der merda
    return false;
  }
}
