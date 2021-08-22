#include "ofApp.h"
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <stdlib.h>
#include "define.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//ランプのインスタンス作成
	for (int i = 0; i < lampNum; i++) {
		lamps.push_back(Lamp(i)); //set id
	}

	//座標の初期化
	resetLampPos(lamps);
	//色の初期化
	resetLampColor(lamps);
	//ランプサイズ(高さ)の設定
	resetLampSize(lamps);
	//ランプ位置の修正
	resizeLampPos(lamps);
	//近隣ランプの登録
	resetNeighborLampId(lamps);

	//シリアルポートopen
	serial.setup(PORTNAME, BAUDRATE);

	//OSC通信
	snd.setup(IP_HOST, PORT_TO_SCON);
	rcv.setup(PORT_TO_LCON);

	cout << endl;
	cout << "---------------------" << endl;
	cout << "--- Process start ---" << endl;
	cout << "---------------------" << endl;
	cout << endl;
}

//--------------------------------------------------------------
void ofApp::update(){

	//ランプの色更新
	for (int i = 0; i < lampNum; i++) {
		lamps[i].update();
	}

	//ランプの伝搬
	//現存するtellerの数だけループ
	for (int i = 0; i < teller.size(); i++) {

		//一定時間経過したら次のランプへ伝搬
		if (clock() - teller[i]->getUtime() > tInterval) {
			teller[i]->setUtime(clock());

			//色をセット
			vector<int> color = teller[i]->getRgbVal();
			lamps[teller[i]->getId()].turnOn(
				color.at(static_cast<int>(Color::Red)),
				color.at(static_cast<int>(Color::Green)),
				color.at(static_cast<int>(Color::Blue))
			);

			//伝搬回数をインクリメント
			teller[i]->incrementCount();

			//目標値をLEDに送信
			sendPacketToLed(lamps, teller[i]->getId());

			//伝搬方向をセット
			bool direction = teller[i]->getDirection();

			//次のIDをセット			
			int nextId = 0;
			if(direction) nextId = lamps[teller[i]->getId()].getNeighborId(static_cast<int>(IdList::Cw));
			else nextId = lamps[teller[i]->getId()].getNeighborId(static_cast<int>(IdList::Ccw));

			teller[i]->setId(nextId);

			//1周したらインスタンス削除
			if (teller[i]->getCount() > tNum) {
				teller.erase(teller.begin() + i);
			}
		}
	}

	//ランプの消灯
	for (int i = 0; i < lamps.size(); i++) {
		if (lamps[i].getState()) {
			if (lamps[i].checkOff()) {
				lamps[i].turnOff(); //目標値 消灯設定
				sendPacketToLed(lamps, i); //目標値をLEDに送信
			}
		}
	}

	//OSC通信受信
	while (rcv.hasWaitingMessages()) {
		rcvMessage();
	}

	/*
	//LEDに送信
	if ((clock() - timeThen) > SEND_INTERVAL) {
		sendPacketToLed(lamps); //LEDにパケット送信
		timeThen = clock();
	}
	*/
}

//--------------------------------------------------------------
void ofApp::draw(){
	//ofBackground(200, 200, 200);
	ofBackground(170, 170, 170);

	//ランプの描画
	for (int i = 0; i < lamps.size(); i++) {
		lamps[i].drawLamp();
		lamps[i].drawId();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//効果音のテスト
	if (key == 'a') {
		soundEcho(1, 1.5f, 1.5f);
	}
	else if (key == 'b') {
		soundEcho(1, 1.5f, -1.5f);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	//ランプをUIから点灯させる処理

	//伝搬者のセット
	int time = clock();
	int id = findNearestLamp(x, y);
	vector<int> color = colorGenerator("random");
	//vector<int> color = colorGenerator("cherryBrossoms");
	//vector<int> color = colorGenerator("fireOnIce");
	//vector<int> color = colorGenerator("snow");
	int count = 0;
	teller.push_back(new Teller(time, id, color, count, true)); //clockwise
	teller.push_back(new Teller(time, id, color, count, false)); //counterclockwise

	//効果音の再生
	int soundId = ofRandom(0, 3) + 1; //[0-2]+1
	float vol = 1.0f;
	float pan = getPan(id);
	soundEcho(soundId, vol, pan);
}

//--------------------------------------------------------------
//2次元ベクトルを表示する関数
void printVec(const vector<vector<float>> v) {
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < v[i].size(); j++) {
			cout << v[i][j] << " ";
		}
		cout << endl;
	}
	cout << "---" << endl;
}

