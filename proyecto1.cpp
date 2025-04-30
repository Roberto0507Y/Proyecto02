#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <regex>
#include <algorithm>

using namespace std;

const string ARCHIVO = "palabras.txt";

// ------------------ FUNCIONES AUXILIARES ------------------

map<string, string> cargarDiccionario() {
    map<string, string> diccionario;
    ifstream file(ARCHIVO);
    string linea;

    while (getline(file, linea)) {
        istringstream ss(linea);
        string palabra, traduccion, funcionalidad;
        if (getline(ss, palabra, '|') &&
            getline(ss, traduccion, '|') &&
            getline(ss, funcionalidad)) {
            diccionario[palabra] = traduccion;
        }
    }

    return diccionario;
}

bool esFuncion(const string& linea) {
    return regex_match(linea, regex(R"(\s*\w+\s+\w+\s*\(.*\)\s*\{?)"));
}

string traducirLinea(const string& linea, const map<string, string>& diccionario) {
    string traducida;
    istringstream ss(linea);
    string palabra;

    while (ss >> palabra) {
        string limpia = palabra;
        limpia.erase(remove_if(limpia.begin(), limpia.end(), ::ispunct), limpia.end());
        if (diccionario.count(limpia)) {
            traducida += diccionario.at(limpia);
        } else {
            traducida += limpia;
        }
        if (!palabra.empty() && ispunct(palabra.back())) {
            traducida += palabra.back();
        }
        traducida += " ";
    }

    return traducida;
}

// ------------------ CRUD PALABRAS ------------------

void agregarPalabra() {
    ofstream archivo(ARCHIVO, ios::app);
    string palabra, traduccion, funcionalidad;

    cout << "Ingrese Palabra: "; cin >> palabra;
    cout << "Ingrese Traduccion: "; cin >> traduccion;
    cin.ignore();
    cout << "Ingrese Funcionalidad: "; getline(cin, funcionalidad);

    archivo << palabra << "|" << traduccion << "|" << funcionalidad << endl;
    archivo.close();
    cout << "Palabra agregada correctamente.\n";
}

void leerPalabras() {
    ifstream archivo(ARCHIVO);
    string linea;
    cout << "\n--- Lista de Palabras ---\n";
    while (getline(archivo, linea)) {
        cout << linea << endl;
    }
    archivo.close();
}

void actualizarPalabra() {
    string palabraBuscar, nuevaLinea;
    cout << "Ingrese la palabra a actualizar: "; cin >> palabraBuscar;
    cin.ignore();

    ifstream archivo(ARCHIVO);
    ofstream temporal("temp.txt");
    string linea;
    bool encontrada = false;

    while (getline(archivo, linea)) {
        istringstream ss(linea);
        string palabra;
        getline(ss, palabra, '|');

        if (palabra == palabraBuscar) {
            string nuevaPalabra, nuevaTraduccion, nuevaFunc;
            cout << "Nueva palabra: "; cin >> nuevaPalabra;
            cout << "Nueva traduccion: "; cin >> nuevaTraduccion;
            cin.ignore();
            cout << "Nueva funcionalidad: "; getline(cin, nuevaFunc);
            temporal << nuevaPalabra << "|" << nuevaTraduccion << "|" << nuevaFunc << endl;
            encontrada = true;
        } else {
            temporal << linea << endl;
        }
    }

    archivo.close();
    temporal.close();
    remove(ARCHIVO.c_str());
    rename("temp.txt", ARCHIVO.c_str());

    if (encontrada)
        cout << "Palabra actualizada.\n";
    else
        cout << "Palabra no encontrada.\n";
}

void eliminarPalabra() {
    string palabraBuscar;
    cout << "Ingrese la palabra a eliminar: "; cin >> palabraBuscar;

    ifstream archivo(ARCHIVO);
    ofstream temporal("temp.txt");
    string linea;
    bool eliminada = false;

    while (getline(archivo, linea)) {
        istringstream ss(linea);
        string palabra;
        getline(ss, palabra, '|');

        if (palabra == palabraBuscar) {
            eliminada = true;
        } else {
            temporal << linea << endl;
        }
    }

    archivo.close();
    temporal.close();
    remove(ARCHIVO.c_str());
    rename("temp.txt", ARCHIVO.c_str());

    if (eliminada)
        cout << "Palabra eliminada.\n";
    else
        cout << "Palabra no encontrada.\n";
}

// ------------------ TRADUCTOR ------------------

void traducirCodigo() {
    map<string, string> diccionario = cargarDiccionario();
    cout << "\nIngrese el código C++ (finalice con línea vacía):\n";
    string linea;
    vector<string> estructuras = {"if", "else", "while", "for", "switch"};
    vector<string> pilaEstructuras;

    cin.ignore();
    while (getline(cin, linea)) {
        if (linea.empty()) break;

        string traduccion = traducirLinea(linea, diccionario);

        if (esFuncion(linea)) {
            cout << traduccion << endl;
            continue;
        }

        if (linea.find("{") != string::npos) {
            for (const string& e : estructuras) {
                if (linea.find(e) != string::npos) {
                    cout << "inicio " << diccionario[e] << endl;
                    pilaEstructuras.push_back(e);
                    break;
                }
            }
        } else if (linea.find("}") != string::npos && !pilaEstructuras.empty()) {
            string ultima = pilaEstructuras.back();
            pilaEstructuras.pop_back();
            cout << "fin " << diccionario[ultima] << endl;
        } else {
            cout << traduccion << endl;
        }
    }
}

// ------------------ MENÚ PRINCIPAL ------------------

int main() {
    int opcion;
    do {
        cout << "\n--- MENÚ ---\n";
        cout << "1. Agregar palabra\n";
        cout << "2. Leer palabras\n";
        cout << "3. Actualizar palabra\n";
        cout << "4. Eliminar palabra\n";
        cout << "5. Traducir código C++\n";
        cout << "6. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch(opcion) {
            case 1: agregarPalabra(); break;
            case 2: leerPalabras(); break;
            case 3: actualizarPalabra(); break;
            case 4: eliminarPalabra(); break;
            case 5: traducirCodigo(); break;
            case 6: cout << "Saliendo...\n"; break;
            default: cout << "Opción inválida.\n";
        }
    } while(opcion != 6);

    return 0;
}
