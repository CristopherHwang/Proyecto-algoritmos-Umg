#include <iostream>
#include <string>
#include <fstream>
#include <cstdio> 
#include <sstream>
#include <iomanip>
#include <ctime>
#include <stdexcept>

using namespace std;

// Variables para las rutas de los archivos (uso de rutas relativas)
string CTASMONETARIAS = "CTASMONETARIAS.txt";
string TRANSACCIONES = "TRANSACCIONES.txt";
string TEMP = "TEMP.txt";


// Función para mostrar el menú
char menu() {
    char opcion = 0;
    cout << "--------------------------------------" << endl;
    cout << "Ingrese la opcion\n";
    cout << "1). Crear cuentas monetarias\n";
    cout << "2). Operar depositos\n";
    cout << "3). Operar retiros\n";
    cout << "4). Transferencias\n";
    cout << "5). Consulta saldo de cuenta\n";
    cout << "6). Salir\n";
    cout << "--------------------------------------" << endl;
    cin >> opcion;
    return opcion;
}


// Función para obtener la fecha y hora actuales
string obtenerFechaHoraActual() {
    time_t ahora = time(0);
    tm *tiempoLocal = localtime(&ahora);
    stringstream fechaHora;
    fechaHora << setfill('0') << setw(2) << tiempoLocal->tm_mday << "/"
              << setw(2) << tiempoLocal->tm_mon + 1 << "/"  
              << tiempoLocal->tm_year + 1900 << " "  
              << setw(2) << tiempoLocal->tm_hour << ":"
              << setw(2) << tiempoLocal->tm_min << ":"
              << setw(2) << tiempoLocal->tm_sec;
    return fechaHora.str();
}

// opcion 1 del menu 
// funcion para crear cuentas
string crearCuentaMonetaria() {
    ofstream cuentas(CTASMONETARIAS, ios::app);  
    if (!cuentas.is_open()) {
        return "Error: No se pudo abrir el archivo de cuentas.";
    }

    string numeroCuenta, nombre, telefono, correo, saldoInicial;

    cout << "Ingrese el numero de cuenta: ";
    cin >> numeroCuenta;
    cout << "Ingrese el nombre del titular: ";
    cin.ignore(); 
    getline(cin, nombre);
    cout << "Ingrese el telefono del titular: ";
    getline(cin, telefono);
    cout << "Ingrese el correo del titular: ";
    getline(cin, correo);
    cout << "Ingrese el saldo inicial: ";
    cin >> saldoInicial;

    cuentas << numeroCuenta << ";" << nombre << ";" << telefono << ";" << correo << ";" << saldoInicial << endl;
    cuentas.close();

    return "Cuenta creada exitosamente.";
}

// opcion 2 del menu 
// Función para operar depósitos
string depositos() {
    string numeroCuenta, linea, monto, nuevoSaldo, fecha;
    bool cuentaEncontrada = false;

    ifstream cuentas(CTASMONETARIAS);
    if (!cuentas.is_open()) {
        return "Error: No se pudo abrir el archivo de cuentas.";
    }

    ofstream temp(TEMP);
    if (!temp.is_open()) {
        cuentas.close();
        return "Error: No se pudo crear el archivo temporal.";
    }

    cout << "Ingrese el numero de cuenta: ";
    cin >> numeroCuenta;

    while (getline(cuentas, linea)) {
        stringstream ss(linea);
        string cuenta, nombre, telefono, correo, saldo;

        getline(ss, cuenta, ';');
        getline(ss, nombre, ';');
        getline(ss, telefono, ';');
        getline(ss, correo, ';');
        getline(ss, saldo, ';');

        if (cuenta == numeroCuenta) {
            cuentaEncontrada = true;
            cout << "Cuenta encontrada: " << nombre << endl;
            cout << "Saldo actual: " << saldo << endl;

            cout << "Ingrese la cantidad a depositar: ";
            cin >> monto;

            try {
                nuevoSaldo = to_string(stoi(saldo) + stoi(monto));
                cout << "Depósito exitoso. Nuevo saldo: " << nuevoSaldo << endl;
                temp << cuenta << ";" << nombre << ";" << telefono << ";" << correo << ";" << nuevoSaldo << endl;
            } catch (invalid_argument &e) {
                cout << "Error: Datos no válidos en el archivo de cuentas." << endl;
                temp << linea << endl;
            }
        } else {
            temp << linea << endl;
        }
    }

    cuentas.close();
    temp.close();

    if (cuentaEncontrada) {
        if (remove(CTASMONETARIAS.c_str()) != 0) {
            return "Error al eliminar el archivo original.";
        }

        if (rename(TEMP.c_str(), CTASMONETARIAS.c_str()) != 0) {
            return "Error al renombrar archivo temporal.";
        }

        ofstream transacciones(TRANSACCIONES, ios::app);
        if (!transacciones.is_open()) {
            return "Error al registrar transaccion.";
        }

        fecha = obtenerFechaHoraActual();
        transacciones << numeroCuenta << ";" << fecha << ";" << monto << ";D" << endl;
        transacciones.close();

        return "Depósito registrado correctamente.";
    } else {
        remove(TEMP.c_str());
        return "Cuenta no encontrada.";
    }
}

