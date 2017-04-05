//-------------------------------------------------------------------------------------------------------
//
//							Wave.h 声音文件事件定义
//


//----------------------------------公用事件-------------------------------------------------------------
//有玩家坐下
#define WAV_SITDOWN			1

//玩家太慢
#define WAV_TOOSLOWLY		2

//游戏开始
#define WAV_START			3

//游戏结束
#define WAV_END				4

//赢
#define WAV_WIN				5

//输
#define WAV_LOST			6

//----------------------------------诈金花--------------------------------------------------------------
#define WAV_GOLDFLOWER_BEGIN 1000

//下注
#define WAV_GOLDFLOWER_DO_GOLD			WAV_GOLDFLOWER_BEGIN+5

//跟注
#define WAV_GOLDFLOWER_FOLLOW			WAV_GOLDFLOWER_BEGIN+6

//加注
#define WAV_GOLDFLOWER_ADD_GOLD			WAV_GOLDFLOWER_BEGIN+7

//放弃
#define WAV_GOLDFLOWER_GIVEUP			WAV_GOLDFLOWER_BEGIN+8

//比牌
#define WAV_GOLDFLOWER_BIPAI			WAV_GOLDFLOWER_BEGIN+9

//----------------------------------梭哈----------------------------------------------------------------
#define WAV_SHOWHAND_BEGIN 2000

//下注
#define WAV_SHOWHAND_DO_GOLD			WAV_GOLDFLOWER_DO_GOLD

//跟注
#define WAV_SHOWHAND_FOLLOW				WAV_GOLDFLOWER_FOLLOW

//加注
#define WAV_SHOWHAND_ADD_GOLD			WAV_GOLDFLOWER_ADD_GOLD

//放弃
#define WAV_SHOWHAND_GIVEUP				WAV_GOLDFLOWER_GIVEUP

//比牌
#define WAV_SHOWHAND_BIPAI				WAV_GOLDFLOWER_BIPAI

//梭哈
#define WAV_SHOWHAND_SHOWHAND			WAV_SHOWHAND_BEGIN+1
//------------------------------------------------------------------------------------------------------


//----------------------------------斗地主----------------------------------------------------------------
#define WAV_LAND 3000

//出牌
#define WAV_LAND_OUTCARD				WAV_LAND+100        //三带一 三带二
#define WAV_LAND_OUTCARD_SINGE			WAV_LAND_OUTCARD+1  //单顺
#define WAV_LAND_OUTCARD_DOUBLE			WAV_LAND_OUTCARD+2  //双顺
#define WAV_LAND_OUTCARD_GIVEUP			WAV_LAND_OUTCARD+3  //不要
#define WAV_LAND_OUTCARD_TEAM_BEHIND	WAV_LAND_OUTCARD+4  //同伴出牌后
#define WAV_LAND_OUTCARD_TEAM_ON		WAV_LAND_OUTCARD+5  //同伴出牌时
#define WAV_LAND_OUTCARD_DS_BEHIND		WAV_LAND_OUTCARD+6  //对手出牌后
#define WAV_LAND_OUTCARD_DS_WIN			WAV_LAND_OUTCARD+7  //对手赢

//叫牌
#define WAV_LAND_CALLSCORE				WAV_LAND+200		 //其他
#define WAV_LAND_CALLSCORE_BANKER		WAV_LAND_CALLSCORE+1 //当地主

//结束
#define WAV_LAND_END					WAV_LAND+300

//赢牌
#define WAV_LAND_WIN					WAV_LAND+400

//------------------------------------------------------------------------------------------------------














