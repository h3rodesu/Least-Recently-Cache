#include<iostream>
#include<iomanip>
#include<list>
#include<unordered_map>
#include<string>
#include<memory>
using namespace std;

class Cache {
protected:
	list <pair<string, string>>Cachelist;
	size_t razmer;
	unordered_map<string,list<pair<string,string>>::iterator>Map;//iterator-чтобы работать с указателем а не с объектои
public:
	Cache(size_t size) : razmer(size) {}
	void put(const string& key, const string& value) {
		auto it = Map.find(key);
		if (it != Map.end()) {//Проверка на дубликаты
			it->second->second =value;//
			Cachelist.splice(Cachelist.begin(), Cachelist, it->second);//Перенос в начало
			return;
		}
		if (Map.size() >= razmer) {//Удаление из хвоста
			string oldkey = Cachelist.back().first;
			Map.erase(oldkey);
				Cachelist.pop_back();
		}
		Cachelist.push_front({ key,value });
		Map[key] = Cachelist.begin();
	}
	void get(const string& key) {
		auto it = Map.find(key);//Поиск строго по ключу
		if (it != Map.end()) {
			cout << "Ключ: " << it->first << "Данные:  " << it->second->second << endl;
			Cachelist.splice(Cachelist.begin(), Cachelist, it->second);//Перемещаем адрес хэш таблицы
			return;
		}
		else {
				cout << "Ничего не найдено! " << endl;
		}
	}
	~Cache() {}
		
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