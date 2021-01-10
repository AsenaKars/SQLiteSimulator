#include<iostream>
#include<string>
#include<cstring>
#include <string>
#include <stdio.h>
#include <vector>
#include <algorithm>


using namespace std;

//if methods return 1 => successful
//else => return values bigger than 2, or throws exceptions


//clasa Exceptions - gestioneaza erori ...
class other_exception : public exception {
public:
	other_exception() {
	}
	other_exception(const char* message) : exception(message) {
	}
};

class CommandIdentifier
{
private:
	string userInput = "";
	int commandMenuSize = 7;
	string commandMenu[7] = { "CREATE", "DROP","DISPLAY","INSERT","DELETE","SELECT","UPDATE" };
	string foundCommand = "";

	//friend class Table;

	//create
	string numeTabel;

	string* numeColoane = new string[5];
	string* tipColoane = new string[5];
	int* dimensiuneColoane = new int[5];
	string* valImplicitaColoane = new string[5];
	int index = 0;

	//insert
	string* values;	 //vector de string uri alocat dinamic
	int indexValues = 0;

	string* packageDelete = new string[2];

	//update
	string* packageUpdate = new string[2]; //vector de string uri alocat dinamic
	string* packageWhere = new string[2]; //vector de string uri alocat dinamic

public:

	string extractCommand(string userInput)
	{
		if (userInput != "")
		{
			this->userInput = userInput;
		}
		else return foundCommand = "";

		string InputCommand = "";

		const char delimitator[] = " ";

		size_t token = this->userInput.find(delimitator, 0); //token =  the index of the first occurance of delimitator from tha start of the string
		InputCommand = this->userInput.substr(0, token); //extrage totul pana la pozitia delimitatorului

		for (int i = 0; i < commandMenuSize; i++)
		{
			if (InputCommand == commandMenu[i])
			{
				foundCommand = commandMenu[i];
				return foundCommand;
			}
		}
		return foundCommand;
	}

	int validateCreateTable(string userInput)
	{
		string InputCommand = userInput;
		char delimitator[] = ",()";
		size_t token = userInput.find(" ", 0);	//finds 'CREATE'
		InputCommand.erase(0, token + 1);
		token = InputCommand.find(" ", 0); //finds 'TABLE'
		//int InputCommandSize = InputCommand.length();


		if (InputCommand.substr(0, token) == "TABLE")
		{
			InputCommand.erase(0, token); //remove 'TABLE'
			InputCommand.erase(remove(InputCommand.begin(), InputCommand.end(), ' '), InputCommand.end()); //to work easier and without errors with our syntax, we remove spaces from the command's line

			token = InputCommand.find("(", 0);

			this->numeTabel = InputCommand.substr(0, token);
			cout << this->numeTabel << endl;
			InputCommand.erase(0, token); //we remove the tableName from our command 


			if (InputCommand.substr(0, 1) == "(" && InputCommand.substr(InputCommand.length() - 1, InputCommand.length()) == ")")
			{
				InputCommand.erase(0, 1); //removes the outer '('
				InputCommand.erase(InputCommand.length() - 1, InputCommand.length()); //removes last ')'

				token = InputCommand.find(')', 0);	// gets the end index of the first column entered in the command

				string columnItem = InputCommand.substr(0, token + 1); // retine intreaga instructiune pt coloana, inclusiv parantezele ei
				InputCommand.erase(0, token + 1); //elimin prima instructiune tip coloana din comanda


				if (columnsyntax_verifier(columnItem) == 1) //dc prima instrc coloana e valida
				{
					while (InputCommand.empty() != 0) //verific daca are succesor; exista rest din comanda, dupa eliminarea primei instructiuni tip coloana
					{
						//while (InputCommand.find('(', 0) != 0) //cat timp restul nu e si ultimul, unicul, e cel putin penultimul

						token = InputCommand.find(')', 0);
						columnItem = InputCommand.substr(0, token + 1);
						InputCommand.erase(0, token);	//sterg continutul columnItem din inputCommand
						if (columnsyntax_verifier(columnItem) == 0) {
							//return 5; //wrong syntax
							throw other_exception("wrong syntax");
						}

						return 1; // every test was successful! the string validates all tests
					}
					return 1; // every test was successful!
				}
				//return 4; // prima instructiune nu a fost valida
				throw other_exception("first column input selection not valid");
			}
			//return 3; // wrong syntax, maybe it doesn't have enclosed brackets
			throw other_exception("wrong syntax or brackets not enclosed");
		}
		//return 2; // key word "TABLE" not found!
		throw other_exception("keyword not found");
	}

