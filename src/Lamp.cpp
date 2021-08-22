#include "Lamp.h"
#include "ofMain.h"
#include <stdio.h>

//rgb値を登録する関数
void Lamp::setRgbValue(int r_, int g_, int b_) {
	//目標値に値をセット
	d_r = r_;
	d_g = g_;
	d_b = b_;
}

//rgb値をリセットする関数
void Lamp::resetRgbValue(int r_, int g_, int b_) {
	d_r = c_r = r_;
	d_g = c_g = g_;
	d_b = c_b = b_;
}

//ランプ座標を登録する関数
void Lamp::setPos(float x_, float y_, float z_) {
	x = x_;
	y = y_;
	z = z_;
}

//ランプIDを登録する関数
void Lamp::setId(int id_) {
	id = id_;
}

//UIでのランプサイズを登録する関数
void Lamp::setSize(int lampSize_) {
	size = lampSize_;
}

//ランプの色を更新する関数
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

//UI上でランプを描画する関数
void Lamp::drawLamp() {
	ofFill();
	ofSetColor(c_r, c_g, c_b);
	ofCircle(x, y, size);
}

//IDを描画する関数
void Lamp::drawId() {
	ofSetColor(ofColor::black);
	ofDrawBitmapString(id, x, y); //文字の描画
}

//ランプ座標を表示する関数
void Lamp::showPos() {
	printf("pos(%d, %.1f, %.1f, %.1f)\n", id, x, y, z);
}

//ランプの色を表示する関数
void Lamp::showColor() {
	printf("color(%d, %d, %d, %d)\n", id, c_r, c_g, c_b);
}

//次に伝搬するIDを表示する関数
void Lamp::showNeighborLampId() {
	cout << "neighborLampId( " << id << " >> ";
	for (auto i : neighborLampId) cout << i << " ";
	cout << ")" << endl;
}

//次に伝搬するIDを登録する関数
void Lamp::setNeighborId(int id_) {
	neighborLampId.push_back(id_);
}

//ランプを点灯する関数
void Lamp::turnOn(int r_, int g_, int b_) {
	//目標値に値をセット
	d_r = int(r_);
	d_g = int(g_);
	d_b = int(b_);
	lighting = true;
	stime = clock(); //起動時刻
}

//ランプを消灯する関数
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
	/*
	//cherryBrossoms
	d_r = 254 * 0.3;
	d_g = 136 * 0.3;
	d_b = 245 * 0.3;
	*/

	/*
	d_r = 255;
	d_g = 150;
	d_b = 150;
	*/
	
	/*
	//fireOnIce
	d_r = 0 * 0.3;
	d_g = 106 * 0.3;
	d_b = 182 * 0.3;
	*/
	
	/*
	//snow
	d_r = 171 * 0.3;
	d_g = 137 * 0.3;
	d_b = 195 * 0.3;
	*/

	lighting = false;
	stime = 0;
}

//設定時間だけ点灯し続けたか確認する関数
bool Lamp::checkOff() {
	if (clock() - stime > duration) return true;
	else return false;
}

//点灯中か否かを取得する関数
bool Lamp::getState() {
	return lighting;
}

//ランプIDを取得する関数
int Lamp::getId() {
	return id;
}

//次に伝搬するIDを取得する関数
int Lamp::getNeighborId(const int n_) { //0-3: 2=近隣+，3=近隣-
	return neighborLampId[n_];
}

//ランプの色(R値)を取得する関数
int Lamp::getRVal() {
	//return c_r; //現在値
	return d_r; //目標値
}

//ランプの色(G値)を取得する関数
int Lamp::getGVal() {
	//return c_g; //現在値
	return d_g; //目標値
}

//ランプの色(B値)を取得する関数
int Lamp::getBVal() {
	//return c_b; //現在値
	return d_b; //目標値
}

//ランプ座標(x)を取得する関数
float Lamp::getPos_x() {
	return x;
}

//ランプ座標(y)を取得する関数
float Lamp::getPos_y() {
	return y;
}

//ランプ座標(z)を取得する関数
float Lamp::getPos_z() {
	return z;
}



