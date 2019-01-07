#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#define DEF "\033[0;00m"
#define RED "\033[0;41m"
#define GREEN "\033[0;42m"
#define YELLOW "\033[0;43m"
#define BNOR "\033[0;44m"
#define VIO "\033[0;45m"
#define BCIEL "\033[0;46m"
#define GRIS "\033[0;47m"
#define DIM 2

char key_pressed()
{
        struct termios oldterm, newterm;
        int oldfd; char c, result = 0;
        tcgetattr (STDIN_FILENO, &oldterm);
        newterm = oldterm; newterm.c_lflag &= ~(ICANON | ECHO);
        tcsetattr (STDIN_FILENO, TCSANOW, &newterm);
        oldfd = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl (STDIN_FILENO, F_SETFL, oldfd | O_NONBLOCK);
        c = getchar();
        tcsetattr (STDIN_FILENO, TCSANOW, &oldterm);
        fcntl (STDIN_FILENO, F_SETFL, oldfd);
        if (c != EOF) {ungetc(c, stdin); result = getchar();}

return result;
}

void affiche(int haut, int lon, int color){
	lon*=2;
	lon*=DIM;
	haut*=DIM;
	int i,j,k,l,m;
	
	for(i=0;i<DIM;i++){
		for(j=0;j<DIM*2;j+=2){
			printf("\033[%d;%dH",1+haut+i,1+lon+j);
			if(color==1)
				printf(BCIEL"  "DEF);
			if(color==2)
				printf(RED"  "DEF);
			if(color==3)
				printf(VIO"  "DEF);
			if(color==4)	
				printf(YELLOW"  "DEF);
//			usleep(1000);
		}
	}
	printf("\033[%d;%dH",0,0);
	printf("\033[?25l");
}

void lectCase(int dim1, int dim2, int final[dim1][dim2]){

	int i,j;
	for(i=0;i<dim1;i++){
                for(j=0;j<dim2;j++){
                        if(final[i][j]==1)
                                affiche(i,j,1);
			else                       
                                affiche(i,j,2);
                }
	}
}
void move(int dim1, int dim2, int final[dim1][dim2],int lonI, int hautI,int lonF,int hautF){
	char touche;
	touche=0;
	while(touche==0){
		touche=0;
		touche=key_pressed();
		if(touche==122){
			if(final[hautI-1][lonI]==0){
                        	if(final[hautI][lonI]==1)
                        	        affiche(hautI,lonI,1);
				else                       
                        	        affiche(hautI,lonI,2);
				affiche(hautI-2,lonI,3);
				hautI-=2;
			}
		}
		if(touche==113){
			if(final[hautI][lonI-1]==0){
                        	if(final[hautI][lonI]==1)
                        	        affiche(hautI,lonI,1);
				else                       
                        	        affiche(hautI,lonI,2);
				affiche(hautI,lonI-2,3);
				lonI-=2;
			}
		}
		if(touche==100){
			if(final[hautI][lonI+1]==0){
                        	if(final[hautI][lonI]==1)
                        	        affiche(hautI,lonI,1);
				else                       
                        	        affiche(hautI,lonI,2);
				affiche(hautI,lonI+2,3);
				lonI+=2;
			}
		}
		if(touche==115){
			if(final[hautI+1][lonI]==0){
                        	if(final[hautI][lonI]==1)
                        	        affiche(hautI,lonI,1);
				else                       
                        	        affiche(hautI,lonI,2);
				affiche(hautI+2,lonI,3);
				hautI+=2;
			}
		}
		touche=0;
		if(hautI==hautF*2+1&&lonI==lonF*2+1)
			return;	
	}

}
int main()
{
        int i=0,j,k,l,m,n,o,p,q,Balise=0;
        int NbLig, NbCol, LgE, CoE, LgS, CoS;
	char lecture,touche;
        FILE* fichier = NULL;
        fichier = fopen( "maze_25x25.txt", "r");


        if (fichier==NULL)
                printf("ERROR");

        else {
		fscanf(fichier,"%d %d %d %d %d %d",&NbLig, &NbCol, &LgE, &CoE, &LgS, &CoS);
		
	}


        m=0;
	i=0;
	j=0;
	
	int dimension=NbLig*NbCol*4;
	char Ligne[NbCol*5];
	char text[NbLig][NbCol][4];
	printf("dimension = %d\n", dimension);

for (i = 0; i<NbLig; i++)
{
    for (j = 0; j<NbCol; j++)
	for ( k = 0 ; k < 4 ; k++)
        	text[i][j][k] = '=';
}
	i=0;
	j=0;
	k=0;
	
	while ( fgets(Ligne,NbCol*5,fichier ) != NULL ) {
		if (strlen(Ligne)>1) {
			for (k = 0 ; k < NbCol*5 ; k++ ) {
				lecture=Ligne[k];
				if(lecture=='M'||lecture=='-'){
//					printf("%d %d %d %c ",i,j,m,lecture); 
					text[i][j][m]=lecture;
					m++;
				}
				else
				{
					m=0;
					if(j==NbCol-1){
						i++;
						j=0;
//						printf("\n");
					}
					else 
						j++;
				} 
			}
		}
	}
	printf("\n");


	printf("FIN\n");	
	for(i=0;i<NbLig;i++) {
		for(j=0;j<NbLig;j++){
			for(k=0;k<4;k++){
				printf("%c",text[i][j][k]);
			}
		printf(" ");
		}
	printf("\n");
	}

	int final[NbLig*2+1][NbCol*2+1];
	for(i=0;i<NbLig*2+1;i++){
		for(j=0;j<NbCol*2+1;j++){
			if( i==0 || j==0 || i==NbCol*2 || j==NbCol*2 || (i%2==0&&j%2==0) )
				final[i][j]=1;
			else
				final[i][j]=0;
			}
		}

	for(i=0;i<NbLig;i++){
		for(j=0;j<NbCol;j++){
			if(text[i][j][0]=='M')
				final[i*2][j*2+1]=1;
			if(text[i][j][3]=='M')
				final[i*2+1][j*2]=1;
		}
	}
	for(i=0;i<NbLig*2+1;i++){
		for(j=0;j<NbCol*2+1;j++){
			if(final[i][j]==1)
				printf (RED"  "DEF); 
			else
				printf("  ");
		}
		printf("\n");
	}

/*
	printf("\033[%d;%dH",0,0);
	printf("\033[8m");
	lectCase(tab[0]*2+1, tab[1]*2+1, final);
	affiche(tab[4]*2+1,tab[5]*2+1,4);
	affiche(tab[2]*2+1,tab[3]*2+1,3);
	move(tab[0]*2+1, tab[1]*2+1,final,tab[2]*2+1,tab[3]*2+1,tab[4],tab[5]);
	printf("\033[%d;%dH",(tab[1]+1)*2,0);
	printf("REUSSI\n");

*/
return 0;
}
