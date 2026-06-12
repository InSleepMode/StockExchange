#pragma once
#include "datetime.h"

struct ticker {
	int id;
	char datetime[20];
	char name[16];
	bool is_buy;
	int amount;
	float price;
};


struct Node {

	ticker data;
	Node* prev;
	Node* next;

};

class DataBase {

private:

	int count;
	Node* begin;
	Node* end;

public:
	~DataBase() {
		deleteDB();
	}
	DataBase() : count(0), begin(nullptr), end(nullptr) {}
	void initDB();
	int add(ticker& X);
	void saveDB(const char filename[]);
	int addRecord(const ticker& X);
	void loadDB(char filename[]);
	void deleleZero();
	Node* findRecord(int what) const;
	void printDB() const;
	void deleteID(Node* index);
	void exportDB(const char* filename) const;
	void deleteDB();
	void domTicker(const char* tickerName);
	void clear();
};





