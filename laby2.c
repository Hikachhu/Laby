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
#define CYAN "\033[0;46m"
#define GRIS "\033[0;47m"
#define DIM 1

void viderBuffer();
void IntegraliteFichiers();
void LectureConfig(int *mur,int *perso, int *arriv ,int *lettre, int *chemin);
void EcritureConfig(char *cara, int *sec);
int  LectureScore();
void clearScreen();
char key_pressed();
void clean();
void copie(int dim1, int dim2 ,int Arr1[dim1][dim2], int Arr2[dim1][dim2]);
void RechargeTableau(int dim1, int dim2 ,int Arr1[dim1][dim2]);
void Verif(int NbLig, int NbCol, char tableau[NbLig][NbCol][4]);
void affiche(int haut, int lon, int color,int dim);
void lectCase(int dim1, int dim2, int final[dim1][dim2]);
int  possible(int dim1,int dim2, int hau,int lon, int laby[dim1][dim2],int *haut, int *droite, int *bas, int *gauche);
int  move(int *Sec2,int *Min,int dim1, int dim2, int final[dim1][dim2], int passage[dim1][dim2], int lonI, int hautI,int lonF,int hautF);
void Alettre(char *cara,int *haut,int *lon);
int  menu();
int  SelectionCouleur();
int  ChoixCouleur();


void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

void IntegraliteFichiers(){
	system("clear");
	FILE* fichier = NULL;
	int retour;
	char lecture[100];
	fichier=fopen("diff.txt","w+");
	fprintf(fichier," ");
	fclose(fichier);
	system("ls > EnsembleFichier.txt");
	system("cmp Ensemble.txt EnsembleFichier.txt > diff.txt");
	system("du -b diff.txt | awk '{ print $1 }' > .taille.txt");
	fichier=fopen(".taille.txt","r");
	fscanf(fichier,"%d",&retour);
	fclose(fichier);

	if (retour>=2)
	{
		printf("Veuillez rajouter le(s) fichier(s) suivant: %d\n",retour);
		system("diff Ensemble.txt EnsembleFichier.txt | grep '>'");
		exit(-1);
	}
	else
	{
		printf("Tout les fichiers sont présents\n");
	}
}
        
/*Lecture du fichier permettant d'avoir les couleurs des différents éléments*/
/*Met dans les variables, la couleurs correspondante
Ex: *mur=1, donc les murs seront gris*/
void LectureConfig(int *mur,int *perso, int *arriv ,int *lettre, int *chemin){
	FILE* fichier = NULL;
	int i=0,j=0;
	fichier = fopen( "option.txt", "r");
	fscanf(fichier,"%d %d %d %d %d",mur,perso,arriv,lettre,chemin);
	fclose(fichier);
}
/*Modification des élément d'un fichier "cara" pour écrire les temps*/
void EcritureConfig(char *cara, int *sec){
	FILE* fichier = NULL;
	int i=0,j=0;
	char name[20];
	sprintf(name,"%s.txt",cara);
	fichier = fopen( name, "a");
	fprintf(fichier,"%d",*sec);
	fclose(fichier);
}


int LectureScore(){
	FILE* fichier = NULL;
	int MeuilleurScore;
	fichier=fopen("score.txt","r");
	fscanf(fichier,"%d",&MeuilleurScore);
	fclose(fichier);
	return MeuilleurScore;
}

/*Fonction permettant d'effacer simplement l'écran*/
void clearScreen()
{
	int i,j;
	for(i=0;i<169;i++){
		for(j=0;j<50;j++){
			printf("\033[%d;%dH",j,i);
			printf(DEF" "DEF);
		}
	}
	printf("\033[%d;%dH",0,0);
			
}

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

/*Demande avant d'utiliser la fonction pour tout effacer*/
void clean(){
	char touche=0;
	printf("\033[%d;%dH",44,0);
	printf("Appuie pour continuer");
	printf("\033[%d;%dH",30,165);
	viderBuffer();
	clearScreen();
}
/*Copie les tableaux*/
void copie(int dim1, int dim2 ,int Arr1[dim1][dim2], int Arr2[dim1][dim2]){
	int i,j;
	for(i=0;i<dim1;i++){
		for(j=0;j<dim2;j++){
			Arr1[i][j]=Arr2[i][j];
		}
	}

}

