#include <iostream>
#include <fstream>
#include "stockexchange.h"
#include <iomanip>
#include "datetime.h"
#include <cstring>

DataBase DB;
Node X;
using namespace DateTime;

void DataBase::initDB() {
	begin = nullptr;
	end = nullptr;
	count = 0;
}


bool printComparator(const Node* first, const Node* second) {

	DateTime::datetime dt_first = DateTime::datetime::dateSeparator(first->data.datetime);
	DateTime::datetime dt_second = DateTime::datetime::dateSeparator(second->data.datetime);
	// Сортировка по дате и времени
	if (dt_first != dt_second) {
		return dt_first < dt_second;
	}
	// Если время одинаковое, сортируем по названию акции лексикографическим методом
	if (strcmp(first->data.name, second->data.name) < 0) {
		return true;
	}

	// Если тикер одинаковый, сортируем по типу (сначала sell, потом buy)
	if (first->data.is_buy != second->data.is_buy) {
		return !first->data.is_buy;
	}

	if (first->data.price != second->data.price) {
		//sell сортируем по возрастанию, buy по убыванию
		if (first->data.is_buy) {
			return first->data.price > second->data.price;
		}
		return first->data.price < second->data.price;
		
	}
	// Сортируем по количеству
	return first->data.amount > second->data.amount;
}

bool domComparator(const Node* first, const Node* second) {
    if (first->data.price != second->data.price) {
        return first->data.price > second->data.price;
    }

    if (first->data.is_buy != second->data.is_buy) {
        return !first->data.is_buy; // sell заявки выше, если цены равны
    }

    DateTime::datetime dt_first = DateTime::datetime::dateSeparator(first->data.datetime);
    DateTime::datetime dt_second = DateTime::datetime::dateSeparator(second->data.datetime);
    
    return first->data.is_buy ? (dt_first < dt_second) : (dt_first > dt_second);
}

