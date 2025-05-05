#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

const string Clients = "Clients.txt";
const string Users = "Users.txt";

enum enChoices
{
	ShowClients = 1,
	AddClient = 2,
	DeleteClient = 3,
	UpdateClient = 4,
	FindClient = 5,
	Transactions = 6,
	ManageUsers = 7,
	Exit = 8
};

enum enTransactions
{
	enDeposit = 1,
	enWithdraw = 2,
	enTotalBalances = 3,
	enExit = 4
};

enum enManageUsers
{
	ShowUsers = 1,
	AddUser = 2,
	DeleteUser = 3,
	UpdateUser = 4,
	FindUser = 5,
	ExitUsers = 6
};

enum enPermitions
{
	enpShowClients = 1,
	enpAddClient = 2,
	enpDeleteClient = 4,
	enpUpdateClient = 8,
	enpFindClient = 16,
	enpTransactions = 32,
	enpManageUsers = 64,
};

struct stClient
{
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double Balance = 0;
};

struct stUser
{
	string Username = "";
	string Password = "";
	int Permissions = 0;
};

stUser LiveUser;

void switchTransactionScreen(enTransactions);

void switchMainMenue(enChoices);

void switchManageUsersMenue(enManageUsers);

void Run();

void backMainMenue();

bool readDecision(string messeage)
{
	char ch = 'Y';
	cout << messeage;
	cin >> ch;
	cin.ignore(1, '\n');
	return (ch != 'N' && ch != 'n');
}

string readString(string message)
{
	string str = "";
	cout << message;
	getline(cin, str);
	return str;
}

stClient readClient(string accountNumber)
{
	cout << "\n";
	stClient Client;
	Client.AccountNumber = accountNumber;
	cout << "Client With Account Number (" << Client.AccountNumber << ") " << "\n";
	cout << "Pin Code: ";
	getline(cin, Client.PinCode);
	cout << "Name: ";
	getline(cin, Client.Name);
	cout << "Phone: ";
	getline(cin, Client.Phone);
	cout << "Balance: ";
	string temp = "";
	getline(cin, temp);
	Client.Balance = stoi(temp);
	return Client;
}

stUser readUser(string username)
{
	cout << "\n";
	stUser User;
	User.Username = username;
	cout << "User Name With (" << User.Username << ") " << "\n";
	cout << "Password: ";
	getline(cin, User.Password);
	if (readDecision("Do you want to give full access [Y|N] ? "))
		User.Permissions = -1;
	else
	{
		cout << "Do you want to give access to:\n";
		if (readDecision("Show Client List [Y|N] ? "))
			User.Permissions += pow(2, 0);
		if (readDecision("Add New Client [Y|N] ? "))
			User.Permissions += pow(2, 1);
		if (readDecision("Delete Client [Y|N] ? "))
			User.Permissions += pow(2, 2);
		if (readDecision("Update Client [Y|N] ? "))
			User.Permissions += pow(2, 3);
		if (readDecision("Find Client [Y|N] ? "))
			User.Permissions += pow(2, 4);
		if (readDecision("Transactions [Y|N] ? "))
			User.Permissions += pow(2, 5);
		if (readDecision("Manage Users [Y|N] ? "))
			User.Permissions += pow(2, 6);
	}
	return User;
}

bool checkPermitions(short permitionToCheck)
{
	permitionToCheck = pow(2, permitionToCheck - 1);
	if (::LiveUser.Permissions == -1)
		return true;
	if ((permitionToCheck & ::LiveUser.Permissions) == permitionToCheck)
		return true;
	else
		return false;
}

bool checkPermitions2(short ch)
{
	ch = pow(2, ch - 1);
	short permition = ::LiveUser.Permissions;
	for (short i = 7; 0 <= i; i--)
	{
		if (permition == -1)
			return true;
		if (pow(2, i) <= permition)
		{
			if (permition == ch)
				return true;
			else if (ch == pow(2, i))
				return true;
			else
				permition -= pow(2, i);
		}
	}
	return false;
}

void printClientCard(stClient Client)
{

	cout << "\n------------------------------------\n";
	cout << "         ***Client**Card***";
	cout << "\n------------------------------------\n";
	cout << "Client With Account Number (" << Client.AccountNumber << ") " << "\n";
	cout << "Pin Code: " << Client.PinCode << "\n";
	cout << "Name: " << Client.Name << "\n";
	cout << "Phone: " << Client.Phone << "\n";
	cout << "Balance: " << Client.Balance << "\n";
	cout << "------------------------------------\n\n";
}