void RechargeTableau(int dim1, int dim2 ,int Arr1[dim1][dim2]){
	int i,j,k,l,m,mur,perso,arriv,lettre,chemin,def;
	LectureConfig(&mur,&perso,&arriv,&lettre,&chemin);
	int valeurMur=Arr1[0][0];
	for(i=0;i<dim1;i++){
		for(j=0;j<dim2;j++){
			if(Arr1[i][j]==valeurMur){
				Arr1[i][j]=mur;
			}
			else{
				Arr1[i][j]=chemin;
			}
		}
	}

}
void Verif(int NbLig, int NbCol, char tableau[NbLig][NbCol][4]){
	for (int i = 0; i < NbLig; i++)
	{
		for (int j = 0; j < NbCol; j++)
		{
			if((tableau[i][j][0]!=tableau[i-1][j][2])&&((i-1)>=0)){
				printf("Le laby est incorrect aux coordonnées:\ni=%d,j=%d,haut\n",i,j);
				printf("Veuillez corrigé le labyrinthe de référence\n");
				exit(-1);
			}
			if((tableau[i][j][1]!=tableau[i][j+1][3])&&j+1<NbCol){
				printf("Le laby est incorrect aux coordonnées:\ni=%d,j=%d,droite\n",i,j);
				printf("Veuillez corrigé le labyrinthe de référence\n");
				exit(-1);
			}
			if((tableau[i][j][2]!=tableau[i+1][j][0])&&i+1<NbLig){
				printf("Le laby est incorrect aux coordonnées:\ni=%d,j=%d,bas\n",i,j);
				printf("Veuillez corrigé le labyrinthe de référence\n");
				exit(-1);
			}
			if((tableau[i][j][3]!=tableau[i][j-1][1])&&j-1>0){
				printf("Le laby est incorrect aux coordonnées:\ni=%d,j=%d,gauche\n",i,j);
				printf("Veuillez corrigé le labyrinthe de référence\n");

				exit(-1);
			}
		}
	}
	printf("Le labyrinthe est correctement lu\n");
	getchar();

}
/*Fonction permettant d'afficher un carré d'une couleur spécifique "color" aux coordonnées haut et lon*/

void affiche(int haut, int lon, int color,int dim){
	lon*=2;
	lon*=dim;
	haut*=dim;
	int i,j,k,l,m,mur,perso,arriv,lettre,chemin,def;
	LectureConfig(&mur,&perso,&arriv,&lettre,&chemin);
		for(i=0;i<dim;i++){
			for(j=0;j<dim*2;j+=2){
				printf("\033[%d;%dH",1+haut+i,1+lon+j);
				if(color==0){
					printf(CYAN"  "DEF);
				}
				if(color==1){
					printf(RED"  "DEF);
				}
				if(color==2){
					printf(VIO"  "DEF);
				}
				if(color==3){	
					printf(YELLOW"  "DEF);
				}
				if(color==4){
					printf(GRIS"  "DEF);
				}
				if(color==5){
					printf(DEF"  "DEF);
				}
			}
		}
	
	printf("\033[%d;%dH",167,0);
	printf("\033[?251");
}

/*Affiche l'intégralité d'un tableau */
void lectCase(int dim1, int dim2, int final[dim1][dim2]){

	int i,j;
	for(i=0;i<dim1;i++){
		for(j=0;j<dim2;j++){
            affiche(i,j,final[i][j],DIM);
        }
	}
}

