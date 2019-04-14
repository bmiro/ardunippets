#include <Arduino.h>
#include <math.h>

/* This example is a metter using an ultrasonic sensors HC-SR04 and
 * display the result on a 4 digit 7 segment display  */

/* Connection pins to the Arduino UNO to adjust */
int pinA = 2;
int pinB = 3;
int pinC = 4;
int pinD = A0;
int pinE = A1;
int pinF = 7;
int pinG = 8;
int pinP = 13;
int D4 = 9;
int D3 = 10;
int D2 = 11;
int D1 = 12;
int PAINT_DELAY = 3;
int DIGITS = 4;

int echoPin = 5;
int triggerPin = 6;

int skip;

int segment_pins[8] = {
    pinA,
    pinB,
    pinC,
    pinD,
    pinE,
    pinF,
    pinG,
    pinP
};
const int digit_pins[4] = {
    D1,
    D2,
    D3,
    D4
};

const int symbols[] = {
   /*********************
    *            A
    *           ---
    *        F |   | B
    *          | G |
    *           ---
    *        E |   | C
    *          |   |
    *           --- . P
    *            D
    *              ABC DEFGP */
    0x00, /* off  0000 0000  */
    0x7E, /* 0 -> 0111 1110  */
    0x30, /* 1 -> 0110 0000  */
    0x6D, /* 2 -> 0110 0000  */ //TODO fullfill
    0x79, /* 3 -> 0110 0000  */
    0x33, /* 4 -> 0110 0000  */
    0x5B, /* 5 -> 0110 0000  */
    0x5F, /* 6 -> 0110 0000  */
    0x70, /* 7 -> 0110 0000  */
    0x7F, /* 8 -> 0110 0000  */
    0x7B, /* 9 -> 0110 0000  */
    0x77, /* A -> 0110 0000  */
    0x1F, /* B -> 0110 0000  */
    0x4E, /* C -> 0110 0000  */
    0x3D, /* D -> 0110 0000  */
    0x4F, /* E -> 0110 0000  */
    0x47  /* F -> 0110 0000  */
};

char mask_a = 0x40;
char mask_b = 0x20;
char mask_c = 0x10;
char mask_d = 0x08;
char mask_e = 0x04;
char mask_f = 0x02;
char mask_g = 0x01;

void display_int(int number) {
    char a, b, c, d, e, f, g, p;
    int digit;
    int offset;
    char segments;
    int digit_value;

    int digit_count = floor(log10(number)) + 1;

    for (int digit = 0; digit < digit_count; digit++) {
        digit_value = (int)(number / pow(10, digit)) % 10;
        offset = digit_value + 1;
        segments = symbols[offset];

        a = ((segments & mask_a) == mask_a) ? HIGH : LOW;
        b = ((segments & mask_b) == mask_b) ? HIGH : LOW;
        c = ((segments & mask_c) == mask_c) ? HIGH : LOW;
        d = ((segments & mask_d) == mask_d) ? HIGH : LOW;
        e = ((segments & mask_e) == mask_e) ? HIGH : LOW;
        f = ((segments & mask_f) == mask_f) ? HIGH : LOW;
        g = ((segments & mask_g) == mask_g) ? HIGH : LOW;

        for (int i = 0; i < DIGITS; i++) {
            if ( i == digit) {
                digitalWrite(digit_pins[i], LOW);
            } else {
                digitalWrite(digit_pins[i], HIGH);
            }
        }

                   digitalWrite(pinA, a);
        /***********************/   /***********************/
        /***********************/   /***********************/
        digitalWrite(pinF, f);   digitalWrite(pinB, b);
        /**/           digitalWrite(pinG, g);
        /***********************/   /***********************/
        /***********************/   /***********************/
        digitalWrite(pinE, e);   digitalWrite(pinC, c);
        /***********************/   /***********************/
        /***********************/   /***********************/
        /**/           digitalWrite(pinD, d);            digitalWrite(pinP, p);
        delay(1);
    }
}

int get_distance() {
	long duration, distanceCm;

	digitalWrite(triggerPin, LOW);
	delayMicroseconds(4);
	digitalWrite(triggerPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(triggerPin, LOW);

	duration = pulseIn(echoPin, HIGH);

	distanceCm = duration * 10 / 292 / 2;
	return distanceCm;
}

void setup() {
    Serial.begin(9600);

    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);
    pinMode(pinC, OUTPUT);
    pinMode(pinD, OUTPUT);
    pinMode(pinE, OUTPUT);
    pinMode(pinF, OUTPUT);
    pinMode(pinG, OUTPUT);
    pinMode(pinP, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);

	pinMode(triggerPin, OUTPUT);
	pinMode(echoPin, INPUT);

    skip = 0;

}

void loop() {
    int d;


    if (skip == 128) {
        d = get_distance();
        skip = 0;
    }

    Serial.println(d);
    display_int(d);
    skip++;

}

