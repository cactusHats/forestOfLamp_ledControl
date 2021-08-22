#include "Teller.h"

//更新時間を取得する関数
void Teller::setUtime(int utime_) {
	utime = utime_;
}

//IDを登録する関数
void Teller::setId(int id_) {
	id = id_;
}

//rgb値を登録する関数
void Teller::setRgbVal(vector<int> color_) {
	color = color_;
}

//カウンタ値を登録する関数
void Teller::setCount(int count_) {
	count = count_;
}

//更新時間を取得する関数
int Teller::getUtime() {
	return utime;
}

//IDを取得する関数
int Teller::getId() {
	return id;
}

//rgb値を取得する関数
vector<int> Teller::getRgbVal() {
	return color;
}

//カウンタ値を取得する関数
int Teller::getCount() {
	return count;
}

//伝搬方向を取得する関数
int Teller::getDirection() {
	return clockwise;
}

//カウンタをインクリメントする関数
void Teller::incrementCount() {
	count++;
}

//カウンタをデクリメントする関数
void Teller::decrementCount() {
	count--;
}
