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