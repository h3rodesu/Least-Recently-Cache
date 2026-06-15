#include<iostream>
#include<iomanip>
#include<list>
#include<unordered_map>
#include<string>
#include<memory>
using namespace std;
struct Node {
public:
	string key;
	string value;
	Node* next;
	Node* prev;
	Node() :next(nullptr), prev(nullptr) {}
	~Node() {}
};
class Cache {
protected:
	size_t razmer;
	Node* head;
	Node* tail;
	unordered_map<string, Node*>Map;//String-тип ключа,Node*-адрес ячейки с данными
public:
	Cache(size_t size): razmer(size), head(nullptr), tail(nullptr) {}

	void InsertAtHead(const string& key, const string& value) {
		Node* NewNode = new Node();
		NewNode->key = key;
		NewNode->value = value;
		Map[key] = NewNode;//Запись в таблицу адреса ячйеки
		if (head == nullptr) {
			head = NewNode;
			tail = NewNode;
		}
		else {
			NewNode->next = head;
			head->prev = NewNode;
			head = NewNode;
		}
		if (Map.size() > razmer) {
			Node* current = tail;
			Map.erase(current->key);//Удаление значения ТАК ЖЕ из таблицы
			tail = current->prev;
			tail->next = nullptr;
			delete(current);
		}

	}

	void get(const string& key) {
		auto it = Map.find(key);//Поиск строго по ключу
		if (it != Map.end()) {
			Node* current = it->second;
			cout << "Ключ: " << it->first << "Данные:  " << it->second->value << "Адрес: " << it->second << endl;
			if (current == head) {
				return;
			}
				if (current != tail) {
					current->prev->next = current->next;
					current->next->prev = current->prev;
				}
				else {//current=tail
					tail = current->prev;
					tail->next = nullptr;
				}
				current->next = head;
				head->prev = current;
				head = current;
			}
		else {
			cout << "Ничего не найдено! " << endl;
		}
	}
	void put(const string& key, const string& value) {
		InsertAtHead(key, value);
	}
	~Cache() {
		Node* current = head;
		Node* NextNode;
		while (current != nullptr) {
			NextNode = current->next;
			delete(current);
			current = NextNode;
		}
}
};

int main() {
	system("chcp 65001 > nul");
	int sizechoice = 0;
	cout << "Введите размер Списка" << endl;
	cin >> sizechoice;
	
	Cache myList(sizechoice);
	
	bool work = true;
	
	while (work == true) {

		int listchoice = 0;
		cout << "1-Добавить элемент, 2-Поиск элемента,3-выход из программы" << endl;
		cin >> listchoice;
		if (listchoice == 1) {
			string key;
			cout << "Введите ключ " << endl;
			cin >> ws;
			getline(cin, key);
			string value;
			cout << "Введите данные: " << endl;
			cin >> ws;
			getline(cin, value);
			myList.put(key, value);
		}
		else if (listchoice == 2) {
			string poisk;
			cout << "Введите ключ" << endl;
			cin >> ws;
			getline(cin, poisk);
			myList.get(poisk);
		}
		else if (listchoice == 3) {
			work = false;
		}
	}
	
	system("Pause");
	return 0;
}