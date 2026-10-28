#include "Schema.hpp"

namespace model::schema {

    const char* Schema::SCHEMA = 
        "CREATE TABLE types ("
            "id_type       INTEGER PRIMARY KEY,"
            "description   TEXT"
        ");"

        "INSERT INTO types VALUES(0, 'Person');"
        "INSERT INTO types VALUES(1, 'Group');"
        "INSERT INTO types VALUES(2, 'Unknown');"

        "CREATE TABLE performers ("
                "id_performer  INTEGER PRIMARY KEY,"
                "id_type       INTEGER,"
                "name          TEXT,"
                "FOREIGN KEY   (id_type) REFERENCES types(id_type)"
        ");"

        "CREATE TABLE persons ("
            "id_person     INTEGER PRIMARY KEY,"
            "stage_name    TEXT,"
            "real_name     TEXT,"
            "birth_date    TEXT,"
            "death_date    TEXT"
        ");"

        "CREATE TABLE groups ("
            "id_group      INTEGER PRIMARY KEY,"
            "name          TEXT,"
            "start_date    TEXT,"
            "end_date      TEXT"
        ");"

        "CREATE TABLE in_group ("
            "id_person     INTEGER,"
            "id_group      INTEGER,"
            "PRIMARY KEY   (id_person, id_group),"
            "FOREIGN KEY   (id_person) REFERENCES persons(id_person)"
            "FOREIGN KEY   (id_group) REFERENCES groups(id_group)"
        ");"

        "CREATE TABLE albums ("
            "id_album      INTEGER PRIMARY KEY,"
            "path          TEXT,"
            "name          TEXT,"
            "year          INTEGER"
        ");"

        "INSERT INTO albums VALUES (0, '', 'Unknown', 0);"

        "CREATE TABLE rolas ("
            "id_rola       INTEGER PRIMARY KEY,"
            "id_performer  INTEGER,"
            "id_album      INTEGER,"
            "path          TEXT,"
            "title         TEXT,"
            "track         INTEGER,"
            "year          INTEGER,"
            "genre         TEXT,"
            "FOREIGN KEY   (id_performer) REFERENCES performers(id_performer)"
            "FOREIGN KEY   (id_album) REFERENCES albums(id_album)"
        ");";

    Schema::Schema() {}

    bool Schema::open(const char* path, sqlite3** db, char** errmsg) {
        
        sqlite3_open(path, db);

        int op = sqlite3_exec(*db, SCHEMA, nullptr, nullptr, errmsg);

        return op == SQLITE_OK;
    }

    bool Schema::close(sqlite3* db) {
        return sqlite3_close(db) == SQLITE_OK;
    }
}