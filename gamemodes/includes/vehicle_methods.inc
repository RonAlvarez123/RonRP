forward OnGetAllServerVehicle();
public OnGetAllServerVehicle() {
    
    new row_count = ServerTotalVehicles = cache_num_rows();
    new result[vehicles];
    printf("cache_num_rows: Server Vehicles are %d", row_count);
    for(new i = 0; i < row_count; i++) {
        cache_get_value_name_int(i, "id", result[ID]);
        cache_get_value_name_int(i, "account_id", result[Account_ID]);
        cache_get_value_name_int(i, "model", result[Model]);
        cache_get_value_name(i, "name", result[Name], MAX_PLAYER_NAME);
        cache_get_value_name(i, "owner", result[Owner], MAX_PLAYER_NAME);
        cache_get_value_name_int(i, "price", result[Price]);
        cache_get_value_name_bool(i, "hasLock", result[hasLock]);
        cache_get_value_name_float(i, "x", result[X_Pos]);
        cache_get_value_name_float(i, "y", result[Y_Pos]);
        cache_get_value_name_float(i, "z", result[Z_Pos]);
        cache_get_value_name_float(i, "angle", result[Angle]);

        new vehicleid;
        vehicleid = CreateVehicle(result[Model], result[X_Pos], result[Y_Pos], result[Z_Pos], result[Angle], DEFAULT_VEHICLE_DEALERSHIP_COLOR1, DEFAULT_VEHICLE_DEALERSHIP_COLOR2, -1, 0);

        ServerVehicle[i][ID] = result[ID];
	    ServerVehicle[i][ORM_ID] = orm_create("vehicles", conn);
        ORMSetupServerVehiclesTable(ServerVehicle[i][ORM_ID], i);

        orm_setkey(ServerVehicle[i][ORM_ID], "id");
        orm_load(ServerVehicle[i][ORM_ID], "OnServerVehicleDataLoaded", "dd", vehicleid, i);
    }
    return 1;
}

forward OnServerVehicleDataLoaded(vehicleid, i);
public OnServerVehicleDataLoaded(vehicleid, i) {
    new string[128];
    switch(orm_errno(ServerVehicle[i][ORM_ID])) {
		case ERROR_OK: {
            ServerVehicle[i][session_ID] = vehicleid;
            format(string, sizeof string, "%d", ServerVehicle[i][Price]);
            ServerVehicle[i][Label] = Create3DTextLabel(string, 0xFFFFFFFF, 0.0, 0.0, 0.0, 10.0, 0, 1 );
            Attach3DTextLabelToVehicle(ServerVehicle[i][Label], ServerVehicle[i][session_ID], 0.0, 0.0, 1.0);
		}
		case ERROR_NO_DATA: {
			print("[OnServerVehicleDataLoaded]: ERROR_NO_DATA");
		}
	}
    return 1;
}

forward OnFreshBoughtVehicleDataLoaded(playerid, i);
public OnFreshBoughtVehicleDataLoaded(playerid, i) {
    new string[128];
    switch(orm_errno(PlayerVehicle[playerid][i][ORM_ID])) {
		case ERROR_OK: {
            UpdatePlayerDataOnBuyVehicle(playerid);
            format(string, sizeof string, "You have successfully bought a %s.", PlayerVehicle[playerid][i][Name], PlayerVehicle[playerid][i][session_ID]);
            SendClientMessage(playerid, DEFAULT_FEEDBACK_MESSAGE_COLOR, string);
		}
		case ERROR_NO_DATA: {
			print("[OnFreshBoughtVehicleDataLoaded]: ERROR_NO_DATA");
		}
	}
    return 1;
}

forward Oncreatevdealership(playerid);
public Oncreatevdealership(playerid) {
    if(cache_insert_id() > 0) {
        return SendClientMessage(playerid, DEFAULT_SERVER_MESSAGE_COLOR, "[SERVER]: Car dealership created successfully.");
    }
    return SendClientMessage(playerid, DEFAULT_SERVER_MESSAGE_COLOR, "[SERVER]: Oncreatevdealership: There was an error.");
}

forward OnGetAllPlayerVehData(playerid);
public OnGetAllPlayerVehData(playerid) {
    new row_count = cache_num_rows();
    Account[playerid][TotalVehicles] = row_count;
    printf("cache_num_rows: Account Vehicles are %d", row_count);

    for(new i = 0; i < row_count; i++) {
        cache_get_value_name_int(i, "id", PlayerVehicle[playerid][i][ID]);
        cache_get_value_name_int(i, "account_id", PlayerVehicle[playerid][i][Account_ID]);
        cache_get_value_name_int(i, "model", PlayerVehicle[playerid][i][Model]);
        cache_get_value_name(i, "name", PlayerVehicle[playerid][i][Name], MAX_PLAYER_NAME);
        cache_get_value_name(i, "owner", PlayerVehicle[playerid][i][Owner], MAX_PLAYER_NAME);
        cache_get_value_name_int(i, "price", PlayerVehicle[playerid][i][Price]);
        cache_get_value_name_bool(i, "hasLock", PlayerVehicle[playerid][i][hasLock]);
        cache_get_value_name_int(i, "color1", PlayerVehicle[playerid][i][Color1]);
        cache_get_value_name_int(i, "color2", PlayerVehicle[playerid][i][Color2]);
        cache_get_value_name_float(i, "x", PlayerVehicle[playerid][i][X_Pos]);
        cache_get_value_name_float(i, "y", PlayerVehicle[playerid][i][Y_Pos]);
        cache_get_value_name_float(i, "z", PlayerVehicle[playerid][i][Z_Pos]);
        cache_get_value_name_float(i, "angle", PlayerVehicle[playerid][i][Angle]);
        PlayerVehicle[playerid][i][isSpawned] = false;

        PlayerVehicle[playerid][i][ORM_ID] = orm_create("vehicles", conn);
        ORMSetupPlayerVehiclesTable(PlayerVehicle[playerid][i][ORM_ID], playerid, i);
        orm_setkey(PlayerVehicle[playerid][i][ORM_ID], "id");
    }
    return 1;
}