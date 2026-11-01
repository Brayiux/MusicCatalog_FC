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

    #pragma region Obtención de datos

    int MusicCatalogDAO::getEntityId(string id_name, string table_name, string id_entity_name, string id_entity) {
        sqlite3_stmt* stmt = nullptr;
        
        if (sqlite3_prepare_v2(db, format("SELECT {} FROM {} WHERE {} = ?;", id_name, table_name, id_entity_name).c_str(), -1, &stmt, nullptr)
                != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al preparar la consulta.");
        }
        sqlite3_bind_text(stmt, 1, id_entity.c_str(), -1, SQLITE_TRANSIENT);
        int op = sqlite3_step(stmt);
        if (op == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return -1;
        }
        if (op != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al ejecutar la consulta.");
        }
        int id = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return id;
    }

    int MusicCatalogDAO::getPerformerId(string name) {
        return getEntityId("id_performer", "performers", "name", name);
    }

    int MusicCatalogDAO::getPersonId(string name) {
        return getEntityId("id_person", "persons", "stage_name", name);
    }

    int MusicCatalogDAO::getGroupId(string name) {
        return getEntityId("id_group", "groups", "name", name);
    }

    int MusicCatalogDAO::getAlbumId(filesystem::path path) {
        return getEntityId("id_album", "albums", "path", path.string());
    }

    int MusicCatalogDAO::getSongId(filesystem::path path) {
        return getEntityId("id_rola", "rolas", "path", path.string());
    }

    Performer* MusicCatalogDAO::getPerformer(int id) {
        // Ejecución de la búsqueda:
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, "SELECT id_type, name FROM performers WHERE id_performer = ?;", -1, &stmt, nullptr)
                != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al preparar la búsqueda.");
        }
        sqlite3_bind_int(stmt, 1, id);
        int op = sqlite3_step(stmt);
        if (op == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return nullptr;
        }
        if (op != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al ejecutar la búsqueda.");
        }
        // Llenado de datos:
        Performer* p = new Performer();

        p->id = id;
        if (sqlite3_column_type(stmt, 0) != SQLITE_NULL) {
            p->type = sqlite3_column_int(stmt, 0);
        }
        const unsigned char* n = sqlite3_column_text(stmt, 1);
        if (n != nullptr) {
            p->name = reinterpret_cast<const char*>(n);
        }

        sqlite3_finalize(stmt);
        // Retorno de la información:
        return p;
    }

    Person* MusicCatalogDAO::getPerson(int id) {
        // Validación de la operación por hacer:
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, "SELECT stage_name, real_name, birth_date, death_date FROM persons WHERE id_person = ?;", -1, &stmt, nullptr)
                != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al preparar la búsqueda.");
        }
        sqlite3_bind_int(stmt, 1, id);
        int op = sqlite3_step(stmt);
        if (op == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return nullptr;
        }
        if (op != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al ejecutar la búsqueda.");
        }
        // Llenado de datos:
        Person* p = new Person();
        p->id = id;
        const unsigned char* sn = sqlite3_column_text(stmt, 0);
        if (sn != nullptr) {
            p->stage_name = reinterpret_cast<const char*>(sn);
        }
        const unsigned char* rn = sqlite3_column_text(stmt, 1);
        if (rn != nullptr) {
            p->real_name = reinterpret_cast<const char*>(rn);
        }
        const unsigned char* bd = sqlite3_column_text(stmt, 2);
        if (bd != nullptr) {
            year_month_day bd_ymd;
            istringstream{reinterpret_cast<const char*>(bd)} >> parse("%F", bd_ymd);
            p->birth_date = bd_ymd;
        }
        const unsigned char* dd = sqlite3_column_text(stmt, 3);
        if (dd != nullptr) {
            year_month_day dd_ymd;
            istringstream{reinterpret_cast<const char*>(dd)} >> parse("%F", dd_ymd);
            p->death_date = dd_ymd;
        }
        sqlite3_finalize(stmt);
        // Retorno de la información:
        return p;
    }

    Group* MusicCatalogDAO::getGroup(int id) {
        // Ejecución de la búsqueda:
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, "SELECT name, start_date, end_date FROM groups WHERE id_group = ?;", -1, &stmt, nullptr)
                != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al preparar la consulta.");
        }
        sqlite3_bind_int(stmt, 1, id);
        int op = sqlite3_step(stmt);
        if (op == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return nullptr;
        }
        if (op != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al ejecutar la consulta.");
        }
        // Llenado de datos:
        Group* g = new Group();

        g->id = id;
        const unsigned char* n = sqlite3_column_text(stmt, 0);
        if (n != nullptr) {
            g->name = reinterpret_cast<const char*>(n);
        }
        const unsigned char* sd = sqlite3_column_text(stmt, 1);
        if (sd != nullptr) {
            year_month_day sd_ymd;
            istringstream{reinterpret_cast<const char*>(sd)} >> parse("%F", sd_ymd);
            g->start_date = sd_ymd;
        }
        const unsigned char* ed = sqlite3_column_text(stmt, 2);
        if (ed != nullptr) {
            year_month_day ed_ymd;
            istringstream{reinterpret_cast<const char*>(ed)} >> parse("%F", ed_ymd);
            g->end_date = ed_ymd;
        }
        sqlite3_finalize(stmt);
        // Retorno de la información:
        return g;
    }
    
    Album* MusicCatalogDAO::getAlbum(int id) {
        // Ejecución de la búsqueda:
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, "SELECT path, name, year FROM albums WHERE id_album = ?;", -1, &stmt, nullptr)
                != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al preparar consulta.");
        }
        sqlite3_bind_int(stmt, 1, id);
        int op = sqlite3_step(stmt);
        if (op == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return nullptr;
        }
        if (op != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al ejecutar la consulta.");
        }
        // Llenado de datos:
        Album* a = new Album();

        a->id = id;
        const unsigned char* p = sqlite3_column_text(stmt, 0);
        if (p != nullptr) {
            a->path = reinterpret_cast<const char*>(p);
        }
        const unsigned char* n = sqlite3_column_text(stmt, 1);
        if (n != nullptr) {
            a->name = reinterpret_cast<const char*>(n);
        }
        if (sqlite3_column_type(stmt, 2) != SQLITE_NULL) {
            a->year = year{sqlite3_column_int(stmt, 2)};
        }

        sqlite3_finalize(stmt);
        // Retorno de la información:
        return a;
    }

    Song* MusicCatalogDAO::getSong(int id) {
        // Ejecución de la búsqueda:
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, "SELECT id_performer, id_album, path, title, track, year, genre FROM rolas WHERE id_rola = ?;", -1, &stmt, nullptr)
                != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al preparar la búsqueda.");
        }
        sqlite3_bind_int(stmt, 1, id);
        int op = sqlite3_step(stmt);
        if (op == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return nullptr;
        }
        if (op != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al ejecutar la consulta.");
        }
        // Llenado de datos:
        Song* s = new Song();
        s->id = id;
        if (sqlite3_column_type(stmt, 0) != SQLITE_NULL) {
            Performer* p = getPerformer(sqlite3_column_int(stmt, 0));
            if (p != nullptr && p->name.has_value()) {
                s->performer = p->name.value();
            }
            delete p;
        }
        if (sqlite3_column_type(stmt, 1) != SQLITE_NULL) {
            Album* a = getAlbum(sqlite3_column_int(stmt, 1));
            if (a != nullptr && a->path.has_value()) {
                s->album = a->path.value().string();
            }
            delete a;
        }
        const unsigned char* p = sqlite3_column_text(stmt, 2);
        if (p != nullptr) {
            s->path = reinterpret_cast<const char*>(p);
        }
        const unsigned char* ti = sqlite3_column_text(stmt, 3);
        if (ti != nullptr) {
            s->title = reinterpret_cast<const char*>(ti);
        }
        if (sqlite3_column_type(stmt, 4) != SQLITE_NULL) {
            s->track = sqlite3_column_int(stmt, 4);
        }
        if (sqlite3_column_type(stmt, 5) != SQLITE_NULL) {
            s->year = year{sqlite3_column_int(stmt, 5)};
        }
        const unsigned char* g = sqlite3_column_text(stmt, 6);
        if (g != nullptr) {
            s->genre = reinterpret_cast<const char*>(g);
        }
        sqlite3_finalize(stmt);
        // Retorno de la información:
        return s;
    }

    Person* MusicCatalogDAO::getPerson(string stage_name) {
        return getPerson(getPersonId(stage_name));
    }

    Group* MusicCatalogDAO::getGroup(string name) {
        return getGroup(getGroupId(name));
    }

    Album* MusicCatalogDAO::getAlbum(filesystem::path path) {
        return getAlbum(getAlbumId(path));
    }

    Song* MusicCatalogDAO::getSong(filesystem::path path) {
        return getSong(getSongId(path));
    }

    #pragma endregion

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
            throw SQLiteException("Error: Al preparar la inserción.");
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

    bool MusicCatalogDAO::insertGroup(Group data) {
        // Validación de entrada:
        if (!data.name.has_value() || data.name.value().empty()) {
            throw NotEnoughInfoException("Error: No se proporciona un nombre de grupo o es vacío.");
        }
        // Validación de la operación por hacer:
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM groups WHERE name = ?;", -1, &stmt, nullptr)
                != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al preparar la consulta.");
        }
        sqlite3_bind_text(stmt, 1, data.name.value().c_str(), -1, SQLITE_TRANSIENT);
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
        if (sqlite3_prepare_v2(db, "INSERT INTO groups (name, start_date, end_date) VALUES (?,?,?);", -1, &stmt, nullptr)
                != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al preparar la inserción.");
        }
        sqlite3_bind_text(stmt, 1, data.name.value().c_str(), -1, SQLITE_TRANSIENT);
        if (data.start_date.has_value()) {
            string sd = format("{}", data.start_date.value());
            sqlite3_bind_text(stmt, 2, sd.c_str(), -1, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 2);
        }
        if (data.end_date.has_value()) {
            string ed = format("{}", data.end_date.value());
            sqlite3_bind_text(stmt, 3, ed.c_str(), -1, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 3);
        }
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al ejecutar la inserción.");
        }
        sqlite3_finalize(stmt);
        // Notificación de la inserción:
        return true;
    }

    bool MusicCatalogDAO::insertInGroup(string person_name, string group_name) {
        // Validación de entrada:
        if (person_name.empty() || group_name.empty()) {
            throw NotEnoughInfoException("Error: Ninguno de los nombres (persona o grupo) pueden ser vacíos.");
        }
        // Validación de la operación por hacer:
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, "SELECT id_person FROM persons WHERE stage_name = ?;", -1, &stmt, nullptr)
                != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al preparar la consulta de la persona.");
        }
        sqlite3_bind_text(stmt, 1, person_name.c_str(), -1, SQLITE_TRANSIENT);

        int op = sqlite3_step(stmt);
        if (op == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return false;
        }
        if (op != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al ejecutar la consulta de la persona.");
        }
        int id_person = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);

        stmt = nullptr;
        if (sqlite3_prepare_v2(db, "SELECT id_group FROM groups WHERE name = ?;", -1, &stmt, nullptr)
                != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al preparar la consulta del grupo.");
        }
        sqlite3_bind_text(stmt, 1, group_name.c_str(), -1, SQLITE_TRANSIENT);

        op = sqlite3_step(stmt);
        if (op == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return false;
        }
        if (op != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al ejecutar la consulta del grupo.");
        }
        int id_group = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        // Inserción y Validación de la operación inserción:
        stmt = nullptr;

        if (sqlite3_prepare_v2(db, "INSERT INTO in_group (id_person, id_group) VALUES (?,?);", -1, &stmt, nullptr)
                != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al preparar la inserción.");
        }
        sqlite3_bind_int(stmt, 1, id_person);
        sqlite3_bind_int(stmt, 2, id_group);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al ejecutar la inserción.");
        }
        sqlite3_finalize(stmt);
        // Notificación de la inserción:
        return true;
    }

    bool MusicCatalogDAO::insertAlbum(Album data) {
        // Validación de entrada:
        if (!data.path.has_value()) {
            throw NotEnoughInfoException("Error: No se proporciona un path del álbum.");
        }
        // Validación de la operación por hacer:
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM albums WHERE path = ?;", -1, &stmt, nullptr)
                != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al preparar la consulta.");
        }
        string path = data.path.value().string();
        sqlite3_bind_text(stmt, 1, path.c_str(), -1, SQLITE_TRANSIENT);
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
        if (sqlite3_prepare_v2(db, "INSERT INTO albums (path, name, year) VALUES (?,?,?);", -1, &stmt, nullptr)
                != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al preparar la inserción.");
        }
        sqlite3_bind_text(stmt, 1, path.c_str(), -1, SQLITE_TRANSIENT);
        if (data.name.has_value()) {
            sqlite3_bind_text(stmt, 2, data.name.value().c_str(), -1, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 2);
        }
        if (data.year.has_value()) {
            sqlite3_bind_int(stmt, 3, static_cast<int>(data.year.value()));
        } else {
            sqlite3_bind_null(stmt, 3);
        }

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al ejecutar la inserción.");
        }
        sqlite3_finalize(stmt);
        // Notificación de la inserción:
        return true;
    }

    bool MusicCatalogDAO::insertSong(Song data) {
        // Validación de entrada:
        if (!data.path.has_value()) {
            throw NotEnoughInfoException("Error: No se proporciona una ruta.");
        }
        if (!data.performer.has_value()) {
            throw NotEnoughInfoException("Error: No se proporciona un intérprete.");
        }
        if (!data.album.has_value()) {
            throw NotEnoughInfoException("Error: No se proporciona un álbum.");
        }
        // Validación de la operación por hacer:
        int id_performer = getPerformerId(data.performer.value());
        if (id_performer == -1) {
            return false;
        }
        int id_album = getAlbumId(data.album.value());
        if (id_album == -1) {
            return false;
        }
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM rolas WHERE path = ?;", -1, &stmt, nullptr)
                != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al preparar la consulta");
        }
        sqlite3_bind_text(stmt, 1, data.path.value().string().c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al ejecutar la consulta.");
        }
        int count = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        if (count > 0 ) {
            return false;
        }
        // Inserción y Validación de la operación inserción:
        stmt = nullptr;
        if (sqlite3_prepare_v2(db, "INSERT INTO rolas (id_performer, id_album, path, title, track, year, genre) VALUES (?,?,?,?,?,?,?);", -1, &stmt, nullptr)
                != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al preparar la inserción.");
        }
        sqlite3_bind_int(stmt, 1, id_performer);
        sqlite3_bind_int(stmt, 2, id_album);
        string path = data.path.value().string();
        sqlite3_bind_text(stmt, 3, path.c_str(), -1, SQLITE_TRANSIENT);
        if (data.title.has_value()) {
            sqlite3_bind_text(stmt, 4, data.title.value().c_str(), -1, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 4);
        }
        if (data.track.has_value()) {
            sqlite3_bind_int(stmt, 5, data.track.value());
        } else {
            sqlite3_bind_null(stmt, 5);
        }
        if (data.year.has_value()) {
            sqlite3_bind_int(stmt, 6, static_cast<int>(data.year.value()));
        } else {
            sqlite3_bind_null(stmt, 6);
        }
        if (data.genre.has_value()) {
            sqlite3_bind_text(stmt, 7, data.genre.value().c_str(), -1, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 7);
        }

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al ejecutar la inserción.");
        }
        sqlite3_finalize(stmt);
        // Notificación de inserción:
        return true;
    }

    #pragma endregion

    #pragma region Eliminación

    bool MusicCatalogDAO::deleteEntity(string table_name, string id_name, int id_entity) {
        // Validación de la operación por hacer:
        if (id_entity == -1) {
            return false;
        }
        // Ejecución de la eliminación:
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, format("DELETE FROM {} WHERE {} = ?;", table_name, id_name).c_str(), -1, &stmt, nullptr)
                != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al preparar la eliminación.");
        }
        sqlite3_bind_int(stmt, 1, id_entity);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al ejecutar la eliminación.");
        }
        sqlite3_finalize(stmt);
        // Notificación de la eliminación:
        return true;
    }
    
    bool MusicCatalogDAO::deletePerformer(string name) {
        return deleteEntity("performers", "id_performer", getPerformerId(name));
    }

    bool MusicCatalogDAO::deletePerson(string name) {
        return deleteEntity("persons", "id_person", getPersonId(name));
    }

    bool MusicCatalogDAO::deleteGroup(string name) {
        return deleteEntity("groups", "id_group", getGroupId(name));
    }

    bool MusicCatalogDAO::deleteInGroup(string person_name, string group_name) {
        // Validación de la operación por hacer:
        int id_person = getPersonId(person_name);
        if (id_person == -1) {
            return false;
        }
        int id_group = getGroupId(group_name);
        if (id_group == -1) {
            return false;
        }
        // Ejecución de la operación eliminar:
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, "DELETE FROM in_group WHERE (id_person, id_group) = (?,?)", -1, &stmt, nullptr)
                != SQLITE_OK) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al preparar la eliminación.");
        }
        sqlite3_bind_text(stmt, 1, person_name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, group_name.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            throw SQLiteException("Error: Al ejecutar la eliminación");
        }
        sqlite3_finalize(stmt);
        // Notificación de la eliminación:
        return true;
    }

    bool MusicCatalogDAO:: deleteAlbum(filesystem::path path) {
        return deleteEntity("albums", "id_album", getAlbumId(path));
    }

    bool MusicCatalogDAO::deleteSong(filesystem::path path) {
        return deleteEntity("rolas", "id_rola", getSongId(path));
    }

    #pragma endregion

    

}