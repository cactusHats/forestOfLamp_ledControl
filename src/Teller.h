#pragma once

#include <vector>
#include <iostream>
using namespace std;

class Teller {
	int utime;
	int id;
	vector<int> color;
	int count;
	bool clockwise = true; //“`”À•ûŒü

public:
	Teller() = default;
	Teller(int utime_, int id_, vector<int> color_, int count_, int clockwise_) :utime(utime_), id(id_), color(color_), count(count_), clockwise(clockwise_) { };
	void setUtime(int utime_);
	void setId(int id_);
	void setRgbVal(vector<int> color_);
	void setCount(int count_);

	int getUtime();
	int getId();
	vector<int> getRgbVal();
	int getCount();

	void incrementCount();
	void decrementCount();

	int getDirection(); //“`”À•ûŒü
};