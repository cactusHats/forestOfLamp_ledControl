#pragma once

#include <vector>
using namespace std;

class Lamp {

	//目標値
	int d_r;
	int d_g;
	int d_b;

	//現在値
	int c_r;
	int c_g;
	int c_b;

	//ランプサイズ(半径)
	int size;

	//id
	int id;

	//ランプ座標
	float x;
	float y;
	float z;

	//近隣のランプID
	vector<int> neighborLampId;

	//点灯開始の時間
	long stime;
	int duration = 2000;
	bool lighting = false;

public:
	//デフォルトコンストラクタ
	Lamp() = default;
	//コンストラクタ
	Lamp(int id_) : id(id_) {};
	
	void setRgbValue(int r_, int g_, int b_);
	void resetRgbValue(int r_, int g_, int b_);
	void setPos(float x_, float y_, float z_);
	void setId(int id_);
	void setSize(int lampSize_);
	void update();
	void drawLamp();
	void drawId();

	void showPos();
	void showColor();
	void showNeighborLampId();

	float getPos_x();
	float getPos_y();
	float getPos_z();
	int getId();

	void setNeighborId(int id_);
	int getNeighborId(int n_);

	bool checkOff();
	void turnOff();
	void turnOn(int r_, int g_, int b_);

	bool getState();

	int getRVal();
	int getGVal();
	int getBVal();
};