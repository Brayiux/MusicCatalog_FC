#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <MusicCatalogDAO.hpp>
#include <Schema.hpp>
#include <NotEnoughInfoException.hpp>
#include <sqlite3.h>
#include <chrono>

using namespace model::connection;
using namespace std::chrono;

#pragma region Obtener ID de entidades
TEST_CASE("Obtener ID de intérpretes") {
    sqlite3* db = nullptr;
    model::schema::Schema schema;
    REQUIRE(schema.open(":memory:", &db, nullptr));
    MusicCatalogDAO dao(db);
    REQUIRE(dao.insertPerformer(0, "Intérprete"));
    SUBCASE("Obtener ID de intérprete existente") {
        CHECK(dao.getPerformerId("Intérprete") == 1);
    }
    SUBCASE("Obtener ID de intérprete no existente") {
        CHECK(dao.getPerformerId("No existente") == -1);
    }
    REQUIRE(schema.close(db));
}

TEST_CASE("Obtener ID de personas") {
    sqlite3* db = nullptr;
    model::schema::Schema schema;
    REQUIRE(schema.open(":memory:", &db, nullptr));
    MusicCatalogDAO dao(db);
    Person pd;
    pd.stage_name = "Persona";
    REQUIRE(dao.insertPerson(pd));
    SUBCASE("Obtener ID de persona existente") {
        CHECK(dao.getPersonId("Persona") == 1);
    }
    SUBCASE("Obtener ID de persona no existente") {
        CHECK(dao.getPersonId("No existente") == -1);
    }
    REQUIRE(schema.close(db));
}

TEST_CASE("Obtener ID de grupos") {
    sqlite3* db = nullptr;
    model::schema::Schema schema;
    REQUIRE(schema.open(":memory:", &db, nullptr));
    MusicCatalogDAO dao(db);
    Group gd;
    gd.name = "Grupo";
    REQUIRE(dao.insertGroup(gd));
    SUBCASE("Obtener ID de grupo existente") {
        CHECK(dao.getGroupId("Grupo") == 1);
    }
    SUBCASE("Obtener ID de grupo no existente") {
        CHECK(dao.getGroupId("No existente") == -1);
    }
    REQUIRE(schema.close(db));
}

TEST_CASE("Obtener ID de álbumes") {
    sqlite3* db = nullptr;
    model::schema::Schema schema;
    REQUIRE(schema.open(":memory:", &db, nullptr));
    MusicCatalogDAO dao(db);
    Album ad;
    ad.path = "~";
    REQUIRE(dao.insertAlbum(ad));
    SUBCASE("Obtener ID de álbum existente") {
        CHECK(dao.getAlbumId("~") == 1);
    }
    SUBCASE("Obtener ID de álbum no existente") {
        CHECK(dao.getAlbumId("~/no_existente") == -1);
    }
    REQUIRE(schema.close(db));
}

TEST_CASE("Obtener ID de canciones") {
    sqlite3* db = nullptr;
    model::schema::Schema schema;
    REQUIRE(schema.open(":memory:", &db, nullptr));
    MusicCatalogDAO dao(db);
    
    REQUIRE(dao.insertPerformer(0, "Persona"));

    Album ad;
    ad.path = "~";
    REQUIRE(dao.insertAlbum(ad));
    
    Song sd;
    sd.path = "~/song.mp3";
    sd.performer = "Persona";
    sd.album = "~";
    REQUIRE(dao.insertSong(sd));
    
    SUBCASE("Obtener ID de canción existente") {
        CHECK(dao.getSongId("~/song.mp3") == 1);
    }
    SUBCASE("Obtener ID de canción no existente") {
        CHECK(dao.getSongId("~/no_existente") == -1);
    }
    REQUIRE(schema.close(db));
}

#pragma endregion



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

#pragma region Pruebas de inserción de registros

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

