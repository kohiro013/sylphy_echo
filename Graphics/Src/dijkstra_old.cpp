#include <iostream>
#include <vector>
#include "position.hpp"
#include "maze.hpp"
#include "potential.hpp"

//コマンドリスト（GO1は１区画前進を意味する）
#define GO1    1//必ず１である必要がある
#define GO2    2
#define GO30  30
#define GO31  31
#define TURNR 32//GO31の次にあれば何でもよい
#define TURNL 33
#define DIA_TO_CLOTHOIDR 34
#define DIA_TO_CLOTHOIDL 35
#define DIA_FROM_CLOTHOIDR 36
#define DIA_FROM_CLOTHOIDL 37
#define DIA_TURNR 38
#define DIA_TURNL 39
#define DIA_GO1 40
#define DIA_GO63 102
#define SNODE 103//ストップを意味する

//コマンドリストここまで
const int runtime[104]={0,//runtime[0]には意味はない．実装上そうなった
10,18,25,32,40,45,49,54,58,63,//GO1,GO2,...を行うためにかかる時間.
67,72,76,81,85,90,94,99,103,108,
112,117,121,126,130,135,139,144,148,153,157,
 8,//TURNRを行うためにかかる時間
 8,//TURNLを行うためにかかる時間
 8,//DIA_TO_CLOTHOIDRを行うためにかかる時間
 8,//DIA_TO_CLOTHOIDLを行うためにかかる時間
 8,//DIA_FROM_CLOTHOIDRを行うためにかかる時間
 8,//DIA_FROM_CLOTHOIDLを行うためにかかる時間
 6,//DIA_TURNRを行うためにかかる時間
 6,//DIA_TURNLを行うためにかかる時間
 7,13,19,24,29,33,40,43,46,49,//DIA_GO1-GO10
53,56,59,62,65,68,72,75,78,81,//DIA_GO11-GO20
84,88,91,94,97,100,103,107,110,113,//DIA_GO21-GO30
116,119,123,126,129,132,135,138,142,145,//DIA_GO31-GO40
148,151,154,158,161,164,167,170,173,177,//DIA_GO41-GO50
180,183,186,189,193,196,199,202,205,208,//DIA_GO51-GO60
212,215,218,                     //DIA_GO61-GO63を行うためにかかる時間
 0//ストップを意味する．何でもいい．
};

// ハッシュ番号をかえす．
// x      は0-31の5bit
// y      は0-31の5bit:y座標（左上が(0,0)）を保存
// d      は0-03の2bit:北向き，東向き，南向き，西向きに対応
// diaflagは0-2の2bit :斜めに向いていない，左斜め向き，右斜め向きに対応
uint16_t encoder( int8_t x, int8_t y, int8_t d, int8_t diaflag )
{
	return (diaflag<<12)+(d<<10)+(y<<5)+x;
}

// ハッシュ番号から座標をかえす
void decoder( uint16_t hash, int8_t *x, int8_t *y, int8_t *d, int8_t *diaflag )
{
	*x =  hash      & (32-1);
	*y = (hash>> 5) & (32-1);
	*d = (hash>>10) & (4-1);
	*diaflag = (hash>>12);
}

void addlist( uint16_t node, uint16_t lis[1<<7], int16_t *lisnum )
{
	int i;
	for( i = 0; i < *lisnum; i++ ) {
		if( lis[i] == node ) break;
	}
	if( i == *lisnum ) {
		lis[*lisnum]=node;
		*lisnum = *lisnum + 1;
	} else;
}

