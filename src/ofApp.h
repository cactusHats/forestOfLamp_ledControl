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

		//�����v�̏�����
		void resetLampPos(vector<Lamp>& lamps_);
		void resetLampColor(vector<Lamp>& lamps_);
		void resetLampSize(vector<Lamp>& lamps_);
		void resizeLampPos(vector<Lamp>& lamps_);
		void resetNeighborLampId(vector<Lamp>& lamps_);
		long map(long x, long in_min, long in_max, long out_min, long out_max);
		
		//�`����
		void createTeller(int id, int red, int green, int blue);
		long timeThen = 0;

		//�T�E���h
		float getPan(int id);
		void soundEcho(int soundId, float vol, float pan);

		//�t�@�C���̓ǂݍ���
		auto readFile(string fileName_, int colNum);

		//�ł��߂������v�̌���
		int findNearestLamp(int x_, int y_);

		//�����v
		vector<Lamp> lamps; //�����v
		vector<Teller*> teller; //�`����
		vector<int> colorGenerator(string choice); //rgb�l����

		//�V���A���ʐM
		ofSerial serial;
		void sendPacketToLed(vector<Lamp>& lamps_, int id);

		//OSC�ʐM
		ofxOscSender snd;
		ofxOscReceiver rcv;
		void rcvMessage();
};