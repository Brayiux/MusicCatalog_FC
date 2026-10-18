#include "include/MusicCatalogDAO.hpp"
#include <sqlite3.h>


using namespace std;

namespace model {

    MusicCatalogDAO::MusicCatalogDAO() {}

    MusicCatalogDAO& MusicCatalogDAO::getInstance() {
        return instance;
    }

    bool MusicCatalogDAO::isInitialized() {
        return initialized;
    }

    bool MusicCatalogDAO::init(sqlite3* db) {
        if (initialized) {
            return false;
        }
        this->db = db;
        initialized = true;
    }
}