void addnode( uint16_t node, uint16_t lis[1<<7], int16_t *lisnum, uint16_t prev[1<<14], uint16_t cost[1<<14] )
{
	int8_t nowx, nowy, nowd, nowdia;
	uint16_t nextnode;
	int8_t dx = 0, dy = 0;
	uint16_t i;
	uint16_t diastraight;
	decoder( node, &nowx, &nowy, &nowd, &nowdia );
	//printf("%2d, %2d, %2d, %2d\r\n", nowx, nowy, nowd, nowdia);

	//直進側を展開
	switch( nowd ) {//メモ：from側の相対座標
		case 0:dx=-1;dy= 0;break;
		case 1:dx= 0;dy=-1;break;
		case 2:dx=+1;dy= 0;break;
		case 3:dx= 0;dy=+1;break;
	}

	// 直進
	if( nowdia == 0 ) {
		if( 0 <= nowx+dx && nowx+dx < MAZE_X && 0 <= nowy+dy && nowy+dy < MAZE_Y ) {
			// 右ターン
			if(application::maze::getInstance().getLocalData(nowx+dx, nowy+dy, nowd, FRONT) == false) {
				nextnode = encoder(nowx+dx, nowy+dy, (nowd + 1) & 0x03, 2);
				if( cost[node]+runtime[DIA_FROM_CLOTHOIDR] < cost[nextnode] || prev[nextnode] == 0 ) {
					prev[nextnode] = DIA_FROM_CLOTHOIDR;
					cost[nextnode] = cost[node]+runtime[DIA_FROM_CLOTHOIDR];
					addlist(nextnode, lis, lisnum);//重複登録をふせぎつつnextnodeを未探索ノードとしてリストに追加
				} else;
			} else;
			// 左ターン
			if(application::maze::getInstance().getLocalData(nowx+dx, nowy+dy, nowd, FRONT) == false) {
				nextnode = encoder(nowx+dx,nowy+dy, (nowd + 3) & 0x03, 1);
				if( cost[node]+runtime[DIA_FROM_CLOTHOIDL] < cost[nextnode] || prev[nextnode] == 0 ) {
					prev[nextnode] = DIA_FROM_CLOTHOIDL;
					cost[nextnode] = cost[node]+runtime[DIA_FROM_CLOTHOIDL];
					addlist(nextnode, lis, lisnum);//重複登録をふせぎつつnextnodeを未探索ノードとしてリストに追加
				} else;
			} else;
		} else;
		// 直線
		for( i = GO1; i <= GO31; i++ ) {
			if( 0 <= nowx+dx*i && nowx+dx*i < MAZE_X && 0 <= nowy+dy*i && nowy+dy*i < MAZE_Y ) {
				if(application::maze::getInstance().getLocalData(nowx+dx*i, nowy+dy*i, nowd, FRONT) == false) {
					nextnode = encoder(nowx+dx*i, nowy+dy*i, nowd, 0);
					if( cost[node]+runtime[i] < cost[nextnode] || prev[nextnode] == 0 ) {
						prev[nextnode] = i;
						cost[nextnode] = cost[node]+runtime[i];
						addlist(nextnode, lis, lisnum);//重複登録をふせぎつつnextnodeを未探索ノードとしてリストに追加
					} else;
				} else {
					break;
				}
			} else {
				break;
			}
		}
	// 斜め
	} else {
		if( 0 <= nowx+dx && nowx+dx < MAZE_X && 0 <= nowy+dy && nowy+dy < MAZE_Y ) {
			if( nowdia == 2 ) {
				if(application::maze::getInstance().getLocalData(nowx+dx, nowy+dy, nowd, FRONT) == false) {//DIA_TO_CLOTHOIDR
					nextnode = encoder(nowx+dx,nowy+dy, nowd, 0);
					if( cost[node]+runtime[DIA_TO_CLOTHOIDR] < cost[nextnode] || prev[nextnode] == 0 ) {
						prev[nextnode] = DIA_TO_CLOTHOIDR;
						cost[nextnode] = cost[node]+runtime[DIA_TO_CLOTHOIDR];
						addlist(nextnode, lis, lisnum);//重複登録をふせぎつつnextnodeを未探索ノードとしてリストに追加
					} else;
				} else;
				if(application::maze::getInstance().getLocalData(nowx+dx, nowy+dy, nowd, FRONT) == false) {//DIA_TURNR
					nextnode = encoder(nowx+dx,nowy+dy, (nowd + 1) & 0x03, 2);
					if( cost[node]+runtime[DIA_TURNR] < cost[nextnode] || prev[nextnode] == 0 ) {
						prev[nextnode] = DIA_TURNR;
						cost[nextnode] = cost[node]+runtime[DIA_TURNR];
						addlist(nextnode, lis, lisnum);//重複登録をふせぎつつnextnodeを未探索ノードとしてリストに追加
					} else;
				} else;
			} else;
			if( nowdia == 1 ) {
				if(application::maze::getInstance().getLocalData(nowx+dx, nowy+dy, nowd, FRONT) == false) {//DIA_TO_CLOTHOIDL
					nextnode = encoder(nowx+dx,nowy+dy, nowd, 0);
					if( cost[node]+runtime[DIA_TO_CLOTHOIDL] < cost[nextnode] || prev[nextnode] == 0 ) {
						prev[nextnode] = DIA_TO_CLOTHOIDL;
						cost[nextnode] = cost[node]+runtime[DIA_TO_CLOTHOIDL];
						addlist(nextnode, lis, lisnum);//重複登録をふせぎつつnextnodeを未探索ノードとしてリストに追加
					} else;
				} else;
				if(application::maze::getInstance().getLocalData(nowx+dx, nowy+dy, nowd, FRONT) == false) {//DIA_TURNL
					nextnode = encoder(nowx+dx,nowy+dy, (nowd + 3) & 0x03, 1);
					if( cost[node]+runtime[DIA_TURNL] < cost[nextnode] || prev[nextnode] == 0 ) {
						prev[nextnode] = DIA_TURNL;
						cost[nextnode] = cost[node]+runtime[DIA_TURNL];
						addlist(nextnode, lis, lisnum);//重複登録をふせぎつつnextnodeを未探索ノードとしてリストに追加
					} else;
				} else;
			} else;
		} else;
		for( diastraight = DIA_GO1; diastraight <= DIA_GO63; diastraight++ ) {//63
			switch( nowd ) {//メモ：from側の相対座標
				case 0:dx=-1;dy= 0;break;
				case 1:dx= 0;dy=-1;break;
				case 2:dx=+1;dy= 0;break;
				case 3:dx= 0;dy=+1;break;
			}
			if( 0<= nowx+dx && nowx+dx<MAZE_X && 0<= nowy+dy && nowy+dy<MAZE_Y ) {
				if(application::maze::getInstance().getLocalData(nowx+dx, nowy+dy, nowd, FRONT) == false) {//DIA_TURNR
					nowx+=dx;
					nowy+=dy;
					nowd=(nowd+nowdia*2-1) & 0x03;//右傾きなら右方向へ，左傾きなら左方向へnowdを回転
					nowdia=3-nowdia;//右傾きと左傾きを切り替える
					nextnode = encoder(nowx,nowy, nowd, nowdia);
					if( cost[node]+runtime[diastraight] < cost[nextnode] || prev[nextnode] == 0 ) {
						prev[nextnode] = diastraight;
						cost[nextnode] = cost[node]+runtime[diastraight];
						//nextnodeを未探索ノードとして追加する．
						addlist( nextnode, lis, lisnum );//重複登録をふせぎつつリストに追加
					} else;
				} else {
					break;
				}
			} else {
				break;
			}
		}
	}
}

