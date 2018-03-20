//CONEXIÓN DEL I2C:
//PIN SDA va al pin A4 de arduino
//PIN SCL va al pin A5 de arduino

//CONEXION DEL SERVO
//Cable rojo -->5v
//Cable marron --> GND
//Cable naranja --> pin 

#include <Wire.h>     //Declaración de libreria para usar modulo I2C
#include <LiquidCrystal_I2C.h>  //Declaracion de libreria para LCD
#include <Servo.h> //Declaración de libreria para el servomotor

//CONFIGURACIÓN DEL LCD
LiquidCrystal_I2C lcd(0x3F, 16, 2); //Ubicación de la memoria en donde esta el I2C

//CONFIGURACIÓN DEL SERVOMOTOR
Servo servo;

//DEFINICIÓN DE PINES 
const int pulsador1 = 4; //Pulsador para subir
const int pulsador2 = 7; //Pulsador para bajar
const int pulsador3 = 8; //Pulsador para dar OK
const int pinServo = 9; //Pin de PWM al que se conecta el Servo
const int ledPin = 13; //Pin para el LED de láser encendido 
const int buzzerPin = 5; // Pin para el buzzer


//BANDERAS
int flag_subir = 0; //Se definene las banderas para ver el estado del pulsador
int flag_bajar = 0;
int flag_ok = 0;

//DEFINICIÓN DE VARIABLES
int pulsador1_anterior = 0; //Variable para verificar el estado de los pulsadores
int pulsador2_anterior = 0;
int pulsador3_anterior = 0;
int estado_pantalla = 0; //variable que determina la pantalla a usarse
int angulo = 0; // variable en la que se va a guardar el valor del angulo que va a tener el polaroid
int inten_max = 0; //Se almacena la maxima intensidad que muestra la fotocelda obtenida del ADC

const int num_lecturas =10; // van a ser la cantidad de lecturas por cada angulo para sacar el promedio, me va a indicar el tamaño del
                           // arreglo (vector) para poder sacar el promedio

double inten_ins_prom = 0; //variable donde se va a almacenar la intensidd instantanea promedio obtenida de la comparacion de intensidades en cada angulo
double inten_instant = 0; //variable donde se va a guardar la intensidad promedio instantanea del angulo para verificar si es la maxima y determina con esta el 
                       //angulo de rotación
float tempC; //variable en la que se almacena la T° en °celcius

String sustancia; //Variable para guardar el nombre de la sustancia seleccionada


void setup() 
{ 
  Serial.begin (9600); //Defino monitor serie
  
  //Definición del Timer1
  //Timer1.initialize (500); //interrumpo el timer cada 150ms porque 50 es muy poco
  //Timer1.attachInterrupt(INT_TMR1);
  TCCR2A = 0x00; //timer operando en modo normal
  TCCR2B = 0x07; //preescaler 1:1024
  TCNT2 = 10; //cada 10 ms overflow
  TIMSK2 = 0x01; //hablita la interrupcion por timer 2

  //Definición LCD
  lcd.begin(); //Se inicia la pantalla
  lcd.backlight (); //Se enciende la luz de la pantalla

  //Definición de los pulsadores
  pinMode (pulsador1, INPUT);  //declaro como entradas los pines
  pinMode (pulsador2, INPUT);
  pinMode (pulsador3, INPUT);

  //Definicnión del luces
  pinMode (ledPin , OUTPUT);

  //Definicion del sonido
  pinMode (buzzerPin,OUTPUT);

  //Definicipon del servomotor
  servo.attach (pinServo); //Infico que servo va a ser manejado con el pin 9 del PWM   
  
  servo.write (0); //Se posiciona el servo en cero grados

}

