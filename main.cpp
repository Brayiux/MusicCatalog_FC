#include <sqlite3.h>
#include <iostream>
#include <filesystem>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <algorithm>
#include <cctype>
#include <vector>

using namespace std;
namespace fs = filesystem;
namespace tl = TagLib;

struct Cancion {
    fs::path ruta;
    string titulo;
    string artista;
    string album;
    unsigned int anio = 0;
    unsigned int numero_pista = 0;
};

bool leerCancion(const fs::path& ruta, Cancion& cancion) {

    tl::FileRef fr(ruta.c_str());

    if (fr.isNull() || (fr.tag() == nullptr)) {
        cout << "Error al leer " << ruta << endl;
        return false;
    }
    tl::Tag* tag = fr.tag();

    cancion.ruta = ruta;
    cancion.titulo = tag->title().to8Bit(true);
    cancion.artista = tag->artist().to8Bit(true);
    cancion.album = tag->album().to8Bit(true);
    cancion.anio = tag->year();
    cancion.numero_pista = tag->track();

    return true;
}

int main() {
    cout << "========== Prueba TagLib ==========" << endl;

    cout << "Introduzca la ruta de un archivo mp3: ";
    string path;
    getline(cin, path);

    fs::path fsPath(path);

    if (!fs::exists(fsPath) || !fs::is_regular_file(fsPath) || fsPath.extension() != ".mp3") {
        cerr << "Error: la ruta no existe o no corresponde a un archivo mp3" << endl;
        return 1;
    }
    
    tl::FileRef fr(fsPath.c_str());

    if (fr.isNull() || (fr.tag() == nullptr)) {
        cerr << "Error al leer " << path << endl;
        return 2;
    }

    tl::Tag* tag = fr.tag();

    cout << "Título: " << tag->title() << '\n';
    cout << "Artista: " << tag->artist() << '\n';

    return 0;
}