int dijkstra( int8_t goalx, int8_t goaly, uint16_t route[255] )
{
	uint16_t prev[1<<14] = {0};//0なら未確定ノード，1以上なら対応したコマンドが過去データ
	uint16_t cost[1<<14] = {0};//コスト
	uint16_t lis[1<<7];//探索ノードをリスト構造で保持リストは未探索ノードの個数分がはいる十分な大きさを確保すること
	int16_t lisnum = 0;
	uint16_t i;
	uint16_t minnum;
	uint16_t minindex;
	int8_t nowx , nowy , nowd, nowdia;
	int8_t dx = 0, dy = 0;
	uint16_t nownode = 0, nextnode = 0;
	uint16_t routenum;
	uint16_t diadistance;

	//初期化：ゴールノードは自明な確定ノード
	prev[encoder(goalx,goaly,0,0)] = SNODE;
	prev[encoder(goalx,goaly,1,0)] = SNODE;
	prev[encoder(goalx,goaly,2,0)] = SNODE;
	prev[encoder(goalx,goaly,3,0)] = SNODE;

	//初期化：リストにゴールノードの隣接ノードをつっこむ
	if(application::maze::getInstance().getLocalData(goalx, goaly, SOUTH, FRONT) == false ) { addnode(encoder(goalx, goaly, FRONT, 0), lis, &lisnum, prev, cost); }
	if(application::maze::getInstance().getLocalData(goalx, goaly, WEST , FRONT) == false ) { addnode(encoder(goalx, goaly, RIGHT, 0), lis, &lisnum, prev, cost); }
	if(application::maze::getInstance().getLocalData(goalx, goaly, NORTH, FRONT) == false ) { addnode(encoder(goalx, goaly, REAR,  0), lis, &lisnum, prev, cost); }
	if(application::maze::getInstance().getLocalData(goalx, goaly, EAST , FRONT) == false ) { addnode(encoder(goalx, goaly, LEFT,  0), lis, &lisnum, prev, cost); }
	//探索開始，ベクトル場を作成する
	while( lisnum ) {
		//リストから最もコストが低いものを探す
		minnum = cost[lis[0]];
		minindex = 0;
		for( i = 1; i < lisnum; i++ ) {//このループに入った段階でlisnum>0が成立するからi=1で初期化してよい
			if( minnum > cost[lis[i]] ) {
				minnum = cost[lis[i]];
				minindex = i;
			} else;
		}
		//minindexから伸びる未探索ノードを追加
		addnode( lis[minindex], lis, &lisnum, prev, cost );
		//初期位置が確定ノードになったら探索終了
		if( lis[minindex] == encoder(0, 0, NORTH, 0) ) {
			lisnum = -1;//ここから抜けたことを番兵値で覚えておく
			break;
		} else;
		//minindexをリストから削除して確定ノード扱いに
		for( i = minindex; i < lisnum - 1; i++ )
			lis[i] = lis[i+1];
		lisnum--;
	}

	//得られたベクトル場にそって，経路を作成する
	if( lisnum == -1 ) {
		//prevノードをたどっていき，コマンドを確定させる
		nownode = encoder( 0, 0, NORTH, 0 );
		routenum = 0;
		while( prev[nownode] != SNODE ) {
			decoder( nownode, &nowx, &nowy, &nowd, &nowdia );
			//printf("%2d, %2d, %2d, %2d\r\n", nowx, nowy, nowd, nowdia);
			switch( nowd ) {
				case 0:dx=+1;dy= 0;break;
				case 1:dx= 0;dy=+1;break;
				case 2:dx=-1;dy= 0;break;
				case 3:dx= 0;dy=-1;break;
			}
			if( GO1 <= prev[nownode] && prev[nownode] <= GO31 ) {
				nextnode = encoder(nowx+dx*prev[nownode], nowy+dy*prev[nownode], nowd, 0);
			} else;
			if( prev[nownode] == DIA_TO_CLOTHOIDR )
				nextnode = encoder(nowx+dx, nowy+dy, nowd, 2);
			if( prev[nownode] == DIA_TO_CLOTHOIDL )
				nextnode = encoder(nowx+dx, nowy+dy, nowd, 1);
			if( prev[nownode] == DIA_FROM_CLOTHOIDR )
				nextnode = encoder(nowx+dy, nowy-dx, (nowd+3)&3, 0);
			if( prev[nownode] == DIA_FROM_CLOTHOIDL )
				nextnode = encoder(nowx-dy, nowy+dx, (nowd+1)&3, 0);
			if( prev[nownode] == DIA_TURNR )
				nextnode = encoder(nowx+dy, nowy-dx, (nowd+3)&3, 2);
			if( prev[nownode] == DIA_TURNL )
 				nextnode = encoder(nowx-dy, nowy+dx, (nowd+1)&3, 1);
			if( prev[nownode] == SNODE )
				nextnode = nownode;
			if( prev[nownode] == 0 )
				return 0;//経路計算が正しく終わらなかった
			//ここから先はnowx,nowy,nowd,nowdiaの内容が壊れても問題ない．
			if( DIA_GO1 <= prev[nownode] && prev[nownode] <= DIA_GO63 ) {
				diadistance= (prev[nownode]-DIA_GO1+2)/2;
				if( nowdia == 2 ) {//右向き斜め走行
					switch( nowd ) {
						case 0:nowx+=diadistance;nowy-=diadistance;break;
						case 1:nowx+=diadistance;nowy+=diadistance;break;
						case 2:nowx-=diadistance;nowy+=diadistance;break;
						case 3:nowx-=diadistance;nowy-=diadistance;break;
					}
				} else {//左向き
					switch( nowd ) {
						case 0:nowx+=diadistance;nowy+=diadistance;break;
						case 1:nowx-=diadistance;nowy+=diadistance;break;
						case 2:nowx-=diadistance;nowy-=diadistance;break;
						case 3:nowx+=diadistance;nowy-=diadistance;break;
					}
				}
				if( (prev[nownode]-DIA_GO1+1)%2 ) {//奇数マスの斜め走行なら
					nowx-=dx;
					nowy-=dy;
					nowd =(nowd+nowdia*2-1) & 0x03;//右傾きなら右方向へ，左傾きなら左方向へnowdを回転
					nowdia = 3-nowdia;//向き反転
				} else;//偶数方向はnowd,nowdiaに変更の必要なし
				nextnode = encoder(nowx, nowy, nowd, nowdia);
			} else;
			route[routenum] = prev[nownode];
			routenum++;
			nownode = nextnode;
		}
		route[routenum] = SNODE;
	} else {//経路がみつからなかった
		return 0;
	}
	return 1;//正常に経路が求まった
}

