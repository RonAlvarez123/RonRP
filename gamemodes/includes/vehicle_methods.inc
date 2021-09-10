// forward OnVehicleServerDataLoaded();
// public OnVehicleServerDataLoaded() {
//     printf("cache_num_rows: %d", cache_num_rows());
// }

forward OnGetAllServerVehicle();
public OnGetAllServerVehicle() {
    printf("cache_num_rows: Server Vehicles are %d", cache_num_rows());
}

forward OnGetAllDealershipVehicle();
public OnGetAllDealershipVehicle() {
    
    new row_count = cache_num_rows();
    new result[vehicles];
    new color1_int, color2_int;
    printf("cache_num_rows: Dealership Vehicles are %d", row_count);
    for(new i = 0; i < row_count; i++) {
        cache_get_value_name_int(i, "id", result[ID]);
        cache_get_value_name_int(i, "account_id", result[Account_ID]);
        cache_get_value_name_int(i, "model", result[Model]);
        cache_get_value_name(i, "name", result[Name], MAX_PLAYER_NAME);
        cache_get_value_name(i, "owner", result[Owner], MAX_PLAYER_NAME);
        cache_get_value_name_int(i, "price", result[Price]);
        cache_get_value_name_bool(i, "hasLock", result[hasLock]);
        cache_get_value_name(i, "color1", result[Color1], MAX_PLAYER_NAME);
        cache_get_value_name(i, "color2", result[Color2], MAX_PLAYER_NAME);
        format(result[Color1], MAX_PLAYER_NAME, "0x%d", color1_int);
        format(result[Color2], MAX_PLAYER_NAME, "0x%d", color2_int);
        cache_get_value_name_float(i, "x", result[X_Pos]);
        cache_get_value_name_float(i, "y", result[Y_Pos]);
        cache_get_value_name_float(i, "z", result[Z_Pos]);
        cache_get_value_name_float(i, "angle", result[Angle]);

        new vehicleid;
        vehicleid = CreateVehicle(result[Model], result[X_Pos], result[Y_Pos], result[Z_Pos], result[Angle], result[Color1], result[Color2], -1, 0);

        Vehicle[vehicleid][ID] = result[ID];
	    Vehicle[vehicleid][ORM_ID] = orm_create("vehicles", conn);
        ORMSetupVehiclesTable(Vehicle[vehicleid][ORM_ID], vehicleid);

        orm_setkey(Vehicle[vehicleid][ORM_ID], "id");
        orm_load(Vehicle[vehicleid][ORM_ID], "OnDealershipVehicleDataLoaded", "d", vehicleid); // CONTINUE HERE
    }
    return 1;
}

forward OnDealershipVehicleDataLoaded(vehicleid);
public OnDealershipVehicleDataLoaded(vehicleid) {
    new string[128];
    switch(orm_errno(Vehicle[vehicleid][ORM_ID])) {
		case ERROR_OK: {
            // print("[OnDealershipVehicleDataLoaded]: ERROR_OK");
            Vehicle[vehicleid][session_ID] = vehicleid;
            format(string, sizeof string, "%d", Vehicle[vehicleid][Price]);
            Vehicle[vehicleid][Label] = Create3DTextLabel(string, 0xFFFFFFFF, 0.0, 0.0, 0.0, 10.0, 0, 1 );
            Attach3DTextLabelToVehicle(Vehicle[vehicleid][Label], Vehicle[vehicleid][session_ID], 0.0, 0.0, 1.0);
		}
		case ERROR_NO_DATA: {
			printf("[OnDealershipVehicleDataLoaded]: ERROR_NO_DATA");
		}
	}
    return 1;
}

forward OnFreshBoughtVehicleDataLoaded(vehicleid, playerid);
public OnFreshBoughtVehicleDataLoaded(vehicleid, playerid) {
    new string[128];
    switch(orm_errno(Vehicle[vehicleid][ORM_ID])) {
		case ERROR_OK: {
            UpdatePlayerDataOnBuyVehicle(playerid);
            print("[OnFreshBoughtVehicleDataLoaded]: ERROR_OK");

            format(string, sizeof string, "[Onbuyvehicle]: You have successfully bought a %s. VehicleID is %d", Vehicle[vehicleid][Name], Vehicle[vehicleid][session_ID]);
            SendClientMessage(playerid, -1, string);
		}
		case ERROR_NO_DATA: {
			printf("[OnFreshBoughtVehicleDataLoaded]: ERROR_NO_DATA");
		}
	}
    return 1;
}

forward Oncreatevdealership(playerid);
public Oncreatevdealership(playerid) {
    if(cache_insert_id() > 0) {
        return SendClientMessage(playerid, -1, "[SERVER]: Car dealership created successfully.");
    }
    return SendClientMessage(playerid, -1, "[SERVER]: Oncreatevdealership: There was an error.");
}