	int columnsyntax_verifier(string columnItem)
	{
		size_t commaPlacement;

		if (columnItem.substr(0, 1) == ",")
			columnItem.erase(0, 1);

		if (columnItem.substr(0, 1) == "(" && columnItem.substr(columnItem.length() - 1, columnItem.length()) == ")")
		{
			columnItem.erase(0, 1); //remove '('
			columnItem.erase(columnItem.length() - 1, columnItem.length()); //remove ')'

			commaPlacement = columnItem.find(',', 0); // here ends the column's name //indexul primei aparitii a virgulei

			numeColoane[index] = columnItem.substr(0, commaPlacement + 1);		//populate DATA
			columnItem.erase(0, commaPlacement + 1); //removes columnName

			commaPlacement = columnItem.find(',', 0); // contine tip // here ends the type
			//we only accept text, integers and floar types
			//the implicit value must be of the same type! --> TODO 

			string type = columnItem.substr(0, commaPlacement); // tip
			tipColoane[index] = columnItem.substr(0, commaPlacement);		//populate DATA

			columnItem.erase(0, commaPlacement + 1); //removes tip

			// next we must verify the size given. It must be of type integer 

			commaPlacement = columnItem.find(',', 0); //contine size
			int size = isNumber(columnItem.substr(0, commaPlacement - 1)); //isNumber returns a number, if convertible to one -- toNumber

			dimensiuneColoane[index] = isNumber(columnItem.substr(0, commaPlacement - 1));		//populate DATA

			if (size == -1)
			{
				//return 11; // syntax error; the size is not a number
				throw other_exception("size is not a number");
			}
			columnItem.erase(0, commaPlacement); //removes size
			string implicitValue = columnItem;
			valImplicitaColoane[index] = columnItem;	//populare DATA

			index++;		//populate DATA

			//now we must verify if the data type is valid & if the implicitValue is of the same type and size
			if (type == "TEXT")
			{
				if (size < (int)implicitValue.length()) {
					//return 10;// size can't fit the implicit value!
					throw other_exception("size can't fit the implicit value");
				}

				//else we should store all data in the columns
				return 1;
			}
			else
				if (type == "INTEGER")
				{
					if (isNumber(implicitValue) == -1) {//if implicit value can't convert to number
						//return 9; // type doesn't match the value given
						throw other_exception("type doesn't match the value given");
					}

					if (size < (int)implicitValue.length()) {
						//return 8; // size can't fit the value given!
						throw other_exception("size can't fit the value given");
					}

					//if it passed all tests we should store all data
					return 1;
				}
				else
					if (type == "FLOAT")
					{
						//precizie de 1 - lungimea partii fractionare de 1

						//remove(implicitValue.begin(), implicitValue.end(), '.');
						//implicitValue(0, implicitValue.end());
						implicitValue.erase(remove(implicitValue.begin(), implicitValue.end(), '.'), implicitValue.end()); //we ignore the floating point for now
						if (isNumber(implicitValue) == -1) {
							//return 9; // type doesn't match the value given (not a number)
							throw other_exception("type doesn't match the value given");
						}
						if (size < (int)implicitValue.length()) {//size - nr caractere
							//return 8; // size can't fit the value given!
							throw other_exception("size can't fit the value given");
						}
						//if it passed all tests we should store all data
						return 1; //totul e ok
					}
					else {
						//return 7; // no suitable type entered
						throw other_exception("type enterd not compatible");
					}

		}
		//return 6; //column identifiers not between brackets!
		throw other_exception("column input selection not between brackets");
	}

	//returns the converted number
	//else, returns -1 if includes any on digit character;
	int isNumber(string numberGiven)
	{
		int number = 0;
		for (unsigned i = 0; i < numberGiven.length(); i++)
		{
			if (!(numberGiven[i] == '0' || numberGiven[i] == '1' || numberGiven[i] == '2' || numberGiven[i] == '3' || numberGiven[i] == '4' || numberGiven[i] == '5' || numberGiven[i] == '6' || numberGiven[i] == '7' || numberGiven[i] == '8' || numberGiven[i] == '9'))
				return -1;
			else number = number * 10 + (int)numberGiven[i];
		}
		return number;
	}

	int validateDropAndDisplayTable(string userInput) {
		string InputCommand = userInput;
		char delimitator[] = ",()";
		size_t token = userInput.find(" ", 0);	//finds 'CREATE'
		InputCommand.erase(0, token + 1);
		token = InputCommand.find(" ", 0); //finds 'TABLE'

		if (InputCommand.substr(0, token) == "TABLE")
		{
			InputCommand.erase(0, token + 1); //remove 'TABLE' -->' numetab'

			token = InputCommand.find(" ", 0);

			numeTabel = InputCommand.substr(0, token);			//populate DATA
			InputCommand.erase(0, token); //we remove the tableName from our command 

			if (!InputCommand.empty()) {
				return 5; //creeaza o legenda a erorilor
			}

			return 1;
		}
		return 2; // key word "TABLE" not found!
	}

	int validateInsert(string userInput, string* types, int* sizes, int noOfColumns) {
		string InputCommand = userInput;

		size_t token = userInput.find(" ", 0);	//finds 'INSERT'
		InputCommand.erase(0, token + 1);
		token = InputCommand.find(" ", 0); //finds 'INTO'

		if (InputCommand.substr(0, token) == "INTO")
		{
			InputCommand.erase(0, token + 1); //removes 'INTO'

			token = InputCommand.find(" ", 0); //indexul cuprinde si nume tabela

			this->numeTabel = InputCommand.substr(0, token);		//populate DATA
			InputCommand.erase(0, token + 1); //we remove the tableName from our command 

			InputCommand.erase(remove(InputCommand.begin(), InputCommand.end(), ' '), InputCommand.end()); //to work easier and without errors with our syntax, we remove spaces from the command's line

			token = InputCommand.find("(", 0); //finds 'VALUES'

			//InputCommand.erase(0, token);//

			//cout << InputCommand; //include doar (...)

			if (InputCommand.substr(0, token) == "VALUES") {

				InputCommand.erase(0, token); //removes 'VALUES' fara sa elimine paranteza
				//cout << InputCommand<<endl;

				if (InputCommand.substr(0, 1) == "(" && InputCommand.substr(InputCommand.length() - 1, InputCommand.length()) == ")")
				{
					InputCommand.erase(0, 1); //removes the outer '('
					InputCommand.erase(InputCommand.length() - 1, InputCommand.length()); //removes last ')'
					cout << InputCommand << endl;

					//if (columnsyntax_verifier(columnItem) == 1) //dc prima instrc coloana e valida e bine; altfel return eroare
					return syntaxVerifierInsert(InputCommand, types, sizes, noOfColumns);
					//return 1;

				//return 4;
				}
				return 3; // wrong syntax, maybe it doesn't have enclosed brackets
			}
			return 2; //key word 'VALUES' not found
		}
		return 2; // key word "INTO" not found!
	}