int possible(int dim1,int dim2, int hau,int lon, int laby[dim1][dim2],int *haut, int *droite, int *bas, int *gauche){
	printf("\033[%d;%dH",20,120);
	printf("      ");
	printf("\033[%d;%dH",21,120);
	printf("      ");
	printf("\033[%d;%dH",22,120);
	printf("      ");
	printf("\033[%d;%dH",23,120);
	printf("      ");
	printf("\033[%d;%dH",24,120);
	printf("      ");
	int mur,perso,arriv,lettre,chemin;	
	LectureConfig(&mur,&perso, &arriv ,&lettre, &chemin);
	*haut=0;
	*bas=0;
	*droite=0;
	*gauche=0;
	int boucle=(-1);
	
	while(1){
		if(laby[hau+1][lon]!=mur){
				printf("\033[%d;%dH",21,120);
				printf("bas");
				*bas=1;
		}

		if(laby[hau][lon+1]!=mur){
				printf("\033[%d;%dH",22,120);
				printf("droite");
				*droite=1;
		}
		if(laby[hau-1][lon]!=mur){
				printf("\033[%d;%dH",20,120);
				printf("haut");
				*haut=1;
		}
		if(laby[hau][lon-1]!=mur){
				printf("\033[%d;%dH",23,120);
				printf("gauche");
				*gauche=1;
		} 
	}
	//lectCase(dim1,dim2,laby);
	printf("\033[%d;%dH",24,120);
	printf("%d %d",hau, lon);	
	return ((*haut)+(*droite)+(*bas)+(*gauche));
}

int ia(int dim1, int dim2, char laby[dim1][dim2][4], int lonI, int hautI,int lonF,int hautF){
	int move=0;
	int m=0,Entree=2;
	for (int i = 0; i < dim1; i++)
	{
		for (int j = 0; j < dim2; j++)
			{
				for (int k = 0;k < 4; k++)
					{
						printf("%c",laby[i][j][k]);
					}
				printf(" ");	
			}	
			printf("\n");
	}
	printf("\n");
	getchar();
	while(!(lonI==lonF && hautI==hautF)){
		move=0;
		for (int i = 0; move==0 && i!=4; i++)
		{
			m=(Entree+i)%4;
			printf("mur en  %d = %d \n", m,laby[lonI][hautI][m]);
			if(laby[lonI][hautI][m]=='-'){
				switch (m){
					case 0:
						hautI--;
						break;
					case 1:
						lonI++;
						break;
					case 2:
						hautI++;
						break;
					case 3:
						lonI--;
						break;
				}
				printf("direction=%d:(haut %d et long %d)\n",m,hautI,lonI);
				Entree=(m+3)%4;
				move++;
				break;
			}
			m=Entree;
		}
		printf("sorti\n");
	}
	return move;

}

int move(int *Sec2,int *Min,int dim1, int dim2, int final[dim1][dim2], int passage[dim1][dim2], int lonI, int hautI,int lonF,int hautF){
	char touche;
	int Sec,Hour=0,mur,perso,arriv,lettre,chemin,def;
	*Min=0;
	Sec=time(NULL);
//	printf("\007");
	printf("\033[%d;%dH",20,120);
	printf("Chronomètre:");
	touche=0;
	LectureConfig(&mur,&perso, &arriv ,&lettre, &chemin);
	int droite,gauche,bas,haut;
	while(!(lonI==lonF&&hautI==hautF)){		
		
		*Sec2=time(NULL);
		printf("\033[%d;%dH",21,120);
		if ((*Sec2-Sec)==60)
		{
			Sec=time(NULL);
			(*Min)++;
		}
		printf("\033[%d;%dH",21,120);
		printf("%02d:%02d",*Min,*Sec2-Sec);
		
		touche=0;
		touche=key_pressed();

		if (touche==32)
		{
			return 2;
		}
		if(touche==122){
			if(final[hautI-1][lonI]==chemin){
                affiche(hautI,lonI,final[hautI][lonI],DIM);
				affiche(hautI-2,lonI,perso,DIM);
				passage[hautI][lonI]=perso;
				passage[hautI-1][lonI]=perso;
				hautI-=2;
			}
		}
		if(touche==113){
			if(final[hautI][lonI-1]==chemin){
                affiche(hautI,lonI,final[hautI][lonI],DIM);
				affiche(hautI,lonI-2,perso,DIM);
				passage[hautI][lonI]=perso;
				passage[hautI][lonI-1]=perso;
				lonI-=2;
			}
		}
		if(touche==100){
			if(final[hautI][lonI+1]==chemin){
                affiche(hautI,lonI,final[hautI][lonI],DIM);
				affiche(hautI,lonI+2,perso,DIM);
				passage[hautI][lonI]=perso;
				passage[hautI][lonI+1]=perso;
				lonI+=2;
			}
		}
		if(touche==115){
			if(final[hautI+1][lonI]==chemin){
                affiche(hautI,lonI,final[hautI][lonI],DIM);
				affiche(hautI+2,lonI,perso,DIM);
				passage[hautI][lonI]=perso;
				passage[hautI+1][lonI]=perso;
				hautI+=2;
			}
		}
		touche=0;
		

	}

	(*Sec2)=time(NULL)-Sec+((*Min)*60);
	return 0;	
}

