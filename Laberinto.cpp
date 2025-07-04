//Angel Euardo Herrera Rodriguez matricula:362312974 Fecha:15 de junio 2025

// El codigo trata de hacer llamar al archivo de texto desde un blog de notas s lo cusl en ells esta diseñado un laberinto en especifco.
//Tambien interactua se resuelve el laberinto desde la entrada hasta haciendo el recorrido completo mediante un caracter "O" que viene siendo el mono en 
//moviento a lo cual va dejando rastro hasta llegar la salida.         S es entrada Y O es salida.




#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <windows.h> // Para Sleep y SetConsoleCursorPosition

using namespace std;

class Laberinto {
private:
    vector<string> mapa;
    vector< vector<bool> > visitado;
    vector< vector< pair<int,int> > > padre; // para reconstruir camino
    int filas, columnas;
    int inicioX, inicioY;
    bool encontrado;

    int dx[4];
    int dy[4];

    HANDLE hConsole;
public:
    Laberinto(const string& archivo) {
        encontrado = false;

dx[0] = 0; dy[0] = 1;   // abajo
dx[1] = 1; dy[1] = 0;   // derecha
dx[2] = -1; dy[2] = 0;  // izquierda
dx[3] = 0; dy[3] = -1;  // arriba

        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        if (!cargarMapa(archivo)) {
            cerr << "No se pudo abrir o procesar el archivo." << endl;
            exit(1);
        }
    }

    bool cargarMapa(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo.c_str());
        if (!archivo.is_open()) return false;

        string linea;
        while (getline(archivo, linea)) {
            mapa.push_back(linea);
        }
        archivo.close();

        filas = (int) mapa.size();

        size_t max_columnas = 0;
        for (int i = 0; i < filas; i++) {
            if (mapa[i].size() > max_columnas)
                max_columnas = mapa[i].size();
        }

        for (int i = 0; i < filas; i++) {
            if (mapa[i].size() < max_columnas)
                mapa[i].resize(max_columnas, ' ');
        }
        columnas = (int) max_columnas;

        visitado.assign(filas, vector<bool>(columnas, false));
        padre.assign(filas, vector< pair<int,int> >(columnas, make_pair(-1,-1)));

        bool inicioEncontrado = false;
        for (int i = 0; i < filas && !inicioEncontrado; i++) {
            size_t pos = mapa[i].find('O');
            if (pos != string::npos) {
                inicioY = i;
                inicioX = (int)pos;
                inicioEncontrado = true;
            }
        }

        return inicioEncontrado;
    }

    void mostrarMapa() {
        // Mueve el cursor a la posición (0,0) sin limpiar la pantalla completa
        COORD coord = {0, 0};
        SetConsoleCursorPosition(hConsole, coord);

        for (int i = 0; i < filas; i++) {
            cout << mapa[i] << " \n"; // El espacio evita posible mezcla de líneas
        }
        Sleep(200); // Pausa pequeña para animación fluida
    }

    void resolver() {
        // Antes de empezar, limpia la pantalla una vez
        system("cls");
        mostrarMapa();

        bfs(inicioX, inicioY);

        if (encontrado) {
            cout << "\n¡Salida encontrada!\n";
            reconstruirCamino();
        } else {
            cout << "\nNo se encontró la salida.\n";
        }

        mostrarMapa();
    }

private:
    void bfs(int x, int y) {
        queue< pair<int,int> > q;
        q.push(make_pair(y, x));
        visitado[y][x] = true;

        while (!q.empty()) {
            pair<int,int> actual = q.front(); q.pop();
            int cy = actual.first;
            int cx = actual.second;

            if (mapa[cy][cx] == 'S') {
                encontrado = true;
                return;
            }

            for (int i = 0; i < 4; i++) {
                int ny = cy + dy[i];
                int nx = cx + dx[i];

                if (nx >= 0 && ny >= 0 && nx < columnas && ny < filas) {
                    if (!visitado[ny][nx] && (mapa[ny][nx] == ' ' || mapa[ny][nx] == 'S')) {
                        visitado[ny][nx] = true;
                        padre[ny][nx] = make_pair(cy, cx);
                        q.push(make_pair(ny, nx));
                    }
                }
            }
        }
    }

    void reconstruirCamino() {
        int sx = -1, sy = -1;
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < columnas; j++) {
                if (mapa[i][j] == 'S') {
                    sy = i;
                    sx = j;
                    break;
                }
            }
            if (sx != -1) break;
        }

        while (padre[sy][sx] != make_pair(-1,-1)) {
            pair<int,int> p = padre[sy][sx];
            int py = p.first;
            int px = p.second;

            if (mapa[py][px] != 'O') {
                mapa[py][px] = 'O';
                mostrarMapa();
            }

            sy = py;
            sx = px;
        }
    }
};

int main() {
    Laberinto lab("laberinto.txt");
    lab.resolver();

    cout << "\nPresiona Enter para salir...";
    cin.get();
    return 0;
}