	//insert opereaza pe linie -> valori neomogene, parcurgere printre coloane
	int syntaxVerifierInsert(string columnValues, string* types, int* sizes, int noOfColumns) {
		//int noOfColumns = Table::noOfColumns;

		string InputCommand = columnValues;
		size_t token;

		values = new string[noOfColumns];

		while (InputCommand.empty() == false) {
			//for (int i = 0; i < noOfColumns; i++) {
			token = InputCommand.find(",", 0);		//index include prima valoare introdusa pt populare

			if (token != string::npos) { //dc gaseste virgula
				values[indexValues] = InputCommand.substr(0, token);	//adaug pe value in values[]
				indexValues++;

				InputCommand.erase(0, token + 1);	//elimin pe value 

			}
			else {//a ajuns la ultima valoare

				//cout << InputCommand << endl;
				values[indexValues] = InputCommand.substr(0, InputCommand.length());	//adaug pe value in values[]
				indexValues++;

				//InputCommand.erase(0, InputCommand.length() + 1);
				InputCommand.clear();
			}

		}
		//[DEBUG]
		/*
		for (int i = 0; i < indexValues; i++) { //NU CREEZA VECTOR
			cout << values[i] << " "<<endl;
		}
		*/


		//indexValues retine nr de values introduse
		if (indexValues != noOfColumns) {
			return 5; // numarul de valori date nu coincide cu numarul de coloane create 
		}

		for (int i = 0; i < indexValues; i++) {

			if (types[i] == "TEXT")
			{
				if (sizes[i] < (int)values[i].length())
					return 10;// size can't fit the implicit value!
			}
			else
				if (types[i] == "INTEGER")
				{
					if (isNumber(values[i]) == -1) //if implicit value can't convert to number
						return 9; // type doesn't match the value given

					if (sizes[i] < (int)values[i].length())
						return 8; // size can't fit the value given!
				}
				else
					if (types[i] == "FLOAT")
					{
						string copyValues = values[i];
						copyValues.erase(remove(copyValues.begin(), copyValues.end(), '.'), copyValues.end()); //we ignore the floating point for now
						if (isNumber(copyValues) == -1)
							return 9; // type doesn't match the value given (not a number)
						if (sizes[i] < (int)values[i].length()) //size - nr caractere
							return 8; // size can't fit the value given!

					}
					else
						return 7; // no suitable type entered
		}

		return 1;
	}

	int validateDelete(string userInput) {
		string InputCommand = userInput;
		size_t token = userInput.find(" ", 0);	//finds 'DELETE'
		InputCommand.erase(0, token + 1);
		token = InputCommand.find(" ", 0); //finds 'FROM'

		//string* packageDelete = new string[2]; //vector de string uri alocat dinamic
		//int indexValues = 0;

		if (InputCommand.substr(0, token) == "FROM")
		{
			InputCommand.erase(0, token + 1); //removes 'FROM'

			token = InputCommand.find(" ", 0); //indexul cuprinde si nume tabela

			this->numeTabel = InputCommand.substr(0, token);			//populate DATA
			InputCommand.erase(0, token + 1); //we remove the tableName from our command 


			token = InputCommand.find(" ", 0); //finds 'WHERE'

			if (InputCommand.substr(0, token) == "WHERE") {

				InputCommand.erase(0, token + 1); //removes 'WHERE' fara sa elimine paranteza

				InputCommand.erase(remove(InputCommand.begin(), InputCommand.end(), ' '), InputCommand.end()); //to work easier and without errors with our syntax, we remove spaces from the command's line

				token = InputCommand.find("=", 0); //index cuprinde date delete
				packageDelete[0] = InputCommand.substr(0, token);

				InputCommand.erase(0, token + 1);

				packageDelete[1] = InputCommand;


				for (int i = 0; i < 2; i++) {
					cout << packageDelete[i] << " " << endl;
				}

				return 1;

			}
			else
				return 2;
		}
		else
			return 2;
	}

	int validateUpdate(string userInput) {

		string InputCommand = userInput;
		size_t token = userInput.find(" ", 0);	//finds 'UPDATE'
		InputCommand.erase(0, token + 1);

		token = InputCommand.find(" ", 0); //indexul cuprinde si nume tabela

		this->numeTabel = InputCommand.substr(0, token);		//populate DATA
		InputCommand.erase(0, token + 1); //we remove the tableName from our command 

		token = InputCommand.find(" ", 0); //finds 'SET'

		if (InputCommand.substr(0, token) == "SET")
		{
			InputCommand.erase(0, token + 1); //removes 'SET'

			token = InputCommand.find("=", 0); //indexul cuprinde nume coloana cu tot cu spatiu

			cout << InputCommand << endl;

			packageUpdate[0] = InputCommand.substr(0, token - 1);//token-1
			InputCommand.erase(0, token + 2); //we remove the nume coloana plus pauza de dupa egal

			token = InputCommand.find(" ", 0); //index include valoare
			packageUpdate[1] = InputCommand.substr(0, token);

			InputCommand.erase(0, token + 1); //removes valoare

			token = InputCommand.find(" ", 0); //finds 'WHERE'

			cout << InputCommand << endl;


			if (InputCommand.substr(0, token) == "WHERE") {
				InputCommand.erase(0, token + 1); //removes 'WHERE' fara sa elimine paranteza

				InputCommand.erase(remove(InputCommand.begin(), InputCommand.end(), ' '), InputCommand.end()); //to work easier and without errors with our syntax, we remove spaces from the command's line

				token = InputCommand.find("=", 0); //index cuprinde date where
				packageWhere[0] = InputCommand.substr(0, token);

				InputCommand.erase(0, token + 1);

				packageWhere[1] = InputCommand;


				for (int i = 0; i < 2; i++) {
					cout << packageUpdate[i] << " ";
				}
				cout << endl;
				for (int i = 0; i < 2; i++) {
					cout << packageWhere[i] << " ";
				}

				return 1;
			}
			else return 2;
		}
		return 2;
	}

