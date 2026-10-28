#pragma once

#include <optional>
#include <chrono>

using namespace std;

namespace model::entities {

    /** @brief Contiene los datos de una persona intérprete de una canción. */
    struct Person {
        /** @brief Identificador único de la persona. */
        optional<int> id;
        /** @brief Nombre artístico. */
        optional<string> stage_name;
        /** @brief Nombre real. */
        optional<string> real_name;
        /** @brief Fecha de nacimiento. */
        optional<chrono::year_month_day> birth_date;
        /** @brief Fecha de muerte. */
        optional<chrono::year_month_day> death_date;
    };
}