//Opcion 3 del menu
// Función para operar retiros
string retiros() {
    string numeroCuenta, linea, monto, nuevoSaldo, fecha;
    bool cuentaEncontrada = false;

    ifstream cuentas(CTASMONETARIAS);
    if (!cuentas.is_open()) {
        return "Error: No se pudo abrir el archivo de cuentas.";
    }

    ofstream temp(TEMP);
    if (!temp.is_open()) {
        cuentas.close();
        return "Error: No se pudo crear el archivo temporal.";
    }

    cout << "Ingrese el numero de cuenta: ";
    cin >> numeroCuenta;

    while (getline(cuentas, linea)) {
        stringstream ss(linea);
        string cuenta, nombre, telefono, correo, saldo;

        getline(ss, cuenta, ';');
        getline(ss, nombre, ';');
        getline(ss, telefono, ';');
        getline(ss, correo, ';');
        getline(ss, saldo, ';');

        if (cuenta == numeroCuenta) {
            cuentaEncontrada = true;
            cout << "Cuenta encontrada: " << nombre << endl;
            cout << "Saldo actual: " << saldo << endl;

            cout << "Ingrese la cantidad del retiro: ";
            cin >> monto;

            try {
                if (stoi(saldo) >= stoi(monto)) {
                    nuevoSaldo = to_string(stoi(saldo) - stoi(monto));
                    cout << "Retiro exitoso. Nuevo saldo: " << nuevoSaldo << endl;
                    temp << cuenta << ";" << nombre << ";" << telefono << ";" << correo << ";" << nuevoSaldo << endl;
                } else {
                    cout << "Fondos insuficientes." << endl;
                    temp << linea << endl;
                }
            } catch (invalid_argument &e) {
                cout << "Error: Datos no válidos en el archivo de cuentas." << endl;
                temp << linea << endl;
            }
        } else {
            temp << linea << endl;
        }
    }

    cuentas.close();
    temp.close();

    if (cuentaEncontrada) {
        if (remove(CTASMONETARIAS.c_str()) != 0) {
            return "Error al eliminar el archivo original.";
        }

        if (rename(TEMP.c_str(), CTASMONETARIAS.c_str()) != 0) {
            return "Error al renombrar archivo temporal.";
        }

        ofstream transacciones(TRANSACCIONES, ios::app);
        if (!transacciones.is_open()) {
            return "Error al registrar transaccion.";
        }

        fecha = obtenerFechaHoraActual();
        transacciones << numeroCuenta << ";" << fecha << ";" << monto << ";R" << endl;
        transacciones.close();

        return "Retiro registrado correctamente.";
    } else {
        remove(TEMP.c_str());
        return "Cuenta no encontrada.";
    }
}

