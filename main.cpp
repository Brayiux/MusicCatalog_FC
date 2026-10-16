#include <sqlite3.h>
#include <iostream>

using namespace std;

int main(){

    sqlite3* db = nullptr;
    int op = sqlite3_open("db.db", &db);

    if (op != SQLITE_OK) {
        cout << "Error al crear la base de datos" << endl;
        sqlite3_close(db);
        return 1;
    }

    cout << "La base de datos fue abierta exitosamente" << endl;
    op = sqlite3_close(db);
    if (op == SQLITE_OK) {
        cout << "La base de datos fue cerrada exitosamente" << endl;
    }
    return 0;
}