#pragma once

enum class Color {
	Red,
	Green,
	Blue,
	NUM_COLOR
};

enum class IdList {
	Current,
	Cw,
	Ccw,
	NUM_IDLIST
};

//�V���A���ʐM
#define BAUDRATE 250000
#define PORTNAME "COM13"
#define SEND_INTERVAL 100 //�}�C�R���ւ̃f�[�^���M�Ԋu[ms]

//LED�̌�
#define NUM_LAMPS 84

//LED�̋P�x�ݒ�
//-- ���X�̒l --
#define IN_MIN 0
#define IN_MAX 255
//-- �ύX��̒l --
#define OUT_MIN 0
#define OUT_MAX 100

//OSC�ʐM
#define IP_HOST "localhost"
#define PORT_TO_SCON 8000 //soundControl�ւ̃|�[�g�ԍ�
#define PORT_TO_LCON 9000 //DB�擾�\�t�g����̃|�[�g�ԍ�

//�����v�̐���
const int tInterval = 400; //�`�����x[ms]
const int tNum = 84; //�`����

//UI�`��p�ݒ�
const int lampRowNum = 8; //�s
const int lampColNum = 11; //��(10)
const int lampNum = lampRowNum * lampColNum - (lampRowNum / 2); //�����v��

const int lampSize = 10;
const float expRate = 1.5; //�g�嗦
const float RedRate = 0.1; //�k����

const float margin_x = 35.5 * expRate; //px
const float margin_y = 31 * expRate; //px
const float interval_x = 60 * expRate; //px
const float interval_y = 30 * expRate; //px