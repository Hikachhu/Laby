void ia(int dim1, int dim2, int hautI, int lonI, int hautF, int lonF, int final[dim1][dim2], int passage[dim1][dim2]){
	int haut,droite,bas,gauche,pas,bloqueur;
	pas=0;	
	while((hautI==hautF && lonI==lonF)){
		bloqueur=0;
		possible(0,dim1,dim2,hautI,lonI,passage,&haut,&droite,&bas,&gauche);
		if ((droite+gauche+haut+bas)==1)
		{
				bloqueur=1;
		}
		possible(1,dim1,dim2,hautI,lonI,passage,&haut,&droite,&bas,&gauche);
		if (droite==1)
		{
			lonI+=2;
			if(bloqueur==1){
				passage[hautI][lonI-1]=1;	
			}
		}
		else if (bas==1)
		{
			hautI+=2;
			if(bloqueur==1){
				passage[hautI-1][lonI]=1;
			}
		}
		else if (gauche==1)
		{
			lonI-=2;
			if(bloqueur==1){
				passage[hautI][lonI+1]=1;
			}
		}
		else
		{
			hautI-=2;
			if(bloqueur==1){
				passage[hautI+1][lonI]=1;
			}
		}
		pas++;

		if(passage[hautI][lonI]==2||passage[hautI][lonI]==0)
			passage[hautI][lonI]=4;
		else
		{
			passage[hautI][lonI]*=4;
		}
		
	
	printf("\033[%d;%dH",26,120);
	printf("numéro : %d (%d %d) (%d %d)" ,passage[hautI][lonI], hautI, hautF ,lonI, lonF);		
	getchar();
	}
		return;
}