void loop() 
{
  if (estado_pantalla == 0)
  {
    lcd.clear(); //Se limpia la pantalla por si quedo algo 
    //Se va a escribir la pantalla 1 y se va a pasar a pantalla 2
    lcd.setCursor (3,0);
    lcd.print ("Bienvenido");
    lcd.setCursor (0,1);
    lcd.print ("FLEU Polarimetro");
    
    delay (3000);
    estado_pantalla = 1;
    lcd.clear();
  }
  servo.write (0); 
  if (estado_pantalla == 1)
  {
    //Se va a escribir la pantalla 1 y se va a pasar a pantalla 2
    lcd.setCursor (0,0);
    lcd.print ("FLEU Polarimetro");
    lcd.setCursor (0,1);
    lcd.print ("Comenzar    OK");
    //verifico pulsador presionado
    if (flag_subir == 1)
    {
      flag_subir = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_bajar == 1)
    {
      flag_bajar = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_ok == 1)
    {
      estado_pantalla = 2;
      lcd.clear();
      flag_ok = 0; //Se baja la bandera subir para poder volver a usarla
    }
  }

  
  if (estado_pantalla == 2)
  {
    //Se va a escribir la pantalla 1 y se va a pasar a pantalla 2
    lcd.setCursor (0,0);
    lcd.print ("Selec. Sustancia");
    lcd.setCursor (0,1);
    lcd.print ("D-frutosa     OK");

    if (flag_subir == 1)
    {
      flag_subir = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_bajar == 1)
    {
      estado_pantalla = 3;
      lcd.clear();
      flag_bajar = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_ok == 1)
    {
      estado_pantalla = 6;
      sustancia = "D-fructosa";
      lcd.clear();
      flag_ok = 0; //Se baja la bandera subir para poder volver a usarla
    }
  }
  if (estado_pantalla == 3)
  {
    //Se va a escribir la pantalla 1 y se va a pasar a pantalla 2
    lcd.setCursor (0,0);
    lcd.print ("Selec. Sustancia");
    lcd.setCursor (0,1);
    lcd.print ("D-glucosa     OK");

    if (flag_subir == 1)
    {
      estado_pantalla = 2;
      lcd.clear();
      flag_subir = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_bajar == 1)
    {
      estado_pantalla = 4;
      lcd.clear();
      flag_bajar = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_ok == 1)
    {
      estado_pantalla = 6;
      sustancia = "D-glucosa";
      lcd.clear();
      flag_ok = 0; //Se baja la bandera subir para poder volver a usarla
    }
  }
  if (estado_pantalla == 4)
  {
    //Se va a escribir la pantalla 1 y se va a pasar a pantalla 2
    lcd.setCursor (0,0);
    lcd.print ("Selec. Sustancia");
    lcd.setCursor (0,1);
    lcd.print ("D-sacarosa    OK");

    if (flag_subir == 1)
    {
      estado_pantalla = 3;
      lcd.clear();
      flag_subir = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_bajar == 1)
    {
      estado_pantalla = 5;
      lcd.clear();
      flag_bajar = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_ok == 1)
    {
      estado_pantalla = 6;
      sustancia = "D-sacarosa";
      lcd.clear();
      flag_ok = 0; //Se baja la bandera subir para poder volver a usarla
    }
  }
  if (estado_pantalla == 5)
  {
    //Se va a escribir la pantalla 1 y se va a pasar a pantalla 2
    lcd.setCursor (0,0);
    lcd.print ("Selec. Sustancia");
    lcd.setCursor (0,1);
    lcd.print ("Lactosa       OK");

    if (flag_subir == 1)
    {
      estado_pantalla = 4;
      lcd.clear();
      flag_subir = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_bajar == 1)
    {
      flag_bajar = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_ok == 1)
    {
      estado_pantalla = 6;
      sustancia = "Lactosa";
      lcd.clear();
      flag_ok = 0; //Se baja la bandera subir para poder volver a usarla
    }
  }
  if (estado_pantalla == 6)
  {
    //Se va a escribir la pantalla 1 y se va a pasar a pantalla 2
    lcd.setCursor (0,0);
    lcd.print (sustancia);
    lcd.setCursor (14,0);
    lcd.print ("OK");
    lcd.setCursor (0,1);
    lcd.print ("Otra sustancia <");

    if (flag_subir == 1)
    {
      estado_pantalla = 2;
      lcd.clear();
      flag_subir = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_bajar == 1)
    {
      flag_bajar = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_ok == 1)
    {
      estado_pantalla = 7;
      lcd.clear();
      flag_ok = 0; //Se baja la bandera subir para poder volver a usarla
    }
  }
  if (estado_pantalla == 7)
  {
    lcd.clear(); //Se limpia la pantalla por si quedo algo 
    //Se va a escribir la pantalla 1 y se va a pasar a pantalla 2
    lcd.setCursor (1,0);
    lcd.print ("Procesando ...");
    lcd.setCursor (4,1);
    lcd.print ("Espere");

    //Enciendo led rojo 
    digitalWrite (ledPin,HIGH);
    
    sensado();

    digitalWrite (ledPin, LOW);
    digitalWrite (buzzerPin, HIGH);
    delay (1000);
    digitalWrite (buzzerPin, LOW);
    temperatura ();
    Serial.print ("El valor del angulo es: "); 
    Serial.println (angulo);
    Serial.print (" y corresponde a un valor de intensidad de: ");
    Serial.println (inten_max);
    Serial.print ("La temperatura sensada es de: ");
    Serial.println (tempC);
    
    /*SACAR DELAY Y PONER FUNCIÓN SENSADO QUE HACE LO SIGUIENTE
     * Enciendo el LED
     * Enciendo el laser
     * senso
     * obtengo valores
     * Apago el laser
     * Apago el LED
     * corrijo por temperatura
     * 
     */
    //verifico pulsador presionado
    if (flag_subir == 1)
    {
      flag_subir = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_bajar == 1)
    {
      flag_bajar = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_ok == 1)
    {
      flag_ok = 0; //Se baja la bandera subir para poder volver a usarla
    } 
    estado_pantalla = 8;
    lcd.clear();
  }
  if (estado_pantalla == 8)
  {
    lcd.setCursor(3,0);
    lcd.print(sustancia);
    lcd.setCursor(0,1);
    lcd.print ("Rot:   ");
    lcd.setCursor (4,1);
    lcd.print (angulo);    
    lcd.setCursor(7,1);
    lcd.print (char(223)); //Para que aparezca el simbolo de grado
    lcd.setCursor (8,1);
    lcd.print ("   .  ");
    lcd.setCursor (14,1);
    lcd.print (char(223)); //Para que aparezca el simbolo de grado
    lcd.setCursor (15,1);
    lcd.print ("Z");
    if (flag_subir == 1)
    {
      flag_subir = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_bajar == 1)
    {
      estado_pantalla = 9;
      lcd.clear();
      flag_bajar = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_ok == 1)
    {
      flag_ok = 0; //Se baja la bandera subir para poder volver a usarla
    } 
  }
  if (estado_pantalla == 9)
  {
    lcd.setCursor(3,0);
    lcd.print(sustancia);
    lcd.setCursor(0,1);
    lcd.print ("C:   ,  g/dl");
    if (flag_subir == 1)
    {
      estado_pantalla = 8;
      lcd.clear();
      flag_subir = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_bajar == 1)
    {
      estado_pantalla = 10;
      lcd.clear();
      flag_bajar = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_ok == 1)
    {
      flag_ok = 0; //Se baja la bandera subir para poder volver a usarla
    } 
  }
    if (estado_pantalla == 10)
  {
    lcd.setCursor(3,0);
    lcd.print(sustancia);
    lcd.setCursor(0,1);
    lcd.print ("C:   ,  ");
    lcd.setCursor(8,1);
    lcd.print (char(223)); //Para que aparezca el simbolo de grado
    lcd.setCursor (9,1);
    lcd.print ("BRIX");
    if (flag_subir == 1)
    {
      estado_pantalla = 9;
      lcd.clear();
      flag_subir = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_bajar == 1)
    {
      estado_pantalla = 11;
      lcd.clear();
      flag_bajar = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_ok == 1)
    {
      flag_ok = 0; //Se baja la bandera subir para poder volver a usarla
    } 
  }
  if (estado_pantalla == 11)
  {
    lcd.setCursor(3,0);
    lcd.print(sustancia);
    lcd.setCursor(0,1);
    lcd.print ("T");
    lcd.setCursor(1,1);
    lcd.print (char(223));
    lcd.setCursor (2,1);
    lcd.print (":");
    lcd.setCursor (3,1);
    lcd.print (tempC);
    lcd.setCursor(8,1);
    lcd.print(char(223));
    lcd.setCursor(9,1);
    lcd.print ("C");
    if (flag_subir == 1)
    {
      estado_pantalla = 10;
      lcd.clear();
      flag_subir = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_bajar == 1)
    {
      estado_pantalla = 12;
      lcd.clear();
      flag_bajar = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_ok == 1)
    {
      flag_ok = 0; //Se baja la bandera subir para poder volver a usarla
    } 
  }
  if (estado_pantalla == 12)
  {
    lcd.setCursor(0,0);
    lcd.print("Rehacer med.  >");
    lcd.setCursor(0,1);
    lcd.print ("Inicio        OK");
    if (flag_subir == 1)
    {
      estado_pantalla = 11;
      lcd.clear();
      flag_subir = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_bajar == 1)
    {
      estado_pantalla = 6;
      lcd.clear();
      flag_bajar = 0; //Se baja la bandera subir para poder volver a usarla
    }
    else if (flag_ok == 1)
    {
      estado_pantalla = 1;
      lcd.clear();
      flag_ok = 0; //Se baja la bandera subir para poder volver a usarla
    } 
  }
}

