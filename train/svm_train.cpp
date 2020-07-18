
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

#include <fstream>
#include <iostream>
#include <list>
#include <stdio.h> // para getline()
#include <stdlib.h> // para atoi()
#include <string>
#include <string.h>
#include <unistd.h> // para getopt()
#include <cv.h>
#include <highgui.h>
#include <map>
#include "opencv2/contrib/contrib.hpp"

using namespace cv;
using namespace std;

int main( int argc, char** argv ) {
  int dimensaoSVM = 0;
  char opcaoGetopt, nomeArqLabelsTreino[ 100 ], nomeArqVetTreino[ 100 ],
    nomeArqVetClassif[ 100 ], tipoKernel[ 20 ], nomeArqPropAcertosTreino[ 100 ],
    nomeArqLabelsClassif[ 100 ];
  nomeArqLabelsTreino[ 0 ] = 0;
  nomeArqVetTreino[ 0 ] = 0;
  while( ( opcaoGetopt = getopt( argc, argv, "a:d:k:l:t:" ) ) != -1 ) {
    switch( opcaoGetopt ) {
    case 'a':
      strcpy( nomeArqPropAcertosTreino, optarg );
      break;
    case 'd':
      dimensaoSVM = atoi( optarg );
      break;
    case 'k':
      strcpy( tipoKernel, optarg );
      break;
    case 'l':
      strcpy( nomeArqLabelsTreino, optarg );
      break;
    case 't':
      strcpy( nomeArqVetTreino, optarg );
      break;
    }
  }
  
  if( dimensaoSVM <= 0 ) {
    cout << "Faltou: -d dimensaoSVM\n";
    return -1;
  }

  if( strlen( nomeArqLabelsTreino ) == 0 ) {
    cout << "Faltou: -l nomeArqLabelsTreino\n";
    return -1;
  }
  if( strlen( nomeArqVetTreino ) == 0 ) {
    cout << "Faltou: -t nomeArqVetTreino\n";
    return -1;
  }
  if( strlen( nomeArqPropAcertosTreino ) == 0 ) {
    cout << "Faltou: -a nomeArqPropAcertosTreino\n";
    return -1;
  }

  // Leitura do arqLabels e registro de seus dados:
  ifstream arqLabels( nomeArqLabelsTreino, ios::in );
  if( arqLabels.fail() ) {
    cout << "Falha na abertura do arquivo " << nomeArqLabelsTreino << endl;
    return -1;
  }
  float label;
  list<float> ltLabels;
  list<float>::iterator itLabels;
  while( arqLabels >> label ) {
    cout << label << endl;
    ltLabels.push_back( label );
  }
  arqLabels.close();
  cout << "Leu " << ltLabels.size() << " labels.\n";
  float labels[ ltLabels.size() ];
  int indLabels = 0;
  for( itLabels = ltLabels.begin();
       itLabels != ltLabels.end(); itLabels++, indLabels++ ) {
    labels[ indLabels ] = *itLabels;
  }
  Mat labelsMat( ltLabels.size(), 1, CV_32FC1, labels );
  
  // Leitura do arqTreino e registro de seus dados:
  ifstream arqVetTreino( nomeArqVetTreino, ios::in );
  if( arqVetTreino.fail() ) {
    cout << "Falha na abertura do arquivo " << nomeArqVetTreino << endl;
    return -1;
  }
  int numCasosTreino = ltLabels.size();
  float vetoresTreinoArq[ numCasosTreino ][ dimensaoSVM ];
  for( int linha = 0; linha < numCasosTreino; linha++ ) {
    for( int coluna = 0; coluna < dimensaoSVM; coluna++ ) {
      arqVetTreino >> vetoresTreinoArq[ linha ][ coluna ];
    }
  }
  arqVetTreino.close();
  cout << "\nVetores para treinamento:\n";
  for( int linha = 0; linha < numCasosTreino; linha++ ) {
    for( int coluna = 0; coluna < dimensaoSVM; coluna++ ) {
      cout << vetoresTreinoArq[ linha ][ coluna ] << " ";
    }
    cout << endl;
  }
  Mat vetTreinoDataMat( numCasosTreino, dimensaoSVM, CV_32FC1,
		       vetoresTreinoArq );
  
  // Configuração e treino da SVM:
  CvSVMParams params;
  params.svm_type = CvSVM::C_SVC;
  if( !strcmp( tipoKernel, "linear" ) )
    params.kernel_type = CvSVM::LINEAR;
  else if( !strcmp( tipoKernel, "poly" ) ) {
    params.kernel_type = CvSVM::POLY;
    params.degree = 3;
    params.gamma = 0.1;
    params.coef0 = 0.01;
  }
  else if( !strcmp( tipoKernel, "rbf" ) )
    params.kernel_type = CvSVM::RBF;
  else if( !strcmp( tipoKernel, "sigmoid" ) )
    params.kernel_type = CvSVM::SIGMOID;
  else {
    cout << "\nTipo de kernel não reconhecido.\n"
	 << "Adotado o linear.\n";
    params.kernel_type = CvSVM::LINEAR;
  }
  cout << "Tipo de kernel: " << tipoKernel << endl;
  params.term_crit = cvTermCriteria( CV_TERMCRIT_ITER, 100, 1e-6 );
  CvSVM SVM;
  SVM.train( vetTreinoDataMat, labelsMat, Mat(), Mat(), params );
  
  // Verifica se os vetores dados para treinamento são corretamente
  // classificados pela mesma SVM:
  cout << "\n\nVerificação da SVM, por classificação dos vetores de treino:"
       << "\n\n";
  int contaAcertos = 0;
  for( int linha = 0; linha < numCasosTreino; linha++ ) {
    cout << "vetor: ";
    float flVetor[ dimensaoSVM ];
    for( int coluna = 0; coluna < dimensaoSVM; coluna++ ) {
      cout << vetoresTreinoArq[ linha ][ coluna ] << " ";
      flVetor[ coluna ] = vetoresTreinoArq[ linha ][ coluna ];
    }
    Mat matVetor( dimensaoSVM, 1, CV_32FC1, flVetor );
    float classePrevista = SVM.predict( matVetor );
    cout << ", classe: " << classePrevista;
    if( classePrevista == labels[ linha ] ) {
      contaAcertos++;
      cout << " (certo)\n";
    }
    else {
      cout << " (errado)\n";
    }
  }
  cout << "\nProporção de acertos: "
       << 100 * float( contaAcertos ) / numCasosTreino
       << "%\n";
  ofstream arqProporcAcertosTreino( nomeArqPropAcertosTreino, ios::out );
  arqProporcAcertosTreino << 100 * float( contaAcertos ) / numCasosTreino
			  << endl;
  arqProporcAcertosTreino.close();

  //Salva o resultado do treinamento.
  SVM.save("resultado.txt");
  return 0;
}
