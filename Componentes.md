# Componentes
### MICROCONTROLADOR
Para este proyecto se está usando el arduino con el procesador ATmega328P.
Lo tienen por ejemplo: 
 - Arduino UNO
 - Arduino nano
 - Arduino pro mini

### SENSOR DE LUZ (LDR)
El LDR es una resistencia que varía su valor con la presencia de la luz.
Podemoso realizar o comprar un módulo que nos dará como resultado un valor digital. (será más fácil para el arduino trabajar con eso)

El módulo LDR es muy común por lo que podría encontrarlo en cualquier tienda electrónica:

![image](https://user-images.githubusercontent.com/88784932/129768056-dd907471-8f3b-4e7a-b09a-4edc8043c3d7.png)

El circuito se basa principalmente en un Op Amp como comparador y un potenciómetro con el que regularás el límite de luz:

![image](https://user-images.githubusercontent.com/88784932/129767971-94d21105-4202-490b-a64d-80ca26a9b020.png)

### SENSOR DE DISTANCIA (IR)
Su funcionamiento se basa principalmente en la emisión y recepción de luz infrarroja. 
Para este proyecto se optó por el sensor: SHARP GP2Y0A21

![image](https://user-images.githubusercontent.com/88784932/129664327-c8058edf-653e-47d5-8487-3cccffb10026.png)

Tiene un rango de 10 a 80cm. (Hay otros sensores de la misma marca con mayor rango)

Su operación consiste en la variación del voltaje según la distancia del obstáculo, para ello se suele usar el conversor analógico a digital, y luego aplicar una fórmula. 

![image](https://user-images.githubusercontent.com/88784932/129771654-eaa26ea2-5d9f-4c24-adb9-86937d618917.png)


!!POSBILE PROBLEMA A COMPROBAR: la luz del sol en la tarde podría influir en la medición, hay que comprobarlo

### Relay/transistor mosfet

### Lista General
