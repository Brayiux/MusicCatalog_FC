#include <sqlite3.h>

namespace schema {
    
    /**
     * Se encarga de manejar la inicialización y finalización de la conexión
     * con la base de datos musical.
     */
    class Schema {
        public:
            /**
             * El esquema en lenguaje SQL.
             */
            static const char* SCHEMA;
            
            /**
             * @brief Construye un esquema de cero.
             */
            Schema();
            /**
             * @brief Crea la base de datos si no existe y abre una conexión
             * con ella bajo un esquema.
             * @param path Ruta donde se crea o abre la base de datos.
             * @param db La base de datos.
             * @param errmsg Mensaje de error en caso de que la base de
             * datos no se pueda abrir/crear.
             * @return @b true si la base de datos se pudo crear/abrir
             * y @b false de lo contrario.
             */
            bool open(const char* path, sqlite3** db, char** errmsg);
            /**
             * @brief Cierra la conexión con la base de datos.
             * @param db La base de datos.
             * @return @b true si la base de datos se cerró exitosamente y
             * @b false si no.
             */
            bool close(sqlite3* db);
    };
}