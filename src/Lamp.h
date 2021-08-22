#pragma once

#include <vector>
using namespace std;

class Lamp {

	//�ڕW�l
	int d_r;
	int d_g;
	int d_b;

	//���ݒl
	int c_r;
	int c_g;
	int c_b;

	//�����v�T�C�Y(���a)
	int size;

	//id
	int id;

	//�����v���W
	float x;
	float y;
	float z;

	//�ߗׂ̃����vID
	vector<int> neighborLampId;

	//�_���J�n�̎���
	long stime;
	int duration = 2000;
	bool lighting = false;

public:
	//�R���X�g���N�^
	Lamp() = default;
	Lamp(int id_) : id(id_) {};
	
	//�����o�֐�
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
	void setNeighborId(int id_);
	void turnOn(int r_, int g_, int b_);
	void turnOff();
	
	bool checkOff();
	bool getState();

	int getId();
	int getNeighborId(int n_);
	int getRVal();
	int getGVal();
	int getBVal();

	float getPos_x();
	float getPos_y();
	float getPos_z();
};