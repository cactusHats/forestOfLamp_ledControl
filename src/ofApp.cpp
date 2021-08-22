#include "ofApp.h"
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <stdlib.h>
#include "define.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//�����v�̃C���X�^���X�쐬
	for (int i = 0; i < lampNum; i++) {
		lamps.push_back(Lamp(i)); //set id
	}

	//���W�̏�����
	resetLampPos(lamps);
	//�F�̏�����
	resetLampColor(lamps);
	//�����v�T�C�Y(����)�̐ݒ�
	resetLampSize(lamps);
	//�����v�ʒu�̏C��
	resizeLampPos(lamps);
	//�ߗ׃����v�̓o�^
	resetNeighborLampId(lamps);

	//�V���A���|�[�gopen
	serial.setup(PORTNAME, BAUDRATE);

	//OSC�ʐM
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

	//�����v�̐F�X�V
	for (int i = 0; i < lampNum; i++) {
		lamps[i].update();
	}

	//�����v�̓`��
	//��������teller�̐��������[�v
	for (int i = 0; i < teller.size(); i++) {

		//��莞�Ԍo�߂����玟�̃����v�֓`��
		if (clock() - teller[i]->getUtime() > tInterval) {
			teller[i]->setUtime(clock());

			//�F���Z�b�g
			vector<int> color = teller[i]->getRgbVal();
			lamps[teller[i]->getId()].turnOn(
				color.at(static_cast<int>(Color::Red)),
				color.at(static_cast<int>(Color::Green)),
				color.at(static_cast<int>(Color::Blue))
			);

			//�`���񐔂��C���N�������g
			teller[i]->incrementCount();

			//�ڕW�l��LED�ɑ��M
			sendPacketToLed(lamps, teller[i]->getId());

			//�`���������Z�b�g
			bool direction = teller[i]->getDirection();

			//����ID���Z�b�g			
			int nextId = 0;
			if(direction) nextId = lamps[teller[i]->getId()].getNeighborId(static_cast<int>(IdList::Cw));
			else nextId = lamps[teller[i]->getId()].getNeighborId(static_cast<int>(IdList::Ccw));

			teller[i]->setId(nextId);

			//1��������C���X�^���X�폜
			if (teller[i]->getCount() > tNum) {
				teller.erase(teller.begin() + i);
			}
		}
	}

	//�����v�̏���
	for (int i = 0; i < lamps.size(); i++) {
		if (lamps[i].getState()) {
			if (lamps[i].checkOff()) {
				lamps[i].turnOff(); //�ڕW�l �����ݒ�
				sendPacketToLed(lamps, i); //�ڕW�l��LED�ɑ��M
			}
		}
	}

	//OSC�ʐM��M
	while (rcv.hasWaitingMessages()) {
		rcvMessage();
	}

	/*
	//LED�ɑ��M
	if ((clock() - timeThen) > SEND_INTERVAL) {
		sendPacketToLed(lamps); //LED�Ƀp�P�b�g���M
		timeThen = clock();
	}
	*/
}