	//[for DEBUG]
	/*
	string getFoundCommand()
	{
		return foundCommand;
	}
	*/

	//constructor fara parametri
	CommandIdentifier() {
		userInput = "";
		foundCommand = "";

		if (packageDelete != nullptr) {
			packageDelete = new string[2];
			for (int i = 0; i < 2; i++) {
				packageDelete[i] = "";
			}
		}
	}

	CommandIdentifier(string userInput, string foundCommand) {

		this->userInput = userInput;
		this->foundCommand = foundCommand;

	}

	//constructor copiere
	CommandIdentifier(const CommandIdentifier& cmd) {
		this->userInput = cmd.userInput;
		this->foundCommand = cmd.foundCommand;
	}

	//op egal
	CommandIdentifier& operator=(const CommandIdentifier& cmd) {
		this->userInput = cmd.userInput;
		this->foundCommand = cmd.foundCommand;

		return *this;
	}

	//destructor
	~CommandIdentifier() {
		if (packageDelete != nullptr) {
			delete[] packageDelete;
		}
	}

	//op negatie
	bool operator!() {
		return foundCommand != "";
	}

	//op incrementare
	CommandIdentifier operator++() {
		this->userInput.push_back(' ');
		return *this;
	}

	//op adunare
	CommandIdentifier operator+(string suffixUserInput) {
		if (suffixUserInput != "") {
			CommandIdentifier copie = *this;
			copie.userInput += suffixUserInput;
			return copie;
		}
	}

	//op indexare
	string& operator[](int index) {
		if (index > 0 && index <= commandMenuSize) {
			return commandMenu[index];
		}
	}

	//op cast
	explicit operator string() {
		return foundCommand;
	}

	//op <
	bool operator<(string input) {
		return userInput.length() < input.length();
	}

	//getter i si setter i
	string getUserInput() {
		return this->userInput;
	}
	void setUserInput(string userInput) {
		this->userInput = userInput;
	}
	string getFoundCommand() {
		return this->foundCommand;
	}
	void setFoundCommand(string foundCommand) {
		this->foundCommand = foundCommand;
	}

	// GETTER & SETTER
	string getNumeTabel() {
		return this->numeTabel;
	}
	void setNumeTabel(string numeTabel) {
		this->numeTabel = numeTabel;
	}
	string getNumeColoane(int i) {
		return numeColoane[i];
	}
	string getTipColoane(int i) {
		return tipColoane[i];
	}
	int getDimensiuneColoane(int i) {
		return dimensiuneColoane[i];
	}
	string getValImplicitaColoane(int i) {
		return valImplicitaColoane[i];
	}
	int getIndex() {
		return index;
	}

	string getValues(int i) {
		return this->values[i];
	}

	string* getPackageDelete() {
		return this->packageDelete;
	}

	string* getPackageUpdate() {
		return this->packageUpdate;
	}
	string* getPackageWhere() {
		return this->packageWhere;
	}


	//op citire, afisare 
	friend ostream& operator<<(ostream&, CommandIdentifier);
	friend istream& operator>>(istream&, CommandIdentifier&);
};

ostream& operator<<(ostream& out, CommandIdentifier cmd) {
	cout << "user input: ";
	out << cmd.userInput << endl;

	if (cmd.packageDelete != nullptr) {
		for (int i = 0; i < 2; i++) {
			out << cmd.packageDelete[i] << " ";
		}
	}
	return out;
}

istream& operator>>(istream& in, CommandIdentifier& cmd) {
	cout << "user input: ";
	getline(in, cmd.userInput);

	return in;
}

class Column
{
private:
	const int columnId;
	string columnName = "";

	string* columnValues;	//vector alocat dinamic

	char* columnType = nullptr;	//typeOf()
	int columnValueSize;

	friend class Table;

	string defaultValue;

	static int noRows;//nr inregistrari
	static int noColumns;


public:
	//constructor implicit
	Column() : columnId(noColumns++)
		//columnId(setNoColumns(noColumns++))
	{
		columnName = "";

		columnType = new char[strlen("") + 1];
		strcpy_s(columnType, strlen("") + 1, ""); //implicit text //va retine, la creare, pt fiecare obiect, tpiul coresp dat atunci (text, number, float)

		columnValueSize = 0;
		defaultValue = "";
		//initializez coloana cu valoarea default
		/*
		for (int i = 0; i < Table::noRows; i++) {	// adauga valori dc exista dimensiune nenula de noRows
			this->columnValues[i] = defaultValue;
		}
		*/

		if (noRows > 0) { //ce fac cand primes sir vid ca default value
			this->columnValues = new string[noRows];
			for (int i = 0; i < noRows; i++) {
				this->columnValues[i] = defaultValue;
			}
		}
	}

	//getter si setter
	static void setNoColumns(int noColumns)
	{
		noColumns = noColumns;
	}

	static void setNoRows(int noRows)
	{
		noRows = noRows;
	}

	static int getNoRows() {
		return noRows;
	}

	static int getNoColumns() {
		return noColumns;
	}
	string getColumnValues(int i) {
		return columnValues[i];
	}

	string getColumnName() {
		return this->columnName;
	}

