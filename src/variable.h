#ifndef variable_h
#define variable_h

#define PWM_CHANNELS 13    //
#define PWM_FREQUENCY 5000 //
#define PWM_RESOLUTION 16  //

uint8_t PWM_GPIOPIN[14] = {32, 33, 25, 26, 27, 14, 12, 13, 23, 19, 18, 5, 4, 0};
uint8_t BTN_GPIOPIN = 15;

long previousMillis = 0;
bool cycle = true;

bool f_i[PWM_CHANNELS] = {false};
bool f_o[PWM_CHANNELS] = {false};
bool p[PWM_CHANNELS + 1] = {false};
bool n[PWM_CHANNELS] = {false};
unsigned long currentp[PWM_CHANNELS];

// DEFO
uint8_t FOR_PWM_CHANNELS = 4; // Outputs
long fade_in = 8;             // Seconds
long fade_out = 8;            // Seconds
long decalage = 2000;         // Milliseconds
long on = 60000;              // Milliseconds
long off = 40000;             // Milliseconds
uint16_t High_value = 52428; // 65535= 100% donc 655,35 = 1% ,52428 = 80% , 58981 = 90%

int value[PWM_CHANNELS] = {0};

uint8_t ref[PWM_CHANNELS];

int btn_State = 1;

bool btn = false;

bool init_w = false;

#define MAX_CLIENT 6
int clientn = 0;
bool list[MAX_CLIENT] = {false}; // client

bool start = true;

#endif