/*Compone el catalogo con todos los productos se la seccion de la pagina
extraida en el archivo "Productos.txt" sin filtro alguno y aplicandole un %*/

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct Producto{
	string codigo;
	string link;
	string descripcion;
	string precio;
};

class VectorDinamico {
private:
	Producto * datos;
	int cantidad;
public:
	VectorDinamico(){datos=nullptr; cantidad=0;}
	void agregar(Producto datoNuevo){
		Producto * nuevoVec = new Producto[cantidad+1];
		for(int i=0; i<cantidad; i++)
			nuevoVec[i] = datos[i];
		nuevoVec[cantidad] = datoNuevo;
		delete[] datos;
		datos = nuevoVec;
		cantidad++;
	}
	Producto getDato(int i){return datos[i];}
	int getCant(){return cantidad;}
};

int main(){

	string linea, link, descripcion, precio, codigo;
	int posInicial, posFinal, contProd=0;
	
	ifstream archivoEntrada;
	archivoEntrada.open("Lito.txt");
	if(!archivoEntrada.is_open()){
		cout << "No se pudo abrir el archivo de entrada.";
		return 0;
	}
	VectorDinamico productos;	//almacena los productos procesados del archivo
	
	//recopila los datos de los productos del bloque completo
	//(link de imagen, codigo, decripcion, precio(aplicado un %))
	while(getline(archivoEntrada, linea)){
		if(linea.find("class=\"image\"") != string::npos){
			posInicial = linea.find("src")+5;
			posFinal = linea.find("\"", posInicial);
			link = linea.substr(posInicial, posFinal-posInicial);
		}
		if(linea.find("class=\"code\"") != string::npos){
			posInicial = linea.find("COD:")+5;
			posFinal = linea.find("</", posInicial);
			codigo = linea.substr(posInicial, posFinal-posInicial);
		}
		if(linea.find("class=\"title\"") != string::npos){
			posInicial = linea.find("</span>")+8;
			posFinal = linea.find("</p>", posInicial);
			descripcion = linea.substr(posInicial, posFinal-posInicial);
		}
		if(linea.find("h4 class") != string::npos){
			posInicial = linea.find("$")+1;
			posFinal = linea.find("</", 1);
			precio = linea.substr(posInicial, posFinal-posInicial);
			if(precio.find(".") != string::npos){
				precio.erase(precio.find("."), 1);
			}
			if(precio.find(",") != string::npos){
				for(char&c : precio){
					if(c == ',')
						c = '.';
				}
			}
			float nPrecio = stof(precio);	//precio tipo numero para poder modificar
			int nPrecioFinal = (nPrecio*1.50+100)/ 100;
			nPrecioFinal = nPrecioFinal*100;
			precio = to_string(nPrecioFinal);
			if(precio.find(".") != string::npos){
				for(char&c : precio){
					if(c == '.')
						c = ',';
				}
			}
			contProd++;
		}
		if(linea.find("class=\"nuevoBtn") != string::npos){
			if(linea.find("Comprar") != string::npos){ //lo agrega solo si esta disponible para compra, si hay stock
				Producto a;
				a.codigo = codigo;
				a.link = link;
				a.descripcion = descripcion;
				a.precio = precio;
				productos.agregar(a);
			}
		}
	}
	
	archivoEntrada.close();
	cout << contProd << " productos encontrados." << endl;
	
	//genera el codigo HTML
	ofstream archivoSalida;
	archivoSalida.open("index.html");
	if(!archivoSalida.is_open()){
		cout << "Error al abrir el archivo.";
	}
	
	contProd = 0;
	
	archivoSalida << 
		"<!DOCTYPE html>\n"
		"<html lang=\"es\">\n"
		"<head>\n"
		"	<meta charset=\"UTF-8\">\n"
		"	<title>Likey Aromas - Catalogo</title>\n"
		"	<link rel=\"stylesheet\" href=\"styles.css\">\n"
		"</head>\n"
		"<body>\n"
		"	<div class=\"encabezado\">\n"
		"		<img src=\"Logo Likey.png\" alt=\"Logo Likey\">\n"
		"	</div>\n"
		"	<div class=\"catalogo\">\n";
	
	
	int cantidad = productos.getCant();
	int codigosLikey[] = {302, 1167, 192, 1214, 417, 1806, 1807, 376, 814, 486,
		566, 1564, 1467, 599, 604, 1730, 1756, 1757, 1610, 1847, 1315, 1301, 47,
		690, 867, 1468, 1396, 1932, 1933, 600, 1458, 1117, 1731, 1057, 1023,
		1071, 1590, 629, 180, 114};
	for(int i=0; i<cantidad; i++){
		Producto a = productos.getDato(i);
		//buscar en los codigos incorporados en Likey
		
		bool band = false;
		for(int i=0; i<40; i++){
			if(stoi(a.codigo) == codigosLikey[i]){
				band = true;
				i=40;	//sale del for
			}
		}
		//si el codigo del producto esta en los codigos de Likey, no lo agrega al catalogo
		if(!band){
			archivoSalida <<
			"		<div class=\"producto\">" << endl <<
			"			<img src=\"" << a.link << "\">" << endl <<
			"			<h3>" << a.descripcion << "</h3>" << endl <<
			"			<div class=\"precio\">$" << a.precio << "</div>" << endl <<
			"		</div>" << endl;
			contProd++;
		}
	}
		

	//Este for agrega todos los productos procesados al catalogo
//	for(auto & a : productos){
//		archivoSalida <<
//						"		<div class=\"producto\">" << endl <<
//						"			<img src=\"" << a.link << "\">" << endl <<
//						"			<h3>" << a.descripcion << "</h3>" << endl <<
//						"			<div class=\"precio\">$" << a.precio << "</div>" << endl <<
//						"		</div>" << endl;
//	}
	
	
	
	archivoSalida << 
		"	</div>\n"
		"</body>\n"
		"</html>";
	
	archivoEntrada.close();
	archivoSalida.close();
	cout << contProd << " productos cargados al catalogo." << endl;
	
	return 0;
}
