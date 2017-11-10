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
LOCAL_SAIDA_VALORES=valores

mkdir -p $LOCAL_SAIDA_RESULTADOS/
mkdir -p $LOCAL_SAIDA_IMAGENS/
mkdir -p $LOCAL_SAIDA_SCRIPTS/
mkdir -p $LOCAL_SAIDA_VALORES/
make

echo "-----------------------------------"
echo "CRIANDO SCRIPTS GRAFICOS DAS ENTRADAS" 
#cria grafico das entradas
PARAMETROS=('$4' '$5')


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
	#Dados=(${Dados[@]} "$NOME") 
	done
done


echo "-----------------------------------"
echo "CRIANDO SCRIPTS DOS RESULTADOS"
COLUNAS=('$0' '$1' '$2' '$3' '$4' '$5')
for ((i=0; i<$NUM_EXPERIMENTOS; i++))
   do
	 echo "${Experimentos[i]}.txt"
	str=${Experimentos[i]}
	split=(${str//-/ })
	for ((j=0; j<$NUM_SEMENTE; j++))
	do 	
		cat > $LOCAL_SAIDA_SCRIPTS/script-saida-${Experimentos[i]}-$j.txt << EOF
	MAX_TABUS = ${split[3]}
	DX = 0.8
	max(x,y) = (x > y) ? x : y
	g(a) = real((6.00-(2.00*DX))*real(a)+DX)
	set title 'Resutado:${Experimentos[i]} Semente:$j'
	set ylabel "Y"
	set xlabel "X"

		corTabu(c) = 65536 *int(255*(max(0,(3-abs(g(c/MAX_TABUS)-4))-abs(g(c/MAX_TABUS)-5)/2)))+ 256 *int(255*(max(0,(4-abs(g(c/MAX_TABUS)-2))-abs(g(c/MAX_TABUS)-4.00)/2.00)))+int(255*(max(0,(3-abs(g(c/MAX_TABUS)-1))-abs(g(c/MAX_TABUS)-2)/2)))

	stats '$LOCAL_INSTANCIAS/${Experimentos[i]}.txt' using 4 nooutput
	unset key
	set style data points
	plot for [i=STATS_min:STATS_max] '$LOCAL_INSTANCIAS/${Experimentos[i]}.txt' using 2:(${COLUNAS[4]} == i ? ${COLUNAS[3]} : 1/0):(corTabu(${COLUNAS[5]})) lt 1 pt i ps 1 lc rgb variable
	replot '$LOCAL_SAIDA_RESULTADOS/saida-${Experimentos[i]}-$j.txt' using 2:3 w l lc 1

	set terminal png
	set output '$LOCAL_SAIDA_IMAGENS/${Experimentos[i]}-$j.png'
	replot
	exit 
EOF
	done
done 

echo "-----------------------------------"
echo "CRIANDO GRAFICOS DOS RESULTADOS" 
for ((i=0; i<$NUM_EXPERIMENTOS; i++))
   do
	echo "${Experimentos[i]}.txt"
	for ((j=0; j<$NUM_SEMENTE; j++))
	do 	
	gnuplot < $LOCAL_SAIDA_SCRIPTS/script-saida-${Experimentos[i]}-$j.txt
  done
done

echo "-----------------------------------"
echo "CRIANDO TABELAS DOS VALORES OBTIDOS PARA CADA SEMENTE"
for ((i=0; i<$NUM_EXPERIMENTOS; i++))
   do
	echo "#${Experimentos[i]}" > $LOCAL_SAIDA_VALORES/${Experimentos[i]}.txt
	echo "${Experimentos[i]}.txt"
	for ((j=0; j<$NUM_SEMENTE; j++))
	do
		NOME=saida-${Experimentos[i]}-$j
		ARQ=$(grep '#' $LOCAL_SAIDA_RESULTADOS/$NOME.txt | tr '\n' " " )
		VALORES=(${ARQ//#/ })
		echo "$j""     ""${VALORES[2]}" >> $LOCAL_SAIDA_VALORES/${Experimentos[i]}.txt
	done
done

for ((i=0; i<$NUM_EXPERIMENTOS; i++))
   do
	 echo ${Experimentos[i]}.txt
	cat > $LOCAL_SAIDA_SCRIPTS/script-valores-${Experimentos[i]}.txt << EOF
	reset

	set grid
	set ylabel "Custo"
	set xlabel "Semente"
	set title "Valores x Semente ${Experimentos[i]}"
	unset key
	plot "$LOCAL_SAIDA_VALORES/${Experimentos[i]}.txt" using 1:2 w l lc 1 
    replot "$LOCAL_SAIDA_VALORES/${Experimentos[i]}.txt" using 1:2 w p pt 7 lc 1
	set terminal png
	set output '$LOCAL_SAIDA_VALORES/${Experimentos[i]}.png'
	replot
	exit 
EOF
   done  

echo "-----------------------------------"
echo "CRIANDO GRAFICOS DOS VALORES OBTIDOS PARA CADA SEMENTE"
for ((i=0; i<$NUM_EXPERIMENTOS; i++))
   do
	echo ${Experimentos[i]}.txt
	gnuplot < $LOCAL_SAIDA_SCRIPTS/script-valores-${Experimentos[i]}.txt
  done