	//constructori cu parametrii
	Column(string columnName, const char* columnType, int columnValueSize, string defaultValue) : columnId(noColumns++) //Create new column
	{
		if (columnName != "") //we checked the rest of the parameters in columnsyntax_verifier // if a create command exists
		{
			this->columnName = columnName;

			if (columnType != nullptr) {
				this->columnType = new char[strlen(columnType) + 1];
				strcpy_s(this->columnType, strlen(columnType) + 1, columnType);
			}

			this->columnValueSize = columnValueSize;
			this->defaultValue = defaultValue;

			if (noRows > 0 && defaultValue != "") { //ce fac cand primesc sir vid ca default value
				this->columnValues = new string[noRows];
				for (int i = 0; i < noRows; i++) {
					this->columnValues[i] = defaultValue;
				}
			}
		}
	}

	Column(string* columnValues) : Column() {

		if (noRows > 0 && columnValues != nullptr) { //ce fac cand primes sir vid ca default value
			this->columnValues = new string[noRows];

			for (int i = 0; i < noRows; i++) {
				this->columnValues[i] = columnValues[i];
			}
		}
	}

	//constructor copiere
	Column(const Column& c) : columnId(c.columnId) //are sens sa se copieze absolut tot, inclusiv id-ul?
	{
		this->columnName = c.columnName;
		if (noRows > 0 && c.defaultValue != "") { //ce fac cand primes sir vid ca default value
			this->columnValues = new string[noRows];
			for (int i = 0; i < noRows; i++) {
				this->columnValues[i] = c.defaultValue;
			}
		}
		if (c.columnType != nullptr) {
			this->columnType = new char[strlen(c.columnType) + 1];
			strcpy_s(this->columnType, strlen(c.columnType) + 1, c.columnType);
		}

		this->columnValueSize = c.columnValueSize;
		this->defaultValue = c.defaultValue;
	}

	//operatorul egal
	Column& operator=(const Column& c) {
		if (columnValues != NULL) {
			delete[] columnValues;
		}
		if (columnType != nullptr) {
			delete[] columnType;
		}

		this->columnName = c.columnName;
		if (noRows > 0 && c.defaultValue != "") { //ce fac cand primes sir vid ca default value
			this->columnValues = new string[noRows];
			for (int i = 0; i < noRows; i++) {
				this->columnValues[i] = c.defaultValue;
			}
		}
		else {
			this->columnValues = nullptr;
		}

		if (c.columnType != nullptr) {
			this->columnType = new char[strlen(c.columnType) + 1];
			strcpy_s(this->columnType, strlen(c.columnType) + 1, c.columnType);
		}
		else {
			this->columnType = nullptr;
		}

		this->columnValueSize = c.columnValueSize;
		this->defaultValue = c.defaultValue;

		return *this;
	}

	//destructor
	~Column() {
		if (columnValues != NULL) {
			delete[] columnValues;
		}
		if (columnType != nullptr) {
			delete[] columnType;
		}
	}

	//supraincarcare op ++, -- //
	//[imbunatatire] ++ se plimba printre coloane; util pt parcurgere pe linie la metodele Insert...->in tabel
	Column operator++() {
		this->columnValueSize++;
		return *this;
	}

	Column operator--() {
		this->columnValueSize--;
		return *this;
	}

	//supraincaracre negatie
	bool operator!() {
		return columnValues != nullptr;
	}

	//supraincarcare op matematic + ; implemneteaza si cazul care conserva comutativitatea
	Column operator+(string extraColumnName) {
		if (extraColumnName != "") {
			Column copie = *this;
			copie.columnName += extraColumnName;//concatenare intre stringuri
			return copie;
		}
		return *this;
	}

	//supraincaracre op []
	string& operator[](int index) {
		if (index >= 0 && index < noRows) {
			return columnValues[index];
		}
		//TODO handle exceptions
	}

	//supraincarcare op cast 
	explicit operator int() {
		return columnValueSize;
	}

	//supraincarcare op < //
	bool operator<(int size) {
		return columnValueSize < size;
	}

	//

	//supraincarcare op >>, <<

	friend ostream& operator<<(ostream&, Column);
	friend istream& operator>>(istream&, Column&);
};

int Column::noColumns = 0;
int Column::noRows = 0;

/*
* const int columnId;
	string columnName;

	string* columnValues;	//vector alocat dinamic

	char* columnType;	//typeOf()
	int columnValueSize;

	friend class Table;

	string defaultValue;
	*/

	//supraincaracre op scriere
ostream& operator<<(ostream& out, Column c) {
	out << "column name: " << c.columnName << endl;
	out << "column values: ";
	if (c.columnValues != nullptr && Column::getNoRows > 0) {
		for (int i = 0; i < Column::getNoRows(); i++) {
			out << c.columnValues[i] << " ";
		}
	}
	out << endl;
	out << "column type: ";
	if (c.columnType != nullptr) {
		out << c.columnType << endl;
	}
	out << "column value size: " << c.columnValueSize << endl;
	out << "default value: " << c.defaultValue << endl;

	return out;
}

//supraincarecare op citire
istream& operator>>(istream& in, Column& c) {
	cout << "column name: ";
	in >> c.columnName;

	cout << "column values: ";
	if (c.columnValues != nullptr)
	{
		delete[] c.columnValues;
	}
	if (Column::getNoRows > 0) {
		c.columnValues = new string[Column::getNoRows()];
		for (int i = 0; i < Column::getNoRows(); i++) {
			cout << "columnValues[" << i << "]= ";
			in >> c.columnValues[i];
		}
	}
	else {
		c.columnName = nullptr;
	}

	cout << "column type: ";
	string buffer;
	in >> buffer;
	if (c.columnType != nullptr) {
		delete[] c.columnValues;
	}
	c.columnType = new char[buffer.length() + 1];
	strcpy_s(c.columnType, buffer.length() + 1, buffer.c_str());

	cout << "column value size: ";
	in >> c.columnValueSize;

	cout << "default value: ";
	in >> c.defaultValue;

	return in;
}

