#include "include/MusicCatalogDAO.hpp"
#include <sqlite3.h>


using namespace std;

namespace model {

    MusicCatalogDAO::MusicCatalogDAO() {}

    MusicCatalogDAO& MusicCatalogDAO::getInstance() {
        return instance;
    }
}