
----------------------------------------------------------------------------------------------------

USE WHServerInfoDB
GO

----------------------------------------------------------------------------------------------------

-- 删除数据
DELETE GameTypeInfo
DELETE GameKindInfo
GO

----------------------------------------------------------------------------------------------------

-- 类型数据
INSERT GameTypeInfo (TypeID, TypeName, SortID, Enable) VALUES ( 1, '棋类' , 100, 1)
INSERT GameTypeInfo (TypeID, TypeName, SortID, Enable) VALUES ( 2, '扑克类',200, 1)
INSERT GameTypeInfo (TypeID, TypeName, SortID, Enable) VALUES ( 3, '麻将类',300, 1)
INSERT GameTypeInfo (TypeID, TypeName, SortID, Enable) VALUES ( 4, '休闲类',400, 1)

----------------------------------------------------------------------------------------------------

-- 通用游戏
INSERT GameKindInfo (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Enable, databaseName) VALUES ( 1, 2, '梭哈', 'ShowHand.exe', 65542, 100, 1, 'WHTreasureDB')
INSERT GameKindInfo (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Enable, databaseName) VALUES ( 2, 1, '中国象棋', 'ChinaChess.exe', 65542, 100, 1, 'WHTreasureDB')

INSERT GameKindInfo (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Enable, databaseName) VALUES ( 3, 1, '围棋', 'WeiQi.exe', 65542, 100, 1, 'WHTreasureDB')
INSERT GameKindInfo (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Enable, databaseName) VALUES ( 4, 2, '挖坑', 'SX_Trench.exe', 65542, 100, 1, 'WHTreasureDB')
INSERT GameKindInfo (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Enable, databaseName) VALUES ( 5, 1, '国际象棋', 'Chess.exe', 65542, 100, 1, 'WHTreasureDB')
INSERT GameKindInfo (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Enable, databaseName) VALUES ( 6, 2, '斗地主', 'Land.exe', 65542, 100, 1, 'WHTreasureDB')
INSERT GameKindInfo (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Enable, databaseName) VALUES ( 7, 3, '麻将', 'Sparrow.exe', 65542, 100, 1, 'WHTreasureDB')
INSERT GameKindInfo (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Enable, databaseName) VALUES ( 8, 2, '跑得快', 'QuickRun.exe', 65542, 100, 1, 'WHTreasureDB')

-- 测试游戏
INSERT GameKindInfo (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Enable, databaseName) VALUES ( 9, 2, '锄大地', 'Hoe.exe', 65542, 100, 1, 'WHTreasureDB')
INSERT GameKindInfo (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Enable, databaseName) VALUES ( 10, 2, '黑杰克', 'BlackJack.exe', 65542, 100, 1, 'WHTreasureDB')
INSERT GameKindInfo (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Enable, databaseName) VALUES ( 11, 2, '升级', 'UpGrade.exe', 65542, 100, 1, 'WHTreasureDB')
INSERT GameKindInfo (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Enable, databaseName) VALUES ( 12, 1, '四国军棋', 'FourEnsign.exe', 65542, 100, 1, 'WHTreasureDB')
INSERT GameKindInfo (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Enable, databaseName) VALUES ( 13, 2, '诈金花', 'GoldFlower.exe', 65542, 100, 1, 'WHTreasureDB')

INSERT GameKindInfo (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Enable, databaseName) VALUES ( 14, 2, '拱猪', 'UncoverPig.exe', 65542, 100, 1, 'WHTreasureDB')
INSERT GameKindInfo (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Enable, databaseName) VALUES ( 15, 2, '跑牌', 'Fast.exe', 65542, 100, 1, 'WHTreasureDB')



----------------------------------------------------------------------------------------------------

GO