void printUserCard(stUser User)
{
	cout << "\n------------------------------------\n";
	cout << "         ***User**Card***";
	cout << "\n------------------------------------\n";
	cout << "User Name (" << User.Username << ") " << "\n";
	cout << "Pin Code: " << User.Password << "\n";
	cout << "Name: " << User.Permissions << "\n";
	cout << "------------------------------------\n\n";
}

string structureToLine(stClient Client, string delim = "#//#")
{
	string str = "";
	str += Client.AccountNumber + delim;
	str += Client.PinCode + delim;
	str += Client.Name + delim;
	str += Client.Phone + delim;
	str += to_string(Client.Balance);
	return str;
}

string structureToLineUser(stUser User, string delim = "#//#")
{
	string str = "";
	str += User.Username + delim;
	str += User.Password + delim;
	str += to_string(User.Permissions);
	return str;
}

vector<string> spilt(string str, string delim = " ")
{
	vector<string> vec = {};
	int pos = 0;
	string word = "";
	while ((pos = str.find(delim)) != std::string::npos)
	{
		word = str.substr(0, pos);
		if (word != "")
			vec.push_back(word);
		str.erase(0, pos + delim.length());
	}
	if (str != "")
		vec.push_back(str);
	return vec;
}

stClient lineToStructure(string line)
{
	vector<string> vec = spilt(line, "#//#");
	stClient Client;
	Client.AccountNumber = vec[0];
	Client.PinCode = vec[1];
	Client.Name = vec[2];
	Client.Phone = vec[3];
	Client.Balance = stod(vec[4]);
	return Client;
}

stUser lineToStructureUser(string line)
{
	vector<string> vec = spilt(line, "#//#");
	stUser User;
	User.Username = vec[0];
	User.Password = vec[1];
	User.Permissions = stoi(vec[2]);
	return User;
}

vector<stClient> loadFileData(string fileName)
{
	vector<stClient> vecClinent {};
	stClient Client;
	fstream MyFile;
	string line = "";
	MyFile.open(fileName, ios::in);
	if (MyFile.is_open())
	{
		while (getline(MyFile, line))
		{
			Client = lineToStructure(line);
			vecClinent.push_back(Client);
		}
	}
	MyFile.close();
	return vecClinent;
}

vector<stUser> loadFileDataUser(string fileName)
{
	vector<stUser> vec {};
	stUser User;
	fstream MyFile;
	string line = "";
	MyFile.open(fileName, ios::in);
	if (MyFile.is_open())
	{
		while (getline(MyFile, line))
		{
			User = lineToStructureUser(line);
			vec.push_back(User);
		}
	}
	MyFile.close();
	return vec;
}

bool isExist(string fileName, string accountNumber)
{
	vector<stClient> vClients = loadFileData(fileName);
	for (stClient Client : vClients)
	{
		if (Client.AccountNumber == accountNumber)
			return true;
	}
	return false;
}

bool isExistUser(string fileName, string username)
{
	vector<stUser> vUsers = loadFileDataUser(fileName);
	for (stUser User : vUsers)
	{
		if (User.Username == username)
			return true;
	}
	return false;
}

void addNewClient(string fileName)
{
	stClient Client;
	Client.AccountNumber = readString("Account Number: ");
	if (isExist(fileName, Client.AccountNumber))
	{
		cout << "Account Number (" << Client.AccountNumber << ") is already exist.\n\n";
		return;
	}
	Client = readClient(Client.AccountNumber);
	string line = structureToLine(Client);
	fstream MyFile;
	MyFile.open(fileName, ios::app);
	if (MyFile.is_open())
	{
		MyFile << line << "\n";
	}
	MyFile.close();
	cout << "Add Client with Account Number (" << Client.AccountNumber << ") is successfully.\n";

}

