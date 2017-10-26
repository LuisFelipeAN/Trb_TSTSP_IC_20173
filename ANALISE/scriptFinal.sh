#!/bin/bash
LOCAL_INSTANCIAS=INSTANCIAS
NUM_EXPERIMENTOS=0
NUM_SEMENTE=3
Dados=() 
Experimentos=()

cd $LOCAL_INSTANCIAS/
INSTANCIAS=*.txt
echo "-----------------------------------"
echo "INSTANCIAS IDENTIFICADAS" 
for i in $INSTANCIAS
   do
	echo ${i%.*}
	AUX="${i%.*}"
	Experimentos=(${Experimentos[@]} "$AUX") 
	NUM_EXPERIMENTOS=$NUM_EXPERIMENTOS+1
done
cd ..

LOCAL_SAIDA_RESULTADOS=resultados
LOCAL_SAIDA_IMAGENS=graficos
LOCAL_SAIDA_SCRIPTS=scripts
LOCAL_SAIDA_GRAFICOS_ENTRADA=entradas

mkdir -p $LOCAL_SAIDA_RESULTADOS/
mkdir -p $LOCAL_SAIDA_IMAGENS/
mkdir -p $LOCAL_SAIDA_SCRIPTS/
mkdir -p $LOCAL_SAIDA_GRAFICOS_ENTRADA/

echo "-----------------------------------"
echo "CRIANDO SCRIPTS GRAFICOS DAS ENTRADAS" 
#cria grafico das entradas
for ((i=0; i<$NUM_EXPERIMENTOS; i++))
   do
	 echo "${Experimentos[i]}.txt"
	cat > $LOCAL_SAIDA_SCRIPTS/script-${Experimentos[i]}.txt << EOF
	reset
	set grid
	set ylabel "Y"
	set xlabel "X"
	set title "Clusters ${Experimentos[i]}"

	plot "$LOCAL_INSTANCIAS/${Experimentos[i]}.txt" using 2:3:4 w p pt 7 lc variable title 'vertices'
	set terminal png
	set output '$LOCAL_SAIDA_GRAFICOS_ENTRADA/${Experimentos[i]}-cluster.png'
	replot
	
	set title "Tabus ${Experimentos[i]}"
	plot "$LOCAL_INSTANCIAS/${Experimentos[i]}.txt" using 2:3:5 w p pt 7 lc variable title 'vertices'
	set terminal png
	set output '$LOCAL_SAIDA_GRAFICOS_ENTRADA/${Experimentos[i]}-tabu.png'
	replot
	exit 
EOF

	
   done  
echo "-----------------------------------"
echo "CRIANDO GRAFICOS DAS ENTRADAS" 
for ((i=0; i<$NUM_EXPERIMENTOS; i++))
   do
	echo "${Experimentos[i]}.txt"
	gnuplot < $LOCAL_SAIDA_SCRIPTS/script-${Experimentos[i]}.txt
   done

echo "-----------------------------------"
echo "PROCESSANDO AS INSTANCIAS"
for ((i=0; i<$NUM_EXPERIMENTOS; i++))
   do
	echo "ENTRADA: ${Experimentos[i]}.txt"
	for ((j=0; j<$NUM_SEMENTE; j++))
   	do
	NOME=saida-${Experimentos[i]}-$j
	./Trabalho_IC $LOCAL_INSTANCIAS/${Experimentos[i]}.txt $LOCAL_SAIDA_RESULTADOS/$NOME.txt $j
	echo "SEMENTE $j: $NOME.txt"
	NUMERO_RESULTADOS=$NUMERO_RESULTADOS+1
	Dados=(${Dados[@]} "$NOME") 
	done
done


echo "-----------------------------------"
echo "CRIANDO SCRIPTS DOS RESULTADOS"
for ((i=0; i<$NUMERO_RESULTADOS; i++))
   do
	 echo ${Dados[i]}.txt
	cat > $LOCAL_SAIDA_SCRIPTS/script-${Dados[i]}.txt << EOF
	reset
	set grid
	set ylabel "Y"
	set xlabel "X"
	set title "${Dados[i]}"

	plot "$LOCAL_SAIDA_RESULTADOS/${Dados[i]}.txt" using 2:3 w l lc 1 title 'rotas'
	replot "$LOCAL_SAIDA_RESULTADOS/${Dados[i]}.txt" using 2:3:4 w p pt 7 lc variable title 'vertices'
	set terminal png
	set output '$LOCAL_SAIDA_IMAGENS/imagem-${Dados[i]}.png'
	replot
	exit 
EOF

	
   done  

echo "-----------------------------------"
echo "CRIANDO GRAFICOS DOS RESULTADOS"
for ((i=0; i< $NUMERO_RESULTADOS; i++))
   do
	echo ${Dados[i]}.txt
	gnuplot < $LOCAL_SAIDA_SCRIPTS/script-${Dados[i]}.txt
  done

