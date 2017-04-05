
----------------------------------------------------------------------------------------------------

USE LobbyGameUserDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_LogonByUserID]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_LogonByUserID]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

CREATE PROC GSP_GP_LogonByUserID
	@dwUserID INT,
	@strPassword VARCHAR(32),
	@strClientIP VARCHAR(15)
WITH ENCRYPTION AS

SET NOCOUNT ON

-- 变量定义
DECLARE @UserID INT
DECLARE @Accounts VARCHAR(32)
DECLARE @LogonPass CHAR(32)
DECLARE @Gender BIT
DECLARE @LogonNullity BIT
DECLARE @UserRight INT
DECLARE @MasterRight INT
DECLARE @FaceID INT
DECLARE @GroupID INT
DECLARE @MemberOrder INT
DECLARE @MemberOverDate DATETIME
DECLARE @Experience INT
DECLARE @LastLogonDate DATETIME
DECLARE @LastLogonIP VARCHAR(15)
DECLARE @ErrorDescribe VARCHAR(256)

-- 社团变量
DECLARE @GroupName VARCHAR(32)

-- 执行逻辑
BEGIN

	-- 效验地址

	-- 查询用户
	SELECT @UserID=UserID, @Accounts=Accounts, @LogonPass=LogonPass, @Gender=Gender, @LogonNullity=LogonNullity, 
	@UserRight=UserRight, @MasterRight=MasterRight, @FaceID=FaceID, @GroupID=GroupID, @MemberOrder=MemberOrder, 
	@MemberOverDate=MemberOverDate, @Experience=Experience, @LastLogonDate=LastLogonDate, @LastLogonIP=LastLogonIP
	FROM UserAccounts WHERE UserID=@dwUserID
	
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
	
	-- 判断会员等级
	IF GETDATE()>@MemberOverDate SET @MemberOrder=0
	
	-- 获取社团信息
	SET @GroupName=''

	-- 更新数据库
	UPDATE UserAccounts SET AllLogonTimes=AllLogonTimes+1,LastLogonDate=GETDATE(),LastLogonIP=@strClientIP
	WHERE UserID=@UserID
	
	-- 输出变量
	SELECT @UserID AS UserID, @Accounts AS Accounts, @FaceID AS FaceID, @Gender AS Gender, @UserRight AS UserRight, 
	@MasterRight AS MasterRight, @GroupID AS GroupID, @GroupName AS GroupName,@MemberOrder AS MemberOrder, 
	@Experience AS Experience, @LastLogonDate AS LastLogonDate, @LastLogonIP AS LastLogonIP
	
END

RETURN 0

----------------------------------------------------------------------------------------------------

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

