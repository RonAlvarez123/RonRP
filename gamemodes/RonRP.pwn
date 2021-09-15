#include <a_samp>
#include <a_mysql>
// #include <fixes>
#include <izcmd>
#include <sscanf2>
#include <logger>
#include <bcrypt>
#include <chrono>
#include "includes/global_variables"

new MySQL:conn;

new Account[MAX_PLAYERS][accounts];
new g_MysqlRaceCheck[MAX_PLAYERS];
new Player[MAX_PLAYERS][players];

new ServerVehicle[MAX_VEHICLES][vehicles];
new PlayerVehicle[MAX_PLAYERS][MAX_VEHICLES_PER_PLAYER][vehicles];
new ServerTotalVehicles;

#include "includes/db_setup"
#include "includes/auth"
#include "includes/dialog"
#include "includes/vehicle_utilities"
#include "includes/vehicle_methods"
#include "includes/commands"

main()
{
	print("\n----------------------------------");
	print(" Blank Gamemode by your name here");
	print("----------------------------------\n");

    // new vehicleNameKo[MAX_PLAYER_NAME];
    // getVehicleName(523, vehicleNameKo);
    // printf("Name of vehicle is %s", vehicleNameKo);

	// new string[128];
	// format(string, sizeof string, " ");
	// new lengthofstring = strlen(string);
	// printf("String length is %d", lengthofstring);
}

public OnGameModeInit()
{
	SetGameModeText("Blank Script");
    ManualVehicleEngineAndLights();
	ShowPlayerMarkers(0);

    new MySQLOpt: option_id = mysql_init_options();
	mysql_set_option(option_id, AUTO_RECONNECT, true);
    conn = mysql_connect(MYSQL_HOSTNAME, MYSQL_USERNAME, MYSQL_PASSWORD, MYSQL_DATABASE, option_id);

    if (conn == MYSQL_INVALID_HANDLE || mysql_errno(conn) != 0) {
		print("MySQL connection failed. Server is shutting down.");
		SendRconCommand("exit"); // close the server if there is no connection
		return 1;
	}

	print("MySQL connection is successful.");

    DBSetupAccountsTable(conn);
    DBSetupPlayersTable(conn);
    DBSetupVehiclesTable(conn);

    getAllServerVehicle(conn);

	// AddPlayerClass(299,1805.2272,-1904.5685,13.4001,92.8548,0,0,0,0,0,0); // objectposition
	CreatePickup(1239, 1, 1805.2272, -1904.5685, 13.4001, 0);
	Create3DTextLabel("/getmats", 0xFF0000FF, 1805.2272, -1904.5685, 13.4001, 10, 0, 0);
	return 1;
}

public OnGameModeExit()
{
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	return 1;
}

public OnPlayerConnect(playerid)
{
	g_MysqlRaceCheck[playerid]++;
	
	// reset player data
	static const empty_account[accounts];
	Account[playerid] = empty_account;
	static const empty_player[players];
	Player[playerid] = empty_player;

    new string[MAX_PLAYER_NAME];
    GetPlayerName(playerid, string, sizeof string);
    Account[playerid][Username] = string;

	new ORM: AccountModel = Account[playerid][ORM_ID] = orm_create("accounts", conn);
	new ORM: PlayerModel = Player[playerid][ORM_ID] = orm_create("players", conn);

    ORMSetupAccountsTable(AccountModel, playerid);
    ORMSetupPlayersTable(PlayerModel, playerid);

	orm_setkey(AccountModel, "username");
    orm_load(AccountModel, "OnPlayerAccountLoaded", "dd", playerid, g_MysqlRaceCheck[playerid]);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    g_MysqlRaceCheck[playerid]++;

	UpdatePlayerData(playerid, reason);
	Account[playerid][IsLoggedIn] = false;

	despawnAllPlayerVehicles(playerid);
	return 1;
}

public OnPlayerSpawn(playerid)
{
    if(Account[playerid][Respawned] == false) {
		Account[playerid][Respawned] = true;
		GivePlayerMoney(playerid, Account[playerid][Cash]);
		getAllPlayerVehData(conn, playerid);
	}
	SetPlayerColor(playerid, COLOR_WHITE);

	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	return 1;
}

public OnPlayerText(playerid, text[])
{
	return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	return 1;
}

public OnRconCommand(cmd[])
{
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	return 1;
}

public OnObjectMoved(objectid)
{
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	new string[128];
	format(string, sizeof string, "You have pickuped a pickup with id = %d", pickupid);
	SendClientMessage(playerid, -1, string);
	return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	return 1;
}

public OnPlayerUpdate(playerid)
{
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
		case DIALOG_UNUSED: return 1;

		case DIALOG_CHANGENAME: {
			if(response) {
				SetPlayerName(playerid, inputtext);
				return 1;
			} else {
				return SendClientMessage(playerid, -1, "You have cancelled the changename procedure");
			}
		}

		case DIALOG_LOGIN: {
			if(!response) return Kick(playerid);

			// bcrypt_hash("admin123", BCRYPT_COST, "OnPasswordHashed");
			// Logger_Log("DIALOG_LOGIN", Logger_S("Inputtext", inputtext));
			bcrypt_check(inputtext, Account[playerid][Password], "OnPasswordChecked", "d", playerid);
			return 1;
		}

		case DIALOG_REGISTER: {
			if(!response) return Kick(playerid);

			if(strlen(inputtext) <= MINIMUM_PASSWORD_LENGTH) {
				return ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Registration", "Your password must be longer than %d characters.\nPlease enter your password:", "Register", "Abort");
			}

			bcrypt_hash(inputtext, BCRYPT_COST, "OnPasswordHashed", "d", playerid);
		}

		case DIALOG_VEHICLE_STORAGE: {
			if(!response) {
				return 1;
			}

			new string[128];

			if(PlayerVehicle[playerid][listitem][isSpawned]) {
				DestroyVehicle(PlayerVehicle[playerid][listitem][session_ID]);
				PlayerVehicle[playerid][listitem][isSpawned] = false;
				Account[playerid][SpawnedVehicles]--;
				format(string, sizeof string, "You have stored your %s.", PlayerVehicle[playerid][listitem][Name]);
				SendClientMessage(playerid, DEFAULT_SERVER_MESSAGE_COLOR, string);
			} else {
				new vehicleid;
				vehicleid = CreateVehicle(PlayerVehicle[playerid][listitem][Model], PlayerVehicle[playerid][listitem][X_Pos], PlayerVehicle[playerid][listitem][Y_Pos], PlayerVehicle[playerid][listitem][Z_Pos], PlayerVehicle[playerid][listitem][Angle], strval(PlayerVehicle[playerid][listitem][Color1]), strval(PlayerVehicle[playerid][listitem][Color2]), -1, 0);

				PlayerVehicle[playerid][listitem][session_ID] = vehicleid;
				PlayerVehicle[playerid][listitem][isSpawned] = true;
				Account[playerid][SpawnedVehicles]++;

				format(string, sizeof string, "You have taken your %s out of your vstorage.", PlayerVehicle[playerid][listitem][Name]);
				SendClientMessage(playerid, DEFAULT_SERVER_MESSAGE_COLOR, string);
			}
		}

		default: return 0;
	}
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	return 1;
}