void addNewUser(string fileName)
{
	stUser User;
	User.Username = readString("Username: ");
	if (isExistUser(fileName, User.Username))
	{
		cout << "Username (" << User.Username << ") is already exist.\n\n";
		return;
	}
	User = readUser(User.Username);
	string line = structureToLineUser(User);
	fstream MyFile;
	MyFile.open(fileName, ios::app);
	if (MyFile.is_open())
	{
		MyFile << line << "\n";
	}
	MyFile.close();
	cout << "Add User with Username (" << User.Username << ") is successfully.\n";
}

void addClients(string fileName)
{
	char awnser = 'Y';
	while (awnser == 'Y' || awnser == 'y')
	{
		addNewClient(fileName);
		cout << "\n\nDo you want add more clients [Y|N]? ";
		cin >> awnser;
		cin.ignore(1, '\n');
	}
}

void addUsers(string fileName)
{
	char awnser = 'Y';
	while (awnser == 'Y' || awnser == 'y')
	{
		addNewUser(fileName);
		cout << "\n\nDo you want add more Users [Y|N]? ";
		cin >> awnser;
		cin.ignore(1, '\n');
	}
}

void showClients(string fileName)
{
	vector<stClient> vecClient = loadFileData(fileName);
	cout << setw(40) << left << "" << "Client List (" << vecClient.size() << ") Client(s).\n";
	cout << "_____________________________________________________";
	cout << "___________________________________________________\n";
	cout << "| " << setw(18) << left << "Account Number";
	cout << "| " << setw(12) << left << "Pin Code";
	cout << "| " << setw(40) << left << "Client Name";
	cout << "| " << setw(12) << left << "Phone";
	cout << "| " << setw(11) << left << "Balance" << "|\n";
	cout << "_____________________________________________________";
	cout << "___________________________________________________\n";
	for (stClient Client : vecClient)
	{
		cout << "| " << setw(18) << left << Client.AccountNumber;
		cout << "| " << setw(12) << left << Client.PinCode;
		cout << "| " << setw(40) << left << Client.Name;
		cout << "| " << setw(12) << left << Client.Phone;
		cout << "| " << setw(11) << left << Client.Balance << "|\n";
	}
	cout << "_____________________________________________________";
	cout << "___________________________________________________\n";
}

void showUsers(string fileName)
{
	vector<stUser> vecUser = loadFileDataUser(fileName);
	cout << setw(40) << left << "" << "User List (" << vecUser.size() << ") User(s).\n";
	cout << "_____________________________________________________";
	cout << "___________________________________________________\n";
	cout << "| " << setw(18) << left << "Username";
	cout << "| " << setw(12) << left << "Password";
	cout << "| " << setw(67) << left << "Permissions" << "|\n";
	cout << "_____________________________________________________";
	cout << "___________________________________________________\n";
	for (stUser User : vecUser)
	{
		cout << "| " << setw(18) << left << User.Username;
		cout << "| " << setw(12) << left << User.Password;
		cout << "| " << setw(67) << left << User.Permissions << "|\n";
	}
	cout << "_____________________________________________________";
	cout << "___________________________________________________\n";
}

void clearAllFileData(string fileName)
{
	fstream MyFile;
	MyFile.open(fileName, ios::out);
	if (MyFile.is_open())
		MyFile << "";
	MyFile.close();
}

void deleteClient(string fileName)
{
	string accountNumber = readString("Account Number: ");
	if (!isExist(fileName, accountNumber))
	{
		cout << "Client with Account Number (" << accountNumber << ") is not found!\n";
		return;
	}
	vector<stClient> vClients = loadFileData(fileName);
	vector<stClient> newClients;
	for (stClient Client : vClients)
	{
		if (Client.AccountNumber == accountNumber)
		{
			cout << "\n";
			printClientCard(Client);
			char awnser;
			cout << "\nDo you want delete Clinet with Account Number (" << Client.AccountNumber << ")? ";
			cin >> awnser;
			cin.ignore(1, '\n');
			if (!(awnser == 'Y' || awnser == 'y'))
				return;
		}
		else
			newClients.push_back(Client);

	}
	clearAllFileData(fileName);
	fstream MyFile;
	MyFile.open(fileName, ios::out);
	if (MyFile.is_open())
	{
		for (stClient Client : newClients)
		{
			string line = structureToLine(Client);
			MyFile << line << "\n";
		}
	}
	MyFile.close();
	cout << "Delete Client with Account Number (" << accountNumber << ") is successfully.\n";
}

