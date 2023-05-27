#include "myShell.hpp"
#include <stdio.h>
#include "arm_math.h"
#include "ntshell.h"
#include "ntlibc.h"
#include "ntopt.h"
#include "communicate.hpp"
#include "imu.hpp"
#include "encoder.hpp"
#include "irSensor.hpp"
#include "suctionFan.hpp"
#include "moduleTest.hpp"
#include "maze.hpp"

// シェルコマンド関数郡
static int usrcmd_help(int argc, char **argv);
static int usrcmd_info(int argc, char **argv);
static int usrcmd_imu(int argc, char **argv);
static int usrcmd_encoder(int argc, char **argv);
static int usrcmd_sensor(int argc, char **argv);
static int usrcmd_fan(int argc, char **argv);
static int usrcmd_module_test(int argc, char **argv);
static int usrcmd_maze(int argc, char **argv);
/*static int usrcmd_path(int argc, char **argv);
static int usrcmd_log(int argc, char **argv);
static int usrcmd_loss_torque(int argc, char **argv);*/

// NT-Shell用ローカル関数群
static int func_read(char *buf, int cnt, void *extobj);
static int func_write(const char *buf, int cnt, void *extobj);
static int user_callback(const char *text, void *extobj);
static int usrcmd_execute(const char *text);
static int usrcmd_ntopt_callback(int argc, char **argv, void *extobj);

// シェルコマンド用構造体
typedef struct {
	const char* cmd;
	const char* desc;
	int (*func)(int, char**);
} cmd_table_t;

// コマンドリスト
static const cmd_table_t cmdlist[] = {
	{ "help", 			"help command.", 		usrcmd_help 		},
	{ "info", 			"system info.", 		usrcmd_info 		},
	{ "imu", 			"IMU debug.",			usrcmd_imu			},
	{ "encoder", 		"Encoder debug.",		usrcmd_encoder		},
	{ "ir_sensor", 		"IR sensor debug.",		usrcmd_sensor		},
	{ "fan",			"suction fan test.",	usrcmd_fan			},
	{ "module_test", 	"module test command.", usrcmd_module_test 	},
	{ "maze",			"maze display.",		usrcmd_maze			},
/*	{ "path",			"path display.",		usrcmd_path			},
	{ "log", 			"log display.",			usrcmd_log			},
	{ "loss_torque",	"adjust loss torque.",	usrcmd_loss_torque 	},*/
};
static ntshell_t nts;

/* ---------------------------------------------------------------
	各ラップ関数
--------------------------------------------------------------- */
static int usrcmd_imu(int argc, char **argv)
{
	module::imu::getInstance().resetReference();
	module::imu::getInstance().monitorDebug();
	return 0;
}

static int usrcmd_encoder(int argc, char **argv)
{
	module::encoder::getInstance().monitorDebug();
	return 0;
}

static int usrcmd_sensor(int argc, char **argv)
{
	if(argc != 2) {
		printf("  ir_sensor raw\r\n");
		printf("  ir_sensor wall\r\n");
		return 0;
	} else if(ntlibc_strcmp(argv[1], "raw") == 0) {
		module::irSensor::getInstance().monitorDebug();
		return 0;
	} else if (ntlibc_strcmp(argv[1], "wall") == 0) {
//		Wall_DebugPrintf();
		return 0;
	} else;
	printf("  Unknown sub command found\r\n");
	return -1;
}

static int usrcmd_fan(int argc, char **argv)
{
	if(argc != 2) {
		printf("  fan start\r\n");
		printf("  fan stop\r\n");
		return 0;
	} else if(ntlibc_strcmp(argv[1], "start") == 0) {
		module::suctionFan::getInstance().start(0.2f);
		return 0;
	} else if(ntlibc_strcmp(argv[1], "stop") == 0) {
		module::suctionFan::getInstance().stop();
		return 0;
	} else;
	printf("  Unknown sub command found\r\n");
	return -1;
}

static int usrcmd_module_test(int argc, char **argv)
{
	module::moduleTest::getInstance().run();
	return 0;
}

