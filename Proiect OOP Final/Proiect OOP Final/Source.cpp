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

	string* numeColoane;
	string* tipColoane;
	int* dimensiuneColoane;
	string* valImplicitaColoane;
	int index = 1;	//lungime!

	//drop index
	string numeIndex;

	//insert
	string* values;	 //vector de string uri alocat dinamic
	int indexValues = 0;

	//TODO variab declarata dinamic (pointer)
	string* packageDelete;

	//select
	string* packageSelect;

	//update
	string* packageUpdate; //vector de string uri alocat dinamic
	string* packageWhere; //vector de string uri alocat dinamic

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
		cout << "Nu s-a gasit comanda." << endl;

	}

	int validateCreateTable(string userInput)
	{
		string InputCommand = userInput;
		char delimitator[] = ",()";
		size_t token = userInput.find(" ", 0);	//finds 'CREATE'
		InputCommand.erase(0, token + 1);
		token = InputCommand.find(" ", 0); //finds 'TABLE'

		numeColoane = new string[index];
		tipColoane = new string[index];
		dimensiuneColoane = new int[index];
		valImplicitaColoane = new string[index];


		if (InputCommand.substr(0, token) == "TABLE")
		{
			InputCommand.erase(0, token); //remove 'TABLE'
			InputCommand.erase(remove(InputCommand.begin(), InputCommand.end(), ' '), InputCommand.end()); //to work easier and without errors with our syntax, we remove spaces from the command's line

			token = InputCommand.find("(", 0);

			this->numeTabel = InputCommand.substr(0, token);
			if (numeTabel == "") {
				return 2;
			}

			InputCommand.erase(0, token); //removes the tableName from our command 


			if (InputCommand.substr(0, 1) == "(" && InputCommand.substr(InputCommand.length() - 1, InputCommand.length()) == ")")
			{
				InputCommand.erase(0, 1); //removes the outer '('
				InputCommand.erase(InputCommand.length() - 1, InputCommand.length()); //removes last ')'

				token = InputCommand.find(')', 0);	// gets the end index of the first column entered in the command

				string columnItem = InputCommand.substr(0, token + 1); // retine intreaga instructiune pt coloana, inclusiv parantezele ei
				InputCommand.erase(0, token + 1); //elimin prima instructiune tip coloana din comanda	


				if (columnsyntax_verifier(columnItem) == 1) //dc prima instrc coloana e valida
				{
					while (InputCommand.length() != 0) //verific daca are succesor; exista rest din comanda, dupa eliminarea primei instructiuni tip coloana
					{

						token = InputCommand.find(')', 0);
						columnItem = InputCommand.substr(1, token + 1);
						InputCommand.erase(0, token + 1);	//sterg continutul columnItem din inputCommand
						if (columnsyntax_verifier(columnItem) != 1) {
							//return 5; //wrong syntax
							throw other_exception("wrong syntax");
						}
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

		string* numeColoane_copy = new string[index];
		string* tipColoane_copy = new string[index];
		int* dimensiuneColoane_copy = new int[index];
		string* valImplicitaColoane_copy = new string[index];


		for (int i = 0; i < index - 1; i++) {
			numeColoane_copy[i] = numeColoane[i];
			tipColoane_copy[i] = tipColoane[i];
			dimensiuneColoane_copy[i] = dimensiuneColoane[i];
			valImplicitaColoane_copy[i] = valImplicitaColoane[i];
		}

		if (numeColoane != nullptr && tipColoane != nullptr && dimensiuneColoane != nullptr && valImplicitaColoane != nullptr) {
			delete[] numeColoane;
			delete[] tipColoane;
			delete[] dimensiuneColoane;
			delete[] valImplicitaColoane;
		}

		if (columnItem.substr(0, 1) == ",")
			columnItem.erase(0, 1);

		numeColoane = new string[index];
		tipColoane = new string[index];
		dimensiuneColoane = new int[index];
		valImplicitaColoane = new string[index];

		if (columnItem.substr(0, 1) == "(" && columnItem.substr(columnItem.length() - 1, columnItem.length()) == ")")
		{
			columnItem.erase(0, 1); //remove '('
			columnItem.erase(columnItem.length() - 1, columnItem.length()); //resmove ')'

			columnItem.erase(remove(columnItem.begin(), columnItem.end(), ' '), columnItem.end());

			commaPlacement = columnItem.find(',', 0); // here ends the column's name //indexul primei aparitii a virgulei

			numeColoane[indexValues] = columnItem.substr(0, commaPlacement);		//populate DATA	nume_tab 
			columnItem.erase(0, commaPlacement + 1); //removes columnName
			//cout << numeColoane[indexValues] << endl;

			commaPlacement = columnItem.find(',', 0); // contine tip 
			//we only accept text, integers and floar types

			string type = columnItem.substr(0, commaPlacement); // tip + spatiul de dinainte
			tipColoane[indexValues] = columnItem.substr(0, commaPlacement);		//populate DATA
			//cout << tipColoane[indexValues] << endl;

			columnItem.erase(0, commaPlacement + 1); //removes tip

			// next we must verify the size given. It must be of type integer 

			commaPlacement = columnItem.find(',', 0); //contine size

			int size = isNumber(columnItem.substr(0, commaPlacement));		//isNumber returns a number, if convertible to one -- toNumber

			dimensiuneColoane[indexValues] = isNumber(columnItem.substr(0, commaPlacement));		//populate DATA
			//cout << dimensiuneColoane[indexValues] << endl;

			if (size == -1)
			{
				//return 11; // syntax error; the size is not a number
				throw other_exception("size is not a number");
			}
			columnItem.erase(0, commaPlacement + 1); //removes size and the comma after it

			string implicitValue = columnItem;
			valImplicitaColoane[indexValues] = columnItem;	//populare DATA
			//cout << valImplicitaColoane[indexValues] << endl;

			indexValues++;		//populate DATA

			//now we must verify if the data type is valid & if the implicitValue is of the same type and size
			if (type == "TEXT")
			{
				if (size < (int)implicitValue.length()) {
					//return 10;// size can't fit the implicit value!
					throw other_exception("size can't fit the implicit value");
				}

				//else we should store all data in the columns
				partialColumnCopy(numeColoane_copy, tipColoane_copy, dimensiuneColoane_copy, valImplicitaColoane_copy);
				index++;

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
					partialColumnCopy(numeColoane_copy, tipColoane_copy, dimensiuneColoane_copy, valImplicitaColoane_copy);
					index++;

					return 1;
				}
				else
					if (type == "FLOAT")
					{
						//precizie de 1 - lungimea partii fractionare de 1

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
						partialColumnCopy(numeColoane_copy, tipColoane_copy, dimensiuneColoane_copy, valImplicitaColoane_copy);
						index++;

						return 1; //totul e ok
					}
					else {
						//return 7; // no suitable type entered
						if (numeColoane_copy != nullptr && tipColoane_copy != nullptr && dimensiuneColoane_copy != nullptr && valImplicitaColoane_copy != nullptr) {
							delete[] numeColoane_copy;
							delete[] tipColoane_copy;
							delete[] dimensiuneColoane_copy;
							delete[] valImplicitaColoane_copy;
						}

						throw other_exception("type enterd not compatible");
					}
		}
		//return 6; //column identifiers not between brackets!
		throw other_exception("column input selection not between brackets");
	}

	void partialColumnCopy(string* numeColoane_copy, string* tipColoane_copy, int* dimensiuneColoane_copy, string* valImplicitaColoane_copy) {

		for (int i = 0; i < index - 1; i++) {
			numeColoane[i] = numeColoane_copy[i];
			tipColoane[i] = tipColoane_copy[i];
			dimensiuneColoane[i] = dimensiuneColoane_copy[i];
			valImplicitaColoane[i] = valImplicitaColoane_copy[i];
		}

		delete[] numeColoane_copy;
		delete[] tipColoane_copy;
		delete[] dimensiuneColoane_copy;
		delete[] valImplicitaColoane_copy;
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
			else {

				number = number * 10 + (int)numberGiven[i] - 48;
			}
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
			if (numeTabel == "") {
				return 2;
			}
			InputCommand.erase(0, token); //removes the tableName from our command 

			if (!InputCommand.empty()) {
				return 5; 
			}

			return 1;
		}
		return 2; // key word "TABLE" not found!
	}
	int validateCreateIndex(string userInput) {
		string InputCommand = userInput;
		char delimitator[] = ",()";
		size_t token = userInput.find(" ", 0);	//finds 'CREATE'
		InputCommand.erase(0, token + 1);
		token = InputCommand.find(" ", 0); //finds 'INDEX'

		if (InputCommand.substr(0, token) == "INDEX")
		{
			InputCommand.erase(0, token + 1);	//remove 'INDEX' -->' numeIndex'

			token = InputCommand.find(" ", 0);

			numeIndex = InputCommand.substr(0, token);			//populate DATA
			if (numeIndex == "") {
				return 2;
			}
			InputCommand.erase(0, token + 1); //removes the indexName from our command 

			token = InputCommand.find(" ", 0);	//finds "ON"	
			if (InputCommand.substr(0, token) == "ON") {
				InputCommand.erase(0, token + 1);

				token = InputCommand.find(" ", 0);	//finds table_name
				numeTabel = InputCommand.substr(0, token);
				if (numeTabel == "") {
					return 2;
				}
				InputCommand.erase(0, token + 1);

				token = InputCommand.find(")", 0);
				numeColoane = new string[index];
				numeColoane[0] = InputCommand.substr(1, token - 1);
				if (numeColoane[0] == "") {
					return 2;
				}
				InputCommand.erase(0, token + 1);

				if (!InputCommand.empty()) {
					return 5;
				}

				return 1;
			}
			else return 2;
		}
		else return 2;
	}

	int validateDropIndex(string inputCommand) {
		string InputCommand = userInput;
		char delimitator[] = ",()";
		size_t token = userInput.find(" ", 0);	//finds 'CREATE'
		InputCommand.erase(0, token + 1);
		token = InputCommand.find(" ", 0); //finds 'INDEX'

		if (InputCommand.substr(0, token) == "INDEX")
		{
			InputCommand.erase(0, token + 1); //remove 'INDEX' -->' numeIndex'

			token = InputCommand.find(" ", 0);

			numeIndex = InputCommand.substr(0, token);			//populate DATA
			if (numeIndex == "") {
				return 2;
			}
			InputCommand.erase(0, token); //we remove the tableName from our command 

			if (!InputCommand.empty()) {
				return 5; 
			}

			return 1;
		}
		return 2; // key word "INDEX" not found!
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
			if (numeTabel == "") {
				return 2;
			}
			InputCommand.erase(0, token + 1); //removes the tableName from our command 

			InputCommand.erase(remove(InputCommand.begin(), InputCommand.end(), ' '), InputCommand.end()); //to work easier and without errors with our syntax, we remove spaces from the command's line

			token = InputCommand.find("(", 0); //finds 'VALUES'

			if (InputCommand.substr(0, token) == "VALUES") {

				InputCommand.erase(0, token); //removes 'VALUES' fara sa elimine paranteza
				//cout << InputCommand<<endl;

				if (InputCommand.substr(0, 1) == "(" && InputCommand.substr(InputCommand.length() - 1, InputCommand.length()) == ")")
				{
					InputCommand.erase(0, 1); //removes the outer '('
					InputCommand.erase(InputCommand.length() - 1, InputCommand.length()); //removes last ')'

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
		//int noOfColumns = Table::noOfColumns;

		string InputCommand = columnValues;
		size_t token;

		values = new string[noOfColumns];

		while (InputCommand.empty() == false) {
			token = InputCommand.find(",", 0);		//index include prima valoare introdusa pt populare

			if (token != string::npos) { //dc gaseste virgula
				if (InputCommand.substr(0, token) == "")
					return 2;
				values[indexValues] = InputCommand.substr(0, token);	//adaug pe value in values[]
				indexValues++;

				InputCommand.erase(0, token + 1);	//elimin pe value 

			}
			else {	//a ajuns la ultima valoare

				if (InputCommand.substr(0, InputCommand.length()) == "")
					return 2;
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
					return 10;	// size can't fit the implicit value!
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
							return 9;	// type doesn't match the value given (not a number)
						if (sizes[i] < (int)values[i].length())		//size - nr caractere
							return 8;	// size can't fit the value given!

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

		packageDelete = new string[2];
		packageWhere = new string[2];

		if (InputCommand.substr(0, token) == "FROM")
		{
			InputCommand.erase(0, token + 1); //removes 'FROM'

			token = InputCommand.find(" ", 0); //indexul cuprinde si nume tabela

			this->numeTabel = InputCommand.substr(0, token);			//populate DATA
			if (numeTabel == "") {
				return 2;
			}
			InputCommand.erase(0, token + 1); //we remove the tableName from our command 


			token = InputCommand.find(" ", 0); //finds 'WHERE'

			if (InputCommand.substr(0, token) == "WHERE") {
				InputCommand.erase(0, token + 1); //removes 'WHERE' fara sa elimine paranteza

				InputCommand.erase(remove(InputCommand.begin(), InputCommand.end(), ' '), InputCommand.end()); //to work easier and without errors with our syntax, we remove spaces from the command's line

				token = InputCommand.find("=", 0); //index cuprinde date where

				if (InputCommand.substr(0, token) == "")
					return 2;

				packageDelete[0] = InputCommand.substr(0, token);

				InputCommand.erase(0, token + 1);	//InputCommand = valoare

				if (InputCommand == "")
					return 2;

				packageDelete[1] = InputCommand;

				/*
				for (int i = 0; i < 2; i++) {
					cout << packageDelete[i] << " " << endl;
				}
				*/

				return 1;
			}
			return 2;
		}
		return 2;
	}

	int validateSelect(string userInput) {
		string InputCommand = userInput;
		size_t token = userInput.find(" ", 0);	//finds 'SELECT'
		InputCommand.erase(0, token + 1);

		packageWhere = new string[2];

		token = InputCommand.find(" ", 0);	//prima grupare; "ALL" sau 
		if (InputCommand.substr(0, token) == "ALL") {
			InputCommand.erase(0, token + 1);

			packageSelect = new string[index];
			packageSelect[0] = "ALL";
		}
		else {
			token = InputCommand.find(")", 0);
			if (token == string::npos) {	//nu exista ")"
				return 2;
			}
			if (InputCommand[0] == '(' && InputCommand[token] == ')') {	//exista paranteze...

				InputCommand.erase(0, 1); //removes the outer '('
				token = InputCommand.find(')', 0);
				InputCommand.erase(token, 1); //removes last ')'	a trecut peste ') FROM num'

				//InputCommand.erase(9, 1);	//ERASE (POZ START, NR CARACTERE)

				if (!(syntaxVerifierSelect(InputCommand.substr(0, token)) > 0)) {	//s-a gasit minim o val de nume coloana	
					return 2;
				}

				InputCommand.erase(0, token + 1);
			}
			else {
				return 2;
			}
		}

		token = InputCommand.find(" ", 0);	//finds 'FROM'

		if (InputCommand.substr(0, token) == "FROM") {
			InputCommand.erase(0, token + 1);

			size_t tokenWhere = InputCommand.find("WHERE", 0);

			if (tokenWhere >= 0 && tokenWhere < InputCommand.length()) { //the optional 'WHERE' clause exists 
				//exista spatiu dupa nume tabela
				token = InputCommand.find(" ", 0);	//finds nume tabela
			}
			else {
				token = InputCommand.find(" ", 0);
				if (token >= 0 && token < InputCommand.length()) {	//exista spatii => exista alte cuvinte (neconforme cu sintaxa)
					return 2;
				}

				this->numeTabel = InputCommand;

				return 1;
			}

			//exista si sintaxa optionala; trebuie validata in continuare

			this->numeTabel = InputCommand.substr(0, token);	//populate DATA
			InputCommand.erase(0, token + 1);

			token = InputCommand.find(" ", 0);

			if (InputCommand.substr(0, token) == "WHERE") {
				InputCommand.erase(0, token + 1); //removes 'WHERE' fara sa elimine paranteza

				InputCommand.erase(remove(InputCommand.begin(), InputCommand.end(), ' '), InputCommand.end()); //to work easier and without errors with our syntax, we remove spaces from the command's line

				token = InputCommand.find("=", 0); //index cuprinde date where

				if (InputCommand.substr(0, token) == "")
					return 2;

				packageWhere[0] = InputCommand.substr(0, token);

				InputCommand.erase(0, token + 1);	//InputCommand = valoare

				if (InputCommand == "")
					return 2;

				packageWhere[1] = InputCommand;

				return 1;
			}

			return 2;
		}

		return 2;
	}

	int syntaxVerifierSelect(string InputCommand) {
		size_t token;

		packageSelect = new string[index];	

		string* packageSelect_copy;

		while (InputCommand.length() != 0) {
			packageSelect_copy = new string[index];

			for (int i = 0; i < index - 1; i++) {
				packageSelect_copy[i] = packageSelect[i];
			}

			if (packageSelect != nullptr) {
				delete[] packageSelect;
			}

			packageSelect = new string[index];


			token = InputCommand.find(",", 0);		//index include prima valoare introdusa pt populare

			if (token >= 0 && token < InputCommand.length()) {

				packageSelect[indexValues] = InputCommand.substr(0, token);	//numele primei coloane
				indexValues++;

				InputCommand.erase(0, token + 1);	//elimin pe value 
			}
			else {	//a ajuns la ultima valoare sau exista o singura valoare

				packageSelect[indexValues] = InputCommand.substr(0, InputCommand.length() + 1);	//adauga numele col
				indexValues++;

				InputCommand.clear();
			}

			for (int i = 0; i < index - 1; i++) {
				packageSelect[i] = packageSelect_copy[i];
			}

			delete[] packageSelect_copy;

			index++;
		}
		/*
		for (int i = 0; i < indexValues; i++) {
			cout << packageSelect[i] << " ";
		}
		*/

		return indexValues;
	}

	int validateUpdate(string userInput) {

		string InputCommand = userInput;
		size_t token = userInput.find(" ", 0);	//finds 'UPDATE'
		InputCommand.erase(0, token + 1);

		packageUpdate = new string[2];
		packageWhere = new string[2];

		token = InputCommand.find(" ", 0); //indexul cuprinde si nume tabela

		this->numeTabel = InputCommand.substr(0, token);		//populate DATA
		if (numeTabel == "") {
			return 2;
		}
		InputCommand.erase(0, token + 1); //we remove the tableName from our command 

		token = InputCommand.find(" ", 0); //finds 'SET'

		if (InputCommand.substr(0, token) == "SET")
		{
			InputCommand.erase(0, token + 1); //removes 'SET'

			token = InputCommand.find("=", 0); //indexul cuprinde nume coloana cu tot cu spatiu

			packageUpdate[0] = InputCommand.substr(0, token - 1);//token-1
			InputCommand.erase(0, token + 2); //we remove the nume coloana plus pauza de dupa egal

			token = InputCommand.find(" ", 0); //index include valoare
			packageUpdate[1] = InputCommand.substr(0, token);

			InputCommand.erase(0, token + 1); //removes valoare

			token = InputCommand.find(" ", 0); //finds 'WHERE'


			if (InputCommand.substr(0, token) == "WHERE") {
				InputCommand.erase(0, token + 1); //removes 'WHERE' fara sa elimine paranteza

				InputCommand.erase(remove(InputCommand.begin(), InputCommand.end(), ' '), InputCommand.end()); //to work easier and without errors with our syntax, we remove spaces from the command's line

				token = InputCommand.find("=", 0); //index cuprinde date where

				if (InputCommand.substr(0, token) == "")
					return 2;

				packageWhere[0] = InputCommand.substr(0, token);

				InputCommand.erase(0, token + 1);	//InputCommand = valoare

				if (InputCommand == "")
					return 2;

				packageWhere[1] = InputCommand;

				/*
				for (int i = 0; i < 2; i++) {
					cout << packageUpdate[i] << " ";
				}
				cout << endl;
				for (int i = 0; i < 2; i++) {
					cout << packageWhere[i] << " ";
				}
				*/

				return 1;
			}
			return 2;
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

		//we need here to construct an object with a given package
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

	// GETTER & SETTER

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

	string getPackageDelete(int i) {
		return packageDelete[i];
	}

	//op citire, afisare 
	friend ostream& operator<<(ostream&, CommandIdentifier&);
	friend istream& operator>>(istream&, CommandIdentifier&);
};

ostream& operator<<(ostream& out, CommandIdentifier& cmd) {
	//cout << "user input: ";
	//out << cmd.userInput << endl;

	
	if (cmd.foundCommand == "CREATE" && cmd.numeIndex != "") {
		out << "Se creeaza indexul " << cmd.numeIndex << " pentru coloana " << cmd.numeColoane[0] << " din tabelul " << cmd.numeTabel << endl;
	}
	else if (cmd.foundCommand == "CREATE") {
		out << "Nume tabel: " << cmd.numeTabel << endl;
		for (int i = 0; i < cmd.index - 1; i++) {
			out << "Nume coloana: ";
			out << cmd.numeColoane[i] << endl;

			out << "Tip coloana: ";
			out << cmd.tipColoane[i] << endl;

			out << "Dimensiune coloana: ";
			out << cmd.dimensiuneColoane[i] << endl;

			out << "Valoare implicita coloana: ";
			out << cmd.valImplicitaColoane[i] << endl;

			out << endl;
		}
		out << "S-a creat tabela " << cmd.numeTabel << endl;
	}
	else if (cmd.getFoundCommand() == "DROP" && cmd.numeIndex != "") {//drop index
		out << "S-a sters indexul " << cmd.numeIndex << endl;
	}
	else if (cmd.getFoundCommand() == "DROP") {//drop table
		out << "S-a sters tabela " << cmd.numeTabel << endl;
	}
	else if (cmd.getFoundCommand() == "DISPLAY") {
		out << "Se afiseaza tabela " << cmd.numeTabel << endl;

	}
	else if (cmd.getFoundCommand() == "INSERT") {
		out << "Se adauga urmatoarele valori in tabela " << cmd.numeTabel << " : ";
		for (int i = 0; i < cmd.indexValues; i++) {
			out << cmd.values[i] << " ";
		}
		out << endl;
	}
	else if (cmd.getFoundCommand() == "DELETE") {
		out << "Se sterge inregistrarea unde coloana " << cmd.packageDelete[0] << " are valoarea " << cmd.packageDelete[1] << " din tabela " << cmd.numeTabel << endl;
	}
	else if (cmd.getFoundCommand() == "SELECT") {
		if (cmd.packageSelect[0] == "ALL") {
			out << "Se afiseaza toate coloanele din tabela " << cmd.numeTabel << endl;
		}
		else {
			out << "Se afiseaza coloanele din tabela: " << cmd.numeTabel << endl;
			for (int i = 0; i < cmd.indexValues; i++) {
				out << cmd.packageSelect[i] << " ";
			}
			out << endl;
		}

		if (cmd.packageWhere != nullptr) {	//e clauza optionala!
			out << "unde coloana " << cmd.packageWhere[0] << " are valoarea " << cmd.packageWhere[1] << endl;
		}
	}
	else if (cmd.getFoundCommand() == "UPDATE") {
		out << "Se actualizeaza tabelul " << cmd.numeTabel << " la inregistrarile unde coloana " << cmd.packageWhere[0]
			<< " are valoarea " << cmd.packageWhere[1] << ", schimband valoarea coloanei " << cmd.packageUpdate[0] << " cu "
			<< cmd.packageUpdate[1] << endl;
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

	static int noRows;	//nr inregistrari
	static int noColumns;


public:
	//constructor implicit
	Column() : columnId(noColumns++)
		//columnId(setNoColumns(noColumns++))
	{
		columnName = "";

		columnType = new char[strlen("") + 1];
		strcpy_s(columnType, strlen("") + 1, ""); //implicit text, va retine, la creare, pt fiecare obiect, tpiul coresp dat atunci (text, number, float)

		columnValueSize = 0;
		defaultValue = "";

		//initializez coloana cu valoarea default
		/*
		for (int i = 0; i < Table::noRows; i++) {	// adauga valori dc exista dimensiune nenula de noRows
			this->columnValues[i] = defaultValue;
		}
		*/

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
	string getColumnValues(int i) {
		return columnValues[i];
	}

	string getColumnName() {
		return this->columnName;
	}

	//constructori cu parametrii
	Column(string columnName, const char* columnType, int columnValueSize, string defaultValue) : columnId(noColumns++) //Create new column
	{
		if (columnName != "")	//we checked the rest of the parameters in columnsyntax_verifier // if a create command exists
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
	Column(const Column& c) : columnId(c.columnId)	//copierea id-ului
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

	Column operator+(string extraColumnName) {
		if (extraColumnName != "") {
			Column copie = *this;
			copie.columnName += extraColumnName;	
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

	Column* columns;	// = new Column[noColumns]; vector de pointer catre Column si

	//static int noRows;	//nr inregistrari
	//static int noColumns;

	static int noOfTables;

public:
	//constructor fara param
	Table() : tableId(noOfTables++)
	{
		tableName = "";
		columns = new Column[Column::noColumns]; 		//aloc dinamic spatiu

		//for (int i = 0; i < Column::noColumns; i++) {
		//	this->columns[i] = Column();
		//}

	}

	//constructor cu parametri
	Table(string tableName, Column* columns) : tableId(noOfTables++) 
	{
		this->tableName = tableName;
		if (Column::noColumns > 0) {
			this->columns = new Column[Column::noColumns]; 		
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
	void createTable(CommandIdentifier cmd) {		// + constructor CommandIdentifier care retine toate gruparile de date
		//apel constrcutor Table
		//initializeaza campuri din commandIdentifier

		this->tableName = cmd.getNumeTabel();

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

		noOfTables++;	//validam dc se creaza tabela

		cout << "Tabela " << this->tableName << " a fost creata.";
	}


	void dropTable() {
		//campul numeTabela e populat anterior la apelul lui createtable()

		//nu validam pt ca se apeleaza pe un obiect

		delete this;	
		//sterg fisierul cu numele din CommandIdentifier::numeTabel

		//cout << "Tabela " << this->tableName << " a fost stearsa.";

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

			//stergere - mutam toate elem din dreapta lui cu o poz spre stanga, micsoram nr de inregistrari cu o unitate
			for (int index = foundRowI; index < Column::getNoRows(); index++) {
				columns[i].getColumnValues(index) = columns[i].getColumnValues(index + 1);
			}
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
			this->columns = new Column[Column::noColumns]; 		
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
			this->columns = new Column[Column::noColumns]; 		
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


	/*
	//testing validateCreateTable
	string userInputCreate1 = "CREATE TABLE nume_tab ((nume_col1, TEXT, 15, col1))";
	string userInputCreate2 = "CREATE TABLE nume_tab ((nume_col1, TEXT, 15, col1),(nume_col2, INTEGER, 5, 123))";

	//cout << a.validateCreateTable(userInputCreate2) << endl;

	a.extractCommand(userInputCreate2);
	a.validateCreateTable(userInputCreate2);
	cout << endl;
	cout << a;
	*/

	/*
	//testing try catch
	try {
		cout << a.validateCreateTable(userInput);
	}
	catch (other_exception e) {
		cout << e.what();
	}
	*/


	/*
	//testing validateDropAndDisplayTable
	string userInputDropDisplay1 = "DROP TABLE Droptable_name";
	cout << a.extractCommand(userInputDropDisplay1) << endl;
	a.validateDropAndDisplayTable(userInputDropDisplay1);
	cout << a;
	*/

	/*
	//testing validateCreateIndex
	string userInputCreateIndex1 = "CREATE INDEX INDEX ON nume_tab (col1)";
	string userInputCreateIndex2 = "CREATE INDEX  ON nume_tab (col1)";

	a.extractCommand(userInputCreateIndex1);
	if (a.validateCreateIndex(userInputCreateIndex1) == 1) {
		cout << a;
	}
	else cout << "Sintaxa incorecta";
	*/

	/*
	//testing validateDropIndex
	string userInputDropIndex1 = "DROP INDEX index_name";
	string userInputDropIndex2 = "DROP  index_name";
	a.extractCommand(userInputDropIndex1);
	if (a.validateDropIndex(userInputDropIndex1) == 1) {
		cout << a;
	}
	else cout << "Sintaxa gresita.";
	*/

	/*
	//testing validateInsert
	string types[] = {"TEXT", "INTEGER", "TEXT"};
	int sizes[] = { 10, 2, 15 };
	int noOfColumns = 3;

	string userInputInsert1 = "INSERT INTO nume_tab VALUES (val1, val2, val3)";
	a.extractCommand(userInputInsert1);
	a.validateInsert(userInputInsert1, types, sizes, noOfColumns);
	cout << a;
	*/

	/*
	//testing validateDelete
	string userInputDelete1 = "DELETE FROM nume_tab WHERE col1 = val1";
	a.extractCommand(userInputDelete1);	//important pt a popula campul foundCommand care decide comportamentul supraincarcarii afisarii
	a.validateDelete(userInputDelete1);
	cout << a;
	*/

	/*
	//testing select
	string userInput1 = "SELECT ALL FROM nume_tab";
	string userInput2 = "SELECT ALL nume_tab WHERE numecol = val";
	string userInput3 = "SELECT (col1, col2, col3) FROM nume_tab WHERE numecol = val";
	a.extractCommand(userInput3);
	a.validateSelect(userInput3);
	cout << a;

	//cout << a.syntaxVerifierSelect("col1,col2,col3");
	*/

	/*
	//testing validateUpdate
	string userInputUpdate1 = "UPDATE nume_tab SET col1 = val1 WHERE col2 = val2";
	string userInputUpdate2 = "UPDATE nume_tab SET col1 = val1 col2 = val2";
	a.extractCommand(userInputUpdate2);
	a.validateUpdate(userInputUpdate2);
	cout << a;

	//cout << a.validateUpdate(userInput);
	*/


	/*
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
	//cout << c; //test op citire


	//testing class elements from class Table
	//Table t;
	//string tableName, Column* columns

	//Column columns[3];			// = { c, c1, c2 };
	//columns[0] = c;
	//columns[1] = c1;
	//columns[2] = c2;
	//cout << columns[0];


	//string nume = "nume";
	//Table t1(nume, &columns);

	//cout << t << endl;
	//cout << t1.getTableName() << endl;


	//test.setTableName("testTabel");

	//cout << test.getTableName();

	//displayTable(test);
	*/

	/*
	//1. creez obiect cmd cu parametrii (userInout, commandFound)
	//2. apelez createTable(cmd) pe obiect tip Tabel()

	Table test;
	test.createTable(cmd);

	*/

	return 0;
}