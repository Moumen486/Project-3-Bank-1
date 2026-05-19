#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<iomanip>

using namespace std;
const string ClientFileName = "Client.txt";

void ShowMainMenu();	

struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};
vector<string> SplitString(string S1, string Delim)
{
	vector<string> vstring;

	short pos = 0;
	string sWord; //define a string varaible

	//use find() function to gt the position of the delimitrs
	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos);// store the word
		if (sWord != "")
		{
			vstring.push_back(sWord);
		}
		S1.erase(0, pos + Delim.length()); // erase () until position 
	}
	if (S1 != "")
	{
		vstring.push_back(S1);
	}

	return vstring;
}
sClient ConvertLineToRecord(string Line, string Separtor = "#//#")
{
	sClient Client;
	vector<string> vClientData;

	vClientData = SplitString(Line, Separtor);

	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);//cast string to double

	return Client;
}
string ConvertRecordToLine(sClient Client, string Separtor = "#//#")
{
	string stClientRecord = "";

	stClientRecord += Client.AccountNumber + Separtor;
	stClientRecord += Client.PinCode + Separtor;
	stClientRecord += Client.Name + Separtor;
	stClientRecord += Client.Phone + Separtor;
	stClientRecord += to_string(Client.AccountBalance);

	return stClientRecord;
}
vector<sClient> LoadClientsDaraFromFile(string FileName)
{
	vector <sClient> vClient;

	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode

	if (MyFile.is_open())
	{

		string Line;
		sClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			vClient.push_back(Client);
		}
		MyFile.close();
	}

	return vClient;
}
bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
	vector <sClient> vClient;
	fstream MyFile;

	MyFile.open(FileName, ios::in);//read mode

	if (MyFile.is_open())
	{
		string Line;
		sClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			vClient.push_back(Client);
		}
		MyFile.close();
	}
	return false;
}
sClient ChangeClientRecord(string AccountNumber)
{
	sClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "\n\n Entre PinCode:";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name: ";
	getline(cin, Client.Name);

	cout << "Enter Phone: ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance: ";
	cin >> Client.AccountBalance;

	return Client;


}	
void PrintClientCard(sClient Client)
{

	cout << "\nThe following are the client details:\n";
	cout << "\nAccount Number: " << Client.AccountNumber;
	cout << "\nPin Code: " << Client.PinCode;
	cout << "\nName: " << Client.Name;
	cout << "\nPhone Number: " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
}
bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClient, sClient& Client)
{

	for (sClient C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}

	}

	return false;
}
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClient)
{
	for (sClient& C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}
vector <sClient> SaveClientDataToFile(string FileName, vector <sClient> vClient)	
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite;

	string DataLine;

	if (MyFile.is_open())
	{
		for (sClient& C : vClient)
		{
			if (C.MarkForDelete == false)
			{
				// we only write records that are not marked for delete
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}

		MyFile.close();
	}
	return vClient;
}
bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClient)
{
	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		PrintClientCard(Client);

		cout << "\n Do Want Delete this Client? N/Y";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClient);
			SaveClientDataToFile(ClientFileName, vClient);

			//Refresh Clients
			vClient = LoadClientsDaraFromFile(ClientFileName);

			cout << "\n\nClient Deleted Successfully.";
			return true;

		}
	}
	else
	{
		cout << "\nClient With Acoount Number (" << AccountNumber << ") is not found";
		return false;
	}
	return false;
}
bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClient)
{
	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		PrintClientCard(Client);

		cout << "\n Do Want Update this Client? N/Y";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			for (sClient& C : vClient)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}

			SaveClientDataToFile(ClientFileName, vClient);

			cout << "\n\nClient Update Successfulyy:";
			return true;
		}
	}
	else
	{
		cout << "\nClient With Acoount Number (" << AccountNumber << ") is not found";
		return false;
	}

}
sClient ReadNewClient()
{
	sClient Client;

	cout << "Please entre your Account Number: ";

	// usage of std ::ws will extract all the whitespaces charachter
	getline(cin >> ws, Client.AccountNumber);

	while (ClientExistsByAccountNumber(Client.AccountNumber, ClientFileName))
	{
		cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number: \n";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << " Entre PinCode: ";
	getline(cin, Client.PinCode);

	cout << " Entre Your Name: ";
	getline(cin, Client.Name);

	cout << "Entre Your Phone: ";
	getline(cin, Client.Phone);

	cout << " Your AccountBalance :";
	cin >> Client.AccountBalance;

	return Client;
}
void AddDataLineToFile(string FileName, string stDataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << stDataLine << endl;
		MyFile.close();
	}

}
void AddNewClient()
{
	sClient Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientFileName, ConvertRecordToLine(Client));

}
void AddNewClients()
{
	char AddMore = 'Y';
	do
	{
		//system("cls");
		cout << "Adding New Client:\n\n";
		AddNewClient();

		cout << "\nClient Added Successfully, do you want to add more clients? Y/N";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}
void ShowAddNewClientsScreen()
{
	cout << "\n----------------------------\n";
	cout << "\tAdd New Clients Screen";
	cout << "\n-----------------------------\n";
	AddNewClients();

}
string ReadClientAccountNumber()
{
	string AccountNumber = "";

	cout << "\nPlease enter AccountNumber:\n";
	cin >> AccountNumber;
	return AccountNumber;
}
void ShowDeleteClientScreen()
{
	cout << "\n----------------------------------------------\n";
	cout << "\tDelete Client Screen";
	cout << "\n_______________________________________________\n";

	vector <sClient> vClient = LoadClientsDaraFromFile(ClientFileName);
	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClient);

}
void ShowUpdateClientScreen()
{
	cout << "\n------------------------------------\n";
	cout << "\tUpdate Client Info Screen ";
	cout << "\n-------------------------------------\n";

	vector <sClient> vClient = LoadClientsDaraFromFile(ClientFileName);
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClient);
}
void ShowFindClientScreen()
{
	cout << "\n---------------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n----------------------------------\n";

	vector <sClient> vClient = LoadClientsDaraFromFile(ClientFileName);
	sClient Client;
	string AccountNumber = ReadClientAccountNumber();
	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
		PrintClientCard(Client);
	else
		cout << "\nClient With Account Number [" << AccountNumber << "] is not found! ";

}
void ShowEndScreen()
{
	cout << "\n----------------------------------\n";
	cout << "\tProgram Ends :)";
	cout << "\n-----------------------------------\n";
}
void PrintClientRecord(sClient Client)
{

	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(15) << left << Client.PinCode;
	cout << "| " << setw(20) << left << Client.Name;
	cout << "| " << setw(15) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;

}
void ShowAllClientsScreen()
{
	vector <sClient> vClient = LoadClientsDaraFromFile(ClientFileName);

	cout << "\n\t\t\t\t\tClient List (" << vClient.size() << ") Client(s).";
	cout << "\n___________________________________________________________________________________\n";
	cout << "\n____________________________________________________________________________________\n";

	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(15) << "Pin Code";
	cout << "| " << left << setw(20) << "Client Name";
	cout << "| " << left << setw(15) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n";
	cout << "\n____________________________________________________________________________________\n";
	cout << "__________________________________________________________________________________________\n";
	if (vClient.size() == 0)
		cout << "\t\t\t\tNo Clients Available in the system :(! ";
	else

	for (sClient Client : vClient)
	{
		PrintClientRecord(Client);
		cout << endl;
	}
	cout << "\n___________________________________________________________________________________________\n";
	cout << "\n___________________________________________________________________________________________\n";

}
enum enMainMenuOptions
{
	eListClients = 1, eAddNewClient = 2,
	eDeleteClient = 3, eUpdateClient = 4,
	eFindClient = 5, eExit = 6
};
void GoBackToMainMenu()
{
	cout << "\n\nPress any key to go back to the main menu...";
	system("pause>0");
	ShowMainMenu();
}
short ReadMainMenuOption()
{
	short Choice = 0;
	cout << "Choose what do you want to do ?[1 to 6]?";
	cin >> Choice;

	return Choice;
}
void PerformMainMenuOption(enMainMenuOptions MainMenuOption)
{
	switch (MainMenuOption)
	{
	case enMainMenuOptions::eListClients:
	{
		system("cls");
		ShowAllClientsScreen();
		GoBackToMainMenu();
		break;
	}
	case enMainMenuOptions::eAddNewClient:
		system("cls");
		ShowAddNewClientsScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eDeleteClient:
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eUpdateClient:
		system("cls");
		ShowUpdateClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eFindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eExit:
		system("cls");
		ShowEndScreen();
			break;
	}
	

}
void ShowMainMenu()
{
	system("cls");
	cout << "_-_-_-_-________________________________-_-_-_-_-_-\n";
	cout << "\t\tMain Menu Screen\n";
	cout << "_-_-_-_-________________________________-_-_-_-_-_-\n";
	cout << "\t[1] Show Clients List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Exit.\n";
	cout << "_-_-_-_-________________________________-_-_-_-_-_-\n";
	PerformMainMenuOption((enMainMenuOptions)ReadMainMenuOption());

}
int main()
{

	ShowMainMenu();
	system("pause>0");
	return 0;
}