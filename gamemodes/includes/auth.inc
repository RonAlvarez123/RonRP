forward OnPlayerAccountLoaded(playerid, race_check);
public OnPlayerAccountLoaded(playerid, race_check) {
	Logger_Log("Dumating sa OnPlayerAccountLoaded");
	if (race_check != g_MysqlRaceCheck[playerid]) return Kick(playerid);

	orm_setkey(Player[playerid][ORM_ID], "id");
	 
	new string[115];
	switch(orm_errno(Player[playerid][ORM_ID])) {
		case ERROR_OK: {
			format(string, sizeof string, "Your username %s is registered.\n  Please login by entering your password.", Player[playerid][Username]);
			ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login", string, "Login", "Abort");
		}
		case ERROR_NO_DATA: {
			format(string, sizeof string, "Welcome %s, you can register\n\tby entering a password:", Player[playerid][Username]);
			ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Registration", string, "Register", "Abort");
		}
	}

	return 1;
}

forward OnPlayerRegister(playerid);
public OnPlayerRegister(playerid) {
	ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Registration", "Account successfully registered, you have been automatically logged in.", "Okay", "");
	Player[playerid][IsLoggedIn] = true;

	Player[playerid][X_Pos] = DEFAULT_POS_X;
	Player[playerid][Y_Pos] = DEFAULT_POS_Y;
	Player[playerid][Z_Pos] = DEFAULT_POS_Z;
	Player[playerid][Angle] = DEFAULT_ANGLE;

	SetSpawnInfo(playerid, NO_TEAM, 299, Player[playerid][X_Pos], Player[playerid][Y_Pos], Player[playerid][Z_Pos], Player[playerid][Angle], 0, 0, 0, 0, 0, 0);
	SpawnPlayer(playerid);
	return 1;
}

forward OnPasswordHashed(playerid);
public OnPasswordHashed(playerid) {
	bcrypt_get_hash(Player[playerid][Password]);
	Logger_Log("OnPasswordHashed",Logger_S("Password hashed", Player[playerid][Password]));

	orm_save(Player[playerid][ORM_ID], "OnPlayerRegister", "d", playerid);
	Logger_Log("Registered na tpos na bcryt na din", Logger_S("Password na nahash", Player[playerid][Password]));
	return 1;
}

forward OnPasswordChecked(playerid);
public OnPasswordChecked(playerid) {
	new bool: match = bcrypt_is_equal();

	if(match) {
		Logger_Log("Password Match");
		SendClientMessage(playerid, -1, "[SERVER] You are now logged");

		Player[playerid][IsLoggedIn] = true;

		SetSpawnInfo(playerid, NO_TEAM, 299, Player[playerid][X_Pos], Player[playerid][Y_Pos], Player[playerid][Z_Pos], Player[playerid][Angle], 0, 0, 0, 0, 0, 0);
		SpawnPlayer(playerid);
	} else {
		Player[playerid][LoginAttempts]++;

		if(Player[playerid][LoginAttempts] >= MINIMUM_LOGIN_ATTEMPTS) {
			ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Login", "You have reached your maximum attempts limit.\n\tYou will now be automatically kicked.", "Okay", "");
			// KICK PLAYER HERE	
		} else {
			ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login", "Password is incorrect. Please enter your password:", "Login", "Abort");
		}
		Logger_Log("Did NOT Match");
	}

	return 1;
}

forward UpdatePlayerData(playerid, reason);
public UpdatePlayerData(playerid, reason) {
	if(Player[playerid][IsLoggedIn] == false) {
		return 0;
	}

	if(reason == 1) {
		GetPlayerPos(playerid, Player[playerid][X_Pos], Player[playerid][Y_Pos], Player[playerid][Z_Pos]);
		GetPlayerFacingAngle(playerid, Player[playerid][Angle]);
		Player[playerid][Cash] = GetPlayerMoney(playerid);
	}

	Player[playerid][Interior] = GetPlayerInterior(playerid);

	orm_save(Player[playerid][ORM_ID]);
	orm_destroy(Player[playerid][ORM_ID]);
	return 1;
}
