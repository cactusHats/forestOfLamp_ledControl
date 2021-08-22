#pragma once

#include "ofMain.h"
#include "Lamp.h"
#include "Teller.h"
#include "ofxOsc.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void mousePressed(int x, int y, int button);

		//ランプの初期化
		void resetLampPos(vector<Lamp>& lamps_);
		void resetLampColor(vector<Lamp>& lamps_);
		void resetLampSize(vector<Lamp>& lamps_);
		void resizeLampPos(vector<Lamp>& lamps_);
		void resetNeighborLampId(vector<Lamp>& lamps_);
		long map(long x, long in_min, long in_max, long out_min, long out_max);
		
		//伝搬者
		void createTeller(int id, int red, int green, int blue);
		long timeThen = 0;

		//サウンド
		float getPan(int id);
		void soundEcho(int soundId, float vol, float pan);

		//ファイルの読み込み
		auto readFile(string fileName_, int colNum);

		//最も近いランプの検索
		int findNearestLamp(int x_, int y_);

		//ランプ
		vector<Lamp> lamps; //ランプ
		vector<Teller*> teller; //伝搬者
		vector<int> colorGenerator(string choice); //rgb値生成

		//シリアル通信
		ofSerial serial;
		void sendPacketToLed(vector<Lamp>& lamps_, int id);

		//OSC通信
		ofxOscSender snd;
		ofxOscReceiver rcv;
		void rcvMessage();
};