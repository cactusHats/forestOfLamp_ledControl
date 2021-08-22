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

//シリアル通信
#define BAUDRATE 250000
#define PORTNAME "COM13"
#define SEND_INTERVAL 100 //マイコンへのデータ送信間隔[ms]

//LEDの個数
#define NUM_LAMPS 84

//LEDの輝度設定
//-- 元々の値 --
#define IN_MIN 0
#define IN_MAX 255
//-- 変更後の値 --
#define OUT_MIN 0
#define OUT_MAX 100

//OSC通信
#define IP_HOST "localhost"
#define PORT_TO_SCON 8000 //soundControlへのポート番号
#define PORT_TO_LCON 9000 //DB取得ソフトからのポート番号

//ランプの制御
const int tInterval = 400; //伝搬速度[ms]
const int tNum = 84; //伝搬回数

//UI描画用設定
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