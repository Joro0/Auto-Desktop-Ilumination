//Variables:
byte cont=0;
byte temp=0;
bool ciclo=true;

//Funciones:
void ChekeoGeneral();
void ChekeoPresencia();
void ChekeoAusencia();

void setup() {
  cli();      //deshabilito interrupciones para configuración
  
  //PUERTOS
  DDRB=0x08;  //B0,1: Infrarojo 1,2 (IN) - IR=0: obstáculo
              //B2:   Luz LDR       (IN) - LDR=1: no hay luz
              //B3:   LED           (OUT)
  
  //TIMER2
  TCCR2A=0x00;
  TCCR2B=0x07;
  TIMSK2=0x01;
  TCNT2=0x00;
   
}

void loop() {
  switch(PINB&0x03){
    case 0x03:
      ChekeoGeneral();
      break;
    case 0x02:
      ChekeoPresencia();
      break;
    case 0x01:
      ChekeoAusencia();
      break;
    default:
      break;
  }
}

void ChekeoGeneral(){                                   //Reviso si hay personas cuando esté oscuro
  Serial.print("Personas:");
  Serial.print(cont);
  Serial.print("\n");
  
  if (cont>0 && bitRead(PINB,2)) bitWrite(PORTB,3,1);   //prendo LED si hay personas y está oscuro
  else bitWrite(PORTB,3,0);                             //apago LED si no hay personas o hay luz
}

void ChekeoPresencia(){                                 //Reviso si alguien entró por completo
  int aux=0;
  ciclo=true;
  while (ciclo){                                        //Loop hasta saber si salió o entró por completo
    switch(PINB&0x03){
      case 0x02:                                        //Espero cambio
        aux=0;
        TCNT2=0x00;                                     //seteo contador
        temp=0;
        break;

      case 0x01:                                        //Posible entrada (prendo led momentaneo)    
      case 0x00:
        aux=1;
        if (bitRead(PINB,2)) bitWrite(PORTB,3,1);
        TCNT2=0x00;                                     //seteo contador
        temp=0;
        break;

      case 0x03:                                        //Confirmación si entró o salió por completo
        sei();                                          //activo contador
        if (temp>=122){                                 //Confirmo luego de 2 segundos (2 seg = 122)
          if (aux==1) cont+=1;
          cli();                                        //desactivo contador
          ciclo=false;
        }
        break;

      default:
        break;
    }
  }
}

void ChekeoAusencia(){
  int aux=0;
  ciclo=true;
  while (ciclo){
    switch(PINB&0x03){
      case 0x01:                                        //Espero cambio
        aux=0;
        TCNT2=0x00;                                     //seteo contador
        temp=0;
        break;

      case 0x02:                                        //Posible salida   
      case 0x00:
        aux=1;
        TCNT2=0x00;                                     //seteo contador
        temp=0;
        break;

      case 0x03:                                        //Confirmación si salió o entró por completo
        sei();                                          //activo contador
        if (temp>=122){                                 //Confirmo luego de 2 segundos
          if (aux==1 && cont>0) cont-=1;
          cli();                                        //desactivo contador
          ciclo=false;
        }
        break;
        
      default:
        break;  
    }
  }
}

ISR(TIMER2_OVF_vect){
  temp+=1;
}
