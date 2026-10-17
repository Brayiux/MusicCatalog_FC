#pragma once

#include <optional>
#include <chrono>
#include <filesystem>


using namespace std;

namespace model {
    
    /** @brief Contiene la información de una canción ubicada en alguna ruta
     * del usuario. */
    struct Song {
        /** @brief Intérprete de la canción. */
        optional<string> performer;
        /** @brief Álbum al cual pertenece. */
        optional<string> album;
        /** @brief Ruta donde se encuentra. */
        optional<filesystem::path> path;
        /** @brief Título. */
        optional<string> title;
        /** @brief Número de pista (en el álbum). */
        optional<unsigned int> track;
        /** @brief Año de lanzamiento. */
        optional<chrono::year> year;
        /** @brief Género. */
        optional<string> genre;
    };
    
}