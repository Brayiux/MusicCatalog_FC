#include "include/MusicCatalogDAO.hpp"
#include <sqlite3.h>
#include <Person.hpp>
#include <Group.hpp>
#include <Album.hpp>
#include <Song.hpp>
#include <chrono>
#include <exception>

using namespace std;
using namespace chrono;

namespace model::connection {
    
    MusicCatalogDAO::MusicCatalogDAO(sqlite3* db) {
        this->db = db;
    }

    #pragma region Inserción



    #pragma endregion

}