//--------------------------------------------------------------
void ofApp::draw(){
	//ofBackground(200, 200, 200);
	ofBackground(170, 170, 170);

	//�����v�̕`��
	for (int i = 0; i < lamps.size(); i++) {
		lamps[i].drawLamp();
		lamps[i].drawId();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//���ʉ��̃e�X�g
	if (key == 'a') {
		soundEcho(1, 1.5f, 1.5f);
	}
	else if (key == 'b') {
		soundEcho(1, 1.5f, -1.5f);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	//�����v��UI����_�������鏈��

	//�`���҂̃Z�b�g
	int time = clock();
	int id = findNearestLamp(x, y);
	vector<int> color = colorGenerator("random");
	//vector<int> color = colorGenerator("cherryBrossoms");
	//vector<int> color = colorGenerator("fireOnIce");
	//vector<int> color = colorGenerator("snow");
	int count = 0;
	teller.push_back(new Teller(time, id, color, count, true)); //clockwise
	teller.push_back(new Teller(time, id, color, count, false)); //counterclockwise

	//���ʉ��̍Đ�
	int soundId = ofRandom(0, 3) + 1; //[0-2]+1
	float vol = 1.0f;
	float pan = getPan(id);
	soundEcho(soundId, vol, pan);
}

//--------------------------------------------------------------
//2�����x�N�g����\������֐�
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
//�t�@�C����ǂݍ��ފ֐�
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
//�}�E�X�̃N���b�N�ʒu����ł��߂������vID���擾����֐�
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

//�F�𐶐�����֐�(UI���瑀�삷��ꍇ)
vector<int> ofApp::colorGenerator(string choice) {

	int mode = 0;
	vector<int> color;

	//�����ݒ�
	std::random_device engine;
	uniform_int_distribution<unsigned> dist1(0, 6); //0�ȏ�6�ȉ�

	//�F�̑I��
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


	//rgb�̎w��
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
//�����v�̕`����W������������֐�
void ofApp::resetLampPos(vector<Lamp>& lamps_) {
	//�ǂݍ��񂾈ʒu�f�[�^���擾
	//vector<vector<float>> pos = readFile("../../lampHeightMaker/lampHeightMaker/keep/lampPos5.dat", 4); //visual studio���痧���グ��ꍇ
	vector<vector<float>> pos = readFile("../lampHeightMaker/lampHeightMaker/keep/lampPos5.dat", 4); //�o�b�`�t�@�C�����痧���グ��ꍇ

	//�C���X�^���X�̈ʒu/id�f�[�^���X�V
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
//�����v�̃T�C�Y������������֐�
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
//���ɓ`�����郉���vID������������֐�
void ofApp::resetNeighborLampId(vector<Lamp>& lamps_) {
	//�ǂݍ���id�f�[�^���擾
	//vector<vector<float>> id = readFile("../../lampListGenerator2/lampListGenerator2/lampOrder.dat", 3); //visual studio���痧���グ��ꍇ
	vector<vector<float>> id = readFile("../lampListGenerator2/lampListGenerator2/lampOrder.dat", 3); //�o�b�`�t�@�C�����痧���グ��ꍇ

	//�����v�ߗׂ�id�f�[�^���X�V
	try {
		for (int i = 0; i < lamps_.size(); i++) { //0-83
			for (int j = 0; j < id[i].size(); j++) { //�����vid�C�ߗ�+�C�ߗ�-
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
//soundControl�ɐM���𑗐M����֐�
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
//�����vID���獶�E�̉��ʃo�����X��Ԃ��֐�
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
//�����v�̐F�������ւ���֐�
void ofApp::resetLampColor(vector<Lamp>& lamps_) {
	//�ǂݍ��񂾐F�f�[�^���擾
	//vector<vector<float>> color = readFile("./bin/data/lampInitialColor.dat", 4); //visual studio���痧���グ��ꍇ
	vector<vector<float>> color = readFile("../forestOfLamp_ledControl/forestOfLamp_ledControl/bin/data/lampInitialColor.dat", 4); //�o�b�`�t�@�C�����痧���グ��ꍇ
	
	//�C���X�^���X�̈ʒu/id�f�[�^���X�V
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
//�����v�̕`��T�C�Y������������֐�
void ofApp::resetLampSize(vector<Lamp>& lamps_) {
	try {
		for (int i = 0; i < lamps_.size(); i++) {
			lamps_[i].setSize(lamps_[i].getPos_z()*RedRate); //Z���̒l�ɏk�������|���ă����v�`��T�C�Y�Ƃ���
		}
	}
	catch(out_of_range& e){
		cerr << e.what() << endl;
	}
}

/*
//--------------------------------------------------------------
//�}�C�R����LED����f�[�^�𑗂�֐�(��)
void ofApp::sendPacketToLed(vector<Lamp>& lamps_) {

	const int lampNum = 14;
	const int packetNum = int(Color::NUM_COLOR) * lampNum + 1;
	uint8_t packets[packetNum];

	//���M�p�P�b�g�Z�b�g
	switch (sendController) {
	case 0:
		packets[0] = 0xFF; //�擪���ʎq
		for (int i = 0; i < lampNum * 1; i++) {
			packets[i*int(Color::NUM_COLOR) + 0 + 1] = map(lamps_[lampNum * 0 + i].getRVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 1 + 1] = map(lamps_[lampNum * 0 + i].getGVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 2 + 1] = map(lamps_[lampNum * 0 + i].getBVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
		}
		break;
	case 1:
		packets[0] = 0xFE; //�擪���ʎq
		for (int i = 0; i < lampNum * 1; i++) {
			packets[i*int(Color::NUM_COLOR) + 0 + 1] = map(lamps_[lampNum * 1 + i].getRVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 1 + 1] = map(lamps_[lampNum * 1 + i].getGVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 2 + 1] = map(lamps_[lampNum * 1 + i].getBVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
		}
		break;
	case 2:
		packets[0] = 0xFD; //�擪���ʎq
		for (int i = 0; i < lampNum * 1; i++) {
			packets[i*int(Color::NUM_COLOR) + 0 + 1] = map(lamps_[lampNum * 2 + i].getRVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 1 + 1] = map(lamps_[lampNum * 2 + i].getGVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 2 + 1] = map(lamps_[lampNum * 2 + i].getBVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
		}
		break;
	case 3:
		packets[0] = 0xFC; //�擪���ʎq
		for (int i = 0; i < lampNum * 1; i++) {
			packets[i*int(Color::NUM_COLOR) + 0 + 1] = map(lamps_[lampNum * 3 + i].getRVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 1 + 1] = map(lamps_[lampNum * 3 + i].getGVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 2 + 1] = map(lamps_[lampNum * 3 + i].getBVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
		}
		break;
	case 4:
		packets[0] = 0xFB; //�擪���ʎq
		for (int i = 0; i < lampNum * 1; i++) {
			packets[i*int(Color::NUM_COLOR) + 0 + 1] = map(lamps_[lampNum * 4 + i].getRVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 1 + 1] = map(lamps_[lampNum * 4 + i].getGVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 2 + 1] = map(lamps_[lampNum * 4 + i].getBVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
		}
		break;
	case 5:
		packets[0] = 0xFA; //�擪���ʎq
		for (int i = 0; i < lampNum * 1; i++) {
			packets[i*int(Color::NUM_COLOR) + 0 + 1] = map(lamps_[lampNum * 5 + i].getRVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 1 + 1] = map(lamps_[lampNum * 5 + i].getGVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
			packets[i*int(Color::NUM_COLOR) + 2 + 1] = map(lamps_[lampNum * 5 + i].getBVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
		}
		break;
	default:
		break;
	}

	//�p�P�b�g���M
	serial.writeBytes(packets, packetNum);

	//�J�E���^ �C���N�������g
	sendController++;
	if (sendController >= 6) sendController = 0;

}
*/

//--------------------------------------------------------------
//�}�C�R����LED����f�[�^�𑗂�֐�(�V)
void ofApp::sendPacketToLed(vector<Lamp>& lamps_, int id) {
	const int packetNum = int(Color::NUM_COLOR) + 1; //�擪���ʎq+RGB
	uint8_t packets[packetNum];

	//���M�p�P�b�g�Z�b�g
	packets[0] = id + 150; //�擪���ʎq 150~233
	packets[1] = map(lamps_[lampNum * 0 + id].getRVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
	packets[2] = map(lamps_[lampNum * 0 + id].getGVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
	packets[3] = map(lamps_[lampNum * 0 + id].getBVal(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);

	//printf("ID=%d, R=%d, G=%d, B=%d\n", packets[0], packets[1], packets[2], packets[3]);

	//�p�P�b�g���M
	serial.writeBytes(packets, packetNum);
}

//--------------------------------------------------------------
//OSC�f�[�^����M����֐�
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
//�`���҂𐶐�����֐�
void ofApp::createTeller(int id_, int red_, int green_, int blue_) {
	//�`���҂̃Z�b�g
	int time = clock();

	vector<int> color;
	color.push_back(red_);
	color.push_back(green_);
	color.push_back(blue_);

	int count = 0;
	teller.push_back(new Teller(time, id_, color, count, true)); //clockwise
	teller.push_back(new Teller(time, id_, color, count, false)); //counterclockwise

	//�T�E���h
	int soundId = ofRandom(0, 3) + 1; //[0-2]+1
	float vol = 1.0f;
	float pan = getPan(id_);
	soundEcho(soundId, vol, pan);
}
