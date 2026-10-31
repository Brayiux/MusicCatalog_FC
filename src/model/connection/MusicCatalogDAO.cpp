#include "include/MusicCatalogDAO.hpp"
#include <sqlite3.h>
#include <Person.hpp>
#include <Group.hpp>
#include <Album.hpp>
#include <Song.hpp>
#include <chrono>
#include <exception>
#include <NotEnoughInfoException.hpp>
#include <SQLiteException.hpp>

using namespace std;
using namespace chrono;
using namespace model::exceptions;

namespace model::connection {
    
    MusicCatalogDAO::MusicCatalogDAO(sqlite3* db) {
        this->db = db;
    }

    #pragma region Inserción

    bool MusicCatalogDAO::insertPerformer(int type, string name) {
        // Validación de entrada:
        if (type != 0 && type != 1 && type != 2) {
            throw out_of_range("Error: El tipo de intérprete sólo puede ser 0, 1 o 2.");
        }
        if (name.empty()) {
            throw NotEnoughInfoException("Error: Se un nombre no vacío para el intérprete.");
        }
        // Validación de la operación por hacer:
        sqlite3_stmt* stmt = nullptr;
        
        if (sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM performers WHERE name = ?;", -1, &stmt, nullptr)
                != SQLITE_OK) {
            throw SQLiteException("Error: Error al preparar la consulta.");
        }
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt)
                != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Error al ejecutar la consulta.");
        }
        int count = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        if (count > 0) {
            return false;
        }
        // Inserción del intérprete en la BD y Validación de la operación inserción:
        stmt = nullptr;
        if (sqlite3_prepare_v2(db, "INSERT INTO performers (id_type, name) VALUES (?,?);", -1, &stmt, nullptr)
                != SQLITE_OK) {
            throw SQLiteException("Error: Error al preparar la inserción.");
        }

        sqlite3_bind_int(stmt, 1, type);
        sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al ejecutar la inserción.");
        }
        sqlite3_finalize(stmt);
        // Notificación de la inserción:
        return true;
    }

    bool MusicCatalogDAO::insertPerson(Person data) {
        // Validación de entrada:
        if (!data.stage_name.has_value() || data.stage_name.value().empty()) {
            throw NotEnoughInfoException("Error: No se proporciona un nombre artístico o es vacío.");
        }
        // Validación de la operación por hacer:
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM persons WHERE stage_name = ?;", -1, &stmt, nullptr)
                != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al preparar la consulta.");
        }
        sqlite3_bind_text(stmt, 1, data.stage_name.value().c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al ejecutar la consulta.");
        }
        int count = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        if (count > 0) {
            return false;
        }
        // Inserción y Validación de la operación inserción:
        stmt = nullptr;
        if (sqlite3_prepare_v2(db, "INSERT INTO persons (stage_name, real_name, birth_date, death_date) VALUES (?,?,?,?);", -1, &stmt, nullptr)
                != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Erorr: al preparar la inserción.");
        }
        sqlite3_bind_text(stmt, 1, data.stage_name.value().c_str(), -1, SQLITE_TRANSIENT);
        if (data.real_name.has_value()) {
            sqlite3_bind_text(stmt, 2, data.real_name.value().c_str(), -1, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 2);
        }
        if (data.birth_date.has_value()) {
            string bd = format("{}", data.birth_date.value());
            sqlite3_bind_text(stmt, 3, bd.c_str(), -1, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 3);
        }
        if (data.death_date.has_value()) {
            string dd = format("{}", data.death_date.value());
            sqlite3_bind_text(stmt, 4, dd.c_str(), -1, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 4);
        }
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al ejecutar la inserción.");
        }
        sqlite3_finalize(stmt);
        // Notificación de la inserción:
        return true;
    }

    #pragma endregion

}