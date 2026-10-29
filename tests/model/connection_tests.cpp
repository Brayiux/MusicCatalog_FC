#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <MusicCatalogDAO.hpp>
#include <Schema.hpp>
#include <NotEnoughInfoException.hpp>
#include <sqlite3.h>

using namespace model::connection;

TEST_CASE("Pruebas de inserción de intérprete") {

    sqlite3* db = nullptr;
    model::schema::Schema schema;
    REQUIRE(schema.open(":memory:", &db, nullptr));

    MusicCatalogDAO dao(db);

    SUBCASE("Inserción de intérpretes de tipos únicamente 0, 1 y 2") {
        REQUIRE(dao.insertPerformer(0, "Persona"));
        REQUIRE(dao.insertPerformer(1, "Grupo"));
        REQUIRE(dao.insertPerformer(2, "Desconocido"));
        CHECK_THROWS(dao.insertPerformer(5, "Intérprete inválido"));

        SUBCASE("La tabla performers tiene 3 registros") {
            sqlite3_stmt* stmt = nullptr;
            REQUIRE(sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM performers;", -1, &stmt, nullptr) == SQLITE_OK);
            REQUIRE(sqlite3_step(stmt) == SQLITE_ROW);
            int count = sqlite3_column_int(stmt, 0);
            CHECK(count == 3);
            REQUIRE(sqlite3_finalize(stmt) == SQLITE_OK);
        }
    }

    SUBCASE("No se insertan nombres vacíos") {
        CHECK_THROWS(dao.insertPerformer(0,""));
    }

    REQUIRE(schema.close(db));
    
}
