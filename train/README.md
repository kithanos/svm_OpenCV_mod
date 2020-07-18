# svm_OpenCV_mod/train
Modificação do programa da SVM da biblioteca do OpenCV - SVM_treino

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

./svm_train com parâmetros:

-d dimensão SVM 
nº de elementos de cada vetor de treino

-l nomeArqLabelsTreino 
O arquivo com "labels" deve conter apenas uma coluna, 
com valores +1 ou -1 (ou 0), conforme as classes dos elementos 
de treino (ver treino_rotulos.ascii)

-t nomeArqVetTreino 
 O arquivo de treino deve conter um vetor de treino em cada linha,
com seus componentes separados entre si por espaços.
(ver vetores treino_dados.ascii)

-k kernel
 "linear", "poly", "rbf" ou "sigmoid"
Define a função de kernel (leva o espaço amostral de dados para 
um outro plano onde as diferenças entre os dados fiquem mais claras).

-a nomeArqPropAcertosTreino
Arquivo gerado, proporção de acertos do treino.


Arquivos gerados: nomeArqPropAcertosTreino e resultado.txt (resultado do treinamento)
com os vetores de suporte.

Exemplo (para executar):

>> ./svm_train -d dimensaoSVM -l nomeArqLabelsTreino -t nomeArqVetTreino -k linear -a nomeArqPropAcertosTreino

>> ./svm_train -d 4 -l treino_rotulos.ascii -t treino_dados.ascii -k linear -a PropAcertosTreino.ascii


Cujo:

treino_dados.ascii - é o arquivo com os vetores de treino.
treino_rotulos.ascii - é o arquivo os rótulos de treino.

ATENÇÃO: COPIAR O ARQUIVO "resultado.txt" para a pasta do /test.

