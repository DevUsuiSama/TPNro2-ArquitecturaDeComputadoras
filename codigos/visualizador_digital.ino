// C++ code
//

#include <Servo.h>

class Configuracion
{
private:
    int pin;

public:
    Configuracion(int pin)
    {
        this->pin = pin;
    }

    void salida()
    {
        pinMode(pin, OUTPUT);
    }

    void entrada()
    {
        pinMode(pin, INPUT);
    }

    int getPin()
    {
        return pin;
    }
};

class VisualizadorDigital
{
private:
    Configuracion *entrada_1;
    Configuracion *entrada_2;
    Configuracion *entrada_3;
    Configuracion *entrada_4;
    Configuracion *transistor_1;
    Configuracion *transistor_2;
    Configuracion *transistor_3;

    void decimalToBCD(int decimal, int bcd[4])
    {
        for (int i = 0; i < 4; i++)
        {
            bcd[i] = decimal % 10;
            decimal /= 10;
        }
    }

    void separarDigitos(int decimal, int &unidades, int &decenas, int &centenas)
    {
        unidades = decimal % 10;
        decenas = (decimal / 10) % 10;
        centenas = (decimal / 100) % 10;
    }

    void enviar(int buffer[], int buffer2[], int buffer3[])
    {
        digitalWrite(entrada_1->getPin(), buffer[0]);
        digitalWrite(entrada_2->getPin(), buffer[1]);
        digitalWrite(entrada_3->getPin(), buffer[2]);
        digitalWrite(entrada_4->getPin(), buffer[3]);
        digitalWrite(transistor_1->getPin(), buffer[4]);
        digitalWrite(transistor_2->getPin(), buffer[5]);
        digitalWrite(transistor_3->getPin(), buffer[6]);

        delay(5);

        digitalWrite(entrada_1->getPin(), buffer2[0]);
        digitalWrite(entrada_2->getPin(), buffer2[1]);
        digitalWrite(entrada_3->getPin(), buffer2[2]);
        digitalWrite(entrada_4->getPin(), buffer2[3]);
        digitalWrite(transistor_1->getPin(), buffer2[4]);
        digitalWrite(transistor_2->getPin(), buffer2[5]);
        digitalWrite(transistor_3->getPin(), buffer2[6]);

        delay(5);

        digitalWrite(entrada_1->getPin(), buffer3[0]);
        digitalWrite(entrada_2->getPin(), buffer3[1]);
        digitalWrite(entrada_3->getPin(), buffer3[2]);
        digitalWrite(entrada_4->getPin(), buffer3[3]);
        digitalWrite(transistor_1->getPin(), buffer3[4]);
        digitalWrite(transistor_2->getPin(), buffer3[5]);
        digitalWrite(transistor_3->getPin(), buffer3[6]);

        delay(5);
    }

public:
    VisualizadorDigital(
        int entrada_1,
        int entrada_2,
        int entrada_3,
        int entrada_4,
        int transitor_1,
        int transitor_2,
        int transitor_3)
    {
        this->entrada_1 = new Configuracion(entrada_1);
        this->entrada_2 = new Configuracion(entrada_2);
        this->entrada_3 = new Configuracion(entrada_3);
        this->entrada_4 = new Configuracion(entrada_4);
        this->transistor_1 = new Configuracion(transitor_1);
        this->transistor_2 = new Configuracion(transitor_2);
        this->transistor_3 = new Configuracion(transitor_3);
    }

    void escribirNumero(int decimal)
    {
        int unidades, decenas, centenas;
        separarDigitos(decimal, unidades, decenas, centenas);

        int bcd[4];
        int buffer[7];
        int buffer2[7];
        int buffer3[7];

        // Escribir unidades
        decimalToBCD(unidades, bcd);
        for (int i = 0; i < 4; i++)
        {
            buffer[i] = (bcd[0] & (1 << i)) ? HIGH : LOW;
        }
        buffer[4] = LOW;  // Transistor de unidades encendido
        buffer[5] = HIGH; // Transistor de decenas apagado
        buffer[6] = HIGH; // Transistor de centenas apagado

        decimalToBCD(decenas, bcd);
        for (int i = 0; i < 4; i++)
        {
            buffer2[i] = (bcd[0] & (1 << i)) ? HIGH : LOW;
        }
        buffer2[4] = HIGH; // Transistor de unidades apagado
        buffer2[5] = LOW;  // Transistor de decenas encendido
        buffer2[6] = HIGH; // Transistor de centenas apagado

        decimalToBCD(centenas, bcd);
        for (int i = 0; i < 4; i++)
        {
            buffer3[i] = (bcd[0] & (1 << i)) ? HIGH : LOW;
        }
        buffer3[4] = HIGH; // Transistor de unidades apagado
        buffer3[5] = HIGH; // Transistor de decenas encendido
        buffer3[6] = LOW;  // Transistor de centenas apagado

        enviar(buffer, buffer2, buffer3);
    }

    void setup()
    {
        entrada_1->salida();
        entrada_2->salida();
        entrada_3->salida();
        entrada_4->salida();
        transistor_1->salida();
        transistor_2->salida();
        transistor_3->salida();
    }
};

class Potenciometro
{
private:
    Configuracion *arduino;

public:
    Potenciometro(int pin)
    {
        arduino = new Configuracion(pin);
    }

    int leer()
    {
        return analogRead(arduino->getPin());
    }
};

class ServoMotor
{
private:
    Configuracion *arduino;
    Servo servo;

public:
    ServoMotor(int pin)
    {
        this->arduino = new Configuracion(pin);
    }

    void girar(int grados)
    {
        servo.write(grados);
        delay(15);
        servo.detach();
    }

    void setup()
    {
        servo.attach(arduino->getPin(), 500, 2500);
    }
};

class MapeoUtil
{
public:
    static int AnalogicoToGrados(int valor, int rangoMinEntrada, int rangoMaxEntrada, int rangoMinSalida, int rangoMaxSalida)
    {
        long valorMapeado = (long)(valor - rangoMinEntrada) * (rangoMaxSalida - rangoMinSalida) / (rangoMaxEntrada - rangoMinEntrada) + rangoMinSalida;
        return valorMapeado;
    }
};

int grados = 0;

VisualizadorDigital *visualizador = new VisualizadorDigital(4, 5, 6, 7, 10, 9, 8);
Potenciometro *potenciometro = new Potenciometro(A0);
ServoMotor *servoMotor = new ServoMotor(3);

void setup()
{
    visualizador->setup();
    servoMotor->setup();
}

void loop()
{
    grados = MapeoUtil::AnalogicoToGrados(potenciometro->leer(), 0, 1023, 0, 180);
    visualizador->escribirNumero(grados);
    servoMotor->girar(grados);
}