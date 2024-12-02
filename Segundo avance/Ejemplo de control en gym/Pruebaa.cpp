//Registro clientes del Gimnasio

#include <iostream>
#include <fstream>
#include <stdlib.h>

//Limpiar pantalla
#ifdef _WIN32
#define CLEAR "cls"
#elif defined(unix)||defined(__unix__)||defined(__unix)||defined(__APPLE__)||defined(__MACH__)
#define CLEAR "clear"
#else
#error "SO no soportado para limpiar pantalla"
#endif

using namespace std;


class menu
{
private:
    string nombre;
    string domicilio;
    string celular;
    string fechaPago;
    string codigoCliente;

public:
    void altaCliente();
    void bajaCliente();
    void modificarCliente();
    void listarClientes();
    void menuPrincipal();
    void detallesCliente();
    void mostarRegistro(string codigo);
    void buscarCliente();
};

void pausa();
void error();
void comprobarArchivo();

void menu::menuPrincipal()
{
    int opcion;
    do
    {
        cout<<"\t\t\t\t***Registro de clientes del Gimnasio***\t\t\t\t\n\n";
        cout<<"1. Dar de alta un cliente\n\n";
        cout<<"2. Mostrar detalles de un cliente\n\n";
        cout<<"3. Listar clientes\n\n";
        cout<<"4. Buscar cliente (Nombre)\n\n";
        cout<<"5. Dar de baja un cliente\n\n";
        cout<<"6. Modificar un cliente\n\n";
        cout<<"7. Salir\n\n";
        cout<<"Opci\242n: ";
        cin>>opcion;
        system(CLEAR);
        switch(opcion)
        {
        default:
            cout<<"Ha ingresado una opci\242n no valida!\n\n";
            break;

        case 1:
            altaCliente();
            break;

        case 2:
            detallesCliente();
            break;

        case 3:
            listarClientes();
            break;

        case 4:
            buscarCliente();
            break;

        case 5:
            bajaCliente();
            break;

        case 6:
            modificarCliente();
            break;

        case 7:
            break;
        }
    }
    while(opcion!=7);
}

void menu::altaCliente()
{
    ofstream escritura;
    ifstream verificador;
    string auxCodigo;
    bool coincidencia=false;
    escritura.open("clientes.txt",ios::app);
    verificador.open("clientes.txt",ios::in);
    if(escritura.is_open())
    {
        cout<<"\t\t\t\t***Dar de alta un cliente***\t\t\t\t\n\n";
        fflush(stdin);
        cout<<"Ingresa el c\242digo del cliente: ";
        getline(cin,auxCodigo);
        if(auxCodigo=="")
            do
            {
                cout<<"c\242digo de cliente no v\240lido!, intentalo nuevamente: ";
                getline(cin,auxCodigo);
            }
            while(auxCodigo=="");
        do
        {
            verificador.seekg(0);
            getline(verificador,codigoCliente);
            while(!verificador.eof())
            {
                getline(verificador,nombre);
                getline(verificador,domicilio);
                getline(verificador,celular);
                getline(verificador,fechaPago);

                if(codigoCliente==auxCodigo)
                {
                    coincidencia=true;
                    cout<<"\n\nYa existe un cliente con ese c\242digo!\n\n";
                    cout<<"El cliente con ese c\242digo es: "<<nombre<<"\n\n";
                    cout<<"Ingresa un c\242digo v\240lido!: ";
                    getline(cin,auxCodigo);
                    if(auxCodigo=="")
                        do
                        {
                            cout<<"\nc\242digo de cliente no v\240lido!, intentalo nuevamente: ";
                            getline(cin,auxCodigo);
                        }
                        while(auxCodigo=="");

                    break;
                }

                getline(verificador,codigoCliente);
            }

            if(verificador.eof()&&auxCodigo!=codigoCliente)
                coincidencia=false;

        }
        while(coincidencia==true);
        system(CLEAR);
        codigoCliente=auxCodigo;
        cout<<"\t\t\t\t***Dar de alta un cliente***\t\t\t\t\n\n";
        cout<<"Ingresa el c\242digo del cliente: ";
        cout<<codigoCliente;
        cout<<"\n\n";
        fflush(stdin);
        cout<<"Ingresa el nombre del cliente: ";
        getline(cin,nombre);
        cout<<"\n\n";
        fflush(stdin);
        cout<<"Ingresa el domicilio del cliente: ";
        getline(cin,domicilio);
        cout<<"\n\n";
        fflush(stdin);
        cout<<"Ingresa el n\243mero de telefono del cliente: ";
        getline(cin,celular);
        cout<<"\n\n";
        fflush(stdin);
        cout<<"Ingresa la fecha de inscripci\242n del cliente: ";
        getline(cin,fechaPago);
        cout<<"\n\n";

        escritura<<codigoCliente<<endl<<nombre<<endl<<domicilio<<endl<<celular
                 <<endl<<fechaPago<<endl;

        cout<<"El registro se ha completado correctamente.\n\n";
    }

    else
    {
        error();
    }

    escritura.close();
    verificador.close();
    pausa();
}

