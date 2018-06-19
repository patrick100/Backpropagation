#include <iostream>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "storage_adaptors.hpp"

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
//#include <windows.h>

#include <random>

#include <fstream>
#include <string>

#include <stdlib.h>

#include <math.h>
#include "funciones.h"
#include <time.h>

#define e 2.71828182846
t alpha = 0.5;

int tipo=0;


using namespace std;
using namespace boost::numeric::ublas;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> prob(0, 1);



void imprimir(matrix<t> &m) {

	for (int i = 0; i<m.size1(); i++) {
		for (int j = 0; j< m.size2(); j++)
		{
		    cout<<m(i,j)<<" ";
			//printf("%.9f ", m(i, j));
		}
		cout << endl;
	}

}

matrix<t> Create_Matrix(int n_fil, int n_col)
{
	matrix<t> m(n_fil, n_col); //y-x-ordered
	for (int i = 0; i != n_fil; ++i) {
		for (int j = 0; j != n_col; ++j) {
			//m(i, j) = prob(gen);
			//m(i, j) = ((double)rand() / (RAND_MAX + 1));
			m(i, j) = ((double) rand() / (RAND_MAX)) + 1;
		}
	}
	return m;
}


void NetaS(matrix<t> &m, matrix<t> &input, matrix<t>& salida) {
	salida = prod(m, input);

	salida.resize(salida.size1() + 1, 1);
	for (int i = 1; i<salida.size1(); i++) {

		if(tipo==1){
			salida(i, 0) = 1 / (1 + pow(e, (-1 * salida(i - 1, 0))));
		}
		if(tipo==2){
			//tanH

			//salida(i-1, 0) = (2 / (1 + pow(e, (-2 * salida(i-1, 0)))))-1;
		}

	}
	salida(0, 0) = 1;
}


void NetaSF(matrix<t> &m, matrix<t> &input, matrix<t>& salida) {
	salida = prod(m, input);
	for (int i = 0; i<salida.size1(); i++) {
		if(tipo==1){
		   salida(i, 0) = 1 / (1 + pow(e,( -1 * salida(i, 0))));
		}
		if(tipo==2){
		   salida(i, 0) = (2 / (1 + pow(e, (-2 * salida(i, 0)))))-1;
		}
	}
}




float calcular_error(std::vector<t>&Sd, matrix<t> &So) {
	float error = 0;
	for (int i = 0; i < So.size1(); i++) {
		//printf("SD:%f SO%f\n", Sd[i] , So(i, 0));
		error += (pow((Sd[i] - So(i, 0)), 2)) / 2;
		//printf("error esss:%f\n", error);
	}
	return error;
}


matrix<t> matrix_hidden;
matrix<t> matrix_output;
matrix<t> Sh;
matrix<t> So;
std::vector<t> delta; t d;
std::vector<t> sump; t p;

void changes_pesos(std::vector<t>&Sd, matrix<t>& input)
{

	delta.clear();
	sump.clear();
	d = 0;
	/******************************MATRIZ OUTPUT***********************/
	for (int i = 0; i < So.size1(); i++) {

		if(tipo==1)
		{
			d = So(i, 0)*(1 - So(i, 0))*(-1*(Sd[i] - So(i, 0)));
		}
		if(tipo==2)
		{
			d = (1 - (So(i, 0)*So(i, 0)))*(-1*(Sd[i] - So(i, 0)));	
		}		

		for (int j = 0; j < Sh.size1(); j++) {
			matrix_output(i, j) = matrix_output(i, j) - alpha*(Sh(j, 0)*d);
		}
		delta.push_back(d);
	}



	//printf("MATRIZ OUTPUT NUEVA:\n"); imprimir(matrix_output);
	
	/******************************MATRIZ HIDDEN***********************/
	//QUITAR EL UNO BIAS
	//cout << "SH SIZE :"<<delta.size() << endl;


	for (int i =0 ; i < Sh.size1(); i++) {
		p = 0;
		for (int k = 0; k < delta.size(); k++) {
			p += matrix_output(k, i)*delta[k];}
		sump.push_back(p);
	}
	//cout <<"SUMP SIZE: "<<sump.size() << endl;
	//cout << "Sh SIZE: " << Sh.size1() << endl;

	for (int i = 0; i < Sh.size1()-1; i++) {

		//if(tipo==1)
		
		p = sump[i]*(Sh(i+1, 0)*(1 - Sh(i+1, 0)));

		for (int j = 0; j < input.size1(); j++) {
			matrix_hidden(i, j) = matrix_hidden(i, j) + alpha*(p*input(j, 0));
		}
	}


}


void inicializar(matrix<t>&m, std::vector<t>&v) {
	for (int i = 0; i < v.size(); i++) {
		m(i, 0) = v[i];
	}
}




