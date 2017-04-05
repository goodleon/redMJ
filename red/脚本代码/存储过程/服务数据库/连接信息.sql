
----------------------------------------------------------------------------------------------------

USE WHServerInfoDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_SL_GetdatabaseInfo]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_SL_GetdatabaseInfo]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

CREATE PROCEDURE GSP_SL_GetdatabaseInfo
	@strdatabaseAddr VARCHAR(15)
WITH ENCRYPTION AS

SET NOCOUNT ON

-- 变量定义
DECLARE @ID INT
DECLARE @databasePort INT
DECLARE @databaseAddr VARCHAR(15)
DECLARE @databaseUser VARCHAR(512)
DECLARE @databasePass VARCHAR(512)
DECLARE @ErrorDescribe VARCHAR(128)

-- 执行逻辑
BEGIN

	-- 查询数据库信息
	SELECT @ID=ID, @databaseAddr=databaseAddr, @databasePort=databasePort, @databaseUser=databaseUser, @databasePass=databasePass 
	FROM SQLdatabaseInfo 
	WHERE databaseAddr=@strdatabaseAddr
	
	-- 判断信息是否存在
	IF @ID IS NULL
	BEGIN
		SET @ErrorDescribe='相关的数据库连接信息不存在！'
		SELECT [ErrorDescribe]=@ErrorDescribe
		RETURN 1
	END
	
	-- 输出变量
	SELECT @ID AS ID, @databaseAddr AS databaseAddr, @databasePort AS databasePort, @databaseUser AS databaseUser, @databasePass AS databasePass 
	
END

RETURN 0

----------------------------------------------------------------------------------------------------

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
