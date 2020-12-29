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

	string* packageDelete = new string[2];

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

		if (InputCommand.substr(0, token) == "TABLE")
		{
			InputCommand.erase(0, token); //remove 'TABLE'
			InputCommand.erase(remove(InputCommand.begin(), InputCommand.end(), ' '), InputCommand.end()); //to work easier and without errors with our syntax, we remove spaces from the command's line

			token = InputCommand.find("(", 0);
			//TODO_Faza2 here we should verify if the table's name already exists in our database; if not we add it to the vector


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
						token = InputCommand.find(')', 0);
						columnItem = InputCommand.substr(0, token + 1);
						InputCommand.erase(0, token);	//sterg continutul columnItem din inputCommand
						if (columnsyntax_verifier(columnItem) == 0) {
							throw other_exception("wrong syntax");
						}

						return 1; // every test was successful! the string validates all tests
					}
					return 1; // every test was successful!
				}
				throw other_exception("first column input selection not valid");
			}
			throw other_exception("wrong syntax or brackets not enclosed");
		}
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
			//TODO_Faza2 we should verify if it already exists in our table

			columnItem.erase(0, commaPlacement + 1); //removes columnName

			commaPlacement = columnItem.find(',', 0); // contine tip // here ends the type

			string type = columnItem.substr(0, commaPlacement); // tip

			columnItem.erase(0, commaPlacement + 1); //removes tip

			commaPlacement = columnItem.find(',', 0); //contine size
			int size = isNumber(columnItem.substr(0, commaPlacement - 1)); //isNumber returns a number, if convertible to one -- toNumber
			if (size == -1)
			{
				//return 11; // syntax error; the size is not a number
				throw other_exception("size is not a number");
			}
			columnItem.erase(0, commaPlacement); //removes size
			string implicitValue = columnItem;


			//now we must verify if the data type is valid & if the implicitValue is of the same type and size
			if (type == "TEXT")
			{
				if (size < (int)implicitValue.length()) {
					throw other_exception("size can't fit the implicit value");
				}

				return 1;
			}
			else
				if (type == "INTEGER")
				{
					if (isNumber(implicitValue) == -1) {//if implicit value can't convert to number
						throw other_exception("type doesn't match the value given");
					}

					if (size < (int)implicitValue.length()) {
						throw other_exception("size can't fit the value given");
					}

					return 1;
				}
				else
					if (type == "FLOAT")
					{
						//precizie de 1 - lungimea partii fractionare de 1

						implicitValue.erase(remove(implicitValue.begin(), implicitValue.end(), '.'), implicitValue.end()); //we ignore the floating point for now
						if (isNumber(implicitValue) == -1) {
							throw other_exception("type doesn't match the value given");
						}
						if (size < (int)implicitValue.length()) {//size - nr caractere
							throw other_exception("size can't fit the value given");
						}
						return 1; //totul e ok
					}
					else {
						throw other_exception("type enterd not compatible");
					}


		}
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

			InputCommand.erase(0, token); //we remove the tableName from our command 

			if (!InputCommand.empty()) {
				return 5; //wrong syntax
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

			InputCommand.erase(0, token + 1); //we remove the tableName from our command 

			InputCommand.erase(remove(InputCommand.begin(), InputCommand.end(), ' '), InputCommand.end()); //to work easier and without errors with our syntax, we remove spaces from the command's line

			token = InputCommand.find("(", 0); //finds 'VALUES'


			if (InputCommand.substr(0, token) == "VALUES") {

				InputCommand.erase(0, token); //removes 'VALUES' fara sa elimine paranteza

				if (InputCommand.substr(0, 1) == "(" && InputCommand.substr(InputCommand.length() - 1, InputCommand.length()) == ")")
				{
					InputCommand.erase(0, 1); //removes the outer '('
					InputCommand.erase(InputCommand.length() - 1, InputCommand.length()); //removes last ')'
					cout << InputCommand << endl;

					return syntaxVerifierInsert(InputCommand, types, sizes, noOfColumns);

				}
				return 3; // wrong syntax, maybe it doesn't have enclosed brackets
			}
			return 2; //key word 'VALUES' not found
		}
		return 2; // key word "INTO" not found!
	}

	//insert opereaza pe linie -> valori neomogene, parcurgere printre coloane
	int syntaxVerifierInsert(string columnValues, string* types, int* sizes, int noOfColumns) {

		string InputCommand = columnValues;
		size_t token;
		string* values = new string[noOfColumns]; //vector de string uri alocat dinamic
		int indexValues = 0;

		while (InputCommand.empty() == false) {
			token = InputCommand.find(",", 0);		//index include prima valoare introdusa pt populare

			if (token != string::npos) { //dc gaseste virgula
				values[indexValues] = InputCommand.substr(0, token);	//adaug pe value in values[]
				indexValues++;

				InputCommand.erase(0, token + 1);	//elimin pe value 

			}
			else {//a ajuns la ultima valoare
				values[indexValues] = InputCommand.substr(0, InputCommand.length());	//adaug pe value in values[]
				indexValues++;

				InputCommand.clear();
			}

		}

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

		//int indexValues = 0;

		if (InputCommand.substr(0, token) == "FROM")
		{
			InputCommand.erase(0, token + 1); //removes 'FROM'

			token = InputCommand.find(" ", 0); //indexul cuprinde si nume tabela

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
		string* packageUpdate = new string[2]; //vector de string uri alocat dinamic
		string* packageWhere = new string[2]; //vector de string uri alocat dinamic


		string InputCommand = userInput;
		size_t token = userInput.find(" ", 0);	//finds 'UPDATE'
		InputCommand.erase(0, token + 1);

		token = InputCommand.find(" ", 0); //indexul cuprinde si nume tabela

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

		//TODO_Faza2 do we need here to construct an object with a given package
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
	{
		columnName = "";

		columnType = new char[strlen("") + 1];
		strcpy_s(columnType, strlen("") + 1, ""); //implicit text //va retine, la creare, pt fiecare obiect, tpiul coresp dat atunci (text, number, float)

		columnValueSize = 0;
		defaultValue = "";

		if (noRows > 0) { 
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

			if (noRows > 0 && defaultValue != "") { 
				this->columnValues = new string[noRows];
				for (int i = 0; i < noRows; i++) {
					this->columnValues[i] = defaultValue;
				}
			}
		}
	}

	Column(string* columnValues) : Column() {

		if (noRows > 0 && columnValues != nullptr) { 
			this->columnValues = new string[noRows];

			for (int i = 0; i < noRows; i++) {
				this->columnValues[i] = columnValues[i];
			}
		}
	}

	//constructor copiere
	Column(const Column& c) : columnId(c.columnId) 
	{
		this->columnName = c.columnName;
		if (noRows > 0 && c.defaultValue != "") { 
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
		if (noRows > 0 && c.defaultValue != "") { 
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

	//supraincarcare op ++, -- 
	//[imbunatatire Faza2] ++ se plimba printre coloane; util pt parcurgere pe linie la metodele Insert...->in tabel
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

	//supraincarcare op matematic +
	Column operator+(string extraColumnName) {
		if (extraColumnName != "") {
			Column copie = *this;
			copie.columnName += extraColumnName; //concatenare intre stringuri
			return copie;
		}
		return *this;
	}

	//supraincaracre op []
	string& operator[](int index) {
		if (index >= 0 && index < noRows) {
			return columnValues[index];
		}
	}

	//supraincarcare op cast 
	explicit operator int() {
		return columnValueSize;
	}

	//supraincarcare op < //
	bool operator<(int size) {
		return columnValueSize < size;
	}

	//supraincarcare op >>, <<

	friend ostream& operator<<(ostream&, Column);
	friend istream& operator>>(istream&, Column&);
};

int Column::noColumns = 0;
int Column::noRows = 0;

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

	Column* columns;

	static int noOfTables;

public:
	//constructor fara param
	Table() : tableId(noOfTables++)
	{
		tableName = "";
		if (Column::noColumns > 0) {
			columns = new Column[Column::noColumns]; 		//aloc dinamic spatiu
			for (int i = 0; i < Column::noColumns; i++) {
				this->columns[i] = Column();
			}
		}
		else {
			columns = nullptr;
		}
	}

	//constructor cu parametri
	Table(string tableName, Column* columns) : tableId(noOfTables++) 
	{
		this->tableName = tableName;
		if (Column::noColumns > 0) {
			this->columns = new Column[Column::noColumns]; 		//aloc dinamic spatiu
			for (int i = 0; i < Column::noColumns; i++) {
				this->columns[i] = columns[i];
			}
		}
		else {
			columns = nullptr;
		}
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
	}

	//op cast explicit
	explicit operator string() {
		return tableName;
	}

	//supraincarcare op < //
	bool operator<(string alternativeTableName) {
		return tableName < alternativeTableName;
	}

	//[TODO_Faza2]
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
	
	//supraincarcare op >>, <<

	friend ostream& operator<<(ostream&, Table);
	friend istream& operator>>(istream&, Table&);
};

int Table::noOfTables = 0;

ostream& operator<<(ostream& out, Table t) {
	out << "table name: " << t.tableName << endl;
	out << "columns: ";
	if (t.columns != nullptr && Column::getNoColumns() > 0) {
		for (int i = 0; i < Column::getNoColumns(); i++) {
			out << t.columns[i] << " ";
		}
		out << endl;
	}
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
//	string userInput;
//	getline(cin, userInput);
//
//	CommandIdentifier a;
//
//	cout << a.extractCommand(userInput); //afiseaza numele comenzii extras din text
//	cout << endl;
//
//	//testing validateCreateTable
//	//cout << a.validateCreateTable(userInput);
//	//cout << endl;
//
//	
//	//cout << userInput;
//	//cout << endl;
//	//cout << a.columnsyntax_verifier(userInput);
//	//cout << endl;
//	//cout << a.getFoundCommand();
//
//	
//
//	//testing validateDropAndDisplayTable
//	//cout << a.validateDropAndDisplayTable(userInput);
//
//	//testing validateInsert
//	/*
//	string types[] = {"TEXT", "INTEGER", "TEXT"};
//	int sizes[] = { 10, 2, 15 };
//	int noOfColumns = 3;
//	cout << a.validateInsert(userInput, types, sizes, noOfColumns);
//	*/
//
//	//testing validateDelete
//	//cout << a.validateDelete(userInput);
//
//	//testing validateUpdate
//	//cout << a.validateUpdate(userInput);
//
//	//testing try catch
//	/*
//	try {
//		cout << a.validateCreateTable(userInput);
//	}
//	catch (other_exception e) {
//		cout << e.what();
//	}
//	*/
//
//	//testing class elements from class Column
//	Column c; //implicit
//	//string columnName, char* columnType, int columnValueSize, string defaultValue) : columnId(Table::noColumns++) //Create new column
//	string numeColoana = "numeCol";
//	Column c1("numeCol", "text", 10, "col");
//	//cout << c << endl;
//	//cout << c1;
//
//	Column c2 = c1; // op copiere
//	//cout << c2;
//
//	c = c1; // op egal
//	//cout << c;
//
//	//cin >> c; //op citire
//	//cout << c; //test op citire
//
//	return 0;
}
