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
	LoginAttempts,
	LoginTimer
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
	Color1[MAX_PLAYER_NAME],
	Color2[MAX_PLAYER_NAME],
	Float: X_Pos,
	Float: Y_Pos,
	Float: Z_Pos,
	Float: Angle,

	session_ID
	Text3D: Label,
}

enum dialogs {
	DIALOG_UNUSED,
	DIALOG_CHANGENAME,
	DIALOG_LOGIN,
	DIALOG_REGISTER
};