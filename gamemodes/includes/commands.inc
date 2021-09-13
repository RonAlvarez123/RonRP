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
    if(!IsPlayerInAnyVehicle(playerid)) {
        return SendClientMessage(playerid, DEFAULT_SERVER_MESSAGE_COLOR, "You are not in a vehicle");
    }

    new vehicleid, string[128];
    vehicleid = GetPlayerVehicleID(playerid);

    new bool: isVehicleOwnedByPlayer = false, iteration;
    for(new i = 0; i < Player[playerid][TotalVehicles]; i++) {
        if(PlayerVehicle[playerid][i][session_ID] == vehicleid) {
            isVehicleOwnedByPlayer = true;
            iteration = i;
        }
    }

    new bool: isVehicleOwnedByServer = false;
    for(new i = 0; i < ServerTotalVehicles; i++) {
        if(ServerVehicle[i][session_ID] == vehicleid) {
            isVehicleOwnedByServer = true;
        }
    }

    if(isVehicleOwnedByPlayer == false && isVehicleOwnedByServer == true) {
        format(string, sizeof string, "This vehicle is owned by %s.", ServerVehicle[iteration][Owner], vehicleid);
        return SendClientMessage(playerid, DEFAULT_SERVER_MESSAGE_COLOR, string);
    } else if(isVehicleOwnedByPlayer == false && isVehicleOwnedByServer == false) {
        return SendClientMessage(playerid, DEFAULT_SERVER_MESSAGE_COLOR, "This vehicle is owned by another player.");
    }

    new engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(PlayerVehicle[playerid][iteration][session_ID], engine, lights, alarm, doors, bonnet, boot, objective);
    
    if(engine == 1) {
        engine = 0;
    } else {
        engine = 1;
    }

    SetVehicleParamsEx(PlayerVehicle[playerid][iteration][session_ID], engine, lights, alarm, doors, bonnet, boot, objective);
    return 1;
}

COMMAND:buyvehicle(playerid) {
    if(IsPlayerInAnyVehicle(playerid) <= 0) {
        return SendClientMessage(playerid, DEFAULT_SERVER_MESSAGE_COLOR, "You are not in any vehicle.");
    }

    new vehicleid;
    vehicleid = GetPlayerVehicleID(playerid);

    new iteration = -1;
    for(new i = 0; i < ServerTotalVehicles; i++) {
        if(ServerVehicle[i][session_ID] == vehicleid) {
            iteration = i;
        }
    }

    if(iteration == -1) {
        return SendClientMessage(playerid, DEFAULT_SERVER_MESSAGE_COLOR, "This vehicle is not for sale.");
    }

    Player[playerid][Cash] = GetPlayerMoney(playerid);
    if(Player[playerid][Cash] < ServerVehicle[iteration][Price]) {
        return SendClientMessage(playerid, DEFAULT_SERVER_MESSAGE_COLOR, "You don't have enough money to buy this vehicle.");
    }

    new query[300], Float:vehicleX, Float:vehicleY, Float:vehicleZ, Float:vehicleAngle;

    /*****************************
    * 
    * ADD QUERY THAT CHECKS THE MODEL OF A VEHICLE TO CUSTOMIZE THE SPAWN POINT OF NEWLY BOUGHT VEHICLE PER CAR DEALERSHIP
    * 
    *****************************/

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

    mysql_format(conn, query, sizeof query, "INSERT INTO `vehicles` (`account_id`, `model`, `name`, `owner`, `price`, `x`, `y`, `z`, `angle`) VALUES ('%d', '%d', '%s', '%s', '%d', '%f', '%f', '%f', '%f')", Player[playerid][ID], ServerVehicle[iteration][Model], ServerVehicle[iteration][Name], Player[playerid][Username], ServerVehicle[iteration][Price], vehicleX, vehicleY, vehicleZ, vehicleAngle);
    mysql_query(conn, query);

    new insert_id = cache_insert_id();
    if(insert_id <= 0) {        
        return SendClientMessage(playerid, -1, "[Onbuyvehicle]: There was an error.");
    }
    
    new vehiclePrice = ServerVehicle[iteration][Price];
	vehiclePrice *= -1;
    GivePlayerMoney(playerid, vehiclePrice);

    vehicleid = CreateVehicle(ServerVehicle[iteration][Model], vehicleX, vehicleY, vehicleZ, vehicleAngle, DEFAULT_VEHICLE_DEALERSHIP_COLOR1, DEFAULT_VEHICLE_DEALERSHIP_COLOR2, -1, 0);
    // HERE
    new playerTotalVehicles = Player[playerid][TotalVehicles];
    Player[playerid][TotalVehicles]++;

    PlayerVehicle[playerid][playerTotalVehicles][session_ID] = vehicleid;

    PlayerVehicle[playerid][playerTotalVehicles][ID] = insert_id;
    PlayerVehicle[playerid][playerTotalVehicles][isSpawned] = true;
    PlayerVehicle[playerid][playerTotalVehicles][Account_ID] = Player[playerid][ID];
    PlayerVehicle[playerid][playerTotalVehicles][ORM_ID] = orm_create("vehicles", conn);
    ORMSetupPlayerVehiclesTable(PlayerVehicle[playerid][playerTotalVehicles][ORM_ID], playerid, playerTotalVehicles);

    orm_setkey(PlayerVehicle[playerid][playerTotalVehicles][ORM_ID], "id");
    orm_load(PlayerVehicle[playerid][playerTotalVehicles][ORM_ID], "OnFreshBoughtVehicleDataLoaded", "dd", playerid, playerTotalVehicles);
    return 1;
}

COMMAND:vstorage(playerid) {
    if(Player[playerid][TotalVehicles] <= 0) {
        return SendClientMessage(playerid, DEFAULT_SERVER_MESSAGE_COLOR, "You don't own any vehicle.");
    }

    new string[128], status[MAX_PLAYER_NAME];
    for(new i = 0; i < Player[playerid][TotalVehicles]; i++) {
        if(PlayerVehicle[playerid][i][isSpawned] == false) {
            format(status, sizeof status, "Stored");
        } else {
            format(status, sizeof status, "Spawned");
        }
        
        if(i == 0) {
            format(string, sizeof string, "Model\tStatus\n%s\t%s\n", PlayerVehicle[playerid][i][Name], status);
        } else {
            format(string, sizeof string, "%s%s\t%s\n", string, PlayerVehicle[playerid][i][Name], status);
        }
    }

    ShowPlayerDialog(playerid, DIALOG_VEHICLE_STORAGE, DIALOG_STYLE_TABLIST_HEADERS, "Vehicle Storage", string, "Spawn/Store", "Cancel");
    return 1;
}

COMMAND:changevehiclecolor(playerid, params[]) {
    new vehicleid, color1, color2;
    if(sscanf(params, "ii", color1, color2)) {
		return SendClientMessage(playerid, -1, "Usage: /changevehiclecolor <color1> <color2>");
    }

    if((color1 < 0 || color1 > 255) || (color2 < 0 || color2 > 255)) {
        return SendClientMessage(playerid, -1, "Please choose a color value from 0 to 255.");
    }
    vehicleid = GetPlayerVehicleID(playerid);
    ChangeVehicleColor(vehicleid, color1, color2);
    return 1;
}

COMMAND:getmats(playerid) {
    if(!IsPlayerInRangeOfPoint(playerid, 2, 1805.2272, -1904.5685, 13.4001)) {
        return SendClientMessage(playerid, -1, "You are out of range.");
    }
    SendClientMessage(playerid, -1, "You can get mats.");
    return 1;
}