void deleteUser(string fileName)
{
	string username = readString("Username: ");
	if (!isExistUser(fileName, username))
	{
		cout << "User with Username (" << username << ") is not found!\n";
		return;
	}
	if (username == "Admin")
	{
		cout << "Cann't Delete Admin User";
		return;
	}
	vector<stUser> vUsers = loadFileDataUser(fileName);
	vector<stUser> newUsers;
	for (stUser User : vUsers)
	{
		if (User.Username == username)
		{
			cout << "\n";
			printUserCard(User);
			char awnser;
			cout << "\nDo you want delete User with Username (" << User.Username << ")? ";
			cin >> awnser;
			cin.ignore(1, '\n');
			if (!(awnser == 'Y' || awnser == 'y'))
				return;
		}
		else
			newUsers.push_back(User);

	}
	clearAllFileData(fileName);
	fstream MyFile;
	MyFile.open(fileName, ios::out);
	if (MyFile.is_open())
	{
		for (stUser User : newUsers)
		{
			string line = structureToLineUser(User);
			MyFile << line << "\n";
		}
	}
	MyFile.close();
	cout << "Delete User with Username (" << username << ") is successfully.\n";
}

void updateClient(string fileName)
{
	string accountNumber = readString("Account Number: ");
	if (isExist(fileName, accountNumber))
	{
		vector<stClient> vClients = loadFileData(fileName);
		vector<stClient> newClients;
		for (stClient Client : vClients)
		{
			if (Client.AccountNumber == accountNumber)
			{
				char awnser;
				printClientCard(Client);
				cout << "\nDo you want update Client with Account Number (" << accountNumber << ")? ";
				cin >> awnser;
				cin.ignore(1, '\n');
				if (!(awnser == 'Y' || awnser == 'y'))
					return;
				Client = readClient(accountNumber);
				newClients.push_back(Client);
			}
			else
			{
				newClients.push_back(Client);
			}
		}
		clearAllFileData(fileName);
		fstream MyFile;
		MyFile.open(fileName, ios::app);
		if (MyFile.is_open())
		{
			for (stClient Client : newClients)
			{
				string line = structureToLine(Client);
				MyFile << line << "\n";
			}
		}
		MyFile.close();
		cout << "update Client with Account Number (" << accountNumber << ") is successfully.\n";
	}
	else
	{
		cout << "\nClient with Account Number (" << accountNumber << ") is not exist.\n";
	}
}

void updateUser(string fileName)
{
	string username = readString("Username: ");
	if (isExistUser(fileName, username))
	{
		vector<stUser> vUsers = loadFileDataUser(fileName);
		vector<stUser> newUsers;
		for (stUser User : vUsers)
		{
			if (User.Username == username)
			{
				char awnser;
				printUserCard(User);
				cout << "\nDo you want update User with Username (" << username << ")? ";
				cin >> awnser;
				cin.ignore(1, '\n');
				if (!(awnser == 'Y' || awnser == 'y'))
					return;
				User = readUser(username);
				newUsers.push_back(User);
			}
			else
			{
				newUsers.push_back(User);
			}
		}
		clearAllFileData(fileName);
		fstream MyFile;
		MyFile.open(fileName, ios::app);
		if (MyFile.is_open())
		{
			for (stUser User : newUsers)
			{
				string line = structureToLineUser(User);
				MyFile << line << "\n";
			}
		}
		MyFile.close();
		cout << "update User with Username (" << username << ") is successfully.\n";
	}
	else
	{
		cout << "\nUser with Username (" << username << ") is not exist.\n";
	}
}

int readChoice(string M)
{
	int cho = 0;
	cout << M;
	cin >> cho;
	cin.ignore(1, '\n');
	return cho;
}

stClient returnClient(string fileName, string accountNumber)
{
	vector<stClient> vClients = loadFileData(fileName);
	for (stClient& Client : vClients)
	{
		if (Client.AccountNumber == accountNumber)
			return Client;
	}
}

stUser returnUser(string fileName, string username)
{
	vector<stUser> vUsers = loadFileDataUser(fileName);
	for (stUser& User : vUsers)
	{
		if (User.Username == username)
			return User;
	}
}

