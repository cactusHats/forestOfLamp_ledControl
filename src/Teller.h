#pragma once

#include <vector>
#include <iostream>
using namespace std;

class Teller {
	int utime; //�X�V����
	int id; //���݂̃����vid
	vector<int> color; //�����v�̐F
	int count; //�J�E���^
	bool clockwise = true; //�`������

public:
	//�R���X�g���N�^
	Teller() = default;
	Teller(int utime_, int id_, vector<int> color_, int count_, int clockwise_) :utime(utime_), id(id_), color(color_), count(count_), clockwise(clockwise_) { };
	
	//�����o�֐�
	void setUtime(int utime_);
	void setId(int id_);
	void setRgbVal(vector<int> color_);
	void setCount(int count_);

	int getUtime();
	int getId();
	int getCount();
	int getDirection();
	vector<int> getRgbVal();

	void incrementCount();
	void decrementCount();
};