//CONEXIÓN DEL I2C:
//PIN SDA va al pin A4 de arduino
//PIN SCL va al pin A5 de arduino

#include <TimerOne.h> //Libreria para poder interrumpir por Timer1
#include <Wire.h>     //Declaración de libreria para usar modulo I2C
#include <LiquidCrystal_I2C.h>  //Declaracion de libreria para LCD

//CONFIGURACIÓN DEL LCD
LiquidCrystal_I2C lcd(0x3F, 16, 2); //Ubicación de la memoria en donde esta el I2C

//CONFIGURACIÓN DEL SERVOMOTOR

//DEFINICIÓN DE PINES 
const int pulsador1 = 5; //Pulsador para subir
const int pulsador2 = 6; //Pulsador para bajar
const int pulsador3 = 7; //Pulsador para dar OK
const int ledPin = 8; //Pin para el LED de láser encendido 


//BANDERAS
int flag_subir = 0; //Se definene las banderas para ver el estado del pulsador
int flag_bajar = 0;
int flag_ok = 0;

//DEFINICIÓN DE VARIABLES
int pulsador1_anterior = 0; //Variable para verificar el estado de los pulsadores
int pulsador2_anterior = 0;
int pulsador3_anterior = 0;
int estado_pantalla = 0; //variable que determina la pantalla a usarse

                           // arreglo (vector) para poder sacar el promedio

                       //angulo de rotación

String sustancia; //Variable para guardar el nombre de la sustancia seleccionada


void setup() 
{ 
  Serial.begin (9600); //Defino monitor serie
  
  //Definición del Timer1
  Timer1.initialize (500); //interrumpo el timer cada 150ms porque 50 es muy poco
  Timer1.attachInterrupt(INT_TMR1);

  //Definición LCD
  lcd.begin(); //Se inicia la pantalla
  lcd.backlight (); //Se enciende la luz de la pantalla

  //Definición de los pulsadores
  pinMode (pulsador1, INPUT);  //declaro como entradas los pines
  pinMode (pulsador2, INPUT);
  pinMode (pulsador3, INPUT);

  //Definicnión del luces
  pinMode (ledPin , OUTPUT);

  //Definicipon del servomotor
  

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
    delay (3000); //para que pase a la otra pantalla pero que esta se vea
    digitalWrite (ledPin, LOW);
    
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
    lcd.print (":  ,  ");
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


void INT_TMR1() //INTERRUPCION TIMER1
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
}