class Table
{
private:
	const int tableId;

	string tableName = "";
	string tableCommands[3] = { "CREATE", "DROP", "DISPLAY" }; //vector definit static

	friend class Column;

	Column* columns;// = new Column[noColumns]; //vector de pointer catre Column si

	//static int noRows;//nr inregistrari
	//static int noColumns;

	static int noOfTables;

public:
	//constructor fara param
	Table() : tableId(noOfTables++)
	{
		tableName = "";
		//if (Column::noColumns > 0) {
		columns = new Column[Column::noColumns]; 		//aloc dinamic spatiu

		//for (int i = 0; i < Column::noColumns; i++) {
		//	this->columns[i] = Column();
		//}

	//}
	//else {
	//	columns = nullptr;
	//}
	}

	//constructor cu parametri
	Table(string tableName, Column* columns) : tableId(noOfTables++) // Create Table... should be linked to create column
	{
		this->tableName = tableName;
		if (Column::noColumns > 0) {
			this->columns = new Column[Column::noColumns]; 		//aloc dinamic spatiu
			for (int i = 0; i < Column::noColumns; i++) {
				this->columns[i] = columns[i];
				//Column();
			}
		}
		else {
			columns = nullptr;
		}

		//compunere; constructorul de Column() de noColumns ori
		//for (int i = 0; i < noColumns; i++) {
			//Column::columnName = columnName[i];
			//Column::Column(columnName[i], );
		//}
	}
	//comanada CREATE TABLE
	void createTable(CommandIdentifier cmd) {		// TODO constructor CommandIdentifier care retine toate gruparile de date
		//apel constrcutor Table
		//initializeaza campuri din commandIdentifier

		this->tableName = cmd.getNumeTabel();
		//cout << "cmd_din_create - " << cmd.getNumeTabel() << endl;
		//cout << "this - " << this->tableName << endl;

		//Column::noColumns = cmd.getIndex();
		Column::noColumns = 1;
		cout << "no_of_columns: " << Column::noColumns << endl;

		for (int i = 0; i < Column::noColumns; i++) {
			columns[i].columnName = cmd.getNumeColoane(i);	//columns[i] este un Column => apeleaza constructorul din clasa Column si ii schimba campurile

					//Column( cmd.getNumeColoane(i), cmd.getTipColoane(i)), cmd.getDimensiuneColoane(i), cmd.getValImplicitaColoane(i) );	//??

			strcpy_s(columns[i].columnType, strlen((cmd.getTipColoane(i)).c_str()) + 1, (cmd.getTipColoane(i)).c_str());

			columns[i].columnValueSize = cmd.getDimensiuneColoane(i);
			columns[i].defaultValue = cmd.getValImplicitaColoane(i);

		}

		noOfTables++;	//TODO validam dc se creaza tabela

		cout << "Tabela " << this->tableName << " a fost creata.";
	}

	/*

	//avem userInput - camp al clasei commandIdentifier
	CommandIdentifier cmdId = new CommandIdentifier();
	string cmd = cmdId.extractCommand();
	if (cmd == 'CREATE') {
	cmd.validateCreateTable(userInput);
	Table t1 = Table();
	t1.createTable(cmd);
	}

	*/

	void dropTable() {
		//campul numeTabela e populat anterior la apelul lui createtable()

		//nu validam pt ca se apeleaza pe un obiect

		delete this;	//dezaloc spatiul; apel destructor
		//TODO sterg fisierul cu numele din CommandIdentifier::numeTabel

		cout << "Tabela " << this->tableName << " a fost stearsa.";

	}

	//displayTable se afla pe global (pt ca fol supraincarcarea afisarii care e prezentata dupa clasa Table)

	void insert(CommandIdentifier cmd) {
		//preluam datele din values[] si le copiem in coloanele tabelului; parcurgere pe linie printre coloane
		for (int i = 0; i < Column::noColumns; i++) {
			Column::setNoRows(Column::getNoColumns() + 1);

			columns[i].columnValues[Column::getNoRows()] = cmd.getValues(i);
		}

		cout << "Inregistrarea a fost adaugata.";
	}

	void deleteFrom(CommandIdentifier cmd) {
		int foundRowI = -1;

		for (int i = 0; i < Column::getNoColumns(); i++) {
			if (columns[i].getColumnName() == (cmd.getPackageDelete())[0]) {	//check numeTabel
				//parcurgem inregistrarile din coloana in care am gasit numele de col cautat
				for (int j = 0; j < Column::getNoRows(); j++) {
					if (columns[i].getColumnValues(j) == (cmd.getPackageDelete())[1]) {	//check valoare
						foundRowI = j;
					}
				}
			}
		}

		for (int i = 0; i < Column::getNoColumns(); i++) {
			//columns[i].getColumnValues(j);
			for (int index = foundRowI; index < Column::getNoRows(); index++) {
				columns[i].getColumnValues(index) = columns[i].getColumnValues(index + 1);
			}
			//"stergere"
			//mutam toate elem din dreapta lui cu o poz spre stanga
			//micsoram nr de inregistrari cu o unitate
		}

		Column::setNoRows(Column::getNoRows() - 1);

		cout << "Inregistrarea a fost stearsa.";
	}

