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
    }

    SUBCASE("No se insertan nombres vacíos") {
        CHECK_THROWS(dao.insertPerformer(0,""));
    }

    REQUIRE(schema.close(db));
}
