#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <MusicCatalogDAO.hpp>
#include <Schema.hpp>
#include <NotEnoughInfoException.hpp>
#include <sqlite3.h>
#include <chrono>

using namespace model::connection;
using namespace std::chrono;

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

TEST_CASE("Pruebas de inserción de persona") {
    sqlite3* db = nullptr;
    model::schema::Schema schema;
    REQUIRE(schema.open(":memory:", &db, nullptr));

    MusicCatalogDAO dao(db);

    SUBCASE("Inserción de personas") {
        Person pd;
        pd.stage_name = "Persona 1";
        pd.real_name = "Nombre Real";
        pd.birth_date = year_month_day{year{2000}, month{1}, day{1}};
        pd.death_date = year_month_day{year{2020}, month{1}, day{1}};
        REQUIRE(dao.insertPerson(pd));
    }

    SUBCASE("No se admiten nombres artísticos vacíos") {
        Person pd;
        pd.stage_name = "";
        pd.real_name = "Nombre Real";
        pd.birth_date = year_month_day{year{2000}, month{1}, day{1}};
        pd.death_date = year_month_day{year{2020}, month{1}, day{1}};
        CHECK_THROWS(dao.insertPerson(pd));
    }

    REQUIRE(schema.close(db));
}
