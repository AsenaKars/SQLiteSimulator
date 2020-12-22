#include<iostream>
#include<string>
#include<cstring>

using namespace std;
/*
* 1. identifies the command (by extracting it from the given text input) -> extractCommand()
* 2. makes multiple validations by the identified command's own syntax -> validateCommand()
*	 outputs an int that should be decoded into a specific error message in manageErrors(); if 0, then the extraction has been made succesfully
* 3. directs to the specific command's flow -> directToCommand() SE POATE ANULA DC O POT INTEGRA IN METODA DE MAI SUS
* 4. deciphers the int output from extractCommand() and displays a specific error message depending on the issue -> manageErrors()
* */
class commandIdentifier {
private:
	char* userInput = nullptr; //?? string alocat dinamic - atributul primeste textul de la consola
						//getInput method that gets text with whitespaces!
	
	char** extractedParams[10]; //vector de string-uri alocat dinamic
	
	int extractedParamsI = 0;
	//char* extractedParams = new char[extractedParamsI+1];//lungime variabila; E OK?

	//ALOCAREA DINAMICA E UTILA PT VECTORI DE LUNGIME VARIABILA!!!

	int commandMenuSize = 9;
	string commandMenu[9] = { "CREATE TABLE", "DROP TABLE", "DISPLAY TABLE", "CREATE INDEX", "DROP INDEX", 
		"INSERT INTO", "DELETE FROM", "SELECT", "UPDATE" }; //vector de string-uri alocat static
	//TODO nu-mi place ca nu pot pune un non constant ca nr de elem ale vectorului!

	string foundCommand = "";

public:
	string extractCommand(char* userInput) {
		strcpy_s(this->userInput, strlen(userInput)+1, userInput);
		//this->userInput = userInput;

		char* inputCommand = nullptr;
		char delimitator[] = " ";
		char* token = strtok(this->userInput, delimitator);	//modific pe var atribut ca sa scap de numele comenzii din text
		strcpy_s(inputCommand, strlen(token) + 1, token);
		strcat_s(inputCommand, strlen(" ") + 1, " ");

		token = strtok(nullptr, delimitator);
		strcat_s(inputCommand, strlen(token) + 1, token);
		//TODO ce fac cu comenzile care nu au nevoie de 2 strtok uri pt ca instructiunea e de un singur cuvant!!
		//era ideala o functie de tip find/ contains aici...

		for (int i = 0; i < commandMenuSize; i++) {
			//if (userInput->find(commandMenu[i]) != string::npos) {
			if (strcmp(inputCommand, commandMenu[i].c_str()) == 0) {
			//if (strstr(strstr(userInput, " ")+1, "(")) {
				foundCommand = commandMenu[i];
				break;
			}
		}
		
		return foundCommand;
	}

	//fol parametrul userInput pt a nu modifica atributul obiectului
	int validateCreateTable(char* userInput) {
		strcpy_s(userInput, strlen(this->userInput)+1, this->userInput);//copiez restul (tot in afara numelui comenzii)

		//strtok
		char delimitator[] = " (),";//studiaza si celelalte comenzi
		int inputParamNo = 0; 

		char* token = strtok(userInput, delimitator);
		strcpy_s(*extractedParams[extractedParamsI], strlen(token)+1, token);	//table name
		extractedParamsI ++;

		//TODO ce se intampla da in text am 2 caractere delimitator unul dupa altul; ce retine token? cumva null? mi-ar fi util sa poata trece peste si sa retina primul sir de caractere nenul
		while (token) {
			//gruparea dintr-o paranteza
			token = strtok(nullptr, delimitator);
			strcpy_s(*extractedParams[extractedParamsI], strlen(token) + 1, token);	//nume coloana
			
			token = strtok(nullptr, delimitator);
			strcpy_s(*extractedParams[extractedParamsI], strlen(token) + 1, token);	//tip

			token = strtok(nullptr, delimitator);
			strcpy_s(*extractedParams[extractedParamsI], strlen(token) + 1, token);	//dimensiune

			token = strtok(nullptr, delimitator);
			strcpy_s(*extractedParams[extractedParamsI], strlen(token) + 1, token);	//valoare implicita

		}

		//TODO extra validation: dc lung vectorului este multiplu de 4 + 1 - nu reusesc sa calc lungimea vectorului (dimensiunea la declarare e una constanta :( )
		//pot incerca, altenativ, sa numar manual paramtrii introdusi in vector
		
		//if ((strlen(&extractedParams) / strlen(&extractedParams[0])) % 4 == 1) {
		//}
		return 0;
	}