void menu::bajaCliente()
{
    ifstream lectura;
    ofstream auxiliar;
    bool encontrado=false;
    string auxCodigo;
    string respuesta;
    lectura.open("clientes.txt",ios::in);
    if(lectura.is_open())
    {
        cout<<"\t\t\t\t***Dar de baja un cliente***\t\t\t\t\n\n";
        auxiliar.open("auxiliar.txt",ios::out);
        fflush(stdin);
        getline(lectura,codigoCliente);
        if(lectura.eof())
        {
            cout<<"No hay ningun cliente registrado";
        }
        else
        {
            lectura.seekg(0);
            cout<<"Ingresa el c\242digo del cliente que deseas dar de baja: ";
            getline(cin,auxCodigo);
            getline(lectura,codigoCliente);
            while(!lectura.eof())
            {
                getline(lectura,nombre);
                getline(lectura,domicilio);
                getline(lectura,celular);
                getline(lectura,fechaPago);
                if(auxCodigo==codigoCliente)
                {
                    encontrado=true;
                    cout<<"\n\nRegistro Encontrado\n\n";
                    cout<<"C\242digo: "<<codigoCliente<<endl;
                    cout<<"Nombre: "<<nombre<<endl;
                    cout<<"Domicilio: "<<domicilio<<endl;
                    cout<<"Celular: "<<celular<<endl;
                    cout<<"Fecha de inscripci\242n: "<<fechaPago<<endl;
                    cout<<"\n\n";
                    cout<<"Realmente deseas dar de baja a: "<<nombre<<" (s/n)?: ";
                    getline(cin,respuesta);
                    if(respuesta=="s"||respuesta=="S"||respuesta=="si"||respuesta=="SI"||
                            respuesta=="Si"||respuesta=="sI")
                    {
                        cout<<"\n\nEl cliente se ha dado de baja correctamente\n\n";
                    }

                    else
                    {

                        cout<<"\n\nCliente conservado\n\n";
                        auxiliar<<codigoCliente<<endl<<nombre<<endl<<domicilio<<endl<<celular
                                <<endl<<fechaPago<<endl;

                    }

                }
                else
                {
                    auxiliar<<codigoCliente<<endl<<nombre<<endl<<domicilio<<endl<<celular
                            <<endl<<fechaPago<<endl;
                }
                getline(lectura,codigoCliente);
            }
            if(encontrado==false)
            {
                cout<<"\n\nNo se encontr\242 el c\242digo: "<<auxCodigo<<"\n\n";
            }
        }
        lectura.close();
        auxiliar.close();
        remove("clientes.txt");
        rename("auxiliar.txt","clientes.txt");

    }
    else
    {
        error();
    }
    pausa();
}

