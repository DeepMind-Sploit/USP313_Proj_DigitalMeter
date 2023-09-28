//PID-2023-03
//Shivneet Kumar, Valraj Singh, Kritish Singh, Yash Maharaj
//s11199367, s11197618, s11195619, s11196606

#include <18F8722.h>
#device ICD=TRUE
#fuses HS,NOLVP,NOWDT
#use delay(clock=20000000)
#include "LCDD.C"
#include <stdlib.h>

int state;
// 4x4 Keypad Function
int read_keypad(void)
{
    output_J(0x00); // clear the ports
    output_C(0x00); // clear the ports
    
    // First column
    output_high(PIN_C2);
    if(input(PIN_J4)) {
        while(input(PIN_J4));
        return(1);
    }
    else if(input(PIN_J5)) {
        while(input(PIN_J5));
        return(4);
    }
    else if(input(PIN_J7)) {
        while(input(PIN_J7));
        return(7);
    }
    else if(input(PIN_J6)) {
        while(input(PIN_J6));
        return(10);
    }
    output_J(0x00); // Clear Port
    output_C(0x00);
    
    // Second column
    output_high(PIN_C3);
    if(input(PIN_J4)) {
        while(input(PIN_J4));
        return(2);
    }
    else if(input(PIN_J5)) {
        while(input(PIN_J5));
        return(5);
    }
    else if(input(PIN_J7)) {
        while(input(PIN_J7));
        return(8);
    }
    else if(input(PIN_J6)) {
        while(input(PIN_J6));
        return(0);
    }
    output_J(0x00); // Clear Port
    output_C(0x00);

    // Third column
    output_high(PIN_C4);
    if(input(PIN_J4)) {
        while(input(PIN_J4));
        return(3);
    }
    else if(input(PIN_J5)) {
        while(input(PIN_J5));
        return(6);
    }
    else if(input(PIN_J7)) {
        while(input(PIN_J7));
        return(9);
    }
    else if(input(PIN_J6)) {
        while(input(PIN_J6));
        return(11);
    }
    output_J(0x00); // Clear Port
    output_C(0x00);

    // Fourth column
    output_high(PIN_C5);
    if(input(PIN_J4)) {
        while(input(PIN_J4));
        return('A');
    }
    else if(input(PIN_J5)) {
        while(input(PIN_J5));
        return('B');
    }
    else if(input(PIN_J7)) {
        while(input(PIN_J7));
        return('C');
    }
    else if(input(PIN_J6)) {
        while(input(PIN_J6));
        return('D');
    }
    output_J(0x00); // Clear Port
    output_C(0x00);
    return(-1); 
}

#define MAX_DIGITS 16

// Ultrasonic sensor pins
#define TRIG_PIN2 PIN_B0
#define ECHO_PIN2 PIN_B1
#define TRIG_PIN PIN_H7
#define ECHO_PIN PIN_H6


// Buzzer pin
#define BUZZER_PIN PIN_B2

const int numReadings = 50; // Number of readings
const int filterSize = 10; // Number of readings to keep for the average
const int sensorSettlingTime = 10; // Time in milliseconds

void setup() {
    // Set up pins
    output_drive(TRIG_PIN);
    input(ECHO_PIN);
    output_drive(BUZZER_PIN);

    // Initialize the LCD
    lcd_init();
// Display a prompt for the reference height
lcd_gotoxy(1, 1);
printf(lcd_putc, "Enter ref height");
}

float measureDistance() {
int32 duration;
float distance;
float correctionFactor;
   
output_high(TRIG_PIN);
delay_us(10); // Change to 10us for better sensor triggering
output_low(TRIG_PIN);

duration = 0;
while (!input(ECHO_PIN)) {}

while (input(ECHO_PIN)) {
    duration++;
    delay_us(1);
}

distance = (duration * (0.0343 / 2)) / 0.29; // Computes the Actual Physical Distance

// Apply a linear correction factor based on the measured distance
correctionFactor = 1.0 + (distance * 0.001); // Change 0.001 to a value that fits your sensor
distance = distance * correctionFactor;

return distance;
}

float measureDistance2() {
    int32 duration;
    float distance;
    float correctionFactor;
       
    output_high(TRIG_PIN2);
    delay_us(10); // Change to 10us for better sensor triggering
    output_low(TRIG_PIN2);

    duration = 0;
    while (!input(ECHO_PIN2)) {}

    while (input(ECHO_PIN2)) {
        duration++;
        delay_us(1);
    }

    distance = (duration * (0.0343 / 2)) / 0.29;

    // Apply a linear correction factor based on the measured distance
    correctionFactor = 1.0 + (distance * 0.001); // Change 0.001 to a value that fits your sensor
    distance = distance * correctionFactor;

    return distance;
}


void bubbleSort(float arr[], int size) { //Sorts the values obtained from the Sensor
for (int i = 0; i < size - 1; i++) {
for (int j = 0; j < size - i - 1; j++) {
if (arr[j] > arr[j + 1]) {
float temp = arr[j];
arr[j] = arr[j + 1];
arr[j + 1] = temp;
}
}
}
}

float getFilteredAverageDistance() {
float readings[numReadings];
float sum = 0.0;
for (int i = 0; i < numReadings; i++) {
    readings[i] = measureDistance();
    delay_ms(sensorSettlingTime); // Allow the sensor to settle
}

bubbleSort(readings, numReadings);

for (int j = (numReadings - filterSize) / 2; j < (numReadings + filterSize) / 2; j++) {
    sum += readings[j];
}

return sum / filterSize;
}

