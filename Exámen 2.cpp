#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;

struct Venta {
    string cliente;
    string idProducto;
    string nombreProducto;
    int cantidad;
    double precioUnitario;
    string tipoPago;
    double subtotal;
    double descuento;
    double total;
};

int cantEfectivo = 0, cantTarjeta = 0, cantTransferencia = 0;
double sumaTotalVendido = 0;

void mostrarCreditos() {
    cout << "========================================" << endl;
    cout << " SISTEMA DE CONTROL DE VENTAS" << endl;
    cout << " Creador: Kevin Andres Rivas Velasquez" << endl;
    cout << " Carnet: 9941-25-2561" << endl;
    cout << "========================================" << endl;
}

void mostrarMenu() {
    cout << "\n--- MENU SISTEMA DE VENTAS ---" << endl;
    cout << "1. Registrar Venta" << endl;
    cout << "2. Mostrar las Ventas" << endl;
    cout << "3. Reporte General" << endl;
    cout << "4. Salir" << endl;
    cout << "Seleccione una opcion: ";
}

double calcularSubtotal(int cantidad, double precio) {
    return cantidad * precio;
}

double calcularDescuento(double subtotal, string tipoPago) {
    double descuento = 0;
    if (subtotal >= 500) {
        descuento += subtotal * 0.05;
    }
    if (tipoPago == "Efectivo" && subtotal >= 300) {
        descuento += subtotal * 0.02;
    }
    return descuento;
}

double calcularTotal(double subtotal, double descuento) {
    return subtotal - descuento;
}

void registrarVenta() {
    Venta v;
    ofstream archivo("ventas.txt", ios::app); 
    int opcionPago;

    cout << "\n--- Registrar Nueva Venta ---" << endl;
    cin.ignore();
    cout << "Nombre del cliente: ";
    getline(cin, v.cliente);
    cout << "ID del producto: ";
    cin >> v.idProducto;
    cin.ignore();
    cout << "Nombre del producto: ";
    getline(cin, v.nombreProducto);
    cout << "Cantidad: ";
    cin >> v.cantidad;
    cout << "Precio unitario: ";
    cin >> v.precioUnitario;
    
    cout << "Seleccione tipo de pago (1.Efectivo, 2.Tarjeta, 3.Transferencia): ";
    cin >> opcionPago;
    
    if (opcionPago == 1) v.tipoPago = "Efectivo";
    else if (opcionPago == 2) v.tipoPago = "Tarjeta";
    else v.tipoPago = "Transferencia";

    // Cálculos
    v.subtotal = calcularSubtotal(v.cantidad, v.precioUnitario);
    v.descuento = calcularDescuento(v.subtotal, v.tipoPago);
    v.total = calcularTotal(v.subtotal, v.descuento);

    // Guardar en archivo
    if (archivo.is_open()) {
        archivo << v.cliente << "|" << v.idProducto << "|" << v.nombreProducto << "|" 
                << v.cantidad << "|" << v.precioUnitario << "|" << v.tipoPago << "|"
                << v.subtotal << "|" << v.descuento << "|" << v.total << endl;
        archivo.close();
        cout << "Venta registrada exitosamente. Total: Q." << v.total << endl;
    } else {
        cout << "Error al abrir el archivo." << endl;
    }
}

void mostrarVentas() {
    ifstream archivo("ventas.txt");
    string linea;
    cout << "\n--- Lista de Ventas ---" << endl;
    cout << left << setw(15) << "Cliente" << setw(10) << "Producto" << setw(10) << "Total" << endl;
    cout << "----------------------------------------" << endl;
    
    if (archivo.is_open()) {
        while (getline(archivo, linea)) {
            cout << linea << endl;
        }
        archivo.close();
    } else {
        cout << "No hay ventas registradas." << endl;
    }
}

void generarReporte() {
    ifstream archivo("ventas.txt");
    string linea, campo;
    double totalGeneral = 0;
    int totalVentas = 0;
    
    cantEfectivo = 0; cantTarjeta = 0; cantTransferencia = 0;

    if (archivo.is_open()) {
        while (getline(archivo, linea)) {
            stringstream ss(linea);
            vector<string> campos;
            while (getline(ss, campo, '|')) {
                campos.push_back(campo);
            }
            if (campos.size() >= 9) {
                totalVentas++;
                totalGeneral += stod(campos[8]);
                if (campos[5] == "Efectivo") cantEfectivo++;
                else if (campos[5] == "Tarjeta") cantTarjeta++;
                else cantTransferencia++;
            }
        }
        archivo.close();
        
        cout << "\n--- Reporte General ---" << endl;
        cout << "Total Ventas: " << totalVentas << endl;
        cout << "Efectivo: " << cantEfectivo << endl;
        cout << "Tarjeta: " << cantTarjeta << endl;
        cout << "Transferencia: " << cantTransferencia << endl;
        cout << "Monto Total Vendido: Q." << fixed << setprecision(2) << totalGeneral << endl;
    } else {
        cout << "No hay datos para el reporte." << endl;
    }
}

int main() {
    int opcion;
    mostrarCreditos();
    
    do {
        mostrarMenu();
        cin >> opcion;
        
        switch (opcion) {
            case 1: registrarVenta(); break;
            case 2: mostrarVentas(); break;
            case 3: generarReporte(); break;
            case 4: cout << "Saliendo del sistema..." << endl; break;
            default: cout << "Opcion no valida." << endl;
        }
    } while (opcion != 4);
    
    return 0;
}