void saveClientChanging(string fileName, stClient Client)
{
	vector<stClient> vClients = loadFileData(fileName);
	for (stClient& C : vClients)
	{
		if (C.AccountNumber == Client.AccountNumber)
		{
			C.PinCode = Client.PinCode;
			C.Name = Client.Name;
			C.Phone = Client.Phone;
			C.Balance = Client.Balance;
			break;
		}
	}
	fstream MyFile;
	MyFile.open(fileName, ios::out);
	if (MyFile.is_open())
	{
		for (stClient& C : vClients)
		{
			string line = structureToLine(C);
			MyFile << line << "\n";
		}
	}
	MyFile.close();
}

void saveUserChanging(string fileName, stUser User)
{
	vector<stUser> vUsers = loadFileDataUser(fileName);
	for (stUser& C : vUsers)
	{
		if (C.Username == User.Username)
		{
			C.Password = User.Password;
			C.Permissions = User.Permissions;
			break;
		}
	}
	fstream MyFile;
	MyFile.open(fileName, ios::out);
	if (MyFile.is_open())
	{
		for (stUser& C : vUsers)
		{
			string line = structureToLineUser(C);
			MyFile << line << "\n";
		}
	}
	MyFile.close();
}

void TransactionsMenue()
{
	system("cls");
	cout << "=============================================\n";
	cout << "         Transactions Menue Screen\n";
	cout << "=============================================\n";
	cout << "   [1] Deposit.\n";
	cout << "   [2] Withdraw.\n";
	cout << "   [3] Total Balances.\n";
	cout << "   [4] Main Menue.\n";
	cout << "=============================================\n";
	switchTransactionScreen((enTransactions)readChoice("Choose what do you want to do [1 to 4] : "));
}

void backTransactionsScreen()
{
	cout << "\n\nPress any key to go back to Transactins Menue...";
	system("pause>0");
	TransactionsMenue();
}

void depositTransaction(string fileName)
{
	cin.ignore(1, '\n');
	string accountNumber = readString("Account Number: ");
	if (isExist(fileName, accountNumber))
	{
		stClient Client = returnClient(fileName, accountNumber);
		printClientCard(Client);
		double amount = 0;
		cout << "\nEnter deposit amount? ";
		cin >> amount;
		cin.ignore(1, '\n');
		char awnser;
		cout << "\nAre you sure you want deposit (" << amount << ") for Account Number (" << accountNumber << ") ? ";
		cin >> awnser;
		cin.ignore(1, '\n');
		if (awnser == 'Y' || awnser == 'y')
		{
			Client.Balance += amount;
			saveClientChanging(fileName, Client);
			cout << "\nTransaction Successfully\n";
		}
	}
	else
		cout << "Client with Account Number (" << accountNumber << ") is not Exist!\n";
}

void withdrawTransaction(string fileName)
{
	cin.ignore(1, '\n');
	string accountNumber = readString("Account Number: ");
	if (isExist(fileName, accountNumber))
	{
		stClient Client = returnClient(fileName, accountNumber);
		printClientCard(Client);
		double amount = 0;
		cout << "\nEnter deposit amount? ";
		cin >> amount;
		cin.ignore(1, '\n');
		while (Client.Balance < amount)
		{
			cout << "Amount Exceeds the balance, you can withdraw up to : " << amount << "\n";
			cout << "\nEnter deposit amount? ";
			cin >> amount;
		}
		char awnser;
		cout << "\nAre you sure you want withdraw (" << amount << ") for Account Number (" << accountNumber << ") ? ";
		cin >> awnser;
		cin.ignore(1, '\n');
		if (awnser == 'Y' || awnser == 'y')
		{
			Client.Balance -= amount;
			saveClientChanging(fileName, Client);
			cout << "\nTransaction Successfully\n";
		}
	}
	else
		cout << "Client with Account Number (" << accountNumber << ") is not Exist!\n";
}

double totalBalances(string fileName)
{
	double sum = 0;
	vector<stClient> vClients = loadFileData(fileName);
	for (stClient Client : vClients)
	{
		sum += Client.Balance;
	}
	return sum;
}

void screenDepositTransaction(string fileName)
{
	system("cls");
	cout << "_______________________________________\n";
	cout << "           Deposit Screen\n";
	cout << "_______________________________________\n";
	depositTransaction(fileName);
	backTransactionsScreen();
}

