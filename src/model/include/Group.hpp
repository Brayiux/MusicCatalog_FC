#pragma once

#include <optional>
#include <chrono>

using namespace std;

namespace model {
    
    /** @brief Contiene los datos de un grupo musical que puede interpretar
     * una canción */
    struct Group {
        /** @brief Identificador único. */
        optional<int> id;
        /** @brief Nombre del grupo. */
        optional<string> name;
        /** @brief Fecha en la cual el grupo se formó. */
        optional<chrono::year_month_day> start_date;
        /** @brief Fecha en la cual el grupo se separó. */
        optional<chrono::year_month_day> end_date;
    };
}