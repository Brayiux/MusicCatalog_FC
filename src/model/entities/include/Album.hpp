#pragma once

#include <optional>
#include <chrono>
#include <filesystem>

using namespace std;

namespace model::entities {

    /** @brief Contiene los datos de un álbum de canciones. */
    struct Album {
        /** @brief Identificador único. */
        optional<int> id;
        /** @brief Ruta del álbum. */
        optional<filesystem::path> path;
        /** @brief Nombre del álbum. */
        optional<string> name;
        /** @brief Año de lanzamiento del álbum. */
        optional<chrono::year> year;
    };
    
}
