#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

using namespace std;

struct Producto {
    int codigo;
    char nombre[50];
    float precio;
    int stock;
    bool activo;
};

const char* ARCHIVO = "productos.dat";

void menu();
void menuProductos();
void registrarProducto();
void listarProductos();
void buscarProductoCodigo();
void buscarProductoNombre();
void modificarPrecio();
void actualizarStock();
void eliminarProducto();
void procesoVenta();
bool existeProducto(int codigo);

int main() {
    menu();
    return 0;
}

void menu() {
    int opcion;
    do {
        cout << "" << endl;
        cout << "=================================" << endl;
        cout << " SISTEMA DE VENTAS E INVENTARIO" << endl;
        cout << "=================================" << endl;
        cout << "1. Gestion de Productos" << endl;
        cout << "2. Proceso de Venta" << endl;
        cout << "3. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1:
                menuProductos();
                break;
            case 2:
                procesoVenta();
                break;
            case 3:
                cout << "Programa finalizado..." << endl;
                cout << "Desarrollado por: Jose Andres Santos Mota" << endl;
                break;
            default:
                cout << "Opcion invalida." << endl;
        }
    } while(opcion != 3);
}

void menuProductos() {
    int opcion;
    do {
        cout << "" << endl;
        cout << "" << endl;
        cout << "===== GESTION DE PRODUCTOS =====" << endl;
        cout << "1. Registrar Producto" << endl;
        cout << "2. Listar Productos Activos" << endl;
        cout << "3. Buscar por Codigo" << endl;
        cout << "4. Buscar por Nombre" << endl;
        cout << "5. Modificar Precio" << endl;
        cout << "6. Actualizar Stock" << endl;
        cout << "7. Eliminar / Desactivar Producto" << endl;
        cout << "8. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1: registrarProducto(); break;
            case 2: listarProductos(); break;
            case 3: buscarProductoCodigo(); break;
            case 4: buscarProductoNombre(); break;
            case 5: modificarPrecio(); break;
            case 6: actualizarStock(); break;
            case 7: eliminarProducto(); break;
            case 8: break;
            default: cout << "Opcion invalida." << endl;
        }
    } while(opcion != 8);
}

void registrarProducto() {
    Producto p;
    cout << "" << endl;
    cout << "" << endl;
    cout << "========== REGISTRO ==========" << endl;
    cout << "Codigo del producto: ";
    cin >> p.codigo;

    if (existeProducto(p.codigo)) {
        cout << "Ya existe un producto con el mismo codigo." << endl;
        return;
    }

    cin.ignore();
    cout << "Nombre: ";
    cin.getline(p.nombre, 50);
    cout << "Precio: Q";
    cin >> p.precio;
    cout << "Stock inicial: ";
    cin >> p.stock;
    p.activo = true;

    ofstream archivo(ARCHIVO, ios::binary | ios::app);
    if(!archivo) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }

    archivo.write(reinterpret_cast<char*>(&p), sizeof(Producto));
    archivo.close();
    cout << "Producto registrado." << endl;
}

void listarProductos() {
    Producto p;
    ifstream archivo(ARCHIVO, ios::binary);
    if(!archivo) {
        cout << "No existen productos registrados." << endl;
        return;
    }
    cout << "" << endl;
    cout << "" << endl;
    cout << "========== LISTADO DE PRODUCTOS ACTIVOS ==========" << endl;
    cout << left << setw(10) << "Codigo" 
         << setw(25) << "Nombre" 
         << setw(12) << "Precio (Q)" 
         << setw(10) << "Stock" << endl;


    while(archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.activo) {
            cout << left << setw(10) << p.codigo 
                 << setw(25) << p.nombre 
                 << setw(12) << fixed << setprecision(2) << p.precio 
                 << setw(10) << p.stock << endl;
        }
    }
    archivo.close();
}

void buscarProductoCodigo() {
    Producto p;
    int codigoBuscado;
    bool encontrado = false;

    cout << "" << endl;
    cout << "" << endl;
    cout << "--- Buscar por Codigo ---" << endl;
    cout << "Ingrese el codigo: ";
    cin >> codigoBuscado;

    ifstream archivo(ARCHIVO, ios::binary);
    if (!archivo) {
        cout << "No hay productos registrados." << endl;
        return;
    }

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == codigoBuscado && p.activo) {
            cout << "Producto Encontrado:" << endl;
            cout << "Codigo: " << p.codigo << endl;
            cout << "Nombre: " << p.nombre << endl;
            cout << "Precio: Q" << p.precio << endl;
            cout << "Stock: " << p.stock << endl;
            encontrado = true;
            break;
        }
    }
    archivo.close();

    if (!encontrado) cout << "Producto no encontrado o inactivo." << endl;
}

void buscarProductoNombre() {
    Producto p;
    char nombreBuscado[50];
    bool encontrado = false;

    cout << "" << endl;
    cout << "" << endl;
    cout << "--- Buscar por Nombre ---" << endl;
    cin.ignore();
    cout << "Ingrese el nombre: ";
    cin.getline(nombreBuscado, 50);

    ifstream archivo(ARCHIVO, ios::binary);
    if (!archivo) {
        cout << "No hay productos registrados." << endl;
        return;
    }

    cout << "" << endl;
    cout << "=====Resultados coincidentes ======" << endl;
    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (strstr(p.nombre, nombreBuscado) != NULL && p.activo) {
            cout << "- [" << p.codigo << "] " << p.nombre << " - Q" << p.precio << " (Stock: " << p.stock << ")" << endl;
            encontrado = true;
        }
    }
    archivo.close();

    if (!encontrado) cout << "No se encontro el producto." << endl;
}

