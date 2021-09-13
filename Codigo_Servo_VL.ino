//LIBRERIAS
#include <Servo.h>
#include <Wire.h>
#include <VL53L0X.h>      //el wen pololu 
                          //https://github.com/pololu/vl53l0x-arduino/tree/master/examples

//FUNCIONES
void ChekeoGeneral();
void ChekeoContinuo();
bool ChekeoPresencia();
void MoverServo(int ang);

//OBJETOS
Servo servo;
VL53L0X sensor;

//VARIABLES
int PINLED    = 4;        //D4
int PINSERVO  = 3;        //D3
int PINLDR    = 2;        //D2: 1=no hay luz

int PULSOMIN  = 350;      //microsegundos servo
int PULSOMAX  = 2300;

int RANGOMIN  = 150;      //distancia sensor (mm)
int RANGOMAX  = 1000;

int ANG_RECTO = 90;    //ajuste (90)   //angulos servo (ajustar previamente)
int ANG_PRES  = 120;  //*auemntar (120) 
int ANG_AUS   = 60;
int ANG_CHEKEO = 170; //*ajustar (160)

int conteo    = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(PINLDR,INPUT);
  pinMode(PINLED,OUTPUT);
  
  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }
  sensor.startContinuous(100);      //Intervalo de tiempo para cada leida
  
  servo.attach(PINSERVO, PULSOMIN, PULSOMAX);
}

void loop() {
  //CHEKEO LUZ
  while (!digitalRead(PINLDR)){    //iniciar chekeo cuando no haya luz (ldr=1)
    Serial.print("HAY LUZ");
    Serial.print("\n");
    delay(500);
  }

  Serial.print("NO HAY LUZ");
  Serial.print("\n");
  Serial.print("PREPARANDO...");
  Serial.print("\n");
  delay(2000);

  ChekeoGeneral();
  
  ChekeoContinuo();

  //Confirmo tiempo para modo sleep

    //Activar modo sleep
      //habría reiniciada cuando despierte?

    //Volver al loop si todavía no es momento
    
  Serial.print("TERMINÓ PRUEBA");
  Serial.print("\n");
  delay(100);
  
  while (1){}
  
}

void ChekeoGeneral(){                     //Chekeo si ya hay una persona en el escritorio
  int distancia = 0;
  
  Serial.print("EN CHEKEO GENERAL");
  Serial.print("\n");
  delay(20);
  
  MoverServo(ANG_CHEKEO);
  delay(1000);

  for(int i=1; i<=3; i++){            //lectura persona (3 revisiones)
    if (ChekeoPresencia()){
      digitalWrite(PINLED,HIGH);
      conteo=1;
      Serial.print("HAY PERSONA");
      Serial.print("\n");
      delay(20);
      return;
    }
  }
  digitalWrite(PINLED,LOW);
  conteo=0;
  Serial.print("NO HAY PERSONA");
  Serial.print("\n");
  delay(20);
  
}

void ChekeoContinuo(){
  int distancia;
  int ESTADO=0;
  const int CHEKEO=0;
  const int CONF_PRES=1;
  const int CONF_AUS=2;
  bool ciclo=true;

  Serial.print("EN CHEKEO CONTINUO");
  Serial.print("\n");
  delay(20);
  while (ciclo){
    switch (ESTADO){
      
      case CHEKEO:
        MoverServo(ANG_RECTO);
        Serial.print("EN CHEKEO");
        Serial.print("\n");
        delay(20);        
        //*FUNCION ESPECIAL: RELOJ (se verá luego)
        while (!ChekeoPresencia()){
          if (!digitalRead(PINLDR)){                   //chekeo luz
            digitalWrite(PINLED,LOW);
            ciclo=false;
            break;
          }
          
          Serial.print("PERSONAS: ");
          Serial.print(conteo);
          Serial.print("\n");
          delay(20);       
        }
        ESTADO=CONF_PRES;
        break;

        
      case CONF_PRES:
        while (ChekeoPresencia()){}     //espero que sensor vuelva a 0 para continuar
        MoverServo(ANG_PRES);
        Serial.print("CONFIRMANDO PRESENCIA");
        Serial.print("\n");
        delay(20);
        for(int i=1; i<=3; i++){        //lectura presencia (3 revisiones)
          if (ChekeoPresencia()){
            conteo+=1;
            digitalWrite(PINLED,HIGH);
            ESTADO=CHEKEO;
            break;
          }
          else ESTADO=CONF_AUS;
        }
        break;


      case CONF_AUS:    //Se optará por lo simple: Ausencia cuando no presencia es falso
                        //El ángulo de Chekeo y Presencia podrían modificarse para evitar falsas entradas.
        Serial.print("CONFIRMANDO AUSENCIA");
        Serial.print("\n");
        delay(20);
        
        if (conteo>0) conteo-=1;
        if (conteo==0) digitalWrite(PINLED,LOW);
        ESTADO=CHEKEO;
        break;
        
      default:
        ESTADO=CHEKEO;
        break;
    }
  }
}

bool ChekeoPresencia(){
  int distancia;
  distancia = sensor.readRangeContinuousMillimeters();
  if (distancia>RANGOMIN && distancia<RANGOMAX){
    return true;
  }
  else{
    return false;
  }
}

void MoverServo(int ang){
  servo.attach(PINSERVO, PULSOMIN, PULSOMAX);
  servo.write(ang);
  delay(200);         //mínimo:     50
                      //normal:     100
  servo.detach();
}
