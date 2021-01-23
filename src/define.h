#pragma once

enum class Color {
	Red,
	Green,
	Blue,
	NUM_COLOR
};

enum class IdList {
	Current,
	Cw,
	Ccw,
	NUM_IDLIST
};

#define BAUDRATE 250000
#define PORTNAME "COM13"

#define NUM_LAMPS 84
#define SEND_INTERVAL 100 //LEDÇ÷ÇÃëóêMïpìx[ms]

//LEDÇÃílïœä∑
#define IN_MIN 0
#define IN_MAX 255
#define OUT_MIN 0
#define OUT_MAX 100

//osc
#define IP_HOST "localhost"
#define PORT_TO_SCON 8000
#define PORT_TO_LCON 9000