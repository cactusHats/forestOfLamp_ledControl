#pragma once

#include <vector>
#include <iostream>
using namespace std;

class Teller {
	int utime; //更新時間
	int id; //現在のランプid
	vector<int> color; //ランプの色
	int count; //カウンタ
	bool clockwise = true; //伝搬方向

public:
	//コンストラクタ
	Teller() = default;
	Teller(int utime_, int id_, vector<int> color_, int count_, int clockwise_) :utime(utime_), id(id_), color(color_), count(count_), clockwise(clockwise_) { };
	
	//メンバ関数
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