void menu::detallesCliente()
{
    string auxCodigo;
    ifstream mostrar;
    bool encontrado=false;
    mostrar.open("clientes.txt",ios::in);
    if(mostrar.is_open())
    {
        fflush(stdin);
        cout<<"\t\t\t\t***Consultar detalles de un cliente***\t\t\t\t\n\n";
        getline(mostrar,codigoCliente);
        if(mostrar.eof())
        {
            cout<<"No hay ningun cliente registrado";
        }
        else
        {
            mostrar.seekg(0);
            cout<<"Ingresa el c\242digo del cliente que deseas consultar detalles: ";
            getline(cin,auxCodigo);
            getline(mostrar,codigoCliente);
            while(!mostrar.eof())
            {
                getline(mostrar,nombre);
                getline(mostrar,domicilio);
                getline(mostrar,celular);
                getline(mostrar,fechaPago);
                if(auxCodigo==codigoCliente)
                {
                    encontrado=true;
                    cout<<"\n\nRegistro Encontrado\n\n";
                    cout<<"C\242digo: "<<codigoCliente<<endl;
                    cout<<"Nombre: "<<nombre<<endl;
                    cout<<"Domicilio: "<<domicilio<<endl;
                    cout<<"Celular: "<<celular<<endl;
                    cout<<"Fecha de inscripci\242n: "<<fechaPago<<endl;
                    cout<<"\n\n";
                }

                getline(mostrar,codigoCliente);
            }

            if(encontrado==false)
            {
                cout<<"\n\nNo se encontro el registro: "<<auxCodigo<<"\n\n";
            }
        }
    }

    else
    {
        error();
    }

    mostrar.close();
    pausa();
}

