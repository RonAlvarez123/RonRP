#define MYSQL_HOSTNAME "localhost"
#define MYSQL_USERNAME "root"
#define MYSQL_PASSWORD "complexpassword"
#define MYSQL_DATABASE "test"

#define DEFAULT_POS_X	1790.4780
#define DEFAULT_POS_Y	-1918.6830
#define DEFAULT_POS_Z	13.3932
#define DEFAULT_ANGLE	359.4301

#define BCRYPT_COST 	12

#define MINIMUM_PASSWORD_LENGTH		5
#define MINIMUM_LOGIN_ATTEMPTS		3

#define MAX_VEHICLES_PER_PLAYER		5

#define DEFAULT_VEHICLE_DEALERSHIP_ACCOUNT_ID   -1
#define DEFAULT_VEHICLE_DEALERSHIP_OWNER        "Dealership"
#define DEFAULT_VEHICLE_DEALERSHIP_HAS_LOCK     false
#define DEFAULT_VEHICLE_DEALERSHIP_COLOR1        0
#define DEFAULT_VEHICLE_DEALERSHIP_COLOR2        0

#define DEFAULT_SERVER_MESSAGE_COLOR            0xFC4E03FF

#define COLOR_WHITE			0xFFFFFFFF

enum accounts
{
	ORM: ORM_ID,

	ID,
	Username[MAX_PLAYER_NAME],
	Password[65],
	Cash,
	Kills,
	Deaths,
	Float: X_Pos,
	Float: Y_Pos,
	Float: Z_Pos,
	Float: Angle,
	Interior,

	bool: IsLoggedIn,
	bool: Respawned,
	TotalVehicles,
	SpawnedVehicles,
	LoginAttempts,
	LoginTimer
};

enum players {
	ORM: ORM_ID,

	ID,
	Account_ID,
	Job1[100],
	Job2[100],
	Material_Packages,
	Materials
};

enum vehicles {
	ORM: ORM_ID,

	ID, // Primary key in DB
	Account_ID, // Linked to the owner's account ID
	Model,
	Name[MAX_PLAYER_NAME],
	Owner[MAX_PLAYER_NAME],
	Price,
	bool: hasLock,
	Color1,
	Color2,
	Float: X_Pos,
	Float: Y_Pos,
	Float: Z_Pos,
	Float: Angle,

	session_ID,
	Text3D: Label,
	bool: isSpawned
}

enum dialogs {
	DIALOG_UNUSED,
	DIALOG_CHANGENAME,
	DIALOG_LOGIN,
	DIALOG_REGISTER,
	DIALOG_VEHICLE_STORAGE
};