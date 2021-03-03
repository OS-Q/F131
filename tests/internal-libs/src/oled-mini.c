// I2C is included here to let the build system automatically be aware of this
// second-level dependency. Otherwise you would need to list it in the Makefile:
// ARDUINO_LIBS = I2C
#include "I2C.h"
#include "Mini_SSD1306.h"

#define OLED_RESET -1

Mini_SSD1306(display,OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Mini_SSD1306.h!");
#endif


void setup()
{
	// The Nokia 5110 display generates the high voltage from the 3.3v
	// line internally! (neat!)
	// Initialize with the I2C addr 0x3C. Some displays use 0x3D instead.
	display_begin(SSD1306_SWITCHCAPVCC, 0x3C,0);
}


void loop()
{
	uint8_t x,y;
	// Show image buffer on the display hardware.
	// Since the buffer is intialized with an Adafruit splashscreen
	// internally, this will display the splashscreen.
	display_display();
	delay (1000);
	// draw some dots in an 8x8 pattern
	for (x=0; x<WIDTH; x+=8)
	{
		for (y=0; y<HEIGHT-16; y+=8)
		{
			display_drawPixel(x,y,INVERSE);
		}
	}
}
