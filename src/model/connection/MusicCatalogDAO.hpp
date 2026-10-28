#pragma once

#include <sqlite3.h>
#include <Person.hpp>
#include <Group.hpp>
#include <Album.hpp>
#include <Song.hpp>

using namespace model::entities;

namespace model::connection {

    /** @brief Proporciona una interfaz para la manipulación y acceso a datos dentro
     * de la base de datos musical. */
    class MusicCatalogDAO {
        private:
            /** @brief Puntero a la base de datos. */
            sqlite3* db = nullptr;
            /** @brief Indica si el DAO está inicializado o no. */
            bool initialized = false;
            /** @brief La única instancia del DAO. */
            static MusicCatalogDAO instance;

            /** @brief Construye una instancia del DAO */
            MusicCatalogDAO();

        public:
            /// @brief 
            /// @return La única instancia del DAO.
            static MusicCatalogDAO& getInstance();
            /**
             * @brief Indica si el DAO ya ha sido inicializado.
             * @return @b true si el DAO ya está inicializado,
             * y @b false si no. 
             */  
            bool isInitialized();
            /**
             * @brief Inicializa el DAO con una base de datos si no está
             * inicializada.
             * @param db - Base de datos con la que se inicializa el DAO.
             * @return @b true si el DAO ya había sido inicializado
             * y @b false si no.
             */
            bool init(sqlite3* db);
            /**
             * @brief Registra una persona en la base de datos e indica
             * si ya estaba registrada.
             * @param p - Datos de la persona.
             * @return @b true si la persona ya estaba registrada y @b false
             * si no.
             */
            bool registerPerson(Person p);
            /**
             * @brief Actualiza la información de una persona e indica si se
             * encontró. Sólo se actualiza la información que es llenada.
             * @param p Nuevos datos de la persona.
             * @param name Nombre de la persona.
             * @return @b true si la persona existe y la información fue actualizada
             * exitosamente, y @b false de lo contrario.
             */
            bool updatePerson(string name, Person p);
            /**
             * @brief Indica si una persona está registrada, por su nombre.
             * @param name Nombre de la persona.
             * @return @b true si ya está registrada y @b false si no.
             */
            bool existsPerson(string name);
            /**
             * @brief Elimina a una persona de la base de datos por su nombre.
             * @param name Nombre de la persona a eliminar.
             * @return @b true si la persona existía y se eliminó correctamente
             * y @b false si no.
             */
            bool deletePerson(string name);
            /**
             * @brief Registra un grupo en la base de datos e indica
             * si ya estaba registrada.
             * @param g Información del grupo.
             * @return @b true si el grupo ya estaba registrado y @b false
             * si no.
             */
            bool registerGroup(Group g);
            /**
             * @brief Actualiza la información de un grupo e indica si se
             * encontró. Sólo se actualiza la información que es llenada.
             * @param p Nuevos datos del grupo.
             * @param name Nombre del grupo.
             * @return @b true si el grupo existe y la información fue actualizada
             * exitosamente, y @b false de lo contrario.
             */
            bool updateGroup(string name, Group g);
            /**
             * @brief Indica si un grupo está registrado, por su nombre.
             * @param name Nombre del grupo.
             * @return @b true si ya está registrado y @b false si no.
             */
            bool existsGroup(string name);
            /**
             * @brief Elimina a un grupo de la base de datos por su nombre.
             * @param name Nombre del grupo a eliminar.
             * @return @b true si el grupo existía y se eliminó correctamente
             * y @b false si no.
             */
            bool deleteGroup(string name);
            /**
             * @brief Registra un álbum en la base de datos e indica
             * si ya estaba registrada.
             * @param a Información del álbum.
             * @return @b true si el álbum ya estaba registrado y @b false
             * si no.
             */
            bool registerAlbum(Album a);
            /**
             * @brief Actualiza la información de un álbum e indica si se
             * encontró. Sólo se actualiza la información que es llenada.
             * @param a Nuevos datos del álbum persona.
             * @param name Nombre del álbum persona.
             * @return @b true si el álbum existe y la información fue actualizada
             * exitosamente, y @b false de lo contrario.
             */
            bool updateAlbum(string name, Album p);
            /**
             * @brief Indica si un álbum está registrada, por su path.
             * @param path Ruta del álbum.
             * @return @b true si ya está registrado y @b false si no.
             */
            bool existsAlbum(filesystem::path path);
            /**
             * @brief Elimina a un álbum de la base de datos por su nombre.
             * @param path Ruta del álbum a eliminar.
             * @return @b true si el álbum existía y se eliminó correctamente
             * y @b false si no.
             */
            bool deleteAlbum(filesystem::path path);
            /**
             * @brief Registra una canción en la base de datos e indica
             * si ya estaba registrada.
             * @param s Información de la canción.
             * @return @b true si la canción ya estaba registrada y @b false
             * si no.
             */
            bool registerSong(Song s);
            /**
             * @brief Actualiza la información de una canción e indica si se
             * encontró. Sólo se actualiza la información que es llenada.
             * @param s Nuevos datos de la canción.
             * @param name Nombre de la canción.
             * @param performer Nombre de su intérprete.
             * @return @b true si la canción existe y la información fue actualizada
             * exitosamente, y @b false de lo contrario.
             */
            bool updateSong(string name, string performer, Song s);
            /**
             * @brief Indica si una canción está registrada, por su nombre e intérprete.
             * @param name Nombre de la canción.
             * @param performer Intérprete de la canción.
             * @return @b true si ya está registrada y @b false si no.
             */
            bool existsSong(string name, string performer);
            /**
             * @brief Elimina una canción de la base de datos por su nombre.
             * @param name Nombre de la canción a eliminar.
             * @param performer Nombre del intérprete de la canción.
             * @return @b true si la canción existía y se eliminó correctamente
             * y @b false si no.
             */
            bool deleteSong(string name, string performer);
    };
    
}