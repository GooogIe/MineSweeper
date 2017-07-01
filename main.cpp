#include <stdlib.h>
#include <iostream>
#include "Field.h"
using namespace std;

int gameMenu(){
	int dec;
	cout<<"================================="<<endl;
	cout<<"Operazioni Disponibili: "<<endl;
	cout<<"1 - Torna al menu"<<endl;
	cout<<"2 - Metti/togli una bandierina"<<endl;
	cout<<"3 - Cancella l'ultima mossa"<<endl;
	cout<<"4 - Salva e torna al menu"<<endl;
	cout<<"5 - Continua"<<endl;
	cout<<"La tua scelta: ";
	cin>>dec;
	cout<<"================================="<<endl;
	return dec;
}




int menu(){
	system("clear");
	int dec;
	cout<<"================================="<<endl;
	cout<<"Cosa vuoi fare?"<<endl;
	cout<<"1 - Nuova partita"<<endl;
	cout<<"2 - Carica la partita salvata"<<endl;
	cout<<"3 - Quit"<<endl;
	cout<<"La tua scelta: ";
	cin>>dec;
	cout<<"================================="<<endl;
	return dec;
}
int main(){
	srand(time(NULL));
	bool playing = true;
	int rows,cols,mines,row=0, col=0,turno=0,opt,status = 1; // Status: 1 = playing 0 = finished
	Field *MyField;
	do{
		switch(menu()){
			case 1: 
				cout<<"Ok, cominciamo."<<endl;
				cout<<"Inserisci il numero di righe: ";
				cin>>rows;
				cout<<"Inserisci il numero di colonne: ";
				cin>>cols;
				cout<<"Inserisci il numero di mine: ";
				cin>>mines;
				MyField = new Field(rows,cols,mines);
				if(MyField->initField()==0){
					break;
				}
				do{
					turno++;
					system("clear");
					cout<<"Ultima mossa: Riga = "<<row<<" Colonna = "<<col<<endl;
					cout<<"================================="<<endl;
					MyField->printUserField();
					switch(gameMenu()){
						case 1:
							status = 0;
							break;
						case 2:
							cout<<"Se c'è già una bandiera in quel punto verrà tolta.";
							cout<<"Inserisci il numero della riga: ";
							cin>>row;
							cout<<"Inserisci il numero della colonna: ";
							cin>>col;
							MyField->placeFlag(row-1,col-1);
							break;
						case 3:
							//MyField->goBack();
							break;
						case 4:
							//MyField->saveToFile();
							break;
						case 5:
							cout<<"Inserisci il numero della riga: ";
							cin>>row;
							cout<<"Inserisci il numero della colonna: ";
							cin>>col;
							system("clear");
							status = MyField->userTurn(row-1,col-1);
							if(MyField->userHasWon()){
								cout<<"Congratulazioni hai vinto!";
								status = 0;
							}
							break;
					}
					cout<<endl<<"Premi un tasto qualsiasi per continuare...";
					cin.ignore(1024, '\n');
					cin.get();
				}while(status != 0);
				delete MyField;
				break;

			case 2:
				playing = false;
				break;

			case 3:
				playing = false;
				break;

			default:
				break;
		}
	}while(playing);
}