TEST_CASE("Pruebas de inserción de grupo") {
    sqlite3* db = nullptr;
    model::schema::Schema schema;
    REQUIRE(schema.open(":memory:", &db, nullptr));

    MusicCatalogDAO dao(db);

    SUBCASE("Inserción de grupo") {
        Group gd;
        gd.name = "Grupo";
        gd.start_date = year_month_day{year{2000}, month{1}, day{1}};
        gd.end_date = year_month_day{year{2020}, month{1}, day{1}};
        REQUIRE(dao.insertGroup(gd));
    }

    SUBCASE("No se admiten nombres de grupo vacíos") {
        Group gd;
        gd.name = "";
        gd.start_date = year_month_day{year{2000}, month{1}, day{1}};
        gd.end_date = year_month_day{year{2020}, month{1}, day{1}};
        CHECK_THROWS(dao.insertGroup(gd));
    }

    REQUIRE(schema.close(db));
}

TEST_CASE("Pruebas de inserción de persona en grupo") {
    sqlite3* db = nullptr;
    model::schema::Schema schema;
    REQUIRE(schema.open(":memory:", &db, nullptr));

    MusicCatalogDAO dao(db);

    Person pd;
    pd.stage_name = "Persona";
    REQUIRE(dao.insertPerson(pd));

    Group gd;
    gd.name = "Grupo";
    REQUIRE(dao.insertGroup(gd));

    SUBCASE("Inserción de persona en grupo") {
        CHECK(dao.insertInGroup("Persona", "Grupo"));
    }

    SUBCASE("No se puede insertar una persona en un grupo que no existe") {
        CHECK(dao.insertInGroup("Persona", "Desconocido") == false);
    }

    SUBCASE("No se puede insertar una persona que no existe en un grupo en un grupo que no existe") {
        CHECK(dao.insertInGroup("Desconocido", "Grupo") == false);
    }

    REQUIRE(schema.close(db));
}

TEST_CASE("Pruebas de inserción de álbum") {
    sqlite3* db = nullptr;
    model::schema::Schema schema;
    REQUIRE(schema.open(":memory:", &db, nullptr));

    MusicCatalogDAO dao(db);

    SUBCASE("Inserción de álbum") {
        Album ad;
        ad.path = "~";
        CHECK(dao.insertAlbum(ad));
    }
    
    SUBCASE("No se puede insertar si no hay path") {
        Album ad;
        CHECK_THROWS(dao.insertAlbum(ad));
    }

    REQUIRE(schema.close(db));
}

TEST_CASE("Pruebas de inserción de canción") {
    sqlite3* db = nullptr;
    model::schema::Schema schema;
    REQUIRE(schema.open(":memory:", &db, nullptr));

    MusicCatalogDAO dao(db);

    REQUIRE(dao.insertPerformer(0, "Persona"));
    Album ad;
    ad.path = "~";
    REQUIRE(dao.insertAlbum(ad));
    
    SUBCASE("Insertar canción con una ruta, intérprete y álbum válidos.") {
        Song sd;
        sd.path = "~/song.mp3";
        sd.performer = "Persona";
        sd.album = "~";
        CHECK(dao.insertSong(sd));
    }
    SUBCASE("Insertar canción sin ruta no es válido") {
        Song sd;
        sd.performer = "Persona";
        sd.album = "~";
        CHECK_THROWS(dao.insertSong(sd));
    }
    SUBCASE("Insertar canción sin intérprete no es válido") {
        Song sd;
        sd.path = "~/song.mp3";
        sd.album = "~";
        CHECK_THROWS(dao.insertSong(sd));
    }
    SUBCASE("Insertar canción sin álbum no es válido") {
        Song sd;
        sd.path = "~/song.mp3";
        sd.performer = "Persona";
        CHECK_THROWS(dao.insertSong(sd));
    }
    SUBCASE("Insertar canción con intérprete no existente no es válido") {
        Song sd;
        sd.path = "~/song.mp3";
        sd.performer = "Desconocido";
        sd.album = "~";
        CHECK(dao.insertSong(sd) == false);
    }
    SUBCASE("Insertar canción con álbum no existente no es válido") {
        Song sd;
        sd.path = "~/song.mp3";
        sd.performer = "Persona";
        sd.album = "Desconocido";
        CHECK(dao.insertSong(sd) == false);
    }

    REQUIRE(schema.close(db));
}

#pragma endregion