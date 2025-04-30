#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>  // Para verificar si la carpeta existe

using namespace std;

namespace fs = std::filesystem;  // Alias para la biblioteca filesystem

struct Palabra {
    string palabra;
    string traduccion;
    string funcionalidad;
};

string obtenerRutaArchivo() {
    string carpeta = "./datos/";
    string archivo = "palabras.txt";
    string rutaArchivo = carpeta + archivo;

    // Verificar si la carpeta existe, si no, crearla
    if (!fs::exists(carpeta)) {
        fs::create_directory(carpeta);
        cout << "Carpeta 'datos' creada." << endl;
    }

    return rutaArchivo;
}

vector<Palabra> cargarPalabras(const string& nombreArchivo) {
    vector<Palabra> palabras;
    ifstream archivo(nombreArchivo.c_str());
    string linea;

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        Palabra p;
        getline(ss, p.palabra, '|');
        getline(ss, p.traduccion, '|');
        getline(ss, p.funcionalidad, '|');
        palabras.push_back(p);
    }

    archivo.close();
    return palabras;
}

void guardarPalabras(const vector<Palabra>& palabras, const string& nombreArchivo) {
    ofstream archivo(nombreArchivo.c_str());
    for (const auto& p : palabras) {
        archivo << p.palabra << "|" << p.traduccion << "|" << p.funcionalidad << "\n";
    }
    archivo.close();
}

void crearPalabra(vector<Palabra>& palabras) {
    Palabra nueva;
    cout << "Ingrese Palabra: ";
    getline(cin, nueva.palabra);
    cout << "Ingrese Traducción: ";
    getline(cin, nueva.traduccion);
    cout << "Ingrese Funcionalidad: ";
    getline(cin, nueva.funcionalidad);

    for (const auto& p : palabras) {
        if (p.palabra == nueva.palabra) {
            cout << "La palabra ya existe.\n";
            return;
        }
    }

    palabras.push_back(nueva);
    cout << "Palabra agregada.\n";
}

void leerPalabras(const vector<Palabra>& palabras) {
    if (palabras.empty()) {
        cout << "No hay palabras registradas.\n";
        return;
    }

    for (size_t i = 0; i < palabras.size(); ++i) {
        cout << i + 1 << ". Palabra: " << palabras[i].palabra << "\n";
        cout << "   Traducción: " << palabras[i].traduccion << "\n";
        cout << "   Funcionalidad: " << palabras[i].funcionalidad << "\n\n";
    }
}

void actualizarPalabra(vector<Palabra>& palabras) {
    string palabraBuscar;
    cout << "Ingrese la palabra a actualizar: ";
    getline(cin, palabraBuscar);

    for (auto& p : palabras) {
        if (p.palabra == palabraBuscar) {
            cout << "Nueva Traducción: ";
            getline(cin, p.traduccion);
            cout << "Nueva Funcionalidad: ";
            getline(cin, p.funcionalidad);
            cout << "Palabra actualizada.\n";
            return;
        }
    }
    cout << "Palabra no encontrada.\n";
}

void borrarPalabra(vector<Palabra>& palabras) {
    string palabraEliminar;
    cout << "Ingrese la palabra a eliminar: ";
    getline(cin, palabraEliminar);

    for (auto it = palabras.begin(); it != palabras.end(); ++it) {
        if (it->palabra == palabraEliminar) {
            palabras.erase(it);
            cout << "Palabra eliminada.\n";
            return;
        }
    }
    cout << "Palabra no encontrada.\n";
}

int main() {
    string nombreArchivo = obtenerRutaArchivo();  // Obtener la ruta del archivo
    vector<Palabra> palabras = cargarPalabras(nombreArchivo);

    int opcion;
    string buffer;
    do {
        cout << "\n--- MENÚ ---\n";
        cout << "1. Crear Palabra\n";
        cout << "2. Leer Palabras\n";
        cout << "3. Actualizar Palabra\n";
        cout << "4. Borrar Palabra\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        getline(cin, buffer);  // Limpia salto de línea

        switch (opcion) {
            case 1:
                crearPalabra(palabras);
                break;
            case 2:
                leerPalabras(palabras);
                break;
            case 3:
                actualizarPalabra(palabras);
                break;
            case 4:
                borrarPalabra(palabras);
                break;
            case 5:
                guardarPalabras(palabras, nombreArchivo);
                cout << "Datos guardados. Saliendo...\n";
                break;
            default:
                cout << "Opción no válida.\n";
        }

    } while (opcion != 5);

    return 0;
}
