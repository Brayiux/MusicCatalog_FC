#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <MusicCatalogDAO.hpp>
#include <Schema.hpp>
#include <NotEnoughInfoException.hpp>

using namespace model::connection;

TEST_CASE("Pruebas de inserción de intérprete") {

    sqlite3* db = nullptr;
    model::schema::Schema schema;
    REQUIRE(schema.open(":memory:", &db, nullptr));

    MusicCatalogDAO dao(db);

    SUBCASE("Inserción de intérpretes de tipos únicamente 0, 1 y 2") {
        CHECK(dao.insertPerformer(0, "Persona"));
        CHECK(dao.insertPerformer(1, "Grupo"));
        CHECK(dao.insertPerformer(2, "Desconocido"));
        CHECK_THROWS(dao.insertPerformer(5, "Intérprete inválido"));
    }

    SUBCASE("No se insertan nombres vacíos") {
        CHECK_THROWS(dao.insertPerformer(0,""));
    }

    REQUIRE(schema.close(db));
    
}