void Alettre(char *cara,int *haut,int *lon){
	int z,mur,perso,arriv,lettre,chemin,def;	
	int decalon=*haut,decahaut=*lon;
	LectureConfig(&mur,&perso,&arriv,&lettre,&chemin);

 	FILE* fichier = NULL;
 	for(z=0;cara[z]!='\0';z++){
		if(cara[z]<='z'&&cara[z]>='a'){
			char final[10];
			int lecture,i=0,j=0;
			int affichage[5][5];
			sprintf(final,"%c.txt",cara[z]);
		        fichier = fopen( final, "r");
		
		        if (fichier==NULL)
			                printf("%s\n",final);
				else{
					for(i=0;i<5;i++){
						fscanf(fichier,"%d %d %d %d %d",&affichage[i][0],&affichage[i][1],&affichage[i][2],&affichage[i][3],&affichage[i][4]);
					}
					
			
					for(i=0;i<5;i++){
						for(j=0;j<5;j++){
							if (affichage[i][j]==3)
									{
										affichage[i][j]=lettre;
									}		
							affiche(i+decalon,j+decahaut,affichage[i][j],1);
				
						}
					}
					decahaut+=5;
	
				}		
			fclose(fichier);
		}
		else if(cara[z]==' ')
			decahaut+=2;
		else if(cara[z]=='A'||cara[z]=='V'){
			char final[10];
			int lecture,i=0,j=0;
			int affichage[5][5];
			if(cara[z]=='A'){
				fichier = fopen( "fleche.txt", "r");
			}
			if (cara[z]=='V'){
				fichier = fopen("vide.txt","r");
			}
	        if (fichier==NULL)
	            printf("%s\n",final);
			else{
				for(i=0;i<5;i++){
					fscanf(fichier,"%d %d %d %d %d",&affichage[i][0],&affichage[i][1],&affichage[i][2],&affichage[i][3],&affichage[i][4]);
				}
			for(i=0;i<5;i++){
				for(j=0;j<5;j++){

					if (affichage[i][j]==3)
							{
								affichage[i][j]=lettre;
							}
		
					affiche(i+decalon,j+decahaut,affichage[i][j],1);
		
				}
			}
				
			decahaut+=5;
		}
				
	}	
	else{
		decahaut=0;
		decalon+=6;
	}
	decahaut++;
}	
}

int menu(){
	int i,j,lon=0,haut=0,r;
	char touche;
	int mur,perso,arriv,lettre,chemin;
	LectureConfig(&mur,&perso,&arriv,&lettre,&chemin);
	clean();
	Alettre("menu",&haut,&lon);
	haut=6;
	Alettre("A",&haut,&lon);
	lon=6;
	for(i=0;i<24;i++){
		for(j=0;j<2;j++){
			affiche(haut+i,lon+j,lettre,1);
		}
	}
	lon=9;
	Alettre("jouer",&haut,&lon);
	haut=12;
	Alettre("couleur",&haut,&lon);
	haut=18;
	Alettre("credit",&haut,&lon);
	haut=24;
	Alettre("quitter",&haut,&lon);
	touche=0;
	haut=6;
	lon=0;
	r=1;
/****Gestion clavier dans le menu******************/
		while(touche==0){
			touche=key_pressed();
			
			if (touche==122)
			{
				touche=0;
				if (r!=1){
					r--;
					Alettre("V",&haut,&lon);
					haut=(6*r);
					Alettre("A",&haut,&lon);
					touche=0;
				}
			}
			else if (touche==115)
			{
				touche=0;
				if(r!=4)
				{
					r++;
					Alettre("V",&haut,&lon);
					haut=(6*r);
					Alettre("A",&haut,&lon);
					touche=0;
				}
			}
			else if (touche==10)
			{
				
			}
			else
				touche=0;


		}		

	clearScreen();
	return r;
}

