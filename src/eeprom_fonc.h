#ifndef eeprom_fonc
#define eeprom_fonc

#include <EEPROM.h>
#define EEPROM_SIZE 32

void eeprom_read()
{
    time_on_Hour = EEPROM.read(0);
    time_on_Minute = EEPROM.read(1);
    time_on_Second = EEPROM.read(2);

    time_off_Hour = EEPROM.read(3);
    time_off_Minute = EEPROM.read(4);
    time_off_Second = EEPROM.read(5);

    lat_coef = EEPROM.read(6);
    lat_coef |= (EEPROM.read(7) << 8);
    long_coef = EEPROM.read(8);
    long_coef |= (EEPROM.read(9) << 8);
    alt_coef = EEPROM.read(10);
    alt_coef |= (EEPROM.read(11) << 8);

    time_zone = (EEPROM.read(12));

    RtcDateTime time_off(now.Year(),
                         now.Month(),
                         now.Day(),
                         time_off_Hour,
                         time_off_Minute,
                         time_off_Second);

#ifdef DEBUG
    Serial.print("EEPROM READ :");
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
    Serial.print(" lat_coef : ");
    Serial.print(lat_coef);
    Serial.print(" long_coef : ");
    Serial.print(long_coef);
    Serial.print(" alt_coef : ");
    Serial.print(alt_coef);
    Serial.print(" time_zone : ");
    Serial.print(time_zone);
    Serial.println();
#endif
} // eeprom_read

void eeprom_write()
{
    EEPROM.write(0, time_on_Hour);
    EEPROM.write(1, time_on_Minute);
    EEPROM.write(2, time_on_Second);

    EEPROM.write(3, time_off_Hour);
    EEPROM.write(4, time_off_Minute);
    EEPROM.write(5, time_off_Second);

    EEPROM.write(6, lat_coef);
    EEPROM.write(7, lat_coef >> 8);
    EEPROM.write(8, long_coef);
    EEPROM.write(9, long_coef >> 8);
    EEPROM.write(10, alt_coef);
    EEPROM.write(11, alt_coef >> 8);

    EEPROM.write(12, time_zone);

    EEPROM.write(30, 'O');
    EEPROM.write(31, 'K');

    EEPROM.commit();

#ifdef DEBUG
    Serial.print("EEPROM WRITE :");
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
    Serial.print(" lat_coef : ");
    Serial.print(lat_coef);
    Serial.print(" long_coef : ");
    Serial.print(long_coef);
    Serial.print(" alt_coef : ");
    Serial.print(alt_coef);
    Serial.print(" time_zone : ");
    Serial.print(time_zone);
    Serial.println();

#endif
} // eeprom_write

void eeprom_write_time_on()
{
    // alarm_set();

    EEPROM.write(0, time_on_Hour);
    EEPROM.write(1, time_on_Minute);
    EEPROM.write(2, time_on_Second);

    EEPROM.commit();

#ifdef DEBUG
    Serial.print("TIME_ON WRITE :");
    Serial.print(" time_on_Hour : ");
    Serial.print(time_on_Hour);
    Serial.print(" time_on_Minute : ");
    Serial.print(time_on_Minute);
    Serial.print(" time_on_Second : ");
    Serial.print(time_on_Second);
    Serial.println(" ");

#endif
} // eeprom_write time_on

void eeprom_write_time_off()
{
    EEPROM.write(3, time_off_Hour);
    EEPROM.write(4, time_off_Minute);
    EEPROM.write(5, time_off_Second);

    EEPROM.commit();

    RtcDateTime time_off(now.Year(),
                         now.Month(),
                         now.Day(),
                         time_off_Hour,
                         time_off_Minute,
                         time_off_Second);

#ifdef DEBUG
    Serial.print("TIME_OFF WRITE :");
    Serial.print("  time_off_Hour : ");
    Serial.print(time_off_Hour);
    Serial.print(" time_off_Minute : ");
    Serial.print(time_off_Minute);
    Serial.print(" time_off_Second : ");
    Serial.print(time_off_Second);
    Serial.println(" ");

#endif
} // eeprom_write time off

void eeprom_write_coef()
{
    EEPROM.write(6, lat_coef);
    EEPROM.write(7, lat_coef >> 8);
    EEPROM.write(8, long_coef);
    EEPROM.write(9, long_coef >> 8);
    EEPROM.write(10, alt_coef);
    EEPROM.write(11, alt_coef >> 8);

    EEPROM.commit();

#ifdef DEBUG
    Serial.print("COEF WRITE :");
    Serial.print(" lat_coef : ");
    Serial.print(lat_coef);
    Serial.print(" long_coef : ");
    Serial.print(long_coef);
    Serial.print(" alt_coef : ");
    Serial.print(alt_coef);
    Serial.println();

#endif
} // eeprom_write coef

void eeprom_write_time_zone()
{
    EEPROM.write(12, time_zone);

    EEPROM.commit();

#ifdef DEBUG
    Serial.print("Time Zone WRITE :");
    Serial.print(" time_zone : ");
    Serial.print(time_zone);
    Serial.println();

#endif
} // eeprom_write time_zone

void init_eeprom()
{
    if (EEPROM.read(30) != 'O' || EEPROM.read(31) != 'K')
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

        now = Rtc.GetDateTime();
        alarm_one = Rtc.GetAlarmOne();
        time_on_Hour = alarm_one.Hour();
        time_on_Minute = alarm_one.Minute();
        time_on_Second = alarm_one.Second();

        eeprom_write();

    } //(EEPROM.read(30) != 'O' || EEPROM.read(31) != 'K')

    if (EEPROM.read(30) == 'O' && EEPROM.read(31) == 'K')
    {
        eeprom_read();
    } //(EEPROM.read(30) == 'O' && EEPROM.read(31) == 'K')
} // init_eeprom()

#endif