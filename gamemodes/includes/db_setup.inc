forward DBSetupAccountsTable(MySQL: DBconnection);
public DBSetupAccountsTable(MySQL: DBconnection) {
    mysql_tquery(DBconnection, "CREATE TABLE IF NOT EXISTS `accounts` (`id` int(11) NOT NULL AUTO_INCREMENT, `username` varchar(24) NOT NULL, `password` varchar(64) NOT NULL, `cash` int(11) NOT NULL, `kills` int(11) NOT NULL, `death` int(11) NOT NULL, `x` float NOT NULL, `y` float NOT NULL, `z` float NOT NULL, `angle` float NOT NULL, `interior` int(11) NOT NULL, `created_at` timestamp NOT NULL DEFAULT current_timestamp(), PRIMARY KEY (`id`))");
	print("Accounts Table Setup Finished!");
    return 1;
}

forward DBSetupPlayersTable(MySQL: DBconnection);
public DBSetupPlayersTable(MySQL: DBconnection) {
    mysql_tquery(DBconnection, "CREATE TABLE IF NOT EXISTS `players` (`id` int(11) NOT NULL AUTO_INCREMENT, `account_id` int(11) NOT NULL, `job1` varchar(100) NOT NULL DEFAULT 'None', `job2` varchar(100) NOT NULL DEFAULT 'None', `materials` int(11) NOT NULL DEFAULT 0, PRIMARY KEY (`id`))");
    print("Players Table Setup Finished!");
    return 1;
}

forward DBSetupVehiclesTable(MySQL: DBconnection);
public DBSetupVehiclesTable(MySQL: DBconnection) {
    mysql_tquery(DBconnection, "CREATE TABLE IF NOT EXISTS `vehicles` (`id` int(11) NOT NULL AUTO_INCREMENT, `account_id` int(11) NOT NULL, `model` int(11) NOT NULL, `name` varchar(24) NOT NULL, `owner` varchar(24) DEFAULT NULL, `price` int(11) NOT NULL, `hasLock` tinyint(1) NOT NULL DEFAULT 0, `color1` int(11) NOT NULL DEFAULT 0, `color2` int(11) NOT NULL DEFAULT 0, `x` float NOT NULL, `y` float NOT NULL, `z` float NOT NULL, `angle` float NOT NULL, PRIMARY KEY (`id`))");
    print("Vehicles Table Setup Finished!");
    return 1;
}

forward ORMSetupAccountsTable(ORM: TableModel, playerid);
public ORMSetupAccountsTable(ORM: TableModel, playerid) {
    orm_addvar_int(TableModel, Account[playerid][ID], "id");
	orm_addvar_string(TableModel, Account[playerid][Username], MAX_PLAYER_NAME, "username");
	orm_addvar_string(TableModel, Account[playerid][Password], 65, "password");
	orm_addvar_int(TableModel, Account[playerid][Cash], "cash");
	orm_addvar_int(TableModel, Account[playerid][Kills], "kills");
	orm_addvar_int(TableModel, Account[playerid][Deaths], "death");
	orm_addvar_float(TableModel, Account[playerid][X_Pos], "x");
	orm_addvar_float(TableModel, Account[playerid][Y_Pos], "y");
	orm_addvar_float(TableModel, Account[playerid][Z_Pos], "z");
	orm_addvar_float(TableModel, Account[playerid][Angle], "angle");
	orm_addvar_int(TableModel, Account[playerid][Interior], "interior");
    return 1;
}

forward ORMSetupPlayersTable(ORM: TableModel, playerid);
public ORMSetupPlayersTable(ORM: TableModel, playerid) {
    orm_addvar_int(TableModel, Player[playerid][ID], "id");
    orm_addvar_int(TableModel, Player[playerid][Account_ID], "account_id");
	orm_addvar_string(TableModel, Player[playerid][Job1], 100, "job1");
	orm_addvar_string(TableModel, Player[playerid][Job2], 100, "job2");
	orm_addvar_int(TableModel, Player[playerid][Materials], "materials");
    return 1;
}

forward ORMSetupServerVehiclesTable(ORM: TableModel, i);
public ORMSetupServerVehiclesTable(ORM: TableModel, i) {
    orm_addvar_int(TableModel, ServerVehicle[i][ID], "id");
    orm_addvar_int(TableModel, ServerVehicle[i][Account_ID], "account_id");
    orm_addvar_int(TableModel, ServerVehicle[i][Model], "model");
	orm_addvar_string(TableModel, ServerVehicle[i][Name], MAX_PLAYER_NAME, "name");
	orm_addvar_string(TableModel, ServerVehicle[i][Owner], MAX_PLAYER_NAME, "owner");
	orm_addvar_int(TableModel, ServerVehicle[i][Price], "price");
	orm_addvar_int(TableModel, ServerVehicle[i][hasLock], "hasLock");
	orm_addvar_string(TableModel, ServerVehicle[i][Color1], MAX_PLAYER_NAME, "color1");
	orm_addvar_string(TableModel, ServerVehicle[i][Color2], MAX_PLAYER_NAME, "color2");
	orm_addvar_float(TableModel, ServerVehicle[i][X_Pos], "x");
	orm_addvar_float(TableModel, ServerVehicle[i][Y_Pos], "y");
	orm_addvar_float(TableModel, ServerVehicle[i][Z_Pos], "z");
	orm_addvar_float(TableModel, ServerVehicle[i][Angle], "angle");
    return 1;
}

forward ORMSetupPlayerVehiclesTable(ORM: TableModel, playerid, i);
public ORMSetupPlayerVehiclesTable(ORM: TableModel, playerid, i) {
    orm_addvar_int(TableModel, PlayerVehicle[playerid][i][ID], "id");
    orm_addvar_int(TableModel, PlayerVehicle[playerid][i][Account_ID], "account_id");
    orm_addvar_int(TableModel, PlayerVehicle[playerid][i][Model], "model");
	orm_addvar_string(TableModel, PlayerVehicle[playerid][i][Name], MAX_PLAYER_NAME, "name");
	orm_addvar_string(TableModel, PlayerVehicle[playerid][i][Owner], MAX_PLAYER_NAME, "owner");
	orm_addvar_int(TableModel, PlayerVehicle[playerid][i][Price], "price");
	orm_addvar_int(TableModel, PlayerVehicle[playerid][i][hasLock], "hasLock");
	orm_addvar_string(TableModel, PlayerVehicle[playerid][i][Color1], MAX_PLAYER_NAME, "color1");
	orm_addvar_string(TableModel, PlayerVehicle[playerid][i][Color2], MAX_PLAYER_NAME, "color2");
	orm_addvar_float(TableModel, PlayerVehicle[playerid][i][X_Pos], "x");
	orm_addvar_float(TableModel, PlayerVehicle[playerid][i][Y_Pos], "y");
	orm_addvar_float(TableModel, PlayerVehicle[playerid][i][Z_Pos], "z");
	orm_addvar_float(TableModel, PlayerVehicle[playerid][i][Angle], "angle");
    return 1;
}