void screenWithdrawTransaction(string fileName)
{
	system("cls");
	cout << "_______________________________________\n";
	cout << "           Withdraw Screen\n";
	cout << "_______________________________________\n";
	withdrawTransaction(fileName);
	backTransactionsScreen();
}

void screenTotalBalances(string fileName)
{
	system("cls");
	vector<stClient> vClients = loadFileData(fileName);
	cout << "\t\t\t     Balances List (" << vClients.size() << ") Client(s)\n";
	cout << "___________________________________________________________________________________\n";
	cout << "| " << setw(16) << left << "Account Number";
	cout << "| " << setw(40) << left << "Client Name";
	cout << "| " << setw(20) << left << "Balances" << "|\n";
	cout << "___________________________________________________________________________________\n";

	for (stClient Client : vClients)
	{
		cout << "| " << setw(16) << left << Client.AccountNumber;
		cout << "| " << setw(40) << left << Client.Name;
		cout << "| " << setw(20) << left << Client.Balance << "|\n";
	}
	cout << "___________________________________________________________________________________\n";

	cout << "\n\t\t\t________________________________________\n";
	cout << "\t\t\t| Total Balances : " << setw(20) << left << totalBalances(fileName) << "|";
	cout << "\n\t\t\t----------------------------------------\n\n\n";
	backTransactionsScreen();
}

void MainMenue()
{
	system("cls");
	cout << "=============================================\n";
	cout << "             Main Menue Screen\n";
	cout << "=============================================\n";
	cout << "   [1] Show Client List.\n";
	cout << "   [2] Add New Client.\n";
	cout << "   [3] Delete Client.\n";
	cout << "   [4] Update Client Info.\n";
	cout << "   [5] Find Client.\n";
	cout << "   [6] Transactions.\n";
	cout << "   [7] Manage Users.\n";
	cout << "   [8] Logout.\n";
	cout << "=============================================\n";
	enChoices ch = (enChoices)stoi(readString("Choose what do you want to do [1 to 8] : "));
	if (checkPermitions((int)ch) || ch == 8)
		switchMainMenue(ch);
	else
	{
		system("cls");
		cout << "__________________________\n";
		cout << "   **Permitin Denied** \n";
		cout << "__________________________\n";
		backMainMenue();
	}
}

void ManageUsersMenue()
{
	system("cls");
	cout << "=============================================\n";
	cout << "           Manage Users Menue Screen\n";
	cout << "=============================================\n";
	cout << "   [1] Show Users List.\n";
	cout << "   [2] Add New User.\n";
	cout << "   [3] Delete User.\n";
	cout << "   [4] Update User Info.\n";
	cout << "   [5] Find User.\n";
	cout << "   [6] Exit.\n";
	cout << "=============================================\n";
	switchManageUsersMenue((enManageUsers)stoi(readString("Choose what do you want to do [1 to 6] : ")));
}


void backMainMenue()
{
	cout << "\nPress any key to go back to Main Menue...";
	system("pause>0");
	MainMenue();
}

void backManageUsersMenue()
{
	cout << "\nPress any key to go back to Manage Users Menue...";
	system("pause>0");
	ManageUsersMenue();
}

void screenShowClients(string fileName)
{
	system("cls");
	showClients(fileName);
	backMainMenue();
}

void screenShowUsers(string fileName)
{
	system("cls");
	showUsers(fileName);
	backManageUsersMenue();
}

void screenAddClients(string fileName)
{
	system("cls");
	cout << "_____________________________________\n";
	cout << "       Add New Clients Screen\n";
	cout << "_____________________________________\n";
	addClients(fileName);
	backMainMenue();
}

void screenAddUsers(string fileName)
{
	system("cls");
	cout << "_____________________________________\n";
	cout << "       Add New Users Screen\n";
	cout << "_____________________________________\n";
	addUsers(fileName);
	backManageUsersMenue();
}

void screenDeleteClient(string fileName)
{
	system("cls");
	cout << "_____________________________________\n";
	cout << "       Delete Client Screen\n";
	cout << "_____________________________________\n";
	deleteClient(fileName);
	backMainMenue();
}

void screenDeleteUser(string fileName)
{
	system("cls");
	cout << "_____________________________________\n";
	cout << "       Delete User Screen\n";
	cout << "_____________________________________\n";
	deleteUser(fileName);
	backManageUsersMenue();
}