int sensado () //no se si tiene que ser void o int para mi tiene que ser int porque despues voy a usar el valor del angulo
               // para cuando lo quiera usar para mostrarlo en la pantalla
{    
  //Lo que hace es recorrer todos los angulos de 0° a 180° y comparar los valores de intensidad 
  // obtenidos para determinar el valor de la intensidad mas grande. En cada comparación el 
  // cuando da mas grande la intensidad se guarda el valor de esa intensidad y valor del angulo
  angulo = 0;
  
  servo.write(0); //inicio el servo en 0  

  for (int i= 0; i<=180; i++) //este ciclo me sirve para barrer todos los grados del servo
  {
    servo.write(i); //muevo el servo al grado marcado por i
    delay (200); //para darle tiempo al ADC
    inten_instant = filtro_dato (); // digo que la intensidad instantanea es el promedio tomada de 10 intensidades en un angulo determinado
   
    if (inten_max <= inten_instant) //comparo si la intensidad actual es mayor a la maxima
    {
      inten_max = inten_instant; // si es menor la intensidad maxima que la instantanea entonces
                                 // la maxima pasa a ser la instantanea y guardo ese valor como maximo
      angulo = i; //guardo el valor del angulo al que esta la intensidad actual que es la maxima
    }
    //delay (1000); //delay de 1 segundos para cambiar la intensidad
  }

  //Serial.print ("El valor del angulo es: ");
  //Serial.println (angulo);
  //Serial.print (" y corresponde a un valor de intensidad de: ");
  //Serial.println (inten_max); 
  //Con estas ultimas 4 lineas lo que hago es decir a que angulo esta la maxima intensidad y 
  // muestro los valores correspondientes

  return (angulo);
}