void Dijkstra_DebugPrintf( int8_t gx, int8_t gy )
{
	uint16_t route[255];
	uint16_t i;
	uint16_t t = 0;

	if( dijkstra(gx, gy, route) ) {
		printf("以下の経路が見つかりました\r\n");
		for(i = 0;route[i]!=SNODE;i++){
			printf("%3d %3d, ", i, route[i]);
			if(route[i] == 0)
				printf("Error\r\n");
			if(GO1<=route[i] && route[i]<=GO31)
				printf("%dマス直進\r\n",route[i]);
			if(DIA_GO1<=route[i] && route[i]<=DIA_GO63)
				printf("%dマス斜め直進\r\n",route[i]-DIA_GO1+1);
			if(route[i]==TURNR)
				printf("右９０度方向へスラローム\r\n");
			if(route[i]==TURNL)
				printf("左９０度方向へスラローム\r\n");
			if(route[i]==DIA_TO_CLOTHOIDR)
				printf("直進から１マス使って斜め右方向へ\r\n");
			if(route[i]==DIA_TO_CLOTHOIDL)
				printf("直進から１マス使って斜め左方向へ\r\n");
			if(route[i]==DIA_FROM_CLOTHOIDR)
				printf("斜め右方向から直進へ\r\n");
			if(route[i]==DIA_FROM_CLOTHOIDL)
				printf("斜め左方向から直進へ\r\n");
			if(route[i]==DIA_TURNR)
				printf("斜めから右９０度方向ターンして斜めへ\r\n");
			if(route[i]==DIA_TURNL)
				printf("斜めから左９０度方向ターンして斜めへ\r\n");
			if(route[i]==SNODE)
				printf("おわり\r\n");
			t += runtime[route[i]];
		}
		printf("%d単位時間でゴールに到達します\r\n\n",t);
	} else {
		printf("軌道が見つからなかった\r\n\n");
	}
}