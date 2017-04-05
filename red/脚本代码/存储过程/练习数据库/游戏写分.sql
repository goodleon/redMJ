
----------------------------------------------------------------------------------------------------

USE WHEducateDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_WriteGameScore]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_WriteGameScore]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

CREATE PROCEDURE GSP_GR_WriteGameScore
  	@dwUserID INT,
  	@lScore INT,
  	@lWinCount INT,
  	@lLostCount INT,
  	@lDrawCount INT,
  	@lFleeCount INT,
  	@lExperience INT,
  	@dwPlayTimeCount INT,
	@dwOnlineTimeCount INT,
	@wKindID INT,
	@wServerID INT,
	@strClientIP VARCHAR(15)
WITH ENCRYPTION AS

SET NOCOUNT ON

BEGIN

	-- 写入积分
	BEGIN
		UPDATE GameScore SET Score=Score+@lScore,WinCount=WinCount+@lWinCount, LostCount=LostCount+@lLostCount,
			DrawCount=DrawCount+@lDrawCount, FleeCount=FleeCount+@lFleeCount, PlayTimeCount=@dwPlayTimeCount
		WHERE UserID=@dwUserID
	END
	
	-- 更新经验值
	UPDATE WHGameUserDBServer.WHGameUserDB.dbo.UserAccounts SET Experience=Experience+@lExperience WHERE UserID=@dwUserID

	-- 写入记录
	INSERT GameScoreLog (UserID,Score,WinCount,LostCount,DrawCount,FleeCount,Experience,PlayTimeCount,OnLineTimeCount,ClientIP,KindID,ServerID) 
	VALUES (@dwUserID,@lScore,@lWinCount,@lLostCount,@lDrawCount,@lFleeCount,@lExperience,@dwPlayTimeCount,0,@strClientIP,@wKindID,@wServerID)

END

RETURN 0
----------------------------------------------------------------------------------------------------

GO

SET QUOTED_IDENTIFIER OFF 
GO

SET ANSI_NULLS ON 
GO