int main()
{
	/*
	int n_neuronas_input=0;
	int n_neuronas_salida=0;
	int num_capas=0;
	int temp_hidden=0;
	int t_function=0;

	std::vector<int> neuronas_hidden;
	std::vector<int> tipos_hidden;
	*/

	//int N_hiden_neurons = 10;
	int N_output_neurons = 3;
	int n_input = 5;//con el bias

	int N_hiden_neurons = 0;


/*
	cout<<"ingrese el numero de capas hidden"<<endl;
	cin>>num_capas;
*/

	cout<<"ingrese el numero de neuronas en la capa hidden"<<endl;
	cin>>N_hiden_neurons;


	cout<<"Tipos de Funcion de Activacion"<<endl;
	cout<<"Sigmoide  ->1"<<endl;
	cout<<"tangente Hiperbolica  ->2"<<endl;


	cout<<"ingrese el tipo de la funcion hidden"<<endl;
	cin>>tipo;






	int total_entradas = 0;
	int total_pruebas = 0;
	std::ifstream f("iris.data"); string lin; for (int i = 0; std::getline(f, lin); total_entradas++); f.close();
	std::ifstream f2("pruebas.data"); string lin2; for (int i = 0; std::getline(f2, lin2); total_pruebas++); f2.close();



	//GRAFICA ERROR
	//ofstream graph_error;
	//graph_error.open("error.dat");
	//clock_t time;


	//MATRIZ CON TODAS LAS ENTRADAS
	std::vector<tinput> total_inputs(total_entradas);
	std::vector<tinput> pruebas(total_pruebas);

	std::vector<t> errores(total_entradas);

	/*CREAR LAS MATRICES*/
	matrix_hidden = Create_Matrix(N_hiden_neurons, n_input);
	matrix_output = Create_Matrix(N_output_neurons, N_hiden_neurons + 1);
	//imprimir(matrix_hidden);
	//imprimir(matrix_output);

	/*LA SALIDA DESEADA 0 */
	std::vector<t> Sd(3);
	matrix<t> input(n_input, 1);

	//cargamos toda la data
	cargar_data(total_inputs,"iris.data");

	t error = 0;
	for (int i = 0; i < total_inputs.size(); i++) {

		//cout << "salida tipo: " << total_inputs[i].second << endl;
		if (total_inputs[i].second == 1) { Sd = { 1,0,0 }; }
		if (total_inputs[i].second == 2) { Sd = { 0,1,0 }; }
		if (total_inputs[i].second == 3) { Sd = { 0,0,1 }; }
		//convertir vector a matrix
		inicializar(input, total_inputs[i].first);//reducir

		NetaS(matrix_hidden, input, Sh);
		NetaSF(matrix_output, Sh, So);

		//FALTA SALIDA DESEADA
		errores[i]= calcular_error(Sd, So);
		error += errores[i];
	}

	error = error / total_inputs.size();
	cout << "ERROR: " << error << endl;



	//graph_error << error <<"  "<< clock() <<endl;

	//t error_ant = error;
	//t dif_error = error;
	//while (error > 0.117) {
	int x = 0;
	//

	//error>0.08
	//while(x<5000){
	while(error>0.10){
		//if (x == 700) { alpha = 0.01; }
		//if (x == 1400) { alpha = 0.0001; }

		error = 0;
		for (int i = 0; i < total_inputs.size(); i++) {

			if (total_inputs[i].second == 1) { Sd = { 1,0,0 }; }
			if (total_inputs[i].second == 2) { Sd = { 0,1,0 }; }
			if (total_inputs[i].second == 3) { Sd = { 0,0,1 }; }
			//convertir vector a matrix
			inicializar(input, total_inputs[i].first);
			changes_pesos(Sd, input);

			//Calcular el nuevo error
			NetaS(matrix_hidden, input, Sh);
			NetaSF(matrix_output, Sh, So);

			//FALTA SALIDA DESEADA
			errores[i] = calcular_error(Sd, So);
			error += errores[i];
		}

		error = error / total_inputs.size();
		//graph_error << error << "  " << clock() << endl;
		//printf("NUEVO ERROR: %.3f \n", error);
        cout<<"ERROR "<< error<<endl;

		//dif_error = error_ant - error;
		//error_ant = error;
		//printf("DIF ERROR: %.9f \n", dif_error);
		x++;
	}

	cout<<"ERROR "<< error<<endl;
	//printf("NUEVO ERROR: %.3f \n", error);


	//cargamos toda la data de pruebas

	cargar_data(pruebas, "pruebas.data");
	//int d1, d2, d3 = 0;
	std::vector<std::vector<int>> tdifusion(3);
	for (int i = 0; i < 3; i++)
		tdifusion[i].resize(3);

	//matrix<t> prueba;
	t max;
	int posmax = 0;
	for (int i = 0; i < pruebas.size(); i++) {

		//Calcular el nuevo error
		if (pruebas[i].second == 1) { cout << "SALIDA DESEADA: " << "1,0,0"<<endl;}
		if (pruebas[i].second == 2) { cout << "SALIDA DESEADA: " << "0,1,0" << endl; }
		if (pruebas[i].second == 3) { cout << "SALIDA DESEADA: " << "0,0,1" << endl; }
		inicializar(input, pruebas[i].first);
		NetaS(matrix_hidden, input, Sh);
		NetaSF(matrix_output, Sh, So);

		//cout << "SALIDA: " << endl;
		max = So(0, 0); posmax = 0;
		for (int j = 1; j < So.size1(); j++) {
			if (max < So(j, 0)) { max = So(j, 0); posmax = j;
			}
		}

		tdifusion[pruebas[i].second - 1][posmax] = tdifusion[pruebas[i].second - 1][posmax]+1;
		//cout << "FILA: " << pruebas[i].second - 1 << "COL" << posmax << endl;
		imprimir(So);
		cout << endl;

		//cout << "PRUEBAS SIZE POS :" << i << endl;
	}
	int tam = pruebas.size()/3;

	//cout << "PRUEBAS SIZE:" << pruebas.size() << endl;
	for (int i = 0; i < tdifusion.size(); i++) {
		for (int j = 0; j < tdifusion[i].size(); j++) {
			cout << (tdifusion[i][j]*100)/tam << " ";
		}
		cout << endl;
	}

	//graph_error.close();
	
	//guardar_pesos(matrix_hidden, "pesos_hidden5");
	//guardar_pesos(matrix_output,"pesos_output5");






}


