forward DBSetupAccountsTable(MySQL: DBconnection);
public DBSetupAccountsTable(MySQL: DBconnection) {
    mysql_tquery(DBconnection, "CREATE TABLE IF NOT EXISTS `accounts` (`id` int(11) NOT NULL AUTO_INCREMENT, `username` varchar(24) NOT NULL, `password` varchar(64) NOT NULL, `cash` int(11) NOT NULL, `kills` int(11) NOT NULL, `death` int(11) NOT NULL, `x` float NOT NULL, `y` float NOT NULL, `z` float NOT NULL, `angle` float NOT NULL, `interior` int(11) NOT NULL, PRIMARY KEY (`id`))");
	print("Accounts Table Setup Finished!");
    return 1;
}

forward DBSetupVehiclesTable(MySQL: DBconnection);
public DBSetupVehiclesTable(MySQL: DBconnection) {
    mysql_tquery(DBconnection, "CREATE TABLE IF NOT EXISTS `vehicles` (`id` int(11) NOT NULL AUTO_INCREMENT, `account_id` int(11) NOT NULL, `model` tinyint(4) NOT NULL, `name` varchar(24) NOT NULL, `owner` varchar(24) DEFAULT NULL, `price` int(11) NOT NULL, `hasLock` tinyint(1) NOT NULL DEFAULT 0, `color1` varchar(24) NOT NULL DEFAULT '000000', `color2` varchar(24) NOT NULL DEFAULT '000000', `x` float NOT NULL, `y` float NOT NULL, `z` float NOT NULL, `angle` float NOT NULL, PRIMARY KEY (`id`))");
    print("Vehicles Table Setup Finished!");
    return 1;
}

forward ORMSetupAccountsTable(ORM: TableModel, playerid);
public ORMSetupAccountsTable(ORM: TableModel, playerid) {
    orm_addvar_int(TableModel, Player[playerid][ID], "id");
	orm_addvar_string(TableModel, Player[playerid][Username], MAX_PLAYER_NAME, "username");
	orm_addvar_string(TableModel, Player[playerid][Password], 65, "password");
	orm_addvar_int(TableModel, Player[playerid][Cash], "cash");
	orm_addvar_int(TableModel, Player[playerid][Kills], "kills");
	orm_addvar_int(TableModel, Player[playerid][Deaths], "death");
	orm_addvar_float(TableModel, Player[playerid][X_Pos], "x");
	orm_addvar_float(TableModel, Player[playerid][Y_Pos], "y");
	orm_addvar_float(TableModel, Player[playerid][Z_Pos], "z");
	orm_addvar_float(TableModel, Player[playerid][Angle], "angle");
	orm_addvar_int(TableModel, Player[playerid][Interior], "interior");
    return 1;
}

// forward ORMSetupVehiclesTable(ORM: TableModel, vehicleid);
// public ORMSetupVehiclesTable(ORM: TableModel, vehicleid) {
//     orm_addvar_int(TableModel, Vehicle[vehicleid][ID], "id");
// 	orm_addvar_string(TableModel, Vehicle[vehicleid][Username], MAX_PLAYER_NAME, "username");
// 	orm_addvar_string(TableModel, Vehicle[vehicleid][Password], 65, "password");
// 	orm_addvar_int(TableModel, Vehicle[vehicleid][Cash], "cash");
// 	orm_addvar_int(TableModel, Vehicle[vehicleid][Kills], "kills");
// 	orm_addvar_int(TableModel, Vehicle[vehicleid][Deaths], "death");
// 	orm_addvar_float(TableModel, Vehicle[vehicleid][X_Pos], "x");
// 	orm_addvar_float(TableModel, Vehicle[vehicleid][Y_Pos], "y");
// 	orm_addvar_float(TableModel, Vehicle[vehicleid][Z_Pos], "z");
// 	orm_addvar_float(TableModel, Vehicle[vehicleid][Angle], "angle");
// 	orm_addvar_int(TableModel, Vehicle[vehicleid][Interior], "interior");
//     return 1;
// }