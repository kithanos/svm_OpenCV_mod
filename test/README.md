# svm_OpenCV_mod/test
Modificação do programa da SVM da biblioteca do OpenCV - SVM_teste

--------------------------------------COMPILAÇÃO------------------------------------------------

Editar o arquivo CMakeLists.txt com a versão do cmake instalada.

Para compilar, execute em no terminal:

>> cmake .

>> make

Pacotes necessários:
OpenCV 2.4

Programa compilado e testado em:
GNU 5.4.0
cmake version 3.5.1

OBS: Caso precise compilar o programa mais de uma vez, apagar todos os arquivos e pasta gerados pelo cmake,
(não apagar o CMakeLists.txt)


--------------------------------------EXECUÇÃO--------------------------------------------------

./svm_test com parâmetros:

-d dimensão SVM 
nº de elementos de cada vetor de treino

-c teste_dados.ascii
Contém os vetores a serem classificados (Teste).

-l rotulos_teste
Contém os rotulos do treino (os rótulos corretos).

-s nomeArqLabelsClassif
Arquivo gerado, Classificação Labels Teste (resultado).

-acertosTeste
 Resultado em acurácia da classificação dos vetores de teste.

Arquivos gerados: nomeArqLabelsClassif e -acertosTeste

ATENÇÃO: Copiar o arquivo "resultado.txt" (resultado do treinamento) 
da pasta /svm_train para essa pasta (/svm_test)


O arquivo com "rotulos_teste" deve conter apenas uma coluna, com valores
+1 ou -1 (ou 0), conforme as classes dos elementos de Teste.
O arquivo de Teste contém os vetores a serem classificados deve conter um vetor de Teste em cada linha,

Exemplo (para executar):

>> ./svm_test -c teste -d 4 -l rotulos_teste  -s classif.ascii  -a acertosTeste

>> ./svm_test -c teste_dados.ascii -d 4 -l teste_rotulos.ascii  -s classif.ascii  -a acertosTeste


Cujo:

teste_dados.ascii - é o arquivo com os vetores de teste.
teste_rotulos.ascii - é o arquivo os rótulos de teste.

OBS: Para este exemplo, os arquivos de treino e teste contém os mesmos vetores,
por isto e absolutamente normal a acurácia do treino e do teste serem as mesmas.