void menu::modificarCliente()
{
    ifstream lectura;
    ifstream verificador;
    ofstream auxiliar;
    string auxCodigo;
    string codigoModif;
    string auxNombre;
    string auxDomicilio;
    string auxCelular;
    string auxFecha;
    bool encontrado=false;
    bool coincidencia=false;
    bool mismoCodigo=false;
    lectura.open("clientes.txt",ios::in);
    if(lectura.is_open())
    {
        cout<<"\t\t\t\t***Modificar los datos de un cliente***\t\t\t\t\n\n";
        verificador.open("clientes.txt",ios::in);
        auxiliar.open("auxiliar.txt",ios::out);
        fflush(stdin);
        getline(lectura,codigoCliente);
        if(lectura.eof())
        {
            cout<<"No hay ningun cliente registrado";
        }
        else
        {
            lectura.seekg(0);
            cout<<"Ingresa el c\242digo del cliente que deseas modificar: ";
            getline(cin,auxCodigo);

            if(auxCodigo=="")
            {
                do
                {
                    cout<<"c\242digo de cliente no v\240lido!, intentalo nuevamente: ";
                    getline(cin,auxCodigo);
                }
                while(auxCodigo=="");
            }

            codigoModif=auxCodigo;

            getline(lectura,codigoCliente);
            while(!lectura.eof())
            {
                getline(lectura,nombre);
                getline(lectura,domicilio);
                getline(lectura,celular);
                getline(lectura,fechaPago);
                if(auxCodigo==codigoCliente)
                {
                    encontrado=true;
                    mostarRegistro(codigoModif);

                    cout<<"**********************************************";
                    cout<<"\n\n";
                    cout<<"Ingresa la nueva informaci\242n para el cliente\n\n";
                    fflush(stdin);
                    cout<<"Ingresa el c\242digo del cliente: ";
                    getline(cin,auxCodigo);
                    if(auxCodigo==codigoModif)
                    {
                        mismoCodigo=true;
                    }
                    if(mismoCodigo==false)
                    {
                        do
                        {
                            if(auxCodigo==codigoModif)
                            {
                                coincidencia=false;
                                break;
                            }
                            verificador.seekg(0);
                            getline(verificador,codigoCliente);
                            while(!verificador.eof())
                            {
                                getline(verificador,nombre);
                                getline(verificador,domicilio);
                                getline(verificador,celular);
                                getline(verificador,fechaPago);

                                if(codigoCliente==auxCodigo)
                                {
                                    coincidencia=true;
                                    cout<<"\n\nYa existe un cliente con ese c\242digo!\n\n";
                                    cout<<"El cliente con ese c\242digo es: "<<nombre<<"\n\n";
                                    cout<<"Ingresa un c\242digo v\240lido!: ";
                                    getline(cin,auxCodigo);

                                    if(auxCodigo=="")
                                    {
                                        do
                                        {
                                            cout<<"\nc\242digo de cliente no v\240lido!, ";
                                            cout<<"intentalo nuevamente: ";
                                            getline(cin,auxCodigo);
                                        }
                                        while(auxCodigo=="");
                                    }
                                    break;
                                }

                                getline(verificador,codigoCliente);
                            }

                            if(verificador.eof()&&auxCodigo!=codigoCliente)
                            {
                                coincidencia=false;
                            }

                        }
                        while(coincidencia==true);
                    }
                    system(CLEAR);
                    cout<<"***Modificar los datos de un cliente***\n\n";
                    cout<<"Ingresa el c\242digo del cliente que deseas modificar: ";
                    cout<<codigoModif;
                    mostarRegistro(codigoModif);
                    cout<<"**********************************************";
                    cout<<"\n\n";
                    cout<<"Ingresa la nueva informaci\242n para el cliente\n\n";
                    cout<<"Ingresa el c\242digo del cliente: ";
                    cout<<auxCodigo;
                    cout<<"\n\n";
                    fflush(stdin);
                    cout<<"Ingresa el nombre del cliente: ";
                    getline(cin,auxNombre);
                    fflush(stdin);
                    cout<<"\n\n";
                    cout<<"Ingresa el domicilio del cliente: ";
                    getline(cin,auxDomicilio);
                    cout<<"\n\n";
                    fflush(stdin);
                    cout<<"Ingresa el n\243mero de telefono del cliente: ";
                    getline(cin,auxCelular);
                    cout<<"\n\n";
                    fflush(stdin);
                    cout<<"Ingresa la fecha de inscripci\242n del cliente: ";
                    getline(cin,auxFecha);
                    cout<<"\n\n";
                    auxiliar<<auxCodigo<<endl<<auxNombre<<endl<<auxDomicilio<<endl<<auxCelular
                            <<endl<<auxFecha<<endl;
                    cout<<"El Registro se ha modificado correctamente.\n\n";
                }


                else
                {

                    auxiliar<<codigoCliente<<endl<<nombre<<endl<<domicilio<<endl<<celular
                            <<endl<<fechaPago<<endl;
                }


                getline(lectura,codigoCliente);
            }

            if(encontrado==false)
            {
                cout<<"\n\nNo se encontr\242 ning\243n registro con ese c\242digo!\n\n";
            }
        }
        lectura.close();
        verificador.close();
        auxiliar.close();
        remove("clientes.txt");
        rename("auxiliar.txt","clientes.txt");

    }
    else
    {
        error();
    }

    pausa();
}

void menu::listarClientes()
{
    int i=0;
    ifstream lectura;
    lectura.open("clientes.txt",ios::in);
    if(lectura.is_open())
    {
        cout<<"\t\t\t\t***Listado de todos los clientes***\t\t\t\t\n\n";
        getline(lectura,codigoCliente);
        while(!lectura.eof())
        {
            i++;
            getline(lectura,nombre);
            getline(lectura,domicilio);
            getline(lectura,celular);
            getline(lectura,fechaPago);
            cout<<"C\242digo: "<<codigoCliente<<endl;
            cout<<"Nombre: "<<nombre<<endl;
            cout<<"Domicilio: "<<domicilio<<endl;
            cout<<"Celular: "<<celular<<endl;
            cout<<"Fecha de inscripci\242n: "<<fechaPago<<endl;
            cout<<"\n\n";
            getline(lectura,codigoCliente);
        }

        if(i==0)
        {
            cout<<"No hay ningun cliente registrado";
        }

        else if(i==1)
        {
            cout<<"Hay un solo cliente registrado en este gimnasio\n\n";
        }

        else if(i>1)
        {

            cout<<"Hay un total de "<<i<<" clientes registrados en este gimnasio\n\n";
        }
    }
    else
    {
        error();
    }
    lectura.close();
    pausa();
}

