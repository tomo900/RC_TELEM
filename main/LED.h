#include "led_strip.h"

class LED {
    public:
        LED(char* name);
        void print_Name();
        void set_Color(uint8_t red, uint8_t green, uint8_t blue);
    private:
        char name[100];
            /// Create the LED strip object
        led_strip_handle_t led_strip = NULL;
};