float getFilteredAverageDistance2() {
float readings[numReadings];
float sum = 0.0;
for (int i = 0; i < numReadings; i++) {
    readings[i] = measureDistance2();
    delay_ms(sensorSettlingTime); // Allow the sensor to settle
}

bubbleSort(readings, numReadings);

for (int j = (numReadings - filterSize) / 2; j < (numReadings + filterSize) / 2; j++) {
    sum += readings[j];
}

return sum / filterSize;
}
void promptOption() {
    lcd_putc('\f');
    lcd_gotoxy(1, 1);
    printf(lcd_putc, "A: Height");
    lcd_gotoxy(1, 2);
    printf(lcd_putc, "B: Volume");
    delay_ms(400); // 
}
void promptOption1() {
    lcd_putc('\f');
    lcd_gotoxy(1, 1);
    printf(lcd_putc, "A: Fixed");
    lcd_gotoxy(1, 2);
    printf(lcd_putc, "B: Variable");
    delay_ms(400); // Add this delay
}
void main() {
    float referenceHeight, radius, volume, height;
    char buffer[MAX_DIGITS + 1] = {0};
    char buffer1[MAX_DIGITS + 1] = {0};
    int position = 0;
    int key, key1;
    int previous_key = -1;
    char *endptr;
    setup();

    while (TRUE) {
        promptOption();
        key = read_keypad();

        if (key != -1 && key != previous_key) {
            if (key == 'A') {
                state = 1;
                referenceHeight = 200;
                break;
            } else if (key == 'B') {
                referenceHeight = 48.4; 
                state = 2;
                break;
            }
            previous_key = key;
            delay_ms(200);
        } else if (key == -1) {
            previous_key = -1;
        }
    }
    if (key == 'B')
    {
        previous_key = -1;
        while (TRUE) {
            promptOption1();
            key1 = read_keypad();
            if (key1 != -1 && key1 != previous_key) {
                if (key1 == 'A') {
                    radius = 10.25;
                    height = 28;
                    break;
                } else if (key1 == 'B') {
                    lcd_putc('\f'); 
                    lcd_gotoxy(1, 1); 
                    printf(lcd_putc, "Radius: %s cm", buffer); 

                    // Get radius from the user
                    while (TRUE) {
                        key = read_keypad();

                        if (key != -1 && key != previous_key) {
                            if (key == 'D') {
                                lcd_putc('\f');
                                position = 0;
                                memset(buffer, 0, sizeof(buffer));
                            } else if (key >= 0 && key <= 9) {
                                if (position < MAX_DIGITS) {
                                    buffer[position] = key + '0';
                                    position++;
                                }
                                lcd_putc('\f');
                                lcd_gotoxy(1, 1);
                                printf(lcd_putc, "Radius: %s cm", buffer);
                            }
                            previous_key = key;
                            delay_ms(200);
                        } else if (key == -1)
                        {
                            previous_key = -1;
                        }

                        if (position > 0 && key == 11) {
                            while (read_keypad() == 11);  // Wait for the key release
                            radius = strtod(buffer, &endptr);
                            break;
                        }
                        
                    } 

                    // Reset for height input
                    position = 0;
                    previous_key = -1;
                    lcd_putc('\f'); // Clear the screen
                    lcd_gotoxy(1, 1); // Position the cursor
                    printf(lcd_putc, "Height: %s cm", buffer1); // Print the prompt

                    // Get height from the user
                    while (TRUE) {
                        key = read_keypad();

                        if (key != -1 && key != previous_key) {
                            if (key == 'D') {
                                lcd_putc('\f');
                                position = 0;
                                memset(buffer1, 0, sizeof(buffer1));
                            } else if (key >= 0 && key <= 9) {
                                if (position < MAX_DIGITS) {
                                    buffer1[position] = key + '0';
                                    position++;
                                }
                                lcd_putc('\f');
                                lcd_gotoxy(1, 1);
                                printf(lcd_putc, "Height: %s cm", buffer1);
                            }
                            previous_key = key;
                            delay_ms(200);
                        } else if (key == -1) {
                            previous_key = -1;
                        }

                        if (position > 0 && key == 11) {
                            while (read_keypad() == 11);  // Wait for the key release
                            height = strtod(buffer1, &endptr);
                            break;
                        }
                    }
                    break;
                }
                previous_key = key1;
                delay_ms(200);
            } else if (key1 == -1) {
                previous_key = -1;
            }
        }
    }

    while (TRUE) {
        float distance;
        if (state == 1) {
            distance = (referenceHeight - getFilteredAverageDistance());
            if (distance<100)
            {
               distance = 0;
            }
        } else {
            distance = (referenceHeight - getFilteredAverageDistance2());
        }
      
        float percentage = (distance) / height * 100;
        if (distance < 0) {
            distance = 0;
        }
        volume = ((3.14)*(radius*radius)*distance)/1000; 
   
        if (percentage < 0) {
            percentage = 0;
        } else if (percentage > 100) {
            percentage = 100;
        }
    
        lcd_putc('\f'); // Clear the LCD screen
        lcd_gotoxy(1, 1);
        if (state == 1) {
            printf(lcd_putc, "Dist: %.1f cm", distance);
        } else {
            printf(lcd_putc, "Volume: %.1f l", volume);
            lcd_gotoxy(1, 2);
            printf(lcd_putc, "Fill: %.1f%%", percentage);

            if (percentage > 80) {
                output_high(BUZZER_PIN);
                delay_ms(100); // Short beep
                output_low(BUZZER_PIN);
            } else if (percentage > 60) {
                output_high(BUZZER_PIN);
                delay_ms(400); // Long beep
                output_low(BUZZER_PIN);
            }
        }
    }
}

