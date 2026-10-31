#pragma once

#include <sqlite3.h>
#include <Person.hpp>
#include <Group.hpp>
#include <Album.hpp>
#include <Song.hpp>

using namespace model::entities;

namespace model::connection {

    /** @brief Proporciona una interfaz para la manipulación y acceso a datos dentro
     * de la base de datos musical. Este objeto debe ser coordinado para garantizar
     * que la base de datos sea consistente.*/
    class MusicCatalogDAO {
        private:
            /** @brief Puntero a la base de datos. */
            sqlite3* db = nullptr;
            /** @brief La única instancia del DAO. */
            static MusicCatalogDAO instance;

            #pragma region Obtención de datos

            /**
             * Obtiene el ID de un intérprete por su nombre, o -1
             * si no lo encuentra.
             * @param name Nombre del intérprete.
             */
            int getPerformerId(string name);
            #pragma endregion

            /**
             * Obtiene el ID de un álbum por su path, o -1
             * si no lo encuentra.
             * @param path Ruta del álbum.
             */
            int getAlbumId(filesystem::path path);
            #pragma endregion

        public:
            /** @brief Construye una instancia del DAO
             * @param db dependencia a la base de datos.
             */
            MusicCatalogDAO(sqlite3* db);

            #pragma region Inserción de registros
            /**
             * @brief Inserta el registro de un intérprete a la base de datos.
             * @param type Tipo de intérprete. 0 para 'Person', 1 para 'Group' y
             * 2 para 'Unknown'.
             * @param name Nombre del intérprete.
             * @return @b true Si ya hay un intérprete registrado con el mismo nombre,
             * y @b false de lo contrario.
             */
            bool insertPerformer(int type, string name);
            /**
             * @brief Inserta el registro de una persona a la base de datos.
             * @param data Datos con los que se llena el registro. Al menos debe tener
             * stage_name.
             * @return @b true Si ya hay una persona registrada con el mismo nombre,
             * y @b false de lo contrario.
             */
            bool insertPerson(Person data);
            /**
             * @brief Inserta el registro de un grupo a la base de datos.
             * @param data Datos con los que se llena el registro. Al menos debe tener
             * group_name.
             * @return @b true Si ya hay una persona registrada con el mismo nombre,
             * y @b false de lo contrario.
             */
            bool insertGroup(Group data);
            /**
             * @brief Inserta el registro de una persona en un grupo a la base de datos.
             * @param person_name Nombre con el cual la persona está registrada en la base de datos.
             * @param group_name Nombre con el cual el grupo está registrado en la base de datos.
             * @return @b true Si no hay una persona y un grupo registrados con los nombres indicados,
             * y @b false de lo contrario.
             */
            bool insertInGroup(string person_name, string group_name);
            /**
             * @brief Inserta el registro de un álbum a la base de datos.
             * @param data Datos con los que se llena el registro. Al menos debe tener
             * path.
             * @return @b true Si ya hay una persona registrada con el mismo nombre,
             * y @b false de lo contrario.
             */
            bool insertAlbum(Album data);
            /**
             * @brief Inserta el registro de una canción a la base de datos.
             * @param data Datos con los que se llena el registro. Al menos debe tener
             * name y performer.
             * @return @b true Si ya hay una canción registrada con el mismo nombre y
             * artista, y @b false de lo contrario.
             */
            bool insertSong(Song data);

            #pragma endregion


            #pragma region Obtención de datos

            /**
             * @returns La información de una persona por su nombre artístico
             * o un puntero nulo si no existe.
             * @param name Nombre artístico de la persona.
             */
            Person* getPersonOrNull(string name);
            /**
             * @returns La información de un grupo por su nombre
             * o un puntero nulo si no existe.
             * @param name Nombre del grupo.
             */
            Group* getGroupOrNull(string name);
            /**
             * @returns La información de un álbum por su path
             * o un puntero nulo si no existe.
             * @param path Ruta del álbum.
             */
            Album* getAlbumOrNull(filesystem::path path);
            /**
             * @returns La información de una persona por su título y el nombre
             * de su intérprete.
             * @param title Título de la canción.
             * @param performer Intérprete.
             */
            Song* getSongOrNull(string title, string performer);

            #pragma endregion
    };
    
}