//--------------------------------------------------------------
//ファイルを読み込む関数
auto ofApp::readFile(string fileName_, int colNum) {

	ifstream file(fileName_);
	
	if (file.fail()) {
		cout << "cannnot open " << fileName_ << endl;
	}
	else {
		cout << "open " << fileName_ << endl;
	}

	int id;
	float d1, d2, d3;
	id = d1 = d2 = d3 = 0;
	vector<vector<float>> data;

	if (colNum == 3) {
		while (file >> id >> d1 >> d2) {
			vector<float> v;
			v.push_back(float(id));
			v.push_back(d1);
			v.push_back(d2);
			data.push_back(v);
		}
	}
	else if (colNum == 4) {
		while (file >> id >> d1 >> d2 >> d3) {
			vector<float> v;
			v.push_back(float(id));
			v.push_back(d1);
			v.push_back(d2);
			v.push_back(d3);
			data.push_back(v);
			//printf(">> %d, %.1f, %.1f, %.1f\n", v[0], v[1], v[2], v[3]);
		}
	}
	else {
		cout << "read error!" << endl;
	}
	
	file.close();

	return data;
}

//--------------------------------------------------------------
//マウスのクリック位置から最も近いランプIDを取得する関数
int ofApp::findNearestLamp(int x_, int y_) {
	x_ -= margin_x - interval_x / 2;
	y_ -= margin_y - interval_y / 2;

	int row = y_ / int(interval_y);

	if (row % 2 == 0) x_ -= interval_x / 2;
	else x_;

	int col = x_ / int(interval_x);

	int num = 0;
	for (int i = 0; i < row; i++) {
		num += lampColNum - 1;
		if (i % 2 != 0) num += 1;
	}
	num += col;

	return lamps[num].getId();
}
//--------------------------------------------------------------

//色を生成する関数(UIから操作する場合)
vector<int> ofApp::colorGenerator(string choice) {

	int mode = 0;
	vector<int> color;

	//乱数設定
	std::random_device engine;
	uniform_int_distribution<unsigned> dist1(0, 6); //0以上6以下

	//色の選択
	if (choice == "random") mode = dist1(engine);
	else if (choice == "red") mode = 0;
	else if (choice == "orange") mode = 1;
	else if (choice == "green") mode = 2;
	else if (choice == "skyblue") mode = 3;
	else if (choice == "blue") mode = 4;
	else if (choice == "purple") mode = 5;
	else if (choice == "pink") mode = 6;
	else if (choice == "cherryBrossoms") mode = 7;	
	else if (choice == "fireOnIce") mode = 8;
	else if (choice == "snow") mode = 9;


	//rgbの指定
	switch (mode) {
	case 0: //red
		color.push_back(255); //r
		color.push_back(127); //g
		color.push_back(127); //b
		break;
	case 1: //orange
		color.push_back(245);
		color.push_back(150);
		color.push_back(52);
		break;
	case 2: //green
		color.push_back(127);
		color.push_back(255);
		color.push_back(127);
		break;
	case 3: //sky blue
		color.push_back(127);
		color.push_back(255);
		color.push_back(255);
		break;
	case 4: //blue
		color.push_back(127);
		color.push_back(191);
		color.push_back(255);
		break;
	case 5: //purple
		color.push_back(127);
		color.push_back(127);
		color.push_back(255);
		break;
	case 6: //pink
		color.push_back(255);
		color.push_back(127);
		color.push_back(255);
		break;
	case 7: //cherryBrossoms
		color.push_back(254);
		color.push_back(212);
		color.push_back(251);
		break;
	case 8: //fireOnIce
		color.push_back(218);
		color.push_back(179);
		color.push_back(0);
		break;
	case 9: //snow
		color.push_back(255);
		color.push_back(255);
		color.push_back(255);
		break;
	default:
		break;
	}

	return color;
}

//--------------------------------------------------------------
//ランプの描画座標を初期化する関数
void ofApp::resetLampPos(vector<Lamp>& lamps_) {
	//読み込んだ位置データを取得
	//vector<vector<float>> pos = readFile("../../lampHeightMaker/lampHeightMaker/keep/lampPos5.dat", 4); //visual studioから立ち上げる場合
	vector<vector<float>> pos = readFile("../lampHeightMaker/lampHeightMaker/keep/lampPos5.dat", 4); //バッチファイルから立ち上げる場合

	//インスタンスの位置/idデータを更新
	try {
		for (int i = 0; i < lampNum; i++) {
			//printf(">> %.f, %.1f, %.1f, %.1f\n", pos[i].at(0), pos[i].at(1), pos[i].at(2), pos[i].at(3));
			lamps_[i].setPos(pos[i].at(1), pos[i].at(2), pos[i].at(3)); //xyz
		}
	}
	catch (out_of_range& e) {
		cerr << e.what() << endl;
	}
}

