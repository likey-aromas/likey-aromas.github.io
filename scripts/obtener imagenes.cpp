#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

struct Producto{
	string codigo;
	string url;
};

int main(int argc, char *argv[]) {

	vector<string> codigos = {"302", "1942", "192", "1214", "417", "1806", "1807",
		"376", "814", "486", "566", "1564", "599", "604", "1730", "1756", "1757",
		"1610", "1996", "1315", "1301", "1934", "1995", "47", "690", "867", "1468",
		"1396", "1932", "1933", "1302", "1864", "2089", "1609", "600", "1458",
		"1117", "1731", "1057", "1023", "1071", "1973", "1590", "180", "2018",
		"2055", "2073", "1987", "1965", "1329"};
	
	ifstream grid("grid.txt", ios::in);
	ofstream salida("urls.txt", ios::out);
	int contEntrada=0, contSalida=0;
	if(grid.is_open() && salida.is_open()){
		string linea;
		while(getline(grid, linea)){
			if(linea.find("<img") != string::npos){
				Producto p;
				int inicio = linea.find("id=\"img") + 8;
				int fin = linea.find("\"", inicio);
				p.codigo = linea.substr(inicio, fin-inicio);
				contEntrada++;
				auto it = find(codigos.begin(), codigos.end(), p.codigo);
				
				if(it!=codigos.end()){
					inicio = linea.find("src=\"");
					inicio += 5;
					fin = linea.find("\"", inicio);
					p.url = linea.substr(inicio, fin-inicio);
					salida << p.codigo << "	" << p.url << endl;
					it = find(codigos.begin(), codigos.end(), p.codigo);
					if(it!=codigos.end())
						codigos.erase(it);
					codigos.erase
					contSalida++;
				}
			}
		}
		cout << contEntrada << " productos leidos." << endl;
		cout << contSalida << " productos guardados." << endl;
		cout << "No se encontraron imagenes de los siguientes codigos: ";
		for(
	}
	else{
		cout << "Error al abrir los archivos.";
	}
	
	
	
	return 0;
}