void modificarPrecio() {
    Producto p;
    int codigoBuscado;
    bool encontrado = false;


    cout << "" << endl;
    cout << "" << endl;
    cout << "--- Modificar Precio ---" << endl;
    cout << "Ingrese el codigo del producto: ";
    cin >> codigoBuscado;

    fstream archivo(ARCHIVO, ios::binary | ios::in | ios::out);
    if (!archivo) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == codigoBuscado && p.activo) {
            cout << "Producto: " << p.nombre << " | Precio actual: Q" << p.precio << endl;
            cout << "Ingrese el nuevo precio: ";
            cin >> p.precio;

            archivo.seekp(-static_cast<int>(sizeof(Producto)), ios::cur);
            archivo.write(reinterpret_cast<char*>(&p), sizeof(Producto));
            encontrado = true;
            cout << "Precio actualizado." << endl;
            break;
        }
    }
    archivo.close();
    if (!encontrado) cout << "Producto no encontrado." << endl;
}

void actualizarStock() {
    Producto p;
    int codigoBuscado;
    bool encontrado = false;


    cout << "" << endl;
    cout << "" << endl;
    cout << "--- Actualizar Stock ---" << endl;
    cout << "Ingrese el codigo del producto: ";
    cin >> codigoBuscado;

    fstream archivo(ARCHIVO, ios::binary | ios::in | ios::out);
    if (!archivo) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == codigoBuscado && p.activo) {
            cout << "Producto: " << p.nombre << " | Stock actual: " << p.stock << endl;
            cout << "Ingrese la nueva cantidad en stock: ";
            cin >> p.stock;

            archivo.seekp(-static_cast<int>(sizeof(Producto)), ios::cur);
            archivo.write(reinterpret_cast<char*>(&p), sizeof(Producto));
            
            encontrado = true;
            cout << "Stock actualizado." << endl;
            break;
        }
    }
    archivo.close();
    if (!encontrado) cout << "Producto no encontrado." << endl;
}

void eliminarProducto() {
    Producto p;
    int codigoBuscado;
    bool encontrado = false;

    cout << "" << endl;
    cout << "--- Eliminar / Desactivar Producto ---" << endl;
    cout << "Ingrese el codigo a desactivar: ";
    cin >> codigoBuscado;

    fstream archivo(ARCHIVO, ios::binary | ios::in | ios::out);
    if (!archivo) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == codigoBuscado && p.activo) {
            p.activo = false;

            archivo.seekp(-static_cast<int>(sizeof(Producto)), ios::cur);
            archivo.write(reinterpret_cast<char*>(&p), sizeof(Producto));
            
            encontrado = true;
            cout << "Producto desactivado de la lista." << endl;
            break;
        }
    }
    archivo.close();
    if (!encontrado) cout << "Producto no encontrado o inactivo." << endl;
}

void procesoVenta() {
    Producto p;
    int codigoBuscado, cantidad;
    bool encontrado = false;

    cout << "" << endl;
    cout << "" << endl;
    cout << "========== PROCESO DE VENTA ==========" << endl;
    cout << "Ingrese el codigo del producto a vender: ";
    cin >> codigoBuscado;

    fstream archivo(ARCHIVO, ios::binary | ios::in | ios::out);
    if (!archivo) {
        cout << "Error en el inventario." << endl;
        return;
    }

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == codigoBuscado && p.activo) {
            encontrado = true;
            cout << "" << endl;
            cout << "--- Informacion del Producto ---" << endl;
            cout << "Nombre: " << p.nombre << endl;
            cout << "Precio unitario: Q" << p.precio << endl;
            cout << "Stock disponible: " << p.stock << endl;

            if (p.stock <= 0) {
                cout << "El producto no cuenta con stock disponible." << endl;
                archivo.close();
                return;
            }

            cout << "Ingrese la cantidad a comprar: ";
            cin >> cantidad;

            if (cantidad > p.stock) {
                cout << "Error: No hay suficiente stock para cubrir la solicitud." << endl;
            } else if (cantidad <= 0) {
                cout << "Error: La cantidad ingresada debe ser mayor a 0." << endl;
            } else {
                float subtotal = p.precio * cantidad;
                float iva = subtotal * 0.12;
                float total = subtotal + iva;


                cout << "" << endl;
                cout << "" << endl;
                cout << "===== DETALLE DE SU COMPRA =====" << endl;
                cout << "Subtotal: Q" << fixed << setprecision(2) << subtotal << endl;
                cout << "IVA : Q" << iva << endl;
                cout << "Total a pagar: Q" << total << endl;
                cout << "================================" << endl;

                p.stock -= cantidad;

                archivo.seekp(-static_cast<int>(sizeof(Producto)), ios::cur);
                archivo.write(reinterpret_cast<char*>(&p), sizeof(Producto));
                
                cout << "Venta realizada y stock actualizado con exito." << endl;
            }
            break;
        }
    }
    archivo.close();
    if (!encontrado) cout << "El producto buscado no existe o no esta activo." << endl;
}

bool existeProducto(int codigo) {
    Producto p;
    ifstream archivo(ARCHIVO, ios::binary);
    if (!archivo) return false;

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == codigo && p.activo) {
            archivo.close();
            return true;
        }
    }
    archivo.close();
    return false;
}