int type(){
	int i,j,lon=0,haut=0,r;
	char touche;
	int mur,perso,arriv,lettre,chemin;
	LectureConfig(&mur,&perso,&arriv,&lettre,&chemin);
	Alettre("menu",&haut,&lon);
	haut=6;
	Alettre("A",&haut,&lon);
	lon=6;
	for(i=0;i<12;i++){
		for(j=0;j<2;j++){
			affiche(haut+i,lon+j,lettre,1);
		}
	}
	lon=9;
	Alettre("manuel",&haut,&lon);
	haut=12;
	Alettre("auto",&haut,&lon);
	touche=0;
	haut=6;
	lon=0;
	r=1;
/****Gestion clavier dans le menu******************/
		while(touche==0){
			touche=key_pressed();
			
			if (touche==122)
			{
				touche=0;
				if (r!=1){
					r--;
					Alettre("V",&haut,&lon);
					haut=(6*r);
					Alettre("A",&haut,&lon);
					touche=0;
				}
			}
			else if (touche==115)
			{
				touche=0;
				if(r!=2)
				{
					r++;
					Alettre("V",&haut,&lon);
					haut=(6*r);
					Alettre("A",&haut,&lon);
					touche=0;
				}
			}
			else if (touche==10)
			{
				
			}
			else
				touche=0;


		}		

	clearScreen();
	return r;
}


int SelectionCouleur(){

	int i,j,lon=0,haut=0,r;
	char touche;
	int mur,perso,arriv,lettre,chemin;
	LectureConfig(&mur,&perso,&arriv,&lettre,&chemin);
	clearScreen();
	Alettre("categorie",&haut,&lon);
	haut=6;
	Alettre("A",&haut,&lon);
	lon=6;
	for(i=0;i<30;i++){
		for(j=0;j<2;j++){
			affiche(haut+i,lon+j,lettre,1);
		}
	}
	lon=9;
	Alettre("mur",&haut,&lon);
	haut=12;
	Alettre("perso",&haut,&lon);
	haut=18;
	Alettre("arrive",&haut,&lon);
	haut=24;
	Alettre("lettre",&haut,&lon);
	haut=30;
	Alettre("chemin",&haut,&lon);
	touche=0;
	haut=6;
	lon=0;
	r=1;
/****Gestion clavier dans le menu******************/
		while(touche==0){
			touche=key_pressed();
			
			if (touche==122)
			{
				touche=0;
				if (r!=1){
					r--;
					Alettre("V",&haut,&lon);
					haut=(6*r);
					Alettre("A",&haut,&lon);
					touche=0;
				}
			}
			else if (touche==115)
			{
				touche=0;
				if(r!=5)
				{
					r++;
					Alettre("V",&haut,&lon);
					haut=(6*r);
					Alettre("A",&haut,&lon);
					touche=0;
				}
			}
			else if (touche==10)
			{
				
			}
			else
				touche=0;

		}		

	clearScreen();
	return --r;
}

