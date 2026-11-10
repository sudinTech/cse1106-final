#include <stdio.h>
#include <pigpio.h>

#define LED_PIN 17

int main()
{
    if (gpioInitialise() < 0)
    {
        return 1;
    }

    gpioSetMode(LED_PIN, PI_OUTPUT);
    gpioWrite(LED_PIN, 0);

    double temperatures[3];

    printf("=== Smart Classroom Comfort Monitor ===\n");

    for (int i = 0; i < 3; i++)
    {
        printf("Enter temperature %d in Fahrenheit: ", i + 1);
        scanf("%lf", &temperatures[i]);

        if (temperatures[i] >= 68.0 && temperatures[i] <= 76.0)
        {
            gpioWrite(LED_PIN, 0);
            printf("Status: NORMAL | LED: OFF\n");
        }
        else
        {
            gpioWrite(LED_PIN, 1);
            printf("Status: WARNING | LED: ON\n");
        }
    }

    gpioWrite(LED_PIN, 0);

    double sum = 0;
    for (int i = 0; i < 3; i++)
    {
        sum += temperatures[i];
    }
    double avg = sum / 3.0;

    printf("\nAverage Temperature: %.2f\n", avg);

    FILE *f = fopen("classroom_temperature_log.txt", "w");
    fprintf(f, "Temperature System Status Warning LED\n");

    for (int i = 0; i < 3; i++)
    {
        if (temperatures[i] >= 68.0 && temperatures[i] <= 76.0)
        {
            fprintf(f, "%.1f°F\tNORMAL\tOFF\n", temperatures[i]);
        }
        else
        {
            fprintf(f, "%.1f°F\tWARNING\tON\n", temperatures[i]);
        }
    }

    fprintf(f, "\nAverage Temperature: %.2f°F\n", avg);
    fclose(f);

    gpioTerminate();
    return 0;
}