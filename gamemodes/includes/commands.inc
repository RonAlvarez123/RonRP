COMMAND:getcash(playerid, params[]) {
	new amount;
	if(sscanf(params, "i", amount)) {
		return SendClientMessage(playerid, -1, "Usage: /getcash <amount>");
	}

	GivePlayerMoney(playerid, amount);
	return 1;
}

COMMAND:createvehicle(playerid, params[]) {
	new vehicleNumber;
	if(sscanf(params, "i", vehicleNumber)) {
		return SendClientMessage(playerid, -1, "Usage: /createvehicle <vehicleID>");
	}

	new string[128];
	new vehicleId;
	switch(vehicleNumber) {
		case 1: {
			vehicleId = CreateVehicle(411, 1801.5839, -1932.4995, 13.3865, 78.3339, 0x1A32BA, 0xD41555, -1, 0);
			format(string, sizeof string, "Vehicle Created: %d", vehicleId);
			SendClientMessage(playerid, 0xD41555, string);
		}
	}
	return 1;
}

COMMAND:createvdealership(playerid, params[]) {
    new vehicleModel, vehiclePrice;
    if(sscanf(params, "ii", vehicleModel, vehiclePrice)) {
        return SendClientMessage(playerid, -1, "Usage: /createvdealership <vehicleModel> <vehiclePrice>");
    }

    new query[300], result_count, Cache:cache_id;
    mysql_format(conn, query, sizeof query, "SELECT COUNT(*) FROM `vehicles` WHERE `account_id` = '-1' AND `model` = '%d'", vehicleModel);
    cache_id = mysql_query(conn, query);
    cache_get_value_index_int(0, 0, result_count);
    cache_delete(cache_id);
    printf("Result count: %d", result_count);

    if(result_count > 0) {
        print("Already exists");
        return SendClientMessage(playerid, -1, "SERVER: Oncreatevdealership - That vehicle is already for sale.");
    }

    print("Pwede i-create");
   
    new vehicleName[MAX_PLAYER_NAME], Float:vehicleX, Float:vehicleY, Float:vehicleZ, Float:vehicleAngle;
    getVehicleName(vehicleModel, vehicleName);
    GetPlayerPos(playerid, vehicleX, vehicleY, vehicleZ);
    GetPlayerFacingAngle(playerid, vehicleAngle);

    mysql_format(conn, query, sizeof query, "INSERT INTO `vehicles` (`account_id`, `model`, `name`, `owner`, `price`, `x`, `y`, `z`, `angle`) VALUES ('%d', '%d', '%s', '%s', '%d', '%f', '%f', '%f', '%f')", DEFAULT_VEHICLE_DEALERSHIP_ACCOUNT_ID, vehicleModel, vehicleName, DEFAULT_VEHICLE_DEALERSHIP_OWNER, vehiclePrice, vehicleX, vehicleY, vehicleZ, vehicleAngle);
    mysql_tquery(conn, query, "Oncreatevdealership", "i", playerid);

    return 1;
}

COMMAND:engine(playerid) {
    new vehicleid, string[128];
    vehicleid = GetPlayerVehicleID(playerid);

    if(Vehicle[vehicleid][Account_ID] != Player[playerid][ID]) {
        format(string, sizeof string, "This vehicle is owned by %s", Vehicle[vehicleid][Owner]);
        return SendClientMessage(playerid, DEFAULT_SERVER_MESSAGE_COLOR, string);
    }

    new engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(Vehicle[vehicleid][session_ID], engine, lights, alarm, doors, bonnet, boot, objective);
    
    if(engine == 1) {
        engine = 0;
    } else {
        engine = 1;
    }

    SetVehicleParamsEx(Vehicle[vehicleid][session_ID], engine, lights, alarm, doors, bonnet, boot, objective);
    return 1;
}

COMMAND:buyvehicle(playerid) {
    if(IsPlayerInAnyVehicle(playerid) <= 0) {
        return SendClientMessage(playerid, DEFAULT_SERVER_MESSAGE_COLOR, "You are not in any vehicle.");
    }

    new vehicleid;
    vehicleid = GetPlayerVehicleID(playerid);

    if(Vehicle[vehicleid][Account_ID] != -1) {
        return SendClientMessage(playerid, DEFAULT_SERVER_MESSAGE_COLOR, "This vehicle is not for sale.");
    }

    Player[playerid][Cash] = GetPlayerMoney(playerid);
    if(Player[playerid][Cash] < Vehicle[vehicleid][Price]) {
        return SendClientMessage(playerid, DEFAULT_SERVER_MESSAGE_COLOR, "You don't have enough money to buy this vehicle.");
    }

    new query[300], Float:vehicleX, Float:vehicleY, Float:vehicleZ, Float:vehicleAngle;

    // switch(Vehicle[vehicleid][Model]) {
    //     default: {
    //         vehicleX = 1797.4886;
    //         vehicleY = -1908.5419;
    //         vehicleZ = 13.3982;
    //         vehicleAngle = 89.4719;
    //     }
    // }

    vehicleX = 1801.9736;
    vehicleY = -1906.1600;
    vehicleZ = 13.3993;
    vehicleAngle = 173.2735;

    mysql_format(conn, query, sizeof query, "INSERT INTO `vehicles` (`account_id`, `model`, `name`, `owner`, `price`, `x`, `y`, `z`, `angle`) VALUES ('%d', '%d', '%s', '%s', '%d', '%f', '%f', '%f', '%f')", Player[playerid][ID], Vehicle[vehicleid][Model], Vehicle[vehicleid][Name], Player[playerid][Username], Vehicle[vehicleid][Price], vehicleX, vehicleY, vehicleZ, vehicleAngle);
    mysql_query(conn, query);

    new insert_id = cache_insert_id();
    if(insert_id <= 0) {        
        return SendClientMessage(playerid, -1, "[Onbuyvehicle]: There was an error.");
    }
    
    new vehiclePrice = Vehicle[vehicleid][Price];
	vehiclePrice *= -1;
    GivePlayerMoney(playerid, vehiclePrice);

    vehicleid = CreateVehicle(Vehicle[vehicleid][Model], vehicleX, vehicleY, vehicleZ, vehicleAngle, DEFAULT_VEHICLE_DEALERSHIP_COLOR1,DEFAULT_VEHICLE_DEALERSHIP_COLOR2, -1, 0);
    Vehicle[vehicleid][session_ID] = vehicleid;

    Vehicle[vehicleid][ID] = insert_id;
    Vehicle[vehicleid][Account_ID] = Player[playerid][ID];
    Vehicle[vehicleid][ORM_ID] = orm_create("vehicles", conn);
    ORMSetupVehiclesTable(Vehicle[vehicleid][ORM_ID], vehicleid);

    orm_setkey(Vehicle[vehicleid][ORM_ID], "id");
    orm_load(Vehicle[vehicleid][ORM_ID], "OnFreshBoughtVehicleDataLoaded", "dd", vehicleid, playerid);
    return 1;
}