int filtro_dato (){

  int inten_muestra [num_lecturas]; // va a ser el arreglo con las intensidades instantaneas de cada angulo
  int vals_correctos = 0; //cdetermina cuantos valores son parecidos en la comparación de todos los valores
  int index = 0; //es la cantidad de valores que se usan en el promedio
  int limite = 5; // es el +-5 que tengo en cuenta para la dispersión de los valores medidos
  int limite2 = 6; //cantidad de comparaciones correctas para que sume
  int total = 0; //para la suma de los promedios de intensidades de muestras correctas
  inten_ins_prom = 0;

 // Serial.print ("El arreglo de intensidades de muestras es "); //para ver que este bien lo que calcule despues
               // ESTO HAY QUE BORRARLO EN EL CODIGO FINAL     
  
  for (int i =0; i<=10; i++)
  {
    
    inten_muestra [i] = analogRead (A0); // guardo 10 valores en el arreglo para despues compararlos
   // Serial.print (inten_muestra[i]); //muestro el vector para ver los valores y controlar esto despues se borra
   // Serial.print (" , "); //para que haya espacios entre los valores 
  }

  for (int i=0; i<=10; i++)//determino el valor que voy a comparar
  {
    for (int j=0; j<=10; j++) //voy a ir comparando con todos los valores
    {
      if (i != j) //comparo si i es distinto de j
      {
        if (abs (inten_muestra [i] - inten_muestra [j]) < limite) //determino si los valores estan a +- 5 datos entre si 
        {
          vals_correctos ++; //aumento en 1 cuando los valores con los que compara i estan a +-5 datos
        }
      }
    }
    
    if (vals_correctos >= limite2) //si la cantidad de comparaciones similares son += a 6 entonces suma para hacer el promedio
    {
      total += inten_muestra [i]; //sumo los valores correctos 
      index ++; // calculo la cantidad de valores correctos en este angulo
    }
    vals_correctos = 0;
  }

  inten_ins_prom = total / index; //calculo el promedio
 // Serial.println (" El promedio es: ");
 // Serial.println (inten_ins_prom);
  
  return (inten_ins_prom);    
}

