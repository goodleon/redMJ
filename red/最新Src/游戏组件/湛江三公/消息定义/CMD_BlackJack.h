#ifndef CMD_BLACKJACK_FILE
#define CMD_BLACKJACK_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

//#define KIND_ID							ID_BACK_JACK						//游戏 I D
#define KIND_ID							29									//游戏 I D
#define GAME_PLAYER						6									//游戏人数
#define GAME_NAME						TEXT("湛江三公")						//游戏名字
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//游戏类型

//版本定义
#define		MAXCLIENTVER			    MAKELONG(0,4)
#define		LESSCLIENTVER			    0
 
//游戏状态定义
#define GS_FREE  						0									//空闲状态
#define	GS_PLAYING				        100									//游戏状态
#define GS_ADDGOLDING   				101									//加注状态

//结束原因
#define GO_NORMAL_FINISH			    0x10								//正常结束

//玩家状态
#define NO_CARD					        0									//无牌状态
#define INIT_CARD						1									//初始状态

//扑克张数
#define CARD_COUNT						3									//扑克张数
//////////////////////////////////////////////////////////////////////////
//服务器命令结构

//命令码定义

#define SUB_S_GAME_START				100									//游戏开始
#define SUB_S_ADD_GOLD					101									//下注结果
#define SUB_S_INIT_CARD					102									//初始发牌
#define SUB_S_USER_LEFT     	    	110									//闲家强退
#define SUB_S_GAME_END					111									//游戏结束

//消息结构体

//空闲状态
struct CMD_S_StatusFree
{
	LONG								dwBasicGold;						//基础金币
};

//加注状态
struct CMD_S_StatusAddGold
{
	WORD				 				wCurrentUser;						//当前玩家
	WORD						        wBankerStation;				    	//庄家位置
	BOOL						        bAddGoldEnd[GAME_PLAYER];	    	//加注结束
	LONG								dwBasicGold;						//基础金币
	LONG								lMaxGold;							//最大下注
	LONG								lBasicGold;							//基础金币
	LONG								lTableGold[2*GAME_PLAYER];			//桌面金币
};

//游戏状态
struct CMD_S_StatusPlay
{
	WORD				 				wCurrentUser;						//当前玩家
	WORD						        wBankerStation;				    	//庄家位置
	BYTE								cbPlayStatus[GAME_PLAYER];			//游戏状态
	BYTE								cbTableCardCount[GAME_PLAYER];		//扑克数目
	BYTE								cbTableCardArray[GAME_PLAYER][13];	//扑克数组
	LONG								lMaxGold;							//最大下注
	LONG								lBasicGold;							//基础金币
	LONG								lTableGold[2*GAME_PLAYER];			//桌面金币
};

//游戏开始
struct CMD_S_GameStart
{	
	WORD						        wBankerStation;				    	//庄家位置
	WORD				 				wCurrentUser;						//当前玩家
	LONG								lMaxGold;							//最大下注
	LONG								lTurnBasicGold;						//最少下注
	BYTE								cbCardData[GAME_PLAYER];			//用户扑克
};

//下注结果
struct CMD_S_AddGold
{
	WORD								wCurrentUser;						//当前用户
	WORD								wPrevUser;							//上一用户
	LONG								lLastAddGold;						//下注数目
	LONG								lMaxGold;							//最大下注
};

//初始发牌
struct CMD_S_InitCard
{	
	BYTE								cbCardData[GAME_PLAYER][CARD_COUNT];//用户扑克
	BYTE								cbCardSum;	            			//扑克总值
	WORD								wDiceCount;							//骰子点数
};

//用户强退
struct CMD_S_UserLeft
{
	WORD								wUserChairID;						//强退用户
	BOOL								bAddGoldEnd;						//加注结束
};

//游戏结束
struct CMD_S_GameEnd
{
	LONG								lTax;								//游戏税收
	LONG								lGameGold[GAME_PLAYER];				//游戏得分
	BYTE								cbUserCard[GAME_PLAYER];			//用户扑克
	BYTE								cbCardSum[GAME_PLAYER];	   	    	//扑克总值
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

//命令码定义
#define SUB_C_ADD_GOLD					1									//用户下注
#define SUB_C_CANCEL_GOLD				2   								//取消下注

//消息结构体

//用户下注
struct CMD_C_AddGold
{
	LONG								lGold;								//下注数目
	WORD								wChairID;							//下注玩家
};

//////////////////////////////////////////////////////////////////////////

#endif