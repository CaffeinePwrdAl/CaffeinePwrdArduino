/*
* light weight WS2812 lib V2.1 - Arduino support
*
* Controls WS2811/WS2812/WS2812B RGB-LEDs
* Author: Matthias Riegler
*
* Mar 07 2014: Added Arduino and C++ Library
*
* September 6, 2014:	Added option to switch between most popular color orders
*						(RGB, GRB, and BRG) --  Windell H. Oskay
* 
* License: GNU GPL v2 (see License.txt)
*/

#include "WS2812.h"
#include <stdlib.h>

WS2812::WS2812(uint16_t num_leds) {
	count_led = num_leds;
	
	pixels = (uint8_t*)malloc(count_led*3);
	#ifdef RGB_ORDER_ON_RUNTIME	
	setColorOrderRGBW();
	#endif
}

cRGB WS2812::get_crgb_at(uint16_t index) {
	
	cRGB px_value;
	
	if(index < count_led) {
		
		uint16_t tmp;
		tmp = index * NUM_CHANELS;

		px_value.r = pixels[OFFSET_R(tmp)];
		px_value.g = pixels[OFFSET_G(tmp)];
		px_value.b = pixels[OFFSET_B(tmp)];
		px_value.w = pixels[OFFSET_W(tmp)];
	}
	
	return px_value;
}

uint8_t WS2812::set_crgb_at(uint16_t index, cRGB px_value) {
	
	if(index < count_led) {
		
		uint16_t tmp;
		tmp = index * NUM_CHANELS;
		
		pixels[OFFSET_R(tmp)] = px_value.r;
		pixels[OFFSET_G(tmp)] = px_value.g;
		pixels[OFFSET_B(tmp)] = px_value.b;	
		pixels[OFFSET_W(tmp)] = px_value.w;	
		return 0;
	} 
	return 1;
}

uint8_t WS2812::set_subpixel_at(uint16_t index, uint8_t offset, uint8_t px_value) {
	if (index < count_led) {
		uint16_t tmp;
		tmp = index * NUM_CHANELS;

		pixels[tmp + offset] = px_value;
		return 0;
	}
	return 1;
}

void WS2812::sync() {
	*ws2812_port_reg |= pinMask; // Enable DDR
	ws2812_sendarray_mask(pixels,NUM_CHANELS*count_led,pinMask,(uint8_t*) ws2812_port,(uint8_t*) ws2812_port_reg );	
}

#ifdef RGB_ORDER_ON_RUNTIME	
void WS2812::setColorOrderGRBW() { // Default color order
	offsetGreen = 0;
	offsetRed = 1;
	offsetBlue = 2;
	offsetWhite = 3;
}

void WS2812::setColorOrderRGBW() {
	offsetRed = 0;
	offsetGreen = 1;
	offsetBlue = 2;
	offsetWhite = 3;
}

void WS2812::setColorOrderBRGW() {
	offsetBlue = 0;
	offsetRed = 1;
	offsetGreen = 2;
	offsetWhite = 3;
}
#endif

WS2812::~WS2812() {
	free(pixels);
	
}

#ifndef ARDUINO
void WS2812::setOutput(const volatile uint8_t* port, volatile uint8_t* reg, uint8_t pin) {
	pinMask = (1<<pin);
	ws2812_port = port;
	ws2812_port_reg = reg;
}
#else
void WS2812::setOutput(uint8_t pin) {
	pinMask = digitalPinToBitMask(pin);
	ws2812_port = portOutputRegister(digitalPinToPort(pin));
	ws2812_port_reg = portModeRegister(digitalPinToPort(pin));
}
#endif 