static int usrcmd_maze(int argc, char **argv)
{
	if(argc == 1) {
//		Maze_LoadFlash();
//		Potential_MakeMap(GOAL_X, GOAL_Y);
//		Maze_Display();
		return 0;
	} else if(ntlibc_strcmp(argv[1], "debug") == 0) {
//		if(MAZE_X >= 32 && MAZE_Y >= 32) {
			application::maze::getInstance().setDebugData();
/*			Potential_MakeMap(1, 2);	// 32x32デバッグ迷路のゴールは(1, 2)
		} else if(MAZE_X >= 16 && MAZE_Y >= 16) {
			Maze_SetDebugData();
			Potential_MakeMap(7, 7);	// 16x16デバッグ迷路のゴールは(7, 7)
		} else;
		Maze_StoreFlash();
*/		application::maze::getInstance().display();
		return 0;
	} else;
	printf("  Unknown sub command found\r\n");
	return -1;
}
/*
static int usrcmd_path(int argc, char **argv)
{
	t_maze 		start_maze = Maze_GetGlobal(0, 0);

	// メモリに壁情報がなければFlashから壁情報を読み込み
	if(start_maze.byte != 0xfd) {
		Maze_LoadFlash();
		Maze_Reset( FASTEST );
		Potential_MakeMap(GOAL_X, GOAL_Y);
	} else;
	Maze_Display();

	// 最短パスの計算
	if(Path_GetSequenceNumber() == 0) {
		Position_Reset();
		Path_Reset();
		Dijkstra_ConvertPath(GOAL_X, GOAL_Y );
	} else;

	//　最短パスの表示
	Path_DisplayAll();

	// ターン速度デバッグ
	//Route_AdjustTurnParameter(4, false);

	return 0;
}

static int usrcmd_log(int argc, char **argv)
{
	Log_ReadRecodeData();
	return 0;
}

static int usrcmd_loss_torque(int argc, char **argv)
{
	Vehicle_AdjustLossTorque();
	return 0;
}
*/
/* ---------------------------------------------------------------
	helpコマンドとinfoコマンドの実行関数
--------------------------------------------------------------- */
static int usrcmd_help(int argc, char **argv)
{
	const cmd_table_t *p = &cmdlist[0];
	for(unsigned int i = 0; i < sizeof(cmdlist) / sizeof(cmdlist[0]); i++) {
		printf("  %s", p->cmd);
		uint8_t space_num = 16 - strlen(p->cmd);
		if( space_num > 0 ) {
			for(int i = 0; i < space_num; i++) {
				printf(" ");
			}
		} else;
		printf(": %s\r\n", p->desc);
		p++;
	}
	return 0;
}

static int usrcmd_info(int argc, char **argv)
{
	if(argc != 2) {
		printf("  info sys\r\n");
		printf("  info ver\r\n");
		return 0;
	} else if(ntlibc_strcmp(argv[1], "sys") == 0) {
		printf("  sylphy_echo_v2\r\n");
		return 0;
	} else if (ntlibc_strcmp(argv[1], "ver") == 0) {
		printf("  Version 1.0\r\n");
		return 0;
	} else;
	printf("  Unknown sub command found\r\n");
	return -1;
}

/* ---------------------------------------------------------------
	送受信用ローカル関数
--------------------------------------------------------------- */
static int func_read(char *buf, int cnt, void *extobj)
{
	for(int16_t i = 0; i < cnt; i++) {
		buf[i] = (char)Communicate_ReceiceDMA();
	}
	return cnt;
}

static int func_write(const char *buf, int cnt, void *extobj)
{
	for (int i = 0; i < cnt; i++) {
		Communicate_TransmitDMA((uint8_t)(buf[i]));
	}
	return cnt;
}

/* ---------------------------------------------------------------
	コールバック関数
--------------------------------------------------------------- */
static int user_callback(const char *text, void *extobj)
{
	usrcmd_execute(text);
	return 0;
}

static int usrcmd_execute(const char *text)
{
	return ntopt_parse(text, usrcmd_ntopt_callback, 0);
}

static int usrcmd_ntopt_callback(int argc, char **argv, void *extobj)
{
	if (argc == 0) {
		return 0;
	}
	const cmd_table_t *p = &cmdlist[0];
	for (unsigned int i = 0; i < sizeof(cmdlist) / sizeof(cmdlist[0]); i++) {
		if (ntlibc_strcmp((const char *)argv[0], p->cmd) == 0) {
//			LED_LightBinary(0x00, 0);
//			Motor_StopPWM();
			return p->func(argc, argv);
		}
		p++;
	}
	printf("Unknown command found.\r\n");
	return 0;
}

/* ---------------------------------------------------------------
	初期設定関数
--------------------------------------------------------------- */
void Myshell_Initialize( void )
{
	void *extobj = 0;

	ntshell_init(&nts, func_read, func_write, user_callback, extobj);
}

void Myshell_Execute( void )
{
	if( (&nts)->initcode != 0x4367 ) {
		return;
	} else;

	unsigned char ch;
	func_read((char *)&ch, sizeof(ch), (&nts)->extobj);
	vtrecv_execute(&((&nts)->vtrecv), &ch, sizeof(ch));
}
