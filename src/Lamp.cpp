#include "Lamp.h"
#include "ofMain.h"
#include <stdio.h>

void Lamp::setRgbValue(int r_, int g_, int b_) {
	//目標値に値をセット
	d_r = r_;
	d_g = g_;
	d_b = b_;
	//printf("color(%d, %d, %d)\n", d_r, d_g, d_b);
}

void Lamp::resetRgbValue(int r_, int g_, int b_) {
	d_r = c_r = r_;
	d_g = c_g = g_;
	d_b = c_b = b_;
}

void Lamp::setPos(float x_, float y_, float z_) {
	x = x_;
	y = y_;
	z = z_;
}

void Lamp::setId(int id_) {
	id = id_;
}

void Lamp::setSize(int lampSize_) {
	size = lampSize_;
}

void Lamp::update() {
	//1ステップで変化
	/*
	c_r = d_r;
	c_g = d_g;
	c_b = d_b;
	*/

	//ゆっくり変化
	if (c_r != d_r) {
		if (c_r < d_r) c_r+=2;
		else c_r-=2;
	}
	if (c_g != d_g) {
		if (c_g < d_g) c_g+=2;
		else c_g-=2;
	}
	if (c_b != d_b) {
		if (c_b < d_b) c_b+=2;
		else c_b -= 2;
	}
}

void Lamp::drawId() {
	ofSetColor(ofColor::black);
	ofDrawBitmapString(id, x, y);
}

void Lamp::drawLamp() {
	//ランプを描画
	ofFill();
	//ofSetColor(c_r, c_g, c_b);
	ofSetColor(c_r, c_g, c_b);
	ofCircle(x, y, size);
}

void Lamp::showPos() {
	printf("pos(%d, %.1f, %.1f, %.1f)\n", id, x, y, z);
}

void Lamp::showColor() {
	printf("color(%d, %d, %d, %d)\n", id, c_r, c_g, c_b);
}

void Lamp::showNeighborLampId() {
	cout << "neighborLampId( " << id << " >> ";
	for (auto i : neighborLampId) cout << i << " ";
	cout << ")" << endl;
}

float Lamp::getPos_x() {
	return x;
}

float Lamp::getPos_y() {
	return y;
}

float Lamp::getPos_z() {
	return z;
}

int Lamp::getId() {
	return id;
}

void Lamp::setNeighborId(int id_) {
	neighborLampId.push_back(id_);
}

int Lamp::getNeighborId(const int n_) { //0-3: 2=近隣+，3=近隣-
	return neighborLampId[n_];
}

bool Lamp::checkOff() {
	if (clock() - stime > duration) return true;
	else return false;
}

void Lamp::turnOff() {
	/*
	d_r *= 0.0;
	d_g *= 0.0;
	d_b *= 0.0;
	*/
	d_r *= 0.1;
	d_g *= 0.1;
	d_b *= 0.1;
	
	/*
	d_r = 220;
	d_g = 220;
	d_b = 220;
	*/
	lighting = false;
	stime = 0;
}

void Lamp::turnOn(int r_, int g_, int b_) {
	//目標値に値をセット
	d_r = int(r_);
	d_g = int(g_);
	d_b = int(b_);
	lighting = true;
	stime = clock(); //起動時刻
}

bool Lamp::getState() {
	return lighting;
}

int Lamp::getRVal() {
	//return c_r;
	return d_r;
}

int Lamp::getGVal() {
	//return c_g;
	return d_g;
}

int Lamp::getBVal() {
	//return c_b;
	return d_b;
}

