#ifndef eeprom_fonc
#define eeprom_fonc

#include <EEPROM.h>
#define EEPROM_SIZE 64

void eeprom_read()
{
    time_on_Hour = EEPROM.read(1);
    time_on_Minute = EEPROM.read(2);
    time_on_Second = EEPROM.read(3);

    time_off_Hour = EEPROM.read(11);
    time_off_Minute = EEPROM.read(12);
    time_off_Second = EEPROM.read(13);

    RtcDateTime time_on(now.Year(),
                        now.Month(),
                        now.Day(),
                        time_on_Hour,
                        time_on_Minute,
                        time_on_Second);
    RtcDateTime time_off(now.Year(),
                         now.Month(),
                         now.Day(),
                         time_off_Hour,
                         time_off_Minute,
                         time_off_Second);

#ifdef DEBUG
    Serial.println("EEPROM READ");
    Serial.print(" time_on.Hour() : ");
    Serial.print(time_on_Hour);
    Serial.print(" time_on.Minute() : ");
    Serial.print(time_on_Minute);
    Serial.print(" time_on.Second() : ");
    Serial.print(time_on_Second);
    Serial.print("  time_off.Hour() : ");
    Serial.print(time_off_Hour);
    Serial.print(" time_off.Minute() : ");
    Serial.print(time_off_Minute);
    Serial.print(" time_off.Second() : ");
    Serial.print(time_off_Second);
    Serial.println(" ");
#endif
} // eeprom_read

void eeprom_write()
{
    EEPROM.write(1, time_on_Hour);
    EEPROM.write(2, time_on_Minute);
    EEPROM.write(3, time_on_Second);

    EEPROM.write(11, time_off_Hour);
    EEPROM.write(12, time_off_Minute);
    EEPROM.write(13, time_off_Second);

    EEPROM.write(62, 'O');
    EEPROM.write(63, 'K');
    EEPROM.commit();

#ifdef DEBUG
    Serial.println("EEPROM WRITE");
    Serial.print(" time_on_Hour : ");
    Serial.print(time_on_Hour);
    Serial.print(" time_on_Minute : ");
    Serial.print(time_on_Minute);
    Serial.print(" time_on_Second : ");
    Serial.print(time_on_Second);
    Serial.print("  time_off_Hour : ");
    Serial.print(time_off_Hour);
    Serial.print(" time_off_Minute : ");
    Serial.print(time_off_Minute);
    Serial.print(" time_off_Second : ");
    Serial.print(time_off_Second);
    Serial.println(" ");

#endif
} // eeprom_write

void eeprom_write_time_on()
{
    // alarm_set();

    EEPROM.write(1, time_on_Hour);
    EEPROM.write(2, time_on_Minute);
    EEPROM.write(3, time_on_Second);

    EEPROM.write(62, 'O');
    EEPROM.write(63, 'K');
    EEPROM.commit();

#ifdef DEBUG
    Serial.println("TIME_ON WRITE");
    Serial.print(" time_on_Hour : ");
    Serial.print(time_on_Hour);
    Serial.print(" time_on_Minute : ");
    Serial.print(time_on_Minute);
    Serial.print(" time_on_Second : ");
    Serial.print(time_on_Second);
    Serial.println(" ");

#endif
} // eeprom_write

void eeprom_write_time_off()
{
    EEPROM.write(11, time_off_Hour);
    EEPROM.write(12, time_off_Minute);
    EEPROM.write(13, time_off_Second);

    EEPROM.write(62, 'O');
    EEPROM.write(63, 'K');
    EEPROM.commit();

#ifdef DEBUG
    Serial.println("TIME_OFF WRITE");
    Serial.print("  time_off_Hour : ");
    Serial.print(time_off_Hour);
    Serial.print(" time_off_Minute : ");
    Serial.print(time_off_Minute);
    Serial.print(" time_off_Second : ");
    Serial.print(time_off_Second);
    Serial.println(" ");

#endif
} // eeprom_write

void init_eeprom()
{
    if (EEPROM.read(62) != 'O' || EEPROM.read(63) != 'K')
    {

#ifdef DEBUG
        Serial.println("failed to initialise EEPROM");
        Serial.println("Formate EEPROM");
#endif

        for (int i = 0; i < EEPROM_SIZE; i++)
        {
            EEPROM.write(i, '\0');
            if (i % 50 == 0)
                delay(100);
        }
#ifdef DEBUG
        Serial.println("EEPROM content cleared!");
#endif

        eeprom_write();

    } //(EEPROM.read(62) != 'O' || EEPROM.read(63) != 'K')

    if (EEPROM.read(62) == 'O' && EEPROM.read(63) == 'K')
    {
        eeprom_read();
    } //(EEPROM.read(62) == 'O' && EEPROM.read(63) == 'K')
} // init_eeprom()

#endif