float temperatura ()
{
  float temp_muestra[num_lecturas];
  int vals_correctos = 0; 
  int index = 0;
  int limite = 1; // es el +-1 que tengo en cuenta para la dispersión de los valores medidos al ser T° 
                  //se va a hacer mas precisa la medicion por eso es 1
  int limite2 = 6; //cantidad de comparaciones correctas para que sume
  float total = 0; //para la suma de los promedios de intensidades de muestras correctas
  float temp = 0;
  float tempMV = 0;
  float milivoltios = 0;
  tempC = 0; 

  for (int i = 0; i<=9; i++) //para que sean 10 datos i final debe ser 9 porque de 0 -9 hay de datos
  {
    temp = analogRead (A1); //se lee el valor que da el sensor de T
    
    milivoltios = (temp/1024.0)*5000; //transformo los datos binarios a mV
    tempMV = milivoltios/10; //transformo los mV a °C
    
    temp_muestra [i] = tempMV;

  //  Serial.print (temp_muestra [i]);
  //  Serial.print (" ; ");
    
  }
//  Serial.println ("fin 10 datos");

  for (int i=0; i<=10; i++)//determino el valor que voy a comparar
  {
    for (int j=0; j<=10; j++) //voy a ir comparando con todos los valores
    {
      if (i != j) //comparo si i es distinto de j
      {
        if (abs (temp_muestra [i] - temp_muestra [j]) < limite) //determino si los valores estan a +- 5 datos entre si 
        {
          vals_correctos ++; //aumento en 1 cuando los valores con los que compara i estan a +-1 datos
        }
      }
    }
    
    if (vals_correctos >= limite2) //si la cantidad de comparaciones similares son += a 6 entonces suma para hacer el promedio
    {
      total += temp_muestra [i]; //sumo los valores correctos 
      index ++; // calculo la cantidad de valores correctos en este angulo
   }
    vals_correctos = 0;

  }

  tempC = total / index;
  //Serial.print ("La T° promedio es: ");
  //Serial.print (tempC);
  //Serial.print (" °C"); 
  
  return (tempC);
}

ISR (TIMER2_OVF_vect)
{
  TCNT2 = 10; //registrador o reinicializador del timer2
   //Serial.println ("interrupcion por timer cada 50ms");
  if (flag_subir == 0 && flag_bajar == 0 && flag_ok == 0) //Verifica que no este haciendo ninguna 
  {                                                       //acción de los pulsadores
    if (digitalRead (pulsador1) == HIGH && pulsador1_anterior == 0) //Sube la bandera de subir para hacer las acciones correspondientes
    {                                   //si se presiono el pulscador correspondiente, si el pulsador esta manteniendose presionado no
        flag_subir = 1;                 //entra en este if y no manda acciones.
    }
    if (digitalRead (pulsador2) == HIGH && pulsador2_anterior == 0) //Sube la bandera de bajar para hacer las acciones correspondientes
    {                                   //si se presiono el pulscador correspondiente
        flag_bajar = 1;
    }
    if (digitalRead (pulsador3) == HIGH && pulsador3_anterior == 0)//Sube la bandera de OK para hacer las acciones correspondientes
    {                                   //si se presiono el pulscador correspondiente
        flag_ok = 1;
    }
  }
  pulsador1_anterior = digitalRead (pulsador1); //para revisar el estado anterior
  pulsador2_anterior = digitalRead (pulsador2);
  pulsador3_anterior = digitalRead (pulsador3);
}



/*void INT_TMR1() //INTERRUPCION TIMER1
{ 
  //Serial.println ("interrupcion por timer cada 50ms");
  if (flag_subir == 0 && flag_bajar == 0 && flag_ok == 0) //Verifica que no este haciendo ninguna 
  {                                                       //acción de los pulsadores
    if (digitalRead (pulsador1) == HIGH && pulsador1_anterior == 0) //Sube la bandera de subir para hacer las acciones correspondientes
    {                                   //si se presiono el pulscador correspondiente, si el pulsador esta manteniendose presionado no
        flag_subir = 1;                 //entra en este if y no manda acciones.
    }
    if (digitalRead (pulsador2) == HIGH && pulsador2_anterior == 0) //Sube la bandera de bajar para hacer las acciones correspondientes
    {                                   //si se presiono el pulscador correspondiente
        flag_bajar = 1;
    }
    if (digitalRead (pulsador3) == HIGH && pulsador3_anterior == 0)//Sube la bandera de OK para hacer las acciones correspondientes
    {                                   //si se presiono el pulscador correspondiente
        flag_ok = 1;
    }
  }
  pulsador1_anterior = digitalRead (pulsador1); //para revisar el estado anterior
  pulsador2_anterior = digitalRead (pulsador2);
  pulsador3_anterior = digitalRead (pulsador3);
}*/