	void update(CommandIdentifier cmd) {
		int foundRowI = -1;

		for (int i = 0; i < Column::getNoColumns(); i++) {
			if (columns[i].getColumnName() == (cmd.getPackageWhere())[0]) {	//check numeTabel
				//parcurgem inregistrarile din coloana in care am gasit numele de col cautat
				for (int j = 0; j < Column::getNoRows(); j++) {
					if (columns[i].getColumnValues(j) == (cmd.getPackageWhere())[1]) {	//check valoare
						foundRowI = j;
					}
				}
			}
		}

		//parcurgere pe linia foundRowI, caut colo cu numeCol dat, la care modific val cu cea data
		for (int i = 0; i < Column::getNoColumns(); i++) {
			if (columns[i].getColumnName() == (cmd.getPackageUpdate())[0]) {	//check numeTabel
				columns[i].getColumnValues(foundRowI) = (cmd.getPackageUpdate())[1];
				break;
			}
		}

		cout << "Valoarea a fost modificata." << endl;
	}

	//constructor copiere
	Table(const Table& t) : tableId(t.tableId) {
		this->tableName = t.tableName;
		if (Column::noColumns > 0 && columns != nullptr) {
			this->columns = new Column[Column::noColumns]; 		//aloc dinamic spatiu
			for (int i = 0; i < Column::noColumns; i++) {
				this->columns[i] = t.columns[i];
			}
		}
		else {
			this->columns = nullptr;
		}
	}

	//operator =
	Table& operator=(const Table& t) {
		if (this->columns != nullptr) {
			delete[] this->columns;
		}

		this->tableName = t.tableName;
		if (Column::noColumns > 0 && columns != nullptr) {
			this->columns = new Column[Column::noColumns]; 		//aloc dinamic spatiu
			for (int i = 0; i < Column::noColumns; i++) {
				this->columns[i] = t.columns[i];
			}
		}
		else {
			this->columns = nullptr;
		}

		return *this;
	}

	//destructor
	~Table() {
		if (this->columns != nullptr) {
			delete[] this->columns;
		}
	}

	//op de negatie
	bool operator!() {
		return columns != nullptr;
	}

	//op increment, postcrement
	Table operator++() {
		this->tableName.push_back(' ');
		return *this;
	}

	//op +
	Table operator+(string sufix) {
		Table copie = *this;
		copie.tableName += sufix;
		return copie;
	}

	//op indexare
	Column& operator[](int index) {
		if (index > 0 && index < Column::noColumns) {
			return columns[index];
		}
		//TODO gestioneaza exceptia
	}

	//op cast explicit
	explicit operator string() {
		return tableName;
	}

	//supraincarcare op < //
	bool operator<(string alternativeTableName) {
		return tableName < alternativeTableName;
	}

	/*
	void CreateColumn(const int ColumnId)

	void InsertLine()
	*/

	//getter i si setter i
	string getTableName() {
		return this->tableName;
	}
	void setTableName(string tableName) {
		this->tableName = tableName;
	}

	static int getNoOfTables() {
		return noOfTables;
	}

	static void setNoOfTables(int noOfTables) {
		Table::noOfTables = noOfTables;
	}
	//
	//supraincarcare op >>, <<

	friend ostream& operator<<(ostream&, Table);
	friend istream& operator>>(istream&, Table&);
};

int Table::noOfTables = 0;

ostream& operator<<(ostream& out, Table t) {
	out << "table name: " << t.tableName << endl;
	out << "columns: ";

	/*
	if (t.columns != nullptr && Column::getNoColumns() > 0) {
		for (int i = 0; i < Column::getNoColumns(); i++) {
			out << t.columns[i] << " ";
		}
		out << endl;
	}
	*/

	//if (t.columns != nullptr && Column::getNoColumns() > 0) {

	for (int i = 0; i < Column::getNoRows(); i++) {
		for (int j = 0; j < Column::getNoColumns(); j++) {
			out << t.columns[j].getColumnValues(i) << " ";
		}
		out << endl;
	}
	//}

	return out;
}

istream& operator>>(istream& in, Table& t) {
	cout << "table name: ";
	in >> t.tableName;

	cout << "columns: ";
	if (t.columns != nullptr) {
		delete[] t.columns;
	}
	if (Column::getNoColumns() > 0) {
		t.columns = new Column[Column::getNoColumns()];
		for (int i = 0; i < Column::getNoColumns(); i++) {
			cout << "columns[" << i << "]= ";
			in >> t.columns[i];
		}
	}
	else {
		t.columns = nullptr;
	}

	return in;
}

void displayTable(Table t) {

	//afiseaza toate coloanele din tabela
	//cout << this->tableName;

	//out(t);
	cout << t;

	//apelez supraincarcarea op << din clasa Table (parcurgere pe linii)
}

class Database
{
private:
	int const databaseId = 1;
	Table* tables;	//pointer catre tabele
	friend class Table;
	string databaseName = "Baza de date 1";
public:
	Database()
	{
		if (Table::getNoOfTables() > 0) {
			tables = new Table[Table::getNoOfTables()];
			for (int i = 0; i < Table::getNoOfTables(); i++) {
				this->tables[i] = Table();
			}
		}
		else {
			tables = nullptr;
		}
	}

	//constructor cu parametri
	Database(Table* tables)
	{
		if (Table::getNoOfTables() > 0) {
			tables = new Table[Table::getNoOfTables()];
			for (int i = 0; i < Table::getNoOfTables(); i++) {
				this->tables[i] = tables[i];
			}
		}
		else {
			tables = nullptr;
		}
	}

	//constructor copiere
	Database(const Database& db) {
		if (Table::getNoOfTables() > 0) {
			this->tables = new Table[Table::getNoOfTables()];
			for (int i = 0; i < Table::getNoOfTables(); i++) {
				this->tables[i] = db.tables[i];
			}
		}
		else {
			this->tables = nullptr;
		}
	}

	//operator =
	Database& operator=(const Database& db) {
		if (this->tables != nullptr) {
			delete[] this->tables;
		}

		if (Table::getNoOfTables() > 0) {
			this->tables = new Table[Table::getNoOfTables()];
			for (int i = 0; i < Table::getNoOfTables(); i++) {
				this->tables[i] = db.tables[i];
			}
		}
		else {
			this->tables = nullptr;
		}
	}