void Merge(Node** arr, int left, int mid, int right, bool(*comparator)(const Node*, const Node*)) {
	int n1 = mid - left + 1;
	int n2 = right - mid;

	Node** L = new Node * [n1];
	Node** R = new Node * [n2];

	for (int i = 0; i < n1; i++){
		L[i] = arr[left + i];
	}
	for (int j = 0; j < n2; j++){
		R[j] = arr[mid + 1 + j];
	}
	int i = 0;
	int j = 0;
	int k = left;

	while (i < n1 && j < n2) {
		if (comparator(L[i], R[j])) {
			arr[k] = L[i];
			i++;
		}
		else {
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}

	while (j < n2) {
		arr[k] = R[j];
		j++;
		k++;
	}

	delete[] L;
	delete[] R;
}

void MergeSort(Node** arr, int left, int right, bool(*comparator)(const Node*, const Node*)) {
	if (left >= right) {
		return;
	}
	int mid = left + (right - left) / 2;
	MergeSort(arr, left, mid, comparator);
	MergeSort(arr, mid + 1, right, comparator);
	Merge(arr, left, mid, right, comparator);
}

void MergeSort(Node** values, int size, bool(*comparator)(const Node*, const Node*)) {
	if (size > 0) {
		MergeSort(values, 0, size - 1, comparator);
	}
}


void DataBase::domTicker(const char* tickerName) {

	Node* current = begin;
	int countCurrent = 0;

	while (current != nullptr) {
		if (std::strcmp(current->data.name, tickerName) == 0 && current->data.amount > 0) {
			countCurrent += 1;
		}
		current = current->next;
	}
	
	if (countCurrent == 0){
		return;
	}
	
	Node** domArray = new Node* [countCurrent];
	current = begin;
	int index = 0;

	while (current != nullptr) {
		if (std::strcmp(current->data.name, tickerName) == 0 && current->data.amount > 0) {
			domArray[index++] = current;
		}
		current = current->next;
	}

	MergeSort(domArray, countCurrent, domComparator);


	for (int i = 0; i < countCurrent; ++i) {
		int day;
		int month; 
		int year; 
		int hour;
		int minute;
		int second;
		sscanf(domArray[i]->data.datetime, "%d.%d.%d %d:%d:%d",
			&day, &month, &year, &hour, &minute, &second);

		std::cout << (domArray[i]->data.is_buy ? "buy" : "sell") << ", "
			<< std::fixed << std::setprecision(2) << domArray[i]->data.price << ", "
			<< domArray[i]->data.amount << ", "
			<< year << "."
			<< std::setw(2) << std::setfill('0') << month << "."
			<< std::setw(2) << std::setfill('0') << day << " "
			<< std::setw(2) << std::setfill('0') << hour << ":"
			<< std::setw(2) << std::setfill('0') << minute << ":"
			<< std::setw(2) << std::setfill('0') << second << "\n";
	}

	delete[] domArray;
}
bool addComparator(const Node* first, const Node* second) {

	if (first->data.price != second->data.price) {
		if (first->data.is_buy) {
			return first->data.price > second->data.price;
		}
		
		return first->data.price < second->data.price;
		
	}

	DateTime::datetime dt_first = DateTime::datetime::dateSeparator(first->data.datetime);
	DateTime::datetime dt_second = DateTime::datetime::dateSeparator(second->data.datetime);
	return dt_first < dt_second;
}

//отработка регистра
bool sameSize(const char* s1, const char* s2) {
	while ((tolower(*s1) == tolower(*s2)) && *s1 != '\0' && *s2 != '\0') {
		s1++;
		s2++;
	}
	return tolower(*s1) == tolower(*s2);
}



int DataBase::add(ticker& X) {

	Node* current = begin;
	int countCurrent = 0;

	while (current != nullptr) {
		if (sameSize(current->data.name, X.name) && 
			((X.is_buy && !current->data.is_buy && current->data.price <= X.price) || 
			(!X.is_buy && current->data.is_buy && current->data.price >= X.price))) {
			countCurrent++;
		}
		current = current->next;
	}

	if (countCurrent > 0) {
		Node** addArray = new Node * [countCurrent];
		current = begin;
		int index = 0;

		while (current != nullptr) {
			if (sameSize(current->data.name, X.name) &&
				((X.is_buy && !current->data.is_buy && current->data.price <= X.price) ||
				(!X.is_buy && current->data.is_buy && current->data.price >= X.price))) {
				addArray[index++] = current;
			}
			current = current->next;
		}

		MergeSort(addArray, countCurrent, addComparator);

		for (int i = 0; i < countCurrent && X.amount > 0; ++i) {
			current = addArray[i];
			if (current->data.amount > 0) {
				int compare_ = std::min(X.amount, current->data.amount);
				X.amount -= compare_;
				current->data.amount -= compare_;
			}
		}

		delete[] addArray;
	}

	if (X.amount >= 0) {
		Node* tmp = new Node;
		tmp->data.id = count + 1;

		strncpy(tmp->data.datetime, X.datetime, 20);
		tmp->data.datetime[19] = '\0';

		strncpy(tmp->data.name, X.name, 16);
		tmp->data.name[15] = '\0';

		tmp->data.is_buy = X.is_buy;
		tmp->data.amount = X.amount;
		tmp->data.price = X.price;

		tmp->next = nullptr;
		tmp->prev = end;

		if (end != nullptr) {
			end->next = tmp;
		}
		else {
			begin = tmp;
		}
		end = tmp;

		++count;
	}

	return count;
}


void DataBase::deleleZero() {

	Node* current = begin;
	Node* previous = nullptr;

	while (current != nullptr) {
		if (current->data.amount == 0) {

			if (previous != nullptr) {
				previous->next = current->next;
			}
			else {
				begin = current->next;
			}

			if (current->next != nullptr) {
				current->next->prev = previous;
			}
			else {
				end = previous;
			}
			Node* toDelete = current;
			current = current->next;
			delete toDelete;
			--count;
		}
		else {
			previous = current;
			current = current->next;
		}
	}
}

void DataBase::printDB() const {
	Node* current = begin;
	int countCurrent = 0;

	while (current != nullptr) {
		countCurrent += 1;
		current = current->next;
	}

	current = begin;
	Node** dateArray = new Node * [countCurrent];
	int index = 0;

	while (current != nullptr) {
		dateArray[index++] = current;
		current = current->next;
	}

	MergeSort(dateArray, countCurrent, printComparator);

	for (int i = 0; i < countCurrent; ++i) {
		int day;
		int month;
		int year;
		int hour;
		int minute;
		int second;
		sscanf(dateArray[i]->data.datetime, "%d.%d.%d %d:%d:%d",
			&day, &month, &year, &hour, &minute, &second);

		std::cout << year << "."
			<< std::setw(2) << std::setfill('0') << month << "."
			<< std::setw(2) << std::setfill('0') << day << " "
			<< std::setw(2) << std::setfill('0') << hour << ":"
			<< std::setw(2) << std::setfill('0') << minute << ":"
			<< std::setw(2) << std::setfill('0') << second << ", "
			<< dateArray[i]->data.name << ", "
			<< (dateArray[i]->data.is_buy ? "buy" : "sell") << ", "
			<< dateArray[i]->data.amount << ", "
			<< std::fixed << std::setprecision(2) << dateArray[i]->data.price << "\n";
	}

	delete[] dateArray;
}
void DataBase::exportDB(const char* filename) const {
	std::ofstream outFile(filename);
	if (!outFile.is_open()) {
		std::cout << "Can not export file " << "\n";
		return;
	}

	Node* current = begin;
	int countCurrent = 0;

	while (current != nullptr) {
		countCurrent += 1;
		current = current->next;
	}

	current = begin;
	Node** dateArray = new Node * [countCurrent];
	int index = 0;

	while (current != nullptr) {
		dateArray[index++] = current;
		current = current->next;
	}

	MergeSort(dateArray, countCurrent, printComparator);

	for (int i = 0; i < countCurrent; ++i) {
		int day;
		int month;
		int year;
		int hour;
		int minute;
		int second;
		sscanf(dateArray[i]->data.datetime, "%d.%d.%d %d:%d:%d",
			&day, &month, &year, &hour, &minute, &second);

		outFile << year << "."
			<< std::setw(2) << std::setfill('0') << month << "."
			<< std::setw(2) << std::setfill('0') << day << " "
			<< std::setw(2) << std::setfill('0') << hour << ":"
			<< std::setw(2) << std::setfill('0') << minute << ":"
			<< std::setw(2) << std::setfill('0') << second << ", "
			<< dateArray[i]->data.name << ", "
			<< (dateArray[i]->data.is_buy ? "buy" : "sell") << ", "
			<< dateArray[i]->data.amount << ", "
			<< std::fixed << std::setprecision(2) << dateArray[i]->data.price << "\n";
	}

	delete[] dateArray;
	outFile.close();

}

Node* DataBase::findRecord(int what) const {
	Node* current = begin;
	while (current != nullptr) {
		if (current->data.id == what) {
			return current;
		}
		current = current->next;
	}

	return nullptr;
}

void DataBase::deleteID(Node* index) {

	if (index != nullptr) {

		if (index->prev != nullptr) {
			index->prev->next = index->next;
		}
		else {
			begin = index->next;
		}
		if (index->next != nullptr) {
			index->next->prev = index->prev;
		}
		else {
			end = index->prev;
		}
		delete index;
		--count;

	}
}

int DataBase::addRecord(const ticker& X) {
	Node* tmp = new Node;
	tmp->data = X;
	tmp->next = nullptr;
	tmp->prev = nullptr;

	if (begin != nullptr) {
		end->next = tmp;
		tmp->prev = end;
		end = tmp;
	}
	else {
		begin = end = tmp;
	}
	++count;
	return count;
}

void DataBase::loadDB(char dbfilename[]) {
	std::ifstream inFile(dbfilename, std::ios_base::binary);
	if (!inFile.is_open()) {
		std::cout << "Can not open database" << "\n";
	}

	deleteDB();

	int count;
	inFile.read((char*)&count, sizeof(count));

	ticker tmp;
	while (inFile.peek() != EOF) {
		inFile.read((char*)&tmp, sizeof(ticker));
		addRecord(tmp);
	}
	inFile.close();
}

void DataBase::saveDB(const char filename[]) {
	std::ofstream outFile(filename, std::ios_base::binary);
	if (!outFile.is_open()) {
		std::cout << "Can not write to file " << "\n";
		return;
	}

	outFile.write((char*)&count, sizeof(count));

	Node* current = begin;
	while (current != nullptr) {
		outFile.write((char*)&current->data, sizeof(ticker));
		current = current->next;
	}

	outFile.close();
}

void DataBase::clear() {
	Node* current = begin;
	while (current != nullptr) {
		Node* nextNode = current->next;
		if (current->data.amount == 0) {
			deleteID(current);
		}
		current = nextNode;
	}
}

void DataBase::deleteDB() {
	Node* current;
	while (begin != nullptr) {
		current = begin;
		begin = begin->next;
		delete current;
	}
	end = nullptr;
	count = 0;
}
