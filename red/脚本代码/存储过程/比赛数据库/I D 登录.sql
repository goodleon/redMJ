
----------------------------------------------------------------------------------------------------

USE WHGameMatchDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_LogonByUserID]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_LogonByUserID]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

CREATE PROC GSP_GR_LogonByUserID
	@dwUserID INT,
	@strPassword CHAR(32),
	@wKindID INT,
	@wServerID INT,
	@strClientIP VARCHAR(15)

	-- 加密脚本
	WITH ENCRYPTION 

AS

SET NOCOUNT ON

-- 用户变量
DECLARE @UserID INT
DECLARE @Accounts VARCHAR(32)
DECLARE @LogonPass VARCHAR(32)
DECLARE @Gender BIT
DECLARE @LogonNullity BIT
DECLARE @UserRight INT
DECLARE @MasterRight INT
DECLARE @FaceID INT
DECLARE @GroupID INT
DECLARE @MemberOrder INT
DECLARE @MemberOverDate datetime
DECLARE @Experience INT
DECLARE @ErrorDescribe VARCHAR(128)

-- 社团变量
DECLARE @GroupName VARCHAR(32)

-- 游戏变量
DECLARE @Score INT
DECLARE @WinCount INT
DECLARE @LostCount INT
DECLARE @DrawCount INT
DECLARE @FleeCount INT
DECLARE @GameUserRight INT
DECLARE @GameMasterRight INT

-- 执行逻辑
BEGIN

	-- 效验地址

	-- 查询用户
	SELECT @UserID=UserID, @Accounts=Accounts, @LogonPass=LogonPass, @Gender=Gender, @LogonNullity=LogonNullity, 
		@UserRight=UserRight, @MasterRight=MasterRight, @FaceID=FaceID, @GroupID=GroupID,@MemberOrder=MemberOrder, 
		@MemberOverDate=MemberOverDate, @Experience=Experience 
	FROM WHGameUserDBServer.WHGameUserDB.dbo.UserAccounts WHERE UserID=@dwUserID
	
	-- 判断用户是否存在
	IF @UserID IS NULL
	BEGIN
		SET @ErrorDescribe='帐号不存在或者密码输入有误，请查证后再次尝试登录！' 
		SELECT [ErrorDescribe]=@ErrorDescribe
		RETURN 1
	END
	
	-- 判断是否禁止登陆
	IF @LogonNullity<>0
	BEGIN
		SET @ErrorDescribe='你的帐户暂时处于冻结状态，请联系客户服务中心了解详细情况！'
		SELECT [ErrorDescribe]=@ErrorDescribe
		RETURN 2
	END

	-- 判断用户密码
	IF @LogonPass<>@strPassword
	BEGIN
		SET @ErrorDescribe='帐号不存在或者密码输入有误，请查证后再次尝试登录！'
		SELECT [ErrorDescribe]=@ErrorDescribe
		RETURN 3
	END

	-- 房间锁定
	IF EXISTS (SELECT UserID FROM GameLocker WHERE UserID=@dwUserID)
	BEGIN
		SET @ErrorDescribe='你已经在比赛游戏房间了，不能同时在进入此游戏房间了！'
		SELECT [ErrorDescribe]=@ErrorDescribe
		RETURN 2
	END
	INSERT GameLocker (UserID,KindID,ServerID) VALUES (@dwUserID,@wKindID,@wServerID)

	-- 读取游戏信息
	SELECT @Score=Score, @WinCount=WinCount, @LostCount=LostCount, @DrawCount=DrawCount, @DrawCount=DrawCount, @FleeCount=FleeCount, 
		   @GameUserRight=UserRight, @GameMasterRight=MasterRight FROM GameScore WHERE UserID=@dwUserID
	IF @Score IS NULL
	BEGIN
		-- 插入游戏信息
		INSERT INTO GameScore (UserID, LastLogonIP, RegisterIP)	VALUES (@dwUserID,@strClientIP,@strClientIP)

		-- 读取游戏信息
		SELECT @Score=Score, @WinCount=WinCount, @LostCount=LostCount, @DrawCount=DrawCount, @DrawCount=DrawCount, @FleeCount=FleeCount, 
			   @GameUserRight=UserRight, @GameMasterRight=MasterRight FROM GameScore WHERE UserID=@dwUserID
	END

	-- 获取社团信息
	SET @GroupName=''

	-- 更新数据库
	UPDATE GameScore SET AllLogonTimes=AllLogonTimes+1, LastLogonDate=Getdate(), LastLogonIP=@strClientIP
	WHERE UserID=@dwUserID

	-- 写入记录
	INSERT GameLogonLog (UserID, Score, WinCount, LostCount, DrawCount, FleeCount, ClientIP, KindID, ServerID) 
	VALUES (@UserID, @Score, @WinCount, @LostCount, @DrawCount, @FleeCount, @strClientIP, @wKindID, @wServerID)
	
	-- 比赛设置
	SET @GameUserRight=268435456

	-- 输出变量
	SELECT @UserID AS UserID, @Accounts AS Accounts, @FaceID AS FaceID, @Gender AS Gender, 
		@UserRight|@GameUserRight AS UserRight, @MasterRight|@GameMasterRight AS MasterRight, @GroupID AS GroupID, 
		@GroupName AS GroupName, @MemberOrder AS MemberOrder, @Experience AS Experience, @Score AS Score,
		@WinCount AS WinCount, @LostCount AS LostCount, @DrawCount AS DrawCount, @FleeCount AS FleeCount 
	
END

RETURN 0

----------------------------------------------------------------------------------------------------

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

