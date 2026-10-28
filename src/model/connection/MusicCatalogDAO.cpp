#include "include/MusicCatalogDAO.hpp"
#include <sqlite3.h>
#include <Person.hpp>
#include <Group.hpp>
#include <Album.hpp>
#include <Song.hpp>
#include <chrono>


using namespace std;
using namespace chrono;

namespace model::connection {

    MusicCatalogDAO::MusicCatalogDAO() {}

    #pragma region Inicialización
    bool MusicCatalogDAO::isInitialized() {
        return initialized;
    }

    bool MusicCatalogDAO::init(sqlite3* db) {
        if (initialized) {
            return false;
        }
        this->db = db;
        initialized = true;
        return true;
    }
    #pragma endregion

    #pragma region Inserción



    #pragma endregion

}