	//destructor
	~Database() {
		if (this->tables != nullptr) {
			delete[] this->tables;
		}
	}

	//supraincarcare op
	//op de negatie
	bool operator!() {
		return tables != nullptr;
	}

	//op increment, postcrement
	Database operator++() {
		this->databaseName.push_back(' ');
		return *this;
	}

	//op +
	Database operator+(string sufix) {
		Database copie = *this;
		copie.databaseName += sufix;
		return copie;
	}

	//op indexare
	Table& operator[](int index) {
		if (index > 0 && index < Table::getNoOfTables()) {
			return tables[index];
		}
		//TODO gestioneaza exceptia
	}

	//op cast explicit
	explicit operator string() {
		return databaseName;
	}

	//getter si setter
	string getDatabaseName() {
		return databaseName;
	}
	void setDatabaseName(string dbName) {
		this->databaseName = dbName;
	}

	//supraincarcare op < 
	bool operator<(string alternativeDbName) {
		return databaseName < alternativeDbName;
	}

	//supraincarcare op >>, <<

	friend ostream& operator<<(ostream&, Database);
	friend istream& operator>>(istream&, Database&);
};

ostream& operator<<(ostream& out, Database db) {
	out << "tables: ";
	if (db.tables != nullptr && Table::getNoOfTables() > 0) {
		for (int i = 0; i < Table::getNoOfTables(); i++) {
			out << db.tables[i] << " ";
		}
		out << endl;
	}
	return out;
}

istream& operator>>(istream& in, Database& db) {

	cout << "tables: ";
	if (db.tables != nullptr) {
		delete[] db.tables;
	}
	if (Table::getNoOfTables() > 0) {
		db.tables = new Table[Table::getNoOfTables()];
		for (int i = 0; i < Table::getNoOfTables(); i++) {
			cout << "tables[" << i << "]= ";
			in >> db.tables[i];
		}
	}
	else {
		db.tables = nullptr;
	}

	return in;
}


int main()
{
	//string userInput;
	//getline(cin, userInput);

	//CommandIdentifier a;

	//cout << a.extractCommand(userInput); //afiseaza numele comenzii extras din text
	//cout << endl;

	//testing validateCreateTable
	//cout << a.validateCreateTable(userInput);
	//cout << endl;

	/*
	cout << userInput;
	cout << endl;
	cout << a.columnsyntax_verifier(userInput);
	cout << endl;
	cout << a.getFoundCommand();

	*/

	/*
	size_t token;
	token = userInput.find('(', 0);
	userInput.erase(0, token + 1);
	userInput.erase(userInput.length() - 1, userInput.length());
	cout << endl;
	cout << userInput;
	cout << endl;
	token = userInput.find(',', 0);
	cout << userInput.substr(0, token);
	*/

	//testing validateDropAndDisplayTable
	//cout << a.validateDropAndDisplayTable(userInput);

	//testing validateInsert
	/*
	string types[] = {"TEXT", "INTEGER", "TEXT"};
	int sizes[] = { 10, 2, 15 };
	int noOfColumns = 3;
	cout << a.validateInsert(userInput, types, sizes, noOfColumns);
	*/

	//testing validateDelete
	//cout << a.validateDelete(userInput);

	//testing validateUpdate
	//cout << a.validateUpdate(userInput);

	//testing try catch
	/*
	try {
		cout << a.validateCreateTable(userInput);
	}
	catch (other_exception e) {
		cout << e.what();
	}
	*/

	//testing class elements from class Column
	//Column c; //implicit
	//string columnName, char* columnType, int columnValueSize, string defaultValue) : columnId(Table::noColumns++) //Create new column
	//string numeColoana = "numeCol";
	//Column c1("numeCol", "text", 10, "col");
	//cout << c << endl;
	//cout << c1;

	//Column c2 = c1; // op copiere
	//cout << c2;

	//c = c1; // op egal
	//cout << c;

	//cin >> c; //op citire
	//cout << c; //op citire

	//testing class elements from class Table
	//Table t;
	//string tableName, Column* columns

	//Column columns[3];			// = { c, c1, c2 };
	//columns[0] = c;
	//columns[1] = c1;
	//columns[2] = c2;
	//cout << columns[0];

	/*
	Column** columns = new Column*[3];			// = { c, c1, c2 };
	columns[0] = new Column[3];
	columns[0] = &c;
	//columns[1] = c1;
	//columns[2] = c2;// = new Column();
	cout << columns[0];
	*/

	//string nume = "nume";
	//Table t1(nume, &columns);

	//cout << t << endl;
	//cout << t1.getTableName() << endl;



	//test.setTableName("testTabel");

	//cout << test.getTableName();

	//displayTable(test);

	//-------------------------
	//1. creez obiect cmd cu parametrii (userInout, commandFound)
	//2. apelez createTable(cmd) pe obiect tip Tabel()

	/*
	string userInp = "CREATE TABLE numeTab ((numecol, TEXT, 10, craciun), (numecol1, TEXT, 10, craciun1)) ";
	//CommandIdentifier cmd = CommandIdentifier(userInp, "CREATE");
	CommandIdentifier cmd;
	cout<<cmd.extractCommand(userInp);
	//cout<<cmd.validateCreateTable(userInp);		//asociaza valori campurilor index si numeTabel din clasa cmd

	try {
		//cout << cmd.validateCreateTable(userInp);
		cmd.validateCreateTable(userInp);
	}
	catch (other_exception e) {
		cout << e.what();
	}

	cout << "cmd - " << cmd.getNumeTabel() << endl;

	Table test;
	test.createTable(cmd);

	*/
	return 0;
}