#include <stdio.h> 


#define G 9.81

int main(void)
{
    float d = 0;
    for(int t = 1; t<=10; t++)
    {
        d = 0.5 * G * t * t;
        printf("%d s, %f m \n", t, d);
    }
}