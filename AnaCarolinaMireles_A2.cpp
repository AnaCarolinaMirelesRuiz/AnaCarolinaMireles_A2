// AnaCarolinaMireles_A2.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLRETURN ret;

    //Asignar un gestor de entorno
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    //Asignar un gestor de conexion
    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    //Conexion a base de datos
    ret = SQLConnect(hDbc, (SQLWCHAR*)L"SQLServer", SQL_NTS, (SQLWCHAR*)L"", SQL_NTS, (SQLWCHAR*)L"", SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        cout << "Conectado a la base de datos exitosamente." << endl;
        SQLHSTMT hStmt;
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        wcout << L"+------------+---------------------------+------------------------+---------------+----------------------+--------------------+------------------+---------------+ " << endl;
        wcout << L"|   Num      |       Nombre              |  Fecha de nacimiento          RFC      |   Centro Trabajo     |     Puesto         |   Descripcion        Directivo   | " << endl;
        wcout << L"+------------+---------------------------+------------------------+---------------+----------------------+--------------------+------------------+---------------+" << endl;
        //Select a la base de datos
        ret = SQLExecDirect(hStmt, (SQLWCHAR*)L"SELECT * FROM Empleados", SQL_NTS);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            int Num_empleado;
            SQLWCHAR Nombre[50];
            SQLWCHAR Apellido_paterno[50];
            SQLWCHAR Apellido_materno[50];
            SQLWCHAR Fecha_Nacimiento[50];
            SQLWCHAR RFC[15];
            SQLWCHAR Centro_Trabajo[50];
            SQLWCHAR Puesto[50];
            SQLWCHAR Descripcion_Puesto[50];
            SQLWCHAR Directivo[10];
            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &Num_empleado, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_WCHAR, Nombre, sizeof(Nombre), NULL);
                SQLGetData(hStmt, 3, SQL_C_WCHAR, Apellido_paterno, sizeof(Apellido_paterno), NULL);
                SQLGetData(hStmt, 4, SQL_C_WCHAR, Apellido_materno, sizeof(Apellido_materno), NULL);
                SQLGetData(hStmt, 5, SQL_C_WCHAR, Fecha_Nacimiento, sizeof(Fecha_Nacimiento), NULL);
                SQLGetData(hStmt, 6, SQL_C_WCHAR, RFC, sizeof(RFC), NULL);
                SQLGetData(hStmt, 7, SQL_C_WCHAR, Centro_Trabajo, sizeof(Centro_Trabajo), NULL);
                SQLGetData(hStmt, 8, SQL_C_WCHAR, Puesto, sizeof(Puesto), NULL);
                SQLGetData(hStmt, 9, SQL_C_WCHAR, Descripcion_Puesto, sizeof(Descripcion_Puesto), NULL);
                SQLGetData(hStmt, 10, SQL_C_WCHAR, Directivo, sizeof(Directivo), NULL);

                //Concatenar apellido
                wstring last_names = wstring(Apellido_paterno) + L" " + wstring(Apellido_materno);
                wstring full_name = wstring(Nombre) + L" " + wstring(last_names);
                wstring dir = wstring(Directivo);
                wstring respuesta;

                if (dir == L"1") {
                    respuesta = L"Si";
                }
                else {
                    respuesta = L"No";
                }

                //Imprimir datos de la fila con alineacion
                wcout << L"| " << setw(6) << left << Num_empleado
                    << L"| " << setw(10) << left << full_name
                    << L"| " << setw(10) << left << Fecha_Nacimiento
                    << L"| " << setw(10) << left << RFC
                    << L"| " << setw(10) << left << Centro_Trabajo
                    << L"| " << setw(10) << left << Puesto
                    << L"| " << setw(10) << left << Descripcion_Puesto
                    << L"| " << setw(10) << left << respuesta << L" |" << endl;
            }
            wcout << L"+------------+---------------------------+------------------------+---------------+----------------------+--------------------+------------------+---------------+" << endl;
        }
        // Liberar el manejador de conexión
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
    else {
        cout << "Fallo la conexion a la base de datos" << endl;
    }

    //Desconexion y liberacion
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}