#include <EtherCard.h> // librería para el uso del módulo ENC28J60
#include <SoftwareSerial.h> // librería para asignar dos pines adicionales para la Comunicación Serial
#include <DFRobotDFPlayerMini.h> // librería para controlar el reproductor MP3 DF Player Mini

#define PILOTO 6 // pin para conectar el LED piloto

SoftwareSerial mySS(8, 9); // RX, TX
DFRobotDFPlayerMini alertaMP3;

static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 }; // mac
byte Ethernet::buffer[700];

// dirección ip
static byte myip[] = { 192,168,1,202 }; // ip
// puerta de acceso
static byte gwip[] = { 192,168,1,1 }; // puerta de enlace

unsigned int myport = 2510; //puerto

char alerta1[] = "P215B7"; // código para prueba semanal
char alerta2[] = "S4S09"; // código para sismo

bool bandera = false; // bandera para indicar si el reproductor MP3 está funcionando
long t = 0; // tanto "t" como "T" se utilizan para medir el tiempo transcurrido desde que el reproductor inició la reproducción de la alerta
long T;

void setup() 
{
  Serial.begin(57600); // se inicia el puerto Serial en caso de necesitarlo
  ether.begin(sizeof Ethernet::buffer, mymac); // se inicializa el módulo ethernet
  ether.staticSetup(myip, gwip); // se asignan los parámetros de ip y puerta de enlace
  ether.udpServerListenOnPort(&udpSerialPrint, myport); // esta función indica desde qué puerto se recibe el mensaje enviado por el transmisor
  mySS.begin(9600); // se inicializa la comunicación serial con el reproductor MP3
  alertaMP3.begin(mySS); // se inicializa el módulo MP3
  alertaMP3.volume(10); // se determina el volumen. Los valores aceptados son 0 - 30
  pinMode(PILOTO, OUTPUT); // se configura como salida
  digitalWrite(PILOTO, LOW); // se asigna un valor bajo al piloto
}

void loop() 
{
  ether.packetLoop(ether.packetReceive()); //función necesaria para el funcionamiento del módulo. siempre debe ser agregada en la función "loop"
}

void udpSerialPrint(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len) // 'esta función es necesaria para recibir el mensaje. Aunque en el programa no se vea en qué momento se manda a llamar, está presente en el "loop"
{
  if(strcmp(data, alerta1) == 0) // compara el mensaje recibido con el mensaje de Alerta1
  {
    digitalWrite(PILOTO, HIGH); // el piloto parpadea si la alerta1 se recibe
    delay(500);
    digitalWrite(PILOTO, LOW);
    delay(500);
  }
  else if(strcmp(data, alerta2) == 0) // compara el mensaje recibido con el mensaje de Alerta1
  {
    if(bandera == false) //si el reproductor está apagado...
    {
      t = millis(); //se inicia el "cronómetro"
      alertaMP3.play(1); // se reproduce la pista 1 en el MP3
      bandera = true; // se indica que el reproductor está encendido
    }
    digitalWrite(PILOTO, HIGH); // el piloto parpadea si la alerta2 se recibe
    delay(500);
    digitalWrite(PILOTO, LOW);
    delay(500);
  }
  T = millis() - t; // se detiene el cronómetro
  if(T >= 60000) // si ha pasado más de un minuto desde que se reprodujo la pista...
  {
    bandera = false; // se indica que el reproductor ya no está reproduciendo nada
  }
}
