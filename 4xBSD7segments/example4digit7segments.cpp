#include <Arduino.h>
#include <math.h>

/*********************
 * Library to interact with 4 digits 7 segments displays.
 * The strategy is turn off other digits while painting the current digit.
 * So you are painting each digit deparated but is not perceptible to the
 * human eye. If the digits flicker reduthe the PAINT_DELAY. If the
 * off segments of the digit has some glow increase ist.
 *
 *                                3 A F 3 2 B
 *                                . . . . . .                                 *
 *                     4          3          2          1                     *
 *                     A          A          A          A                     *
 *                    ---        ---        ---        ---                    *
 *                 F |   | B  F |   | B  F |   | B  F |   | B                 *
 *                   | G |      | G |      | G |      | G |                   *
 *                    ---        ---        ---        ---                    *
 *                 E |   | C  E |   | C  E |   | C  E |   | C                 *
 *                   |   |      |   |      |   |      |   |                   *
 *                    --- .dot   --- .dot   --- .dot   --- .dot               *
 *                     D          D          D          D                     *
 *                                                                            *
 *                                . . . . . .                                 *
 *                                E D d C G 1
 *                                    o
 *                                    t
 *
 *   So... If pins 1, B and C are set to HIGH the following will be displayed:
 *
 *   4     3     2     1
 *  ---   ---   ---   ---
 * |   # |   | |   | |   |
 * |   # |   | |   | |   |
 *  ---   ---   ---   ---
 * |   # |   | |   | |   |
 * |   # |   | |   | |   |
 *  ---   ---   ---   ---
 *
 * If pins 1, 2, A,B,G,E,D the following will be displayed:
 *
 *   4     3     2     1
 *  ###   ###   ---   ---
 * |   # |   # |   | |   |
 * |   # |   # |   | |   |
 *  ###   ###   ---   ---
 * #   | #   | |   | |   |
 * #   | #   | |   | |   |
 *  ###   ###   ---   ---
 *
 *  To display the 1 and the 2 at the same time we will paint the 1 for a while,
 *  turn it off and paint the 2 for a while, produing the desited 12 effet.
 ******************************************************************************/

/* Connection pins to the Arduino UNO to adjust */
int pinA = 2;
int pinB = 3;
int pinC = 4;
int pinD = 5;
int pinE = 6;
int pinF = 7;
int pinG = 8;
int pinP = 13;
int D4 = 9;
int D3 = 10;
int D2 = 11;
int D1 = 12;
int PAINT_DELAY = 3;
int DIGITS = 4;

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


void simple_example() {
    /* Writes manually 1234 to the display with an straightforward code */

    /* 1
     *   4     3     2     1
     *  ---   ---   ---   ---
     * |   | |   | |   | |   #
     * |   | |   | |   | |   #
     *  ---   ---   ---   ---
     * |   | |   | |   | |   #
     * |   | |   | |   | |   #
     *  ---   ---   ---   ---
     */
    digitalWrite(D1, LOW);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, HIGH);

               digitalWrite(pinA, LOW);
    /***********************/   /***********************/
    /***********************/   /***********************/
    digitalWrite(pinF, LOW);   digitalWrite(pinB, HIGH);
               digitalWrite(pinG, LOW);
    /***********************/   /***********************/
    /***********************/   /***********************/
    digitalWrite(pinE, LOW);   digitalWrite(pinC, HIGH);
    /***********************/   /***********************/
    /***********************/   /***********************/
               digitalWrite(pinD, LOW);

    delay(PAINT_DELAY);

    /* 2
     *   4     3     2     1
     *  ---   ---   ###   ---
     * |   | |   | |   # |   |
     * |   | |   | |   # |   |
     *  ---   ---   ###   ---
     * |   | |   | #   | |   |
     * |   | |   | #   | |   |
     *  ---   ---   ###   ---
     */
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, HIGH);

               digitalWrite(pinA, HIGH);
    /***********************/   /***********************/
    /***********************/   /***********************/
    digitalWrite(pinF, LOW);   digitalWrite(pinB, HIGH);
               digitalWrite(pinG, HIGH);
    /***********************/   /***********************/
    /***********************/   /***********************/
    digitalWrite(pinE, HIGH);   digitalWrite(pinC, LOW);
    /***********************/   /***********************/
    /***********************/   /***********************/
               digitalWrite(pinD, HIGH);

    delay(PAINT_DELAY);

    /* 3
     *   4     3     2     1
     *  ---   ###   ---   ---
     * |   | |   # |   | |   |
     * |   | |   # |   | |   |
     *  ---   ###   ---   ---
     * |   | |   # |   | |   |
     * |   | |   # |   | |   |
     *  ---   ###   ---   ---
     */
    digitalWrite(D1, HIGH);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, LOW);
    digitalWrite(D4, HIGH);

               digitalWrite(pinA, HIGH);
    /***********************/   /***********************/
    /***********************/   /***********************/
    digitalWrite(pinF, LOW);   digitalWrite(pinB, HIGH);
               digitalWrite(pinG, HIGH);
    /***********************/   /***********************/
    /***********************/   /***********************/
    digitalWrite(pinE, LOW);   digitalWrite(pinC, HIGH);
    /***********************/   /***********************/
    /***********************/   /***********************/
               digitalWrite(pinD, HIGH);

    delay(PAINT_DELAY);

    /* 4
     *   4     1     2     3
     *  ---   ---   ---   ---
     * #   # |   | |   | |   |
     * #   # |   | |   | |   |
     *  ###   ---   ---   ---
     * |   # |   | |   | |   |
     * |   # |   | |   | |   |
     *  ---   ---   ---   ---
     */
    digitalWrite(D1, HIGH);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, LOW);
    
               digitalWrite(pinA, LOW);
    /***********************/   /***********************/
    /***********************/   /***********************/
    digitalWrite(pinF, HIGH);   digitalWrite(pinB, HIGH);
               digitalWrite(pinG, HIGH);
    /***********************/   /***********************/
    /***********************/   /***********************/
    digitalWrite(pinE, LOW);   digitalWrite(pinC, HIGH);
    /***********************/   /***********************/
    /***********************/   /***********************/
               digitalWrite(pinD, LOW);

    delay(PAINT_DELAY);
}

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

void display_hex() {
}

void display_fload() {
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
}

void loop() {
    //Serial.println("4 Digit 7 Segment example started");
    // simple_example();
    display_int(1993);
}