void screenUpdateClient(string fileName)
{
	system("cls");
	cout << "_____________________________________\n";
	cout << "       Update Client Screen\n";
	cout << "_____________________________________\n";
	updateClient(fileName);
	backMainMenue();
}

void screenUpdateUser(string fileName)
{
	system("cls");
	cout << "_____________________________________\n";
	cout << "       Update User Screen\n";
	cout << "_____________________________________\n";
	updateUser(fileName);
	backManageUsersMenue();
}

void screenFindClient(string fileName)
{
	system("cls");
	cout << "___________________________________________________\n";
	cout << "             Find Client Screen\n";
	cout << "___________________________________________________\n";
	string accountNumber = readString("Account Number for Client to be is find: ");
	if (isExist(fileName, accountNumber))
	{
		cout << "\nClient with Account Number (" << accountNumber << ") is exist.\n";
		stClient Client = returnClient(fileName, accountNumber);
		printClientCard(Client);
	}
	else
	{
		cout << "\nClient with Account Number (" << accountNumber << ") is not exist.\n\n";
	}
	backMainMenue();
}

void screenFindUser(string fileName)
{
	system("cls");
	cout << "___________________________________________________\n";
	cout << "             Find User Screen\n";
	cout << "___________________________________________________\n";
	string username = readString("Username for User to be is find: ");
	if (isExistUser(fileName, username))
	{
		cout << "\nUser with Username (" << username << ") is exist.\n";
		stUser User = returnUser(fileName, username);
		printUserCard(User);
	}
	else
	{
		cout << "\nUser with Username (" << username << ") is not exist.\n\n";
	}
	backManageUsersMenue();
}

void switchMainMenue(enChoices choice)
{
	switch (choice)
	{
	case enChoices::ShowClients:
		screenShowClients(::Clients);
		break;
	case enChoices::AddClient:
		screenAddClients(::Clients);
		break;
	case enChoices::DeleteClient:
		screenDeleteClient(::Clients);
		break;
	case enChoices::UpdateClient:
		screenUpdateClient(::Clients);
		break;
	case enChoices::FindClient:
		screenFindClient(::Clients);
		break;
	case enChoices::Transactions:
		TransactionsMenue();
		break;
	case enChoices::ManageUsers:
		ManageUsersMenue();
		break;
	case enChoices::Exit:
		Run();
		break;
	default:
		screenShowClients(::Clients);
		break;
	}
}

void switchManageUsersMenue(enManageUsers m)
{
	switch (m)
	{
	case enManageUsers::ShowUsers:
		screenShowUsers(::Users);
		break;
	case enManageUsers::AddUser:
		screenAddUsers(::Users);
		break;
	case enManageUsers::DeleteUser:
		screenDeleteUser(::Users);
		break;
	case enManageUsers::UpdateUser:
		screenUpdateUser(::Users);
		break;
	case enManageUsers::FindUser:
		screenFindUser(::Users);
		break;
	case enManageUsers::ExitUsers:
		MainMenue();
		break;
	default:
		screenShowUsers(::Users);
		break;
	}
}

void switchTransactionScreen(enTransactions t)
{
	switch (t)
	{
	case enTransactions::enDeposit:
		screenDepositTransaction(::Clients);
		break;
	case enTransactions::enWithdraw:
		screenWithdrawTransaction(::Clients);
		break;
	case enTransactions::enTotalBalances:
		screenTotalBalances(::Clients);
		break;
	case enTransactions::enExit:
		MainMenue();
		break;
	}
}

bool isValidLogin(string username, string password)
{
	stUser User;
	if (isExistUser(::Users, username))
	{
		User = returnUser(::Users, username);
		if (User.Password == password)
		{
			::LiveUser = User;
			return true;
		}
	}
	return false;
}

void login()
{
	system("cls");
	cout << "________________________________\n";
	cout << "            Login\n";
	cout << "________________________________\n";
	string username = readString("Username: ");
	string password = readString("Password: ");
	while (!isValidLogin(username, password))
	{
		system("cls");
		cout << "________________________________\n";
		cout << "            Login\n";
		cout << "________________________________\n";
		cout << "Invalid Username / Password!" << "\n";
		username = readString("Username: ");
		password = readString("Password: ");
	}
}

void Run()
{
	login();
	MainMenue();
}

int main()
{
	Run();
	return 0;
}