//--------------------------------------------------------------
//ランプのサイズを初期化する関数
void ofApp::resizeLampPos(vector<Lamp>& lamps_) {
	try {
		for (int i = 0; i < lamps_.size(); i++) {
			lamps_[i].setPos(lamps_[i].getPos_x() * expRate, lamps_[i].getPos_y() * expRate, lamps_[i].getPos_z());
		}
	}
	catch (out_of_range& e) {
		cerr << e.what() << endl;
	}
}

//--------------------------------------------------------------
//次に伝搬するランプIDを初期化する関数
void ofApp::resetNeighborLampId(vector<Lamp>& lamps_) {
	//読み込んだidデータを取得
	//vector<vector<float>> id = readFile("../../lampListGenerator2/lampListGenerator2/lampOrder.dat", 3); //visual studioから立ち上げる場合
	vector<vector<float>> id = readFile("../lampListGenerator2/lampListGenerator2/lampOrder.dat", 3); //バッチファイルから立ち上げる場合

	//ランプ近隣のidデータを更新
	try {
		for (int i = 0; i < lamps_.size(); i++) { //0-83
			for (int j = 0; j < id[i].size(); j++) { //ランプid，近隣+，近隣-
				lamps_[i].setNeighborId(int(id[i].at(j)));
			}
		}
	}
	catch (out_of_range& e) {
		cerr << e.what() << endl;
	}
}

long ofApp::map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//--------------------------------------------------------------
//soundControlに信号を送信する関数
void ofApp::soundEcho(int soundId, float vol, float pan) {
	ofxOscMessage m;

	switch(soundId) {
	case 1:
		m.setAddress("/lamp/spund/1/play");
		break;
	case 2:
		m.setAddress("/lamp/spund/2/play");
		break;
	case 3:
		m.setAddress("/lamp/spund/2/play");
		break;
	defaut:
		break;
	}

	m.addFloatArg(vol);
	m.addFloatArg(pan);
	snd.sendMessage(m);
}

//--------------------------------------------------------------
//ランプIDから左右の音量バランスを返す関数
float ofApp::getPan(int id) {
	if (0 <= id && id <= 9) {
		id -= (10 * 0) + (11 * 0);
		id += 0.5;
	}
	else if (10 <= id && id <= 20) {
		id -= (10 * 1) + (11 * 0);
	}
	else if (21 <= id && id <= 30) {
		id -= (10 * 1) + (11 * 1);
		id += 0.5;
	}
	else if (31 <= id && id <= 41) {
		id -= (10 * 2) + (11 * 1);
	}
	else if (42 <= id && id <= 51) {
		id -= (10 * 2) + (11 * 2);
		id += 0.5;
	}
	else if (52 <= id && id <= 62) {
		id -= (10 * 3) + (11 * 2);
	}
	else if (63 <= id && id <= 72) {
		id -= (10 * 3) + (11 * 3);
		id += 0.5;
	}
	else if (73 <= id && id <= 83) {
		id -= (10 * 4) + (11 * 3);
	}

	float pan = ofMap(id, 0, 10, -10, 10, true);
	return pan /= 10.0f; //return from -1.0f to 1.0f
}

//--------------------------------------------------------------
//ランプの色を初期関する関数
void ofApp::resetLampColor(vector<Lamp>& lamps_) {
	//読み込んだ色データを取得
	//vector<vector<float>> color = readFile("./bin/data/lampInitialColor.dat", 4); //visual studioから立ち上げる場合
	vector<vector<float>> color = readFile("../forestOfLamp_ledControl/forestOfLamp_ledControl/bin/data/lampInitialColor.dat", 4); //バッチファイルから立ち上げる場合
	
	//インスタンスの位置/idデータを更新
	try {
		for (int i = 0; i < lamps_.size(); i++) {
			lamps_[i].resetRgbValue(int(color[i].at(1)), int(color[i].at(2)), int(color[i].at(3))); //rgb
		}
	}
	catch (out_of_range& e) {
		cerr << e.what() << endl;
	}
}

//--------------------------------------------------------------
//ランプの描画サイズを初期化する関数
void ofApp::resetLampSize(vector<Lamp>& lamps_) {
	try {
		for (int i = 0; i < lamps_.size(); i++) {
			lamps_[i].setSize(lamps_[i].getPos_z()*RedRate); //Z軸の値に縮小率を掛けてランプ描画サイズとする
		}
	}
	catch(out_of_range& e){
		cerr << e.what() << endl;
	}
}

