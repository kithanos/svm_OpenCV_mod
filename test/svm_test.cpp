
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
  char opcaoGetopt, nomeArqLabelsTeste[ 100 ], nomeArqVetTeste[ 100 ],
    nomeArqVetClassif[ 100 ], tipoKernel[ 20 ], nomeArqPropAcertosTeste[ 100 ],
    nomeArqLabelsClassif[ 100 ];
  nomeArqLabelsTeste[ 0 ] = 0;
  nomeArqVetTeste[ 0 ] = 0;
 
  while( ( opcaoGetopt = getopt( argc, argv, "a:c:d:l:s:" ) ) != -1 ) {
    switch( opcaoGetopt ) {
      case 'a':
      strcpy( nomeArqPropAcertosTeste, optarg );
      break;
    case 'c':
      strcpy( nomeArqVetClassif, optarg );
      break;
    case 'd':
      dimensaoSVM = atoi( optarg );
      break;
    case 'l':
      strcpy( nomeArqLabelsTeste, optarg );
      break;
    case 's':
      strcpy( nomeArqLabelsClassif, optarg );
      break;
    }
  }
   if( dimensaoSVM <= 0 ) {
  cout << "Faltou: -d dimensaoSVM\n";
  return -1;
  }
  if( strlen( nomeArqVetClassif ) == 0 ) {
  cout << "Faltou: -c nomeArqVetClassif\n";
  return -1;
  }
  if( strlen( nomeArqLabelsTeste ) == 0 ) {
  cout << "Faltou: -l nomeArqLabelsTeste\n";
  return -1;
  }

  if( strlen( nomeArqPropAcertosTeste ) == 0 ) {
  cout << "Faltou: -a nomeArqPropAcertosTeste\n";
  return -1;
  }
  if( strlen( nomeArqLabelsClassif ) == 0 ) {
  cout << "Faltou: -s nomeArqLabelsClassif\n";
  return -1;
  }
  
  //Leitura do arqLabels e registro de seus dados:
  ifstream arqLabels( nomeArqLabelsTeste, ios::in );
  if( arqLabels.fail() ) {
    cout << "Falha na abertura do arquivo " << nomeArqLabelsTeste << endl;
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

  
  // Leitura do arqClassif e registro de seus dados:
  CvSVM SVM;
  ifstream arqClassif( nomeArqVetClassif, ios::in );
  if( arqClassif.fail() ) {
    cout << "Falha na abertura do arquivo " << nomeArqVetClassif << endl;
    return -1;
  }
  int contaLinhasArqClassif = 0;
  string linhaArqClassif;
  while( getline( arqClassif, linhaArqClassif ) ) {
    contaLinhasArqClassif++;
  }
  arqClassif.close();
  cout << endl << nomeArqVetClassif << " tem " << contaLinhasArqClassif
       << " vetores.\n";
  arqClassif.open( nomeArqVetClassif, ios::in );
  int numCasosClassif = contaLinhasArqClassif;
  float vetoresClassifArq[ numCasosClassif ][ dimensaoSVM ];
  for( int linha = 0; linha < numCasosClassif; linha++ ) {
    for( int coluna = 0; coluna < dimensaoSVM; coluna++ ) {
      arqClassif >> vetoresClassifArq[ linha ][ coluna ];
    }
  }
  arqClassif.close();
  cout << "\nVetores a serem classificados:\n";
  for( int linha = 0; linha < numCasosClassif; linha++ ) {
    for( int coluna = 0; coluna < dimensaoSVM; coluna++ ) {
      cout << vetoresClassifArq[ linha ][ coluna ] << " ";
    }
    cout << endl;
  }

  int contaAcertos = 0;
  int numCasosTeste = ltLabels.size();
  
  // Classifica os vetores de nomeArqVetClassif:
  SVM.load("resultado.txt");
  cout << "\n\nClassificação dos vetores de " << nomeArqVetClassif << " pela SVM\n\n";
  ofstream arqLabelsClassif( nomeArqLabelsClassif, ios::out );
  for( int linha = 0; linha < numCasosClassif; linha++ ) {
    cout << "vetor: ";
    float flVetor[ dimensaoSVM ];
    for( int coluna = 0; coluna < dimensaoSVM; coluna++ ) {
      cout << vetoresClassifArq[ linha ][ coluna ] << " ";
      flVetor[ coluna ] = vetoresClassifArq[ linha ][ coluna ];
    }
    Mat matVetor( dimensaoSVM, 1, CV_32FC1, flVetor );
    float classePrevista = SVM.predict( matVetor );
    cout << ", classe: " << classePrevista << endl;
    arqLabelsClassif << classePrevista << endl;

    
    
    if( classePrevista == labels[ linha ] ) {
      contaAcertos++;
      cout << " (certo)\n";
    }
    else {
      cout << " (errado)\n";
    }
  }
  cout << "\nProporção de acertos: "
       << 100 * float( contaAcertos ) / numCasosTeste
       << "%\n";

  arqLabelsClassif.close();
  
   ofstream arqProporcAcertosTeste( nomeArqPropAcertosTeste, ios::out );
  arqProporcAcertosTeste << 100 * float( contaAcertos ) / numCasosTeste
			  << endl;
  arqProporcAcertosTeste.close(); 
  

  return 0;
}
