#include <EtherCard.h> // librería para el uso del módulo ENC28J60

#define ALERTA1 5 // pin para recibir la Alerta1
#define ALERTA2 4 // pin para recibir la Alerta2
#define PILOTO 6 // pin para conectar el LED piloto

static byte mymac[] = { 0x74, 0x69, 0x69, 0x2D, 0x23, 0x62}; // mac
byte Ethernet::buffer[700];

// declaración de puertos
const int dstPort PROGMEM = 2510; // puerto destino
const int srcPort PROGMEM = 2510; // puerto fuente

// dirección ip
static byte myip[] = { 192,168,1, 201 }; //ip
// puerta de acceso
static byte gwip[] = { 192,168,1, 1 }; //puerta de enlace
// máscara
static byte mask[] = { 255, 255, 255,0 }; //máscara
//dirección de Broadcast
static byte broadip[] = { 255,255,255,255 }; // ip para broadcast

//Texto a enviar
char alerta1[] = "P215B7"; // código para prueba semanal
char alerta2[] = "S4S09"; // código para sismo

void setup()
{
  Serial.begin(57600); // se inicia el puerto Serial en caso de necesitarlo
  if(ether.begin(sizeof Ethernet::buffer, mymac) == 0) // se inicializa el módulo ethernet
    Serial.println( "Failed to access Ethernet controller"); // se envía un mensaje de error en caso de  no poder iniciar el módulo
  ether.staticSetup(myip, gwip, NULL, mask); // se asignan los parámetros de ip, puerta de enlace, dns y máscara

  //Se asigna el modo de los pines
  pinMode(ALERTA1, INPUT_PULLUP); //se configura como entrada
  pinMode(ALERTA2, INPUT_PULLUP); // se configura como entrada
  pinMode(PILOTO, OUTPUT); // se configura como salida
  digitalWrite(PILOTO, LOW); // se asigna un valor bajo al piloto
  
  ether.printIp("IP:  ", ether.myip); // se imprime la dirección ip en el puerto serial
  ether.printIp("GW:  ", ether.gwip); // se imprime la puerta de enlace en el puerto serial
}

void loop()
{
  ether.packetLoop(ether.packetReceive()); //función necesaria para el funcionamiento del módulo. siempre debe ser agregada en la función loop
  int A1, A2; // se declaran las variables para recibir el valor leído en los pines para las alertas
  A1 = digitalRead(ALERTA1); //se lee el pin para Alerta1
  A2 = digitalRead(ALERTA2); //se lee el pin para Alerta2
  if(A1 == 0) //Si se recibe una señal en la Alerta1...
  {
    ether.sendUdp(alerta1, sizeof(alerta1), srcPort, broadip, dstPort); // se envía la alerta por medio de ethernet utilizando el protocolo udp
    digitalWrite(PILOTO, HIGH); // se configura el piloto para parpadear lo que dure la señal de alerta
    delay(500);
    digitalWrite(PILOTO, LOW);
    delay(500);
  }
  if(A2 == 0) //Si se recibe una señal en la Alerta2...
  {
    ether.sendUdp(alerta2, sizeof(alerta2), srcPort, broadip, dstPort); // se envía la alerta por medio de ethernet utilizando el protocolo udp
    digitalWrite(PILOTO, HIGH); // se configura el piloto para parpadear lo que dure la señal de alerta
    delay(500);
    digitalWrite(PILOTO, LOW);
    delay(500);
  }
}
