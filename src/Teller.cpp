#include "Teller.h"

//�X�V���Ԃ��擾����֐�
void Teller::setUtime(int utime_) {
	utime = utime_;
}

//ID��o�^����֐�
void Teller::setId(int id_) {
	id = id_;
}

//rgb�l��o�^����֐�
void Teller::setRgbVal(vector<int> color_) {
	color = color_;
}

//�J�E���^�l��o�^����֐�
void Teller::setCount(int count_) {
	count = count_;
}

//�X�V���Ԃ��擾����֐�
int Teller::getUtime() {
	return utime;
}

//ID���擾����֐�
int Teller::getId() {
	return id;
}

//rgb�l���擾����֐�
vector<int> Teller::getRgbVal() {
	return color;
}

//�J�E���^�l���擾����֐�
int Teller::getCount() {
	return count;
}

//�`���������擾����֐�
int Teller::getDirection() {
	return clockwise;
}

//�J�E���^���C���N�������g����֐�
void Teller::incrementCount() {
	count++;
}

//�J�E���^���f�N�������g����֐�
void Teller::decrementCount() {
	count--;
}
