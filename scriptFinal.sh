#!/bin/bash
NUM_EXPERIMENTOS=9
NUM_SEMENTE=2
Dados=() 
Experimentos=("13i-10-lin318-64" "14i-9-st70-14" "15i-10-kroB100-20" "16i-10-pcb442-89" "17i-10-pr439-88" "18i-25-eil101-21" "19i-25-gil262-53" "20i-25-kroA100-20" "21i-25-lin105-21")
LOCAL_SAIDA_RESULTADOS=resultados
LOCAL_SAIDA_IMAGENS=graficos
LOCAL_SAIDA_SCRIPTS=scripts
LOCAL_SAIDA_GRAFICOS_ENTRADA=entradas
mkdir $LOCAL_SAIDA_RESULTADOS/
mkdir $LOCAL_SAIDA_IMAGENS/
mkdir $LOCAL_SAIDA_SCRIPTS/
mkdir $LOCAL_SAIDA_GRAFICOS_ENTRADA/

for ((i=0; i<$NUM_EXPERIMENTOS; i++))
   do
	for ((j=0; j<$NUM_SEMENTE; j++))
   	do
	./Trabalho_IC ${Experimentos[i]}.txt $LOCAL_SAIDA_RESULTADOS/saida_${Experimentos[i]}_$j.txt $j
	NOME=saida_${Experimentos[i]}_$j
	Dados=(${Dados[@]} "$NOME") 
	done
done
NUMERO_RESULTADOS=$NUM_SEMENTE*$NUM_EXPERIMENTOS
for ((i=0; i<NUMERO_RESULTADOS; i++))
   do
	echo ${Dados[i]}
done

#cria grafico das entradas
for ((i=0; i<$NUM_EXPERIMENTOS; i++))
   do
     echo “$i”
	 echo ${Dados[i]}
	cat > $LOCAL_SAIDA_SCRIPTS/script_${Experimentos[i]}.txt << EOF
	reset
	set grid
	set ylabel "Y"
	set xlabel "X"
	set title "Clusters ${Experimentos[i]}"

	plot "${Experimentos[i]}.txt" using 2:3:4 w p pt 7 lc variable title 'vertices'
	set terminal png
	set output '$LOCAL_SAIDA_GRAFICOS_ENTRADA/${Experimentos[i]}_cluster.png'
	replot
	
	set title "Tabus ${Experimentos[i]}"
	plot "${Experimentos[i]}.txt" using 2:3:5 w p pt 7 lc variable title 'vertices'
	set terminal png
	set output '$LOCAL_SAIDA_GRAFICOS_ENTRADA/${Experimentos[i]}_tabu.png'
	replot

	exit 
EOF

	
   done  
for ((i=0; i< $NUM_EXPERIMENTOS; i++))
   do
	gnuplot < $LOCAL_SAIDA_SCRIPTS/script_${Experimentos[i]}.txt
   done

#cria graficos das saidas
for ((i=0; i<$NUMERO_RESULTADOS; i++))
   do
     echo “$i”
	 echo ${Dados[i]}
	cat > $LOCAL_SAIDA_SCRIPTS/script_${Dados[i]}.txt << EOF
	reset
	set grid
	set ylabel "Y"
	set xlabel "X"
	set title "${Dados[i]}"

	plot "$LOCAL_SAIDA_RESULTADOS/${Dados[i]}.txt" using 2:3 w l lc 1 title 'rotas'
	replot "$LOCAL_SAIDA_RESULTADOS/${Dados[i]}.txt" using 2:3:4 w p pt 7 lc variable title 'vertices'
	set terminal png
	set output '$LOCAL_SAIDA_IMAGENS/imagem_${Dados[i]}.png'
	replot
	exit 
EOF

	
   done  
for ((i=0; i< $NUMERO_RESULTADOS; i++))
   do
	gnuplot < $LOCAL_SAIDA_SCRIPTS/script_${Dados[i]}.txt
   done
