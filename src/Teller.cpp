#include "Teller.h"

/*
Teller::Teller(int utime_, int id_, vector<int> color_, int count_) {
	utime = utime_;
	id = id_;
	color = color_;
	count = count_;
}
*/

void Teller::setUtime(int utime_) {
	utime = utime_;
}

void Teller::setId(int id_) {
	id = id_;
}

void Teller::setRgbVal(vector<int> color_) {
	color = color_;
}

void Teller::setCount(int count_) {
	count = count_;
}

int Teller::getUtime() {
	return utime;
}

int Teller::getId() {
	return id;
}

vector<int> Teller::getRgbVal() {
	return color;
}

int Teller::getCount() {
	return count;
}

void Teller::incrementCount() {
	count++;
}

void Teller::decrementCount() {
	count--;
}

int Teller::getDirection() {
	return clockwise;
}