/*
//--------------------------------------------------------------
//マイコンにLED制御データを送る関数(旧)
void ofApp::sendPacketToLed(vector<Lamp>& lamps_) {

	const int lampNum = 14;
	const int packetNum = int(Color::NUM_COLOR) * lampNum + 1;
	uint8_t packets[packetNum];

	//送信パケットセット
	switch (sendController) {
	case 0:
		packets[0] = 0xFF; //先頭識別子
		for (int i = 0; i < lampNum * 1; i++) {
			packets[i*int(Color::NUM_COLOR) + 0 + 1] = map(lamps_[lampNum * 0 + i].getRVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 1 + 1] = map(lamps_[lampNum * 0 + i].getGVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 2 + 1] = map(lamps_[lampNum * 0 + i].getBVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
		}
		break;
	case 1:
		packets[0] = 0xFE; //先頭識別子
		for (int i = 0; i < lampNum * 1; i++) {
			packets[i*int(Color::NUM_COLOR) + 0 + 1] = map(lamps_[lampNum * 1 + i].getRVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 1 + 1] = map(lamps_[lampNum * 1 + i].getGVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 2 + 1] = map(lamps_[lampNum * 1 + i].getBVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
		}
		break;
	case 2:
		packets[0] = 0xFD; //先頭識別子
		for (int i = 0; i < lampNum * 1; i++) {
			packets[i*int(Color::NUM_COLOR) + 0 + 1] = map(lamps_[lampNum * 2 + i].getRVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 1 + 1] = map(lamps_[lampNum * 2 + i].getGVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 2 + 1] = map(lamps_[lampNum * 2 + i].getBVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
		}
		break;
	case 3:
		packets[0] = 0xFC; //先頭識別子
		for (int i = 0; i < lampNum * 1; i++) {
			packets[i*int(Color::NUM_COLOR) + 0 + 1] = map(lamps_[lampNum * 3 + i].getRVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 1 + 1] = map(lamps_[lampNum * 3 + i].getGVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 2 + 1] = map(lamps_[lampNum * 3 + i].getBVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
		}
		break;
	case 4:
		packets[0] = 0xFB; //先頭識別子
		for (int i = 0; i < lampNum * 1; i++) {
			packets[i*int(Color::NUM_COLOR) + 0 + 1] = map(lamps_[lampNum * 4 + i].getRVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 1 + 1] = map(lamps_[lampNum * 4 + i].getGVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 2 + 1] = map(lamps_[lampNum * 4 + i].getBVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
		}
		break;
	case 5:
		packets[0] = 0xFA; //先頭識別子
		for (int i = 0; i < lampNum * 1; i++) {
			packets[i*int(Color::NUM_COLOR) + 0 + 1] = map(lamps_[lampNum * 5 + i].getRVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 1 + 1] = map(lamps_[lampNum * 5 + i].getGVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 2 + 1] = map(lamps_[lampNum * 5 + i].getBVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
		}
		break;
	default:
		break;
	}

	//パケット送信
	serial.writeBytes(packets, packetNum);

	//カウンタ インクリメント
	sendController++;
	if (sendController >= 6) sendController = 0;

}
*/

//--------------------------------------------------------------
//マイコンにLED制御データを送る関数(新)
void ofApp::sendPacketToLed(vector<Lamp>& lamps_, int id) {
	const int packetNum = int(Color::NUM_COLOR) + 1; //先頭識別子+RGB
	uint8_t packets[packetNum];

	//送信パケットセット
	packets[0] = id + 150; //先頭識別子 150~233
	packets[1] = map(lamps_[lampNum * 0 + id].getRVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
	packets[2] = map(lamps_[lampNum * 0 + id].getGVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
	packets[3] = map(lamps_[lampNum * 0 + id].getBVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);

	//printf("ID=%d, R=%d, G=%d, B=%d\n", packets[0], packets[1], packets[2], packets[3]);

	//パケット送信
	serial.writeBytes(packets, packetNum);
}

//--------------------------------------------------------------
//OSCデータを受信する関数
void ofApp::rcvMessage() {
	ofxOscMessage m;
	rcv.getNextMessage(&m);

	int id = 0;
	int red = 0;
	int green = 0;
	int blue = 0;

	id = m.getArgAsInt(0);
	red = m.getArgAsInt(1);
	green = m.getArgAsInt(2);
	blue = m.getArgAsInt(3);

	printf("get osc msg: id=%d R=%d, G=%d, B=%d \n", id, red, green, blue);

	if (m.getAddress() == "/lamp/rgbVal") {
		createTeller(id, red, green, blue);
	}

}

//--------------------------------------------------------------
//伝搬者を生成する関数
void ofApp::createTeller(int id_, int red_, int green_, int blue_) {
	//伝搬者のセット
	int time = clock();

	vector<int> color;
	color.push_back(red_);
	color.push_back(green_);
	color.push_back(blue_);

	int count = 0;
	teller.push_back(new Teller(time, id_, color, count, true)); //clockwise
	teller.push_back(new Teller(time, id_, color, count, false)); //counterclockwise

	//サウンド
	int soundId = ofRandom(0, 3) + 1; //[0-2]+1
	float vol = 1.0f;
	float pan = getPan(id_);
	soundEcho(soundId, vol, pan);
}