	int validateDropOrDisplayTable(char* userInput) {
		strcpy_s(userInput, strlen(this->userInput) + 1, this->userInput);//copiez restul (tot in afara numelui comenzii)
		
		char delimitator[] = " ";

		char* token = strtok(userInput, delimitator);
		strcpy_s(*extractedParams[extractedParamsI], strlen(token) + 1, token);	//table name
	
		return 0;
	}

	int validateInsertInto(char* userInput) {
		strcpy_s(userInput, strlen(this->userInput) + 1, this->userInput);//copiez restul (tot in afara numelui comenzii)

		char delimitator[] = " (";

		char* token = strtok(userInput, delimitator);
		strcpy_s(*extractedParams[extractedParamsI], strlen(token) + 1, token);	//table name

		token = strtok(nullptr, delimitator);
		if (strcmp(token, "VALUES") == 0) {
			//TODO cum validez elementele introduse intr-o linie? 
			//ar trebui sa fie conform tipului dictat in instructiunea create table
		}

		//introduci valorile in extractedParams[]

		//TODO for dupa nr 
		//TODO si check dupa tipul valorilor introduse; trebuie sa corespunda ce atributele coloanei respective
		
		//token = strtok(nullptr, delimitator);
		//strcpy_s(*extractedParams[extractedParamsI], strlen(token) + 1, token);	//nume coloana

		return 0;
	}

	int validateDeleteFrom(char* useInput) {
		strcpy_s(userInput, strlen(this->userInput) + 1, this->userInput);//copiez restul (tot in afara numelui comenzii)

		char delimitator[] = " ";

		char* token = strtok(userInput, delimitator);
		strcpy_s(*extractedParams[extractedParamsI], strlen(token) + 1, token);	//table name

		token = strtok(nullptr, delimitator);
		if (strcmp(token, "WHERE") == 0) {

		}
		//TODO "where" trebuie sa apara o singura data!
	}

	//validare generala; 
	//1.nr de acolade par
	//2.nr de paranteze rotunde par
	bool generalValidation() {
		int noOfCurlyBracketsOpened = 0;
		int noOfCurlyBracketsClosed = 0;
		int noOfBracketsOpened = 0;
		int noOfBracketsClosed = 0;

		for (int i = 0; i < strlen(userInput); i++) {
			if (strcmp(userInput+i, "{") == 0) {
				noOfCurlyBracketsOpened ++;
			}
			if (strcmp(userInput + i, "}") == 0) {
				noOfCurlyBracketsClosed ++;
			}
			if (strcmp(userInput + i, "(") == 0) {
				noOfBracketsOpened++;
			}
			if (strcmp(userInput + i, ")") == 0) {
				noOfBracketsClosed++;
			}
		}

		if (noOfCurlyBracketsOpened == noOfCurlyBracketsClosed && (noOfBracketsOpened == noOfBracketsClosed)) {
			return true;
		}
		return false;
	}

	int validateCommand() {
		if (foundCommand == "") {
			cout << "The command is not registered in the database commands menu or it could be written in small letters (if this, please rewrite it with capital letters.).";
			//?? permitem si scrierea comenzilor cu litera mica?

			//3. nr param = nr virgule + 1

		}
		else if (generalValidation) {
			if (foundCommand == "CREATE TABLE") {
				validateCreateTable(userInput); //copy by value => nu va modifica variabila atribut
				//creaza tabela()
			}
			else if (foundCommand == "DROP TABLE") {
				validateDropOrDisplayTable(userInput);
			}
			else if (foundCommand == "DISPLAY TABLE") {
				validateDropOrDisplayTable(userInput);
			}
			else if (foundCommand == "CREATE INDEX") {
				//validateCreateIndex();
			}
			else if (foundCommand == "DROP INDEX") {
				//validateDropIndex();
			}
			else if (foundCommand == "INSERT INTO") {
				validateInsertInto(userInput);
			}
			else if (foundCommand == "DELETE FROM") {
				validateDeleteFrom(userInput);
			}
			else if (foundCommand == "SELECT") {
				//TODO
				//validateSelect();
			}
			else if (foundCommand == "UPDATE") {
				//TODO
				//validateUpdate();
			}
		}
	}
};