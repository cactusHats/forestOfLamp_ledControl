#include "Lamp.h"
#include "ofMain.h"
#include <stdio.h>

//rgb�l��o�^����֐�
void Lamp::setRgbValue(int r_, int g_, int b_) {
	//�ڕW�l�ɒl���Z�b�g
	d_r = r_;
	d_g = g_;
	d_b = b_;
}

//rgb�l�����Z�b�g����֐�
void Lamp::resetRgbValue(int r_, int g_, int b_) {
	d_r = c_r = r_;
	d_g = c_g = g_;
	d_b = c_b = b_;
}

//�����v���W��o�^����֐�
void Lamp::setPos(float x_, float y_, float z_) {
	x = x_;
	y = y_;
	z = z_;
}

//�����vID��o�^����֐�
void Lamp::setId(int id_) {
	id = id_;
}

//UI�ł̃����v�T�C�Y��o�^����֐�
void Lamp::setSize(int lampSize_) {
	size = lampSize_;
}

//�����v�̐F���X�V����֐�
void Lamp::update() {
	//1�X�e�b�v�ŕω�
	/*
	c_r = d_r;
	c_g = d_g;
	c_b = d_b;
	*/

	//�������ω�
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

//UI��Ń����v��`�悷��֐�
void Lamp::drawLamp() {
	ofFill();
	ofSetColor(c_r, c_g, c_b);
	ofCircle(x, y, size);
}

//ID��`�悷��֐�
void Lamp::drawId() {
	ofSetColor(ofColor::black);
	ofDrawBitmapString(id, x, y); //�����̕`��
}

//�����v���W��\������֐�
void Lamp::showPos() {
	printf("pos(%d, %.1f, %.1f, %.1f)\n", id, x, y, z);
}

//�����v�̐F��\������֐�
void Lamp::showColor() {
	printf("color(%d, %d, %d, %d)\n", id, c_r, c_g, c_b);
}

//���ɓ`������ID��\������֐�
void Lamp::showNeighborLampId() {
	cout << "neighborLampId( " << id << " >> ";
	for (auto i : neighborLampId) cout << i << " ";
	cout << ")" << endl;
}

//���ɓ`������ID��o�^����֐�
void Lamp::setNeighborId(int id_) {
	neighborLampId.push_back(id_);
}

//�����v��_������֐�
void Lamp::turnOn(int r_, int g_, int b_) {
	//�ڕW�l�ɒl���Z�b�g
	d_r = int(r_);
	d_g = int(g_);
	d_b = int(b_);
	lighting = true;
	stime = clock(); //�N������
}

//�����v����������֐�
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

//�ݒ莞�Ԃ����_�������������m�F����֐�
bool Lamp::checkOff() {
	if (clock() - stime > duration) return true;
	else return false;
}

//�_�������ۂ����擾����֐�
bool Lamp::getState() {
	return lighting;
}

//�����vID���擾����֐�
int Lamp::getId() {
	return id;
}

//���ɓ`������ID���擾����֐�
int Lamp::getNeighborId(const int n_) { //0-3: 2=�ߗ�+�C3=�ߗ�-
	return neighborLampId[n_];
}

//�����v�̐F(R�l)���擾����֐�
int Lamp::getRVal() {
	//return c_r; //���ݒl
	return d_r; //�ڕW�l
}

//�����v�̐F(G�l)���擾����֐�
int Lamp::getGVal() {
	//return c_g; //���ݒl
	return d_g; //�ڕW�l
}

//�����v�̐F(B�l)���擾����֐�
int Lamp::getBVal() {
	//return c_b; //���ݒl
	return d_b; //�ڕW�l
}

//�����v���W(x)���擾����֐�
float Lamp::getPos_x() {
	return x;
}

//�����v���W(y)���擾����֐�
float Lamp::getPos_y() {
	return y;
}

//�����v���W(z)���擾����֐�
float Lamp::getPos_z() {
	return z;
}