void pausa()
{
    cout<<"\n\nPresiona Enter para continuar...";
    fflush(stdin);
    cin.get();
    system(CLEAR);
}

int main()
{
    system ("color f0");
    menu inicio;
    comprobarArchivo();
    inicio.menuPrincipal();
    return 0;
}

void error()
{
    cout<<" No se pudo abrir el archivo de registros asegurese que el archivo se";
    cout<<" encuentre en la misma ubicaci\242n \n que el programa o  que el archivo de texto se ";
    cout<<"llame: clientes.txt, si el archivo tiene otro nombre \n ren\242mbrelo al ya mencionado.";
    cout<<"\n\n";
    cout<<" Este error tambien puede ser porque es la primera vez que usa el programa y ";
    cout<<"no se ha hecho ningun registro.";
    cout<<"\n Si este es el caso le invito a seleccionar la opcion de dar de alta un cliente y ";
    cout<<"dar de alta al primer cliente \n para asi dejar de ver este error. :)";
}

void menu::mostarRegistro(string codigo)
{
    ifstream mostrar;
    mostrar.open("clientes.txt",ios::in);
    getline(mostrar,codigoCliente);
    while(!mostrar.eof())
    {
        getline(mostrar,nombre);
        getline(mostrar,domicilio);
        getline(mostrar,celular);
        getline(mostrar,fechaPago);
        if(codigo==codigoCliente)
        {
            cout<<"\n\nRegistro Encontrado\n\n";
            cout<<"C\242digo: "<<codigoCliente<<endl;
            cout<<"Nombre: "<<nombre<<endl;
            cout<<"Domicilio: "<<domicilio<<endl;
            cout<<"Celular: "<<celular<<endl;
            cout<<"Fecha de inscripci\242n: "<<fechaPago<<endl;
            cout<<"\n\n";
        }

        getline(mostrar,codigoCliente);
    }

    mostrar.close();
}

void comprobarArchivo()
{
    ifstream archivo;
    archivo.open("clientes.txt",ios::in);
    if(!archivo.is_open())
    {
        cout<<"No se pudo abrir el archivo de registros o aun no se ha registrado ningun cliente!";
        cout<<"\n\n";
        cout<<"Para crear el archivo de registros selecciona la opcion de dar de alta un cliente.";
        cout<<"\n";
        pausa();
        system(CLEAR);
    }
}

void menu::buscarCliente()
{
    ifstream lectura;
    lectura.open("clientes.txt",ios::in);
    int encontrados;
    string buscarNombre;

    cout<<"***Buscar Cliente***"<<endl<<endl;
    if(lectura.is_open())
    {
        cout<<"Ingresa el nombre del cliente: ";
        fflush(stdin);
        getline(cin,buscarNombre);
        encontrados=0;
        cout<<endl;

        getline(lectura,codigoCliente);
        while(!lectura.eof())
        {
            getline(lectura,nombre);
            getline(lectura,domicilio);
            getline(lectura,celular);
            getline(lectura,fechaPago);
            if(nombre.find(buscarNombre)!=string::npos)
            {
                cout<<"C\242digo: "<<codigoCliente<<endl;
                cout<<"Nombre: "<<nombre<<endl;
                cout<<"Domicilio: "<<domicilio<<endl;
                cout<<"Celular: "<<celular<<endl;
                cout<<"Fecha de inscripci\242n: "<<fechaPago<<endl;
                cout<<"\n\n";
                encontrados++;
            }

            getline(lectura,codigoCliente);
        }
        if (encontrados)
        {
            cout<<"Se encontro "<<encontrados<<" registro(s) con la palabra buscada"<<endl;
        }
        else
        {
            cout<<"No se encontro algun registro con la palabra buscada"<<endl;
        }
    }
    else
    {
        cout<<endl<<"No se pudo abrir el archivo!";
    }
    lectura.close();
    pausa();
}