//Opcion 4 del menu
// Función para operar transferencias
string transferencias() {
    string cuentaOrigen, cuentaDestino, linea, monto, nuevoSaldoOrigen, nuevoSaldoDestino;
    bool cuentaOrigenEncontrada = false, cuentaDestinoEncontrada = false;
    string saldoOrigen, saldoDestino;

    ifstream cuentas(CTASMONETARIAS);
    if (!cuentas.is_open()) {
        return "Error: No se pudo abrir el archivo de cuentas.";
    }

    ofstream temp(TEMP);
    if (!temp.is_open()) {
        cuentas.close();
        return "Error: No se pudo crear el archivo temporal.";
    }

    cout << "Ingrese el numero de cuenta origen: ";
    cin >> cuentaOrigen;
    cout << "Ingrese el numero de cuenta destino: ";
    cin >> cuentaDestino;
    cout << "Ingrese el monto a transferir: ";
    cin >> monto;

    while (getline(cuentas, linea)) {
        stringstream ss(linea);
        string cuenta, nombre, telefono, correo, saldo;

        getline(ss, cuenta, ';');
        getline(ss, nombre, ';');
        getline(ss, telefono, ';');
        getline(ss, correo, ';');
        getline(ss, saldo, ';');

        // Actualizar la cuenta de origen
        if (cuenta == cuentaOrigen) {
            cuentaOrigenEncontrada = true;
            saldoOrigen = saldo;
            if (stoi(saldoOrigen) >= stoi(monto)) {
                nuevoSaldoOrigen = to_string(stoi(saldoOrigen) - stoi(monto));
                temp << cuenta << ";" << nombre << ";" << telefono << ";" << correo << ";" << nuevoSaldoOrigen << endl;
            } else {
                cout << "Fondos insuficientes en la cuenta de origen." << endl;
                temp << linea << endl;
            }
        } 
        // Actualizar la cuenta de destino
        else if (cuenta == cuentaDestino) {
            cuentaDestinoEncontrada = true;
            saldoDestino = saldo;
            nuevoSaldoDestino = to_string(stoi(saldoDestino) + stoi(monto));
            temp << cuenta << ";" << nombre << ";" << telefono << ";" << correo << ";" << nuevoSaldoDestino << endl;
        } 
        else {
            temp << linea << endl;
        }
    }

    cuentas.close();
    temp.close();

    if (cuentaOrigenEncontrada && cuentaDestinoEncontrada) {
        if (remove(CTASMONETARIAS.c_str()) != 0) {
            return "Error al eliminar el archivo original.";
        }

        if (rename(TEMP.c_str(), CTASMONETARIAS.c_str()) != 0) {
            return "Error al renombrar archivo temporal.";
        }

        ofstream transacciones(TRANSACCIONES, ios::app);
        if (!transacciones.is_open()) {
            return "Error al registrar transacción.";
        }

        string fecha = obtenerFechaHoraActual();
        transacciones << cuentaOrigen << ";" << fecha << ";" << monto << ";T-D" << endl;  
        transacciones << cuentaDestino << ";" << fecha << ";" << monto << ";T-C" << endl; 
        transacciones.close();

        return "Transferencia registrada correctamente.";
    } else {
        remove(TEMP.c_str());
        return "Error: Cuenta origen o destino no encontrada.";
    }
}

//Opcion 5 del menu
// Función para consultar el saldo de una cuenta
string consultaSaldo() {
    string numeroCuenta, linea;
    bool cuentaEncontrada = false;

    ifstream cuentas(CTASMONETARIAS);
    if (!cuentas.is_open()) {
        return "Error: No se pudo abrir el archivo de cuentas.";
    }

    cout << "Ingrese el numero de cuenta: ";
    cin >> numeroCuenta;

    while (getline(cuentas, linea)) {
        stringstream ss(linea);
        string cuenta, nombre, telefono, correo, saldo;

        getline(ss, cuenta, ';');
        getline(ss, nombre, ';');
        getline(ss, telefono, ';');
        getline(ss, correo, ';');
        getline(ss, saldo, ';');

        if (cuenta == numeroCuenta) {
            cuentaEncontrada = true;
            cout << "Cuenta encontrada: " << nombre << endl;
            cout << "Saldo actual: " << saldo << endl;
            break;
        }
    }

    cuentas.close();

    if (!cuentaEncontrada) {
        return "Error: Cuenta no encontrada.";
    }
    return "Consulta de saldo realizada exitosamente.";
}


// Función principal con ciclo para mostrar el menú
int main() {
    char opcion;
    do {
        opcion = menu();
        switch (opcion) {
            case '1':
                cout << "Has seleccionado: Crear cuentas monetarias" << endl;
                cout << crearCuentaMonetaria() << endl;
                cout << "--------------------------------------" << endl;
                break;
            case '2':
                cout << "Has seleccionado: Operar depositos" << endl;
                cout << depositos() << endl;
                cout << "--------------------------------------" << endl;
                break;
            case '3':
                cout << "Has seleccionado: Operar retiros" << endl;
                cout << retiros() << endl;
                cout << "--------------------------------------" << endl;
                break;
            case '4':
                cout << "Has seleccionado: Transferencias" << endl;
                cout << transferencias() << endl;
                cout << "--------------------------------------" << endl;
                break;
            case '5':
                cout << "Has seleccionado: Consulta saldo de cuenta" << endl;
                cout << consultaSaldo() << endl;
                cout << "--------------------------------------" << endl;
                break;
            case '6':
                cout << "Saliendo del programa..." << endl;
                cout << "--------------------------------------" << endl;
                break;
            default:
                cout << "Opcion no valida, por favor intente nuevamente." << endl;
                cout << "--------------------------------------" << endl;
        }
    } while (opcion != '6');
    return 0;
}