int ChoixCouleur(){

	int i,j,lon=0,haut=0,r;
	char touche;
	int mur,perso,arriv,lettre,chemin;
	LectureConfig(&mur,&perso,&arriv,&lettre,&chemin);
	clearScreen();
	Alettre("couleur",&haut,&lon);
	haut=6;
	Alettre("A",&haut,&lon);
	lon=6;
	for(i=0;i<30;i++){
		for(j=0;j<2;j++){
			affiche(haut+i,lon+j,lettre,1);
		}
	}
	lon=9;
	Alettre("cyan",&haut,&lon);
	haut=12;
	Alettre("rouge",&haut,&lon);
	haut=18;
	Alettre("violet",&haut,&lon);
	haut=24;
	Alettre("jaune",&haut,&lon);
	haut=30;
	Alettre("gris",&haut,&lon);
	touche=0;
	haut=6;
	lon=0;
	r=1;
/****Gestion clavier dans le menu******************/
		while(touche==0){
			touche=key_pressed();
			
			if (touche==122)
			{
				touche=0;
				if (r!=1){
					r--;
					Alettre("V",&haut,&lon);
					haut=(6*r);
					Alettre("A",&haut,&lon);
					touche=0;
				}
			}
			else if (touche==115)
			{
				touche=0;
				if(r!=5)
				{
					r++;
					Alettre("V",&haut,&lon);
					haut=(6*r);
					Alettre("A",&haut,&lon);
					touche=0;
				}
			}
			else if (touche==10)
			{
				
			}
			else
				touche=0;

		}		

	return --r;
}

void ChangementCouleur(int position, int couleur){
	FILE* fichier = NULL;
	int reglage[6],i=0,j=0,mur=0,perso=0,arriv=0,lettre=0,chemin=0;
	LectureConfig(&mur,&perso,&arriv,&lettre,&chemin);
	fichier = fopen( "option.txt", "w+");
	reglage[0]=mur;
	reglage[1]=perso;
	reglage[2]=arriv;
	reglage[3]=lettre;
	reglage[4]=chemin;
	reglage[5]=reglage[position];
	reglage[position]=couleur;
	if (reglage[0]==reglage[4])
	{
		printf("Désoler mais un mur ne peut avoir la meme couleur que le chemin\n");
		reglage[position]=reglage[5];
	}
	else if(reglage[1]==reglage[4])
	{
		printf("Désoler mais le perso ne peut avoir la meme couleur que le chemin\n");
		reglage[position]=reglage[5];	
	}
	
	fprintf(fichier,"%d %d %d %d %d",reglage[0],reglage[1],reglage[2],reglage[3],reglage[4]);

	fclose(fichier);
}

