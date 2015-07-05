/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2010 EQEMu Development Team (http://eqemulator.net)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY except by those people which sell it, which
	are required to give you total support for your newly bought product;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#include "../common/global_define.h"
#include "database.h"
#include "error_log.h"
#include "login_server.h"

#pragma warning( disable : 4267 )

extern ErrorLog *server_log;
extern LoginServer server;

#pragma comment(lib, "mysqlclient.lib")

Database::Database(string user, string pass, string host, string port, string name)
{
	this->user = user;
	this->pass = pass;
	this->host = host;
	this->name = name;

	db = mysql_init(nullptr);
	if(db)
	{
		my_bool r = 1;
		mysql_options(db, MYSQL_OPT_RECONNECT, &r);
		if(!mysql_real_connect(db, host.c_str(), user.c_str(), pass.c_str(), name.c_str(), atoi(port.c_str()), nullptr, 0))
		{
			mysql_close(db);
			server_log->Log(log_database, "Failed to connect to MySQL database. Error: %s", mysql_error(db));
			exit(1);
		}
	}
	else
	{
		server_log->Log(log_database, "Failed to create db object in MySQL database.");
	}
}

Database::~Database()
{
	if(db)
	{
		mysql_close(db);
	}
}

#pragma region Player Account Info
void Database::CreateLSAccount(unsigned int id, std::string name, std::string password, std::string email, unsigned int created_by, std::string LastIPAddress, std::string creationIP)
{
	bool activate = 0;
	if (!db)
	{
		return;
	}
	if (server.config->LoadOption("auto_account_activate", "login.ini") == "TRUE")
	{
		activate = 1;
	}
	char tmpUN[1024];
	mysql_real_escape_string(db, tmpUN, name.c_str(), name.length());

	string query = "INSERT INTO " +
						server.config->LoadOption("account_table", "login.ini") +
					" SET LoginServerID = " + std::to_string(id) + ", " +
						"AccountName = '" + tmpUN + "', " +
						"AccountPassword = sha('" +	password + "'), " +
						"AccountCreateDate = now(), " +
						"LastLoginDate = now(), " +
						"LastIPAddress = '" + LastIPAddress + "', " +
						"client_unlock = '" + std::to_string(activate) + "', " +
						"created_by = '" + std::to_string(created_by) + "', " +
						"creationIP = '" + creationIP + "'";

	if (mysql_query(db, query.c_str()) != 0)
	{
		server_log->Log(log_error, "Mysql query failed: %s", query.c_str());
	}
}

void Database::UpdateLSAccount(unsigned int id, string ip_address)
{
	if (!db)
	{
		return;
	}

	string query = "UPDATE " +
		server.config->LoadOption("account_table", "login.ini") +
		" SET " +
		"LastIPAddress = '" + ip_address + "', " +
		"LastLoginDate = now() " +
		"WHERE " +
		"LoginServerID = " + std::to_string(id);

	if (mysql_query(db, query.c_str()) != 0)
	{
		server_log->Log(log_error, "Mysql query failed: %s", query.c_str());
	}
}

bool Database::GetAccountLockStatus(std::string name)
{
	if (!db)
	{
		return false;
	}

	string query = "SELECT "
						"client_unlock "
					"FROM " +
						server.config->LoadOption("account_table", "login.ini") +
					" WHERE "
						"AccountName = '" + name + "'";

	if (mysql_query(db, query.c_str()) != 0)
	{
		server_log->Log(log_error, "Mysql query failed: %s", query.c_str());
		return false;
	}

	res = mysql_use_result(db);

	if (res)
	{
		while ((row = mysql_fetch_row(res)))
		{
			int unlock = atoi(row[0]);
			if (unlock == 1)
			{
				mysql_free_result(res);
				return true;
			}
			else
			{
				mysql_free_result(res);
				return false;
			}
		}
	}
	return false;
}

void Database::UpdateAccessLog(unsigned int account_id, std::string account_name, std::string IP, unsigned int accessed, std::string reason)
{
	if(!db)
	{
		return;
	}

	char tmpUN[1024];
	mysql_real_escape_string(db, tmpUN, account_name.c_str(), account_name.length());

	string query = "INSERT INTO " +
						server.config->LoadOption("access_log_table", "login.ini") +
					" SET " +
						"account_id = " + std::to_string(account_id) + ", " +
						"account_name = '" + tmpUN +"', " +
						"IP = '" + IP + "', " +
						"accessed = '" + std::to_string(accessed) + "', " +
						"reason = '" + reason + "'";

	if(mysql_query(db, query.c_str()) != 0)
	{
		server_log->Log(log_error, "Mysql query failed: %s", query.c_str());
	}
}

bool Database::GetLoginDataFromAccountName(string name, string &password, unsigned int &id)
{
	if(!db)
	{
		return false;
	}

	char tmpUN[1024];
	mysql_real_escape_string(db, tmpUN, name.c_str(), name.length());(tmpUN, name.c_str(), name.length());

	string query = "SELECT "
						"LoginServerID, AccountPassword "
					"FROM " +
						server.config->LoadOption("account_table", "login.ini") +
					" WHERE " +
						"AccountName = '" + tmpUN + "'";

	if(mysql_query(db, query.c_str()) != 0)
	{
		server_log->Log(log_error, "Mysql query failed: %s", query.c_str());
		return false;
	}

	res = mysql_use_result(db);

	if(res)
	{
		while((row = mysql_fetch_row(res)) != nullptr)
		{
			id = atoi(row[0]);
			password = row[1];
			mysql_free_result(res);
			return true;
		}
	}
	server_log->Log(log_database, "Mysql query returned no result: %s", query.c_str());
	return false;
}
#pragma endregion

#pragma region World Server Account Info
bool Database::CreateWorldRegistration(string long_name, string short_name, unsigned int &id)
{
	if (!db)
	{
		return false;
	}

	char escaped_long_name[201];
	char escaped_short_name[101];
	unsigned long length;
	length = mysql_real_escape_string(db, escaped_long_name, long_name.substr(0, 100).c_str(), long_name.substr(0, 100).length());
	escaped_long_name[length + 1] = 0;
	length = mysql_real_escape_string(db, escaped_short_name, short_name.substr(0, 100).c_str(), short_name.substr(0, 100).length());
	escaped_short_name[length + 1] = 0;

	string query = "SELECT max(ServerID) FROM " + server.config->LoadOption("world_registration_table", "login.ini");

	if (mysql_query(db, query.c_str()) != 0)
	{
		server_log->Log(log_error, "Mysql query failed: %s", query.c_str());
		return false;
	}

	res = mysql_use_result(db);
	if (res)
	{
		if ((row = mysql_fetch_row(res)) != nullptr)
		{
			id = atoi(row[0]) + 1;
			mysql_free_result(res);

			string query = "INSERT INTO " +
				server.config->LoadOption("world_registration_table", "login.ini") +
				" SET " +
				"ServerID = '" + std::to_string(id) + "', " +
				"ServerLongName = '" + escaped_long_name + "', " +
				"ServerShortName = '" + escaped_short_name + "', " +
				"ServerListTypeID = 3, " +
				"ServerAdminID = 0, " +
				"ServerTrusted = 0, " +
				"ServerTagDescription = ''";

			if (mysql_query(db, query.c_str()) != 0)
			{
				server_log->Log(log_error, "Mysql query failed: %s", query.c_str());
				return false;
			}
			return true;
		}
	}
	server_log->Log(log_database, "World registration did not exist in the database for %s %s", long_name.c_str(), short_name.c_str());
	return false;
}

void Database::UpdateWorldRegistration(unsigned int id, string long_name, string ip_address)
{
	if (!db)
	{
		return;
	}

	char escaped_long_name[101];
	unsigned long length;
	length = mysql_real_escape_string(db, escaped_long_name, long_name.substr(0, 100).c_str(), long_name.substr(0, 100).length());
	escaped_long_name[length + 1] = 0;

	string query = "UPDATE " +
		server.config->LoadOption("world_registration_table", "login.ini") +
		" SET " +
		"ServerLastLoginDate = now(), " +
		"ServerLastIPAddr = '" + ip_address + "', " +
		"ServerLongName = '" + escaped_long_name + "' " +
		"WHERE " +
		"ServerID = '" + std::to_string(id) + "'";

	if (mysql_query(db, query.c_str()) != 0)
	{
		server_log->Log(log_error, "Mysql query failed: %s", query.c_str());
	}
}

bool Database::GetWorldRegistration(string long_name, string short_name, unsigned int &id, string &desc, unsigned int &list_id,
		unsigned int &trusted, string &list_desc, string &account, string &password)
{
	if(!db)
	{
		return false;
	}

	char escaped_short_name[101];
	unsigned long length;
	length = mysql_real_escape_string(db, escaped_short_name, short_name.substr(0, 100).c_str(), short_name.substr(0, 100).length());
	escaped_short_name[length+1] = 0;

	string query = "SELECT "
						"WSR.ServerID, WSR.ServerTagDescription, WSR.ServerTrusted, SLT.ServerListTypeID, SLT.ServerListTypeDescription, WSR.ServerAdminID "
					"FROM " +
						server.config->LoadOption("world_registration_table", "login.ini") +
					" AS WSR JOIN " +
						server.config->LoadOption("world_server_type_table", "login.ini") +
					" AS SLT ON "
						"WSR.ServerListTypeID = SLT.ServerListTypeID"
					" WHERE "
						"WSR.ServerShortName = '" + escaped_short_name + " '";

	if(mysql_query(db, query.c_str()) != 0)
	{
		server_log->Log(log_error, "Mysql query failed: %s", query.c_str());
		return false;
	}

	res = mysql_use_result(db);
	if(res)
	{
		if((row = mysql_fetch_row(res)) != nullptr)
		{
			id = atoi(row[0]);
			desc = row[1];
			trusted = atoi(row[2]);
			list_id = atoi(row[3]);
			list_desc = row[4];
			int db_account_id = atoi(row[5]);
			mysql_free_result(res);

			if(db_account_id > 0)
			{
				string query = "SELECT "
									"AccountName, AccountPassword "
								"FROM " +
									server.config->LoadOption("world_admin_registration_table", "login.ini") +
								" WHERE "
									"ServerAdminID = " + std::to_string(db_account_id);

				if(mysql_query(db, query.c_str()) != 0)
				{
					server_log->Log(log_error, "Mysql query failed: %s", query.c_str());
					return false;
				}

				res = mysql_use_result(db);
				if(res)
				{
					if((row = mysql_fetch_row(res)) != nullptr)
					{
						account = row[0];
						password = row[1];
						mysql_free_result(res);
						return true;
					}
				}
				server_log->Log(log_database, "Mysql query returned no result: %s", query.c_str());
				return false;
			}
			return true;
		}
	}
	server_log->Log(log_database, "Mysql query returned no result: %s", query.c_str());
	return false;
}
#pragma endregion

#pragma region Server Setup
bool Database::CreateServerSettings()
{
	server_log->Log(log_debug, "Entering Server Settings database setup.");

	if (!db)
	{
		server_log->Log(log_error, "MySQL Not connected.");
		return false;
	}

	server_log->Log(log_debug, "MySQL is connected, continuing.");

	string check_table_query = "SHOW TABLES LIKE 'tblloginserversettings'";

	if (mysql_query(db, check_table_query.c_str()) == 0)
	{
		//no idea why this fails.
		server_log->Log(log_debug, "Server Settings table does not exist, creating.");
		string create_table_query =
			"CREATE TABLE `tblloginserversettings` ("
			"`type` varchar(50) NOT NULL,"
			"`value` varchar(50) NOT NULL,"
			"`category` varchar(20) NOT NULL,"
			"`description` varchar(99) NOT NULL,"
			"`defaults` varchar(50) NOT NULL,"
			"PRIMARY KEY(`type`)"
			") ENGINE = InnoDB DEFAULT CHARSET = latin1";

		if (mysql_query(db, create_table_query.c_str()) != 0)
		{
			server_log->Log(log_error, "Mysql query failed: %s", create_table_query.c_str());
			return false;
		}
		server_log->Log(log_database, "Server Settings table created, continuing.");
	}

	if (mysql_query(db, check_table_query.c_str()) != 0)
	{
		server_log->Log(log_database, "Server Settings tables exist, continuing.");
	}

	//string check_settings_query = "SELECT * FROM `tblloginserversettings` WHERE `type`";

	//if (mysql_query(db, check_settings_query.c_str()) != 0)
	//{
	//	server_log->Log(log_database, "Server Settings table exists but settings tables do not, creating settings entries.");
	//	string query =
	//		"INSERT INTO `tblloginserversettings` VALUES ('access_log_table', '', 'schema', 'location for access logs, failed logins and goodIP.', 'tblaccountaccesslog');"
	//		"INSERT INTO `tblloginserversettings` VALUES('account_table', '', 'schema', 'location of all client account info for login server only.', 'tblLoginServerAccounts');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('auto_account_activate', '', 'options', 'set this to TRUE to allow new accounts to log in.', 'TRUE');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('auto_account_create', '', 'options', 'set this to TRUE to auto create accounts on player first log in.', 'TRUE');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('clientport', '', 'Old', 'port for clients to connect to.', '6000');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('dump_packets_in', '', 'options', 'debugging', 'FALSE');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('dump_packets_out', '', 'options', 'debugging', 'FALSE');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('failed_login_log', '', 'options', 'set this to TRUE to log failed log in attempts.', 'TRUE');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('good_loginIP_log', '', 'options', 'set this to TRUE to log successful account log ins.', 'TRUE');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('listen_port', '', 'options', 'this is the port we listen on for world connection.', '5998');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('local_network', '', 'options', 'set to the network ip that world server is on.', '127.0.0.1');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('mode', '', 'security', 'encryption mode the plugin uses.', '5');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('network_ip', '', 'options', 'set to the network ip that world server is on.', '127.0.0.1');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('opcodes', '', 'Old', 'opcode file for client compatibility. (Old means classic/mac)', 'login_opcodes_oldver.conf');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('plugin', '', 'security', 'the encryption type the login server uses.', 'EQEmuAuthCrypto');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('reject_duplicate_servers', '', 'options', 'set this to TRUE to force unique server name connections.', 'TRUE');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('salt', '', 'options', 'for account security make this a numeric random number.', '12345678');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('trace', '', 'options', 'debugging', 'FALSE');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('unregistered_allowed', '', 'options', 'set this to TRUE to allow any server to connect.', 'TRUE');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('world_admin_registration_table', '', 'schema', 'location of administrator account info for this login server.', 'tblServerAdminRegistration');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('world_registration_table', '', 'schema', 'location of registered or connection records of servers connecting to this loginserver.', 'tblWorldServerRegistration');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('world_server_type_table', '', 'schema', 'location of server type descriptions.', 'tblServerListType');"
	//		"INSERT INTO `tblloginserversettings` VALUES ('world_trace', '', 'options', 'debugging', 'FALSE');";

	//	if (mysql_query(db, query.c_str()) != 0)
	//	{
	//		server_log->Log(log_error, "Mysql query failed: %s", query.c_str());
	//		return false;
	//	}
	//}
	return true;
}
#pragma endregion
