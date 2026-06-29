#include<iostream>
#include<iomanip>
#include<list>
#include<unordered_map>
#include<string>
#include<memory>
#include<pqxx/pqxx>


using namespace std;

class Cache {
private:
	list <pair<string, string>>Cachelist;
	size_t razmer;
	unordered_map<string,list<pair<string,string>>::iterator>Map;//iterator-чтобы работать с указателем а не с объектои
	pqxx::connection& conn;
public:
	Cache(size_t size,pqxx::connection &con) : razmer(size),conn(con) {}
	void put(const string& key, const string& value) {
		auto it = Map.find(key);	
		if (it != Map.end()) {//Проверка на дубликаты
			it->second->second =value;//
			Cachelist.splice(Cachelist.begin(), Cachelist, it->second);//Перенос в начало
			return;
		}
		if (Map.size() >= razmer) {//Удаление из хвоста
			string oldkey = Cachelist.back().first;
			string oldvalue = Cachelist.back().second;
			pqxx::work tx(conn);
			string add = "INSERT INTO Cache(key,value)VALUES (" + tx.quote(oldkey) + "," +tx.quote(oldvalue) + ")ON CONFLICT (key) DO UPDATE SET value = EXCLUDED.value;";
			tx.exec(add);
			tx.commit();
			
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
		else if (it == Map.end()) {
			pqxx::work tx(conn);
			string query_table_find = "SELECT value FROM Cache WHERE key="+tx.quote(key)+"";
			pqxx::result res = tx.exec(query_table_find);
			if (!res.empty()) {
				string dbres = res[0][0].as<string>();
				cout << "По ключу " << key << "Найден результат " << dbres <<"[FROM DATABASE]"<< endl;
			}
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
	
	pqxx::connection connect ("dbname=Least_Recently_Used_Cache user=postgres password=1234 host=localhost port=5432");
	pqxx::work tx(connect);
	string query_table = "CREATE TABLE IF NOT EXISTS Cache(key VARCHAR(150) PRIMARY KEY, value VARCHAR(150),time_add TIMESTAMP DEFAULT NOW());";
	tx.exec(query_table);
	tx.commit();
	Cache myList(sizechoice, connect);
	
	bool work = true;
	
	while (work == true) {
		try {
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
		catch (exception& e) {
			cout << "Внимание! " << e.what() << endl;
		}

	}
	
	system("Pause");
	return 0;
}