int main()
{
IntegraliteFichiers();
int i=0,j,k,l,m,n,o,p,q,Balise=0;
int mur,perso,arriv,lettre,chemin;

	LectureConfig(&mur,&perso,&arriv,&lettre,&chemin);


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

	i=0;
	j=0;
	k=0;
	
	while ( fgets(Ligne,NbCol*5,fichier ) != NULL ) {
		if (strlen(Ligne)>1) {
			for (k = 0 ; k < NbCol*5 ; k++ ) {
				lecture=Ligne[k];
				if(lecture=='M'||lecture=='-'){
					text[i][j][m]=lecture;
					m++;
				}
				else
				{
					m=0;
					if(j==NbCol-1){
						i++;
						j=0;
					}
					else 
						j++;
				} 
			}
		}
	}

	//Verif(NbLig,NbCol,text);
	int final[NbLig*2+1][NbCol*2+1];
	int passage[NbLig*2+1][NbCol*2+1];

	for(i=0;i<NbLig*2+1;i++){
		for(j=0;j<NbCol*2+1;j++){
			if( i==0 || j==0 || i==NbCol*2 || j==NbCol*2 || (i%2==0&&j%2==0) )
				final[i][j]=0;
			else
				final[i][j]=1;
		}
	}

	for (int i = 0; i < NbLig; i++)
	{
		for (int j = 0; j < NbCol; j++)
			{
				for (int k = 0;k < 4; k++)
					{
						printf("%c",text[i][j][k]);
					}
				printf(" ");	
			}	
			printf("\n");
	}
	printf("\n");
	getchar();
	for(i=0;i<=NbLig;i++){
		for(j=0;j<=NbCol;j++){
	
			if(text[i][j][0]=='M')
				final[i*2][j*2+1]=0;
			if(text[i][j][3]=='M')
				final[i*2+1][j*2]=0;
		}
	}
	for (int i = 0; i < NbLig; i++)
	{
		for (int j = 0; j < NbCol; j++)
			{
				for (int k = 0;k < 4; k++)
					{
						printf("%c",text[i][j][k]);
					}
				printf(" ");	
			}	
			printf("\n");
	}
	printf("\n");
	getchar();
	RechargeTableau(NbCol*2+1,NbLig*2+1,final);
	copie(NbCol*2+1,NbLig*2+1,passage,final);
/*	for(i=0;i<NbLig*2+1;i++){
		for(j=0;j<NbCol*2+1;j++){
			if(final[i][j]==1)
				printf (RED"  "DEF); 
			else
				printf("  ");
		}
		printf("\n");
	}
*/
	int couleur,mouvement,categorie,haut=0,lon=0,sec=0,min=0,Score1,Mouvement;
	int choix;
	clearScreen();
	Alettre("bienvenue_tu es dans le_laby",&haut,&lon);
		do{
			LectureConfig(&mur,&perso,&arriv,&lettre,&chemin);
			choix=menu();
				switch(choix){
				case 1:
					switch(type()){
					case 1:
						copie(NbCol*2+1,NbLig*2+1,passage,final);
						printf("quand tu seras dans le laby tu pourras utliser les commandes suivantes\n");
						printf("z=haut\nq=gauche\ns=bas\nd=droite\n");
						printf("Tu peut quitter le labyrinthe à tout moment en appuyant sur la touche ESPACE\n");
						clean();
						printf("\033[%d;%dH",0,0);
						lectCase(NbLig*2+1, NbCol*2+1, final);
						affiche(CoS*2+1,LgS*2+1,arriv,DIM);
						affiche(LgE*2+1,CoE*2+1,perso,DIM);
						Mouvement=move(&sec,&min,NbLig*2+1, NbCol*2+1,final,passage,LgE*2+1,CoE*2+1,LgS*2+1,CoS*2+1);
						if (Mouvement==0)
						{		
							clean();
							printf("\033[%d;%dH",0,0);
							if (LectureScore()>sec)
							{
								printf("Bravo tu as battu le meuilleur score de %d",LectureScore()-sec);
							}
							else{
								printf("Dommage tu as raté le meuilleur score de %d",sec-LectureScore());
							}
							printf("s \nTu as mis %02d min %02d sec",sec/60,sec%60);
							EcritureConfig("score",&sec);
							system("sort -n score.txt | head -n 10> score2.txt; mv score2.txt  score.txt");
							getchar();
							printf("\033[%d;%dH",(NbCol+1)*2,0);
							clean();
							Alettre("tu as gagne",&lon,&haut);	
							clean();
							printf("Tu as pris ce chemin :");
							clean();
							lectCase(NbLig*2+1, NbCol*2+1, passage);
							printf("\033[%d;%dH",(NbCol+1)*2,0);
						}
						else{
							clearScreen();
							printf("\033[%d;%dH",0,0);
							printf("Vous avez quitter le laby\nRetour au menu\n");
						}
						break;
					case 2:
						printf("ia1\n");

						mouvement=ia(NbLig,NbCol,text,LgE,CoE,LgS,CoS);
						printf("ia2 %d\n",mouvement);
						break;
				}
				break;
				case 2:
					categorie=SelectionCouleur();
					couleur=ChoixCouleur();
					ChangementCouleur(categorie,couleur);
					RechargeTableau(NbCol*2+1,NbLig*2+1,final);
					break;
				case 3:
					printf("\033[%d;%dH",0,0);
					printf("Application réalisé dans le cadre du projet info de 1ère Année de l'ESIEA une grande ecole d'ingenieure\nFlorian Morin: Programmeur\nRaphaël Ptithaddad: Design et chef de projet\n");
					break;
				case 4:
					Alettre("au revoir",&lon,&haut);
					break;
			}

		}while(choix!=4);
	fclose(fichier);

	clean();

return 0;
}
