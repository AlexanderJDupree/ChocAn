BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "transactions" (
	"service_date"	INTEGER NOT NULL,
	"filed_date"	INTEGER NOT NULL,
	"provider_id"	INTEGER NOT NULL,
	"member_id"	INTEGER NOT NULL,
	"service_code"	INTEGER NOT NULL,
	"comments"	TEXT,
	PRIMARY KEY("filed_date"),
	FOREIGN KEY("service_code") REFERENCES "services"("code"),
	FOREIGN KEY("provider_id") REFERENCES "accounts"("chocan_id"),
	FOREIGN KEY("member_id") REFERENCES "accounts"("chocan_id")
);
CREATE TABLE IF NOT EXISTS "accounts" (
	"chocan_id"	INTEGER UNIQUE,
	"f_name"	TEXT NOT NULL,
	"l_name"	TEXT NOT NULL,
	"street"	TEXT NOT NULL,
	"city"	TEXT NOT NULL,
	"state"	TEXT NOT NULL,
	"zip"	TEXT NOT NULL,
	"type"	TEXT NOT NULL,
	"status"	TEXT,
	PRIMARY KEY("chocan_id"),
	FOREIGN KEY("status") REFERENCES "account_status"("status"),
	FOREIGN KEY("type") REFERENCES "account_type"("type")
);
CREATE TABLE IF NOT EXISTS "services" (
	"code"	INTEGER,
	"cost"	INTEGER NOT NULL,
	"name"	TEXT NOT NULL,
	PRIMARY KEY("code")
);
CREATE TABLE IF NOT EXISTS "account_type" (
	"type"	TEXT NOT NULL UNIQUE,
	PRIMARY KEY("type")
);
CREATE TABLE IF NOT EXISTS "account_status" (
	"status"	TEXT NOT NULL UNIQUE,
	PRIMARY KEY("status")
);
INSERT INTO "transactions" VALUES (1574380800,1574380800,177607040,123123123,321321,'This dude is way too addicted to chocolate');
INSERT INTO "transactions" VALUES (1574467200,1574467200,987654321,123412345,654321,'Wubba lubba dub dub');
INSERT INTO "transactions" VALUES (1574554329,1574554329,123451234,123123123,123456,'Reccommend daily backrubs');
INSERT INTO "transactions" VALUES (1575691233,1575691233,123451234,123123123,598470,'I dont know what im doing');
INSERT INTO "transactions" VALUES (1575691235,1575691235,123451234,123123123,883948,'Lets get schwifty');
INSERT INTO "accounts" VALUES (123123123,'John','Doe','1234 Cool St.','Portland','OR','97030','Member','Valid');
INSERT INTO "accounts" VALUES (123412345,'Morty','Smith','137 Smith st.','Meeseeks','NJ','86453','Member','Valid');
INSERT INTO "accounts" VALUES (123451234,'Vince','Feelgood','1989 Motley Crue ln.','Los Angeles','CA','90510','Provider','Valid');
INSERT INTO "accounts" VALUES (123456789,'Chuck','Norris','1234 Walker St.','Fort Worth','TX','76036','Manager','Valid');
INSERT INTO "accounts" VALUES (177607040,'Homer','Simpson','742 Evergreen Ter.','Springfield','IL','12345','Provider','Valid');
INSERT INTO "accounts" VALUES (321321321,'Jane','Doe','1234 Lame St.','Portland','OR','97236','Member','Suspended');
INSERT INTO "accounts" VALUES (987654321,'Rick','Sanchez','137 Smith st.','Meeseeks','NJ','87654','Provider','Valid');
INSERT INTO "services" VALUES (121121,20,'Group Therapy');
INSERT INTO "services" VALUES (598470,99.99,'Dietitian Session');
INSERT INTO "services" VALUES (883948,45.00,'Aerobic Exercise Session');
INSERT INTO "services" VALUES (123123,65,'Addiction Consulting');
INSERT INTO "services" VALUES (123456,39.99,'Back Rub');
INSERT INTO "services" VALUES (321321,79.99,'Addiction Treatment');
INSERT INTO "services" VALUES (654321,88.88,'ChocAn Special');
INSERT INTO "account_type" VALUES ('Manager');
INSERT INTO "account_type" VALUES ('Provider');
INSERT INTO "account_type" VALUES ('Member');
INSERT INTO "account_status" VALUES ('Valid');
INSERT INTO "account_status" VALUES ('Suspended');
COMMIT;
