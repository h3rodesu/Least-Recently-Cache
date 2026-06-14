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
};

class Cache {
private:
	size_t razmer;
	Node* head;
	Node* tail;
	std::unordered_map<std::string, Node*>Map;
public:
	Cache(size_t size) : razmer(size), head(nullptr), tail(nullptr) {}

	void InsertAtHead(const string&key,const string&value  ) {
		Node* NewNode = new Node();
		NewNode->key = key;//Записали в ячейку КЛЮЧ(Поле для поиска)
		NewNode->value = value;//Записали данные в ячйеку
		Map[key] = NewNode;//Записали адрес ячейки в хэш таблицу 
		if (head == nullptr) {
			 head=NewNode;
			 tail = NewNode;
		}
		else {
			NewNode->next = head;
			head->prev = NewNode;
			head = NewNode;
		}
	}

	void get(string& choice) {
		auto it = Map.find(choice);
		if (it != Map.end()) {
			cout << "Результат(ключ):" << it->first << "Результат(данные)"<<it->second->value << endl;//ОБЯЗАТЕЛЬНО second->value ЧТОБЫ НЕ ВЫВЕСТИ АДРЕС
		}
		else {
			cout << "Результатов нет" << endl;
		}
	}
	void put(const string& key, const string& value) {
		InsertAtHead(key, value);
}
};
int main() {

	system("chcp 65001 > nul"); 
	Cache myCache(3);
	myCache.put("Honda Civic", "Здесь лежит значение");//put а не InsertAtHEAD !!!!!1111!!!111!!!
	myCache.put("Toyota MarkII", "Здесь тоже значение");
	myCache.put("Mitsubishi Lancer", "Здесь лежит значение");

	string car1 = "Toyota MarkII";//Текстовые ключи	
	string car2 = "Honda Civic";
	string fakecar = "Nissan Silvia";//Проверка так называемая

	myCache.get(car1);
	myCache.get(car2);
	myCache.get(fakecar);

	system("Pause");
	return 0;
}