#ifndef srv_handle_h
#define srv_handle_h

#include <EEPROM.h>
#define EEPROM_SIZE 64

void eeprom_read()
{
    FOR_PWM_CHANNELS = EEPROM.read(1);

    fade_in = EEPROM.read(2);
    fade_in |= (EEPROM.read(3) << 8);
    fade_in |= (EEPROM.read(4) << 16);
    fade_in |= (EEPROM.read(5) << 24);

    decalage = EEPROM.read(6);
    decalage |= (EEPROM.read(7) << 8);
    decalage |= (EEPROM.read(8) << 16);
    decalage |= (EEPROM.read(9) << 24);

    on = EEPROM.read(10);
    on |= (EEPROM.read(11) << 8);
    on |= (EEPROM.read(12) << 16);
    on |= (EEPROM.read(13) << 24);

    off = EEPROM.read(14);
    off |= (EEPROM.read(15) << 8);
    off |= (EEPROM.read(16) << 16);
    off |= (EEPROM.read(17) << 24);

    fade_out = EEPROM.read(18);
    fade_out |= (EEPROM.read(19) << 8);
    fade_out |= (EEPROM.read(20) << 16);
    fade_out |= (EEPROM.read(21) << 24);

#ifdef DEBUG
    Serial.println("EEPROM READ");
    Serial.print(" FOR_PWM_CHANNELS : ");
    Serial.print(FOR_PWM_CHANNELS);
    Serial.print(" ");
    Serial.print(" fade_in : ");
    Serial.print(fade_in);
    Serial.print(" ");
    Serial.print(" decalage : ");
    Serial.print(decalage);
    Serial.print(" ");
    Serial.print(" on : ");
    Serial.print(on);
    Serial.print(" ");
    Serial.print(" off : ");
    Serial.print(off);
    Serial.print(" fade_out : ");
    Serial.print(fade_out);
    Serial.println(" ");
#endif
} //eeprom_read

void eeprom_write()
{
    EEPROM.write(1, FOR_PWM_CHANNELS);

    EEPROM.write(2, fade_in);
    EEPROM.write(3, fade_in >> 8);
    EEPROM.write(4, fade_in >> 16);
    EEPROM.write(5, fade_in >> 24);

    EEPROM.write(6, decalage);
    EEPROM.write(7, decalage >> 8);
    EEPROM.write(8, decalage >> 16);
    EEPROM.write(9, decalage >> 24);

    EEPROM.write(10, on);
    EEPROM.write(11, on >> 8);
    EEPROM.write(12, on >> 16);
    EEPROM.write(13, on >> 24);

    EEPROM.write(14, off);
    EEPROM.write(15, off >> 8);
    EEPROM.write(16, off >> 16);
    EEPROM.write(17, off >> 24);

    EEPROM.write(18, fade_out);
    EEPROM.write(19, fade_out >> 8);
    EEPROM.write(20, fade_out >> 16);
    EEPROM.write(21, fade_out >> 24);

    EEPROM.write(62, 'O');
    EEPROM.write(63, 'K');
    EEPROM.commit();

#ifdef DEBUG
    Serial.println("EEPROM WRITE");
    Serial.print(" FOR_PWM_CHANNELS : ");
    Serial.print(FOR_PWM_CHANNELS);
    Serial.print(" ");
    Serial.print(" fade_in : ");
    Serial.print(fade_in);
    Serial.print(" ");
    Serial.print(" decalage : ");
    Serial.print(decalage);
    Serial.print(" ");
    Serial.print(" on : ");
    Serial.print(on);
    Serial.print(" ");
    Serial.print(" off : ");
    Serial.print(off);
    Serial.print(" fade_out : ");
    Serial.print(fade_out);
    Serial.println(" ");

#endif
} //eeprom_write

void load_spec()
{
    eeprom_read();
    for (int i = 0; i < MAX_CLIENT; i++)
    {
        if (list[i])
        {
            webSocket.sendTXT(i, "ba:" + String(lround(FOR_PWM_CHANNELS)));
            webSocket.sendTXT(i, "bb:" + String(lround(fade_in)));
            webSocket.sendTXT(i, "bc:" + String(lround(decalage/1000)));

            webSocket.sendTXT(i, "bd:" + String(lround(on/1000)));
            webSocket.sendTXT(i, "be:" + String(lround(off/1000)));
            webSocket.sendTXT(i, "bf:" + String(lround(fade_out)));
        }

    } // for client

} //load_spec

void save_spec()
{
    eeprom_write();

    btn = false;
    start = true;
    WiFi.mode(WIFI_OFF);
    cycle = true;
    // pwm_loop();
} //save_spec

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
} //init_eeprom()

//////////////////////////////////////////srv_handle_set////////////////////////////////////////
void srv_handle_set()
{
    for (uint8_t i = 0; i < server.args(); i++)
    {

        //////////////////////////////////////////setting html///////////////////////////////////////////
        if (server.argName(i) == "mem")
        {
            int LMem = (uint8_t)strtol(server.arg(i).c_str(), NULL, 10);
#ifdef DEBUGSPEC
            Serial.print("mem=");
            Serial.println(LMem);
#endif

            if (LMem == 0)
            {
                load_spec();
            } //if(Mem==0){

            if (LMem == 1)
            {
                save_spec();
            } //if(Mem==1){
        }     //serveur mem

    } //serveur args
    server.send(200, "text/plain", "OK");
} //srv_handle_set()

#endif