#pragma once

#include <optional>
#include <string>

using namespace std;

namespace model::entities {
    
    /** @brief Contiene los datos de un intérprete. */
    struct Performer {
        /** @brief ID del intérprete en la base de datos. */
        optional<int> id;
        /** @brief Tipo de intérprete: 0 - Persona, 1 - Grupo, 3 - Desconocido. */
        optional<int> type;
        /** @brief Nombre del intérprete. */
        optional<string> name;
    };
}