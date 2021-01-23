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
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		//ランプの初期化
		void resetLampPos(vector<Lamp>& lamps_);
		void resetLampColor(vector<Lamp>& lamps_);
		void resetLampSize(vector<Lamp>& lamps_);
		void resizeLampPos(vector<Lamp>& lamps_);
		void resetNeighborLampId(vector<Lamp>& lamps_);
		long map(long x, long in_min, long in_max, long out_min, long out_max);

		//osc
		void soundEcho(int soundId, float vol, float pan);

		float getPan(int id);

		//ファイルの読み込み
		auto readFile(string fileName_, int colNum);

		//最も近いランプの検索
		int findNearestLamp(int x_, int y_);

		const int lampRowNum = 8; //行
		const int lampColNum = 11; //列(10)
		const int lampNum = lampRowNum * lampColNum - (lampRowNum / 2); //ランプ個数

		const int lampSize = 10;
		const float expRate = 1.5; //拡大率
		const float RedRate = 0.1; //縮小率

		const float margin_x = 35.5 * expRate; //px
		const float margin_y = 31 * expRate; //px
		const float interval_x = 60 * expRate; //px
		const float interval_y = 30 * expRate; //px

		vector<Lamp> lamps; //ランプのvector

		//伝搬役
		vector<Teller*> teller;
		//updateTime, id, r, g, b, cnt
		vector<int> colorGenerator(string choice);

		const int tInterval = 500; //伝搬速度[ms]
		const int tNum = 84; //伝搬回数

		ofSerial serial;
		//void sendPacketToLed(vector<Lamp>& lamps_);
		//int sendController = 0;
		void sendPacketToLed(vector<Lamp>& lamps_, int id);

		long timeThen = 0;

		//osc
		ofxOscSender snd;
		ofxOscReceiver rcv;
		void rcvMessage();

		void createTeller(int id, int red, int green, int blue);
};