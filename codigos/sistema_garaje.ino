// C++ code
//
#include <LiquidCrystal.h>
#include <Servo.h>

const int sensorPIR = 10;
const int alarma = 9;
const int servoMotor = 8;

int estado = 0;
int cont_error = 0;
String msg = "";

LiquidCrystal lcd_1(12, 11, 5, 4, 3, 2);
Servo servo;

void setup()
{
    servo.attach(servoMotor, 500, 2500);
    Serial.begin(9600);
    pinMode(alarma, OUTPUT);
    pinMode(sensorPIR, INPUT);
    lcd_1.begin(16, 2);
    lcd_1.print("Sensor PIR:");
    lcd_1.setCursor(0, 1);
    lcd_1.print(">Sin Actividad<");
    servo.write(0);
}

void loop()
{
    estado = digitalRead(sensorPIR);
    if (estado)
    {
        lcd_1.setCursor(0, 1);
        lcd_1.write(">Con Actividad<");

        Serial.println("Identifiquese:");

        // Esperar hasta recibir un mensaje válido
        while (Serial.available() == 0)
        {
            // Esperar a que haya datos disponibles en el serial
        }

        msg = Serial.readString();
        if (msg.compareTo("123") == 0)
        {
            servo.write(90);
            Serial.println("Bienvenido a Casa!");
            lcd_1.clear();
            lcd_1.print("Bienvenido!!");
            lcd_1.setCursor(0, 1);
            lcd_1.print(":-)");
            delay(8000);
            lcd_1.clear();
            lcd_1.print("Sensor PIR:");
            lcd_1.setCursor(0, 1);
            lcd_1.print(">Sin Actividad<");
            servo.write(0);
            msg = "";
        }
        else
        {
            cont_error++;
            if (cont_error == 2)
            {
                Serial.println("Alejese de la zona");
                lcd_1.clear();
                lcd_1.print("Alarma On");
                digitalWrite(alarma, HIGH);
                delay(8000);
                digitalWrite(alarma, LOW);
                lcd_1.clear();
                lcd_1.print("Sensor PIR:");
                lcd_1.setCursor(0, 1);
                lcd_1.print(">Sin Actividad<");
                servo.write(0);
                msg = "";
            }
            else
            {
                msg = "";
                Serial.println("Identificacion Incorrecta");
            }
        }
    }
    else
    {
        lcd_1.setCursor(0, 1);
        lcd_1.write(">Sin Actividad<");
    }
}
