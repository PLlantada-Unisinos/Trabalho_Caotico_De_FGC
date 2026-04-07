#include <iostream>
#include <cmath>
#include <ncurses.h>

using namespace std;

class Esfera {
    
    public:
    
    float m00;    float m01;    float m02;    float m03;
    float m10;    float m11;    float m12;    float m13;
    float m20;    float m21;    float m22;    float m23;
    float m30;    float m31;    float m32;    float m33;
    
    Esfera(float m00_, float m01_, float m02_, float m03_,
           float m10_, float m11_, float m12_, float m13_,
           float m20_, float m21_, float m22_, float m23_,
           float m30_, float m31_, float m32_, float m33_)
           {
    
    this->m00 = m00_;   this->m01 = m01_;   this->m02 = m02_;   this->m03 = m03_;
    this->m10 = m10_;   this->m11 = m11_;   this->m12 = m12_;   this->m13 = m13_;
    this->m20 = m20_;   this->m21 = m21_;   this->m22 = m22_;   this->m23 = m23_;
    this->m30 = m30_;   this->m31 = m31_;   this->m32 = m32_;   this->m33 = m33_;
   
           }

    void print() {
    cout<<"{"<<m00<<"}"<<"{"<<m01<<"}"<<"{"<<m02<<"}"<<"{"<<m03<<"}"<<endl;
    cout<<"{"<<m10<<"}"<<"{"<<m11<<"}"<<"{"<<m12<<"}"<<"{"<<m13<<"}"<<endl;
    cout<<"{"<<m20<<"}"<<"{"<<m21<<"}"<<"{"<<m22<<"}"<<"{"<<m23<<"}"<<endl;
    cout<<"{"<<m30<<"}"<<"{"<<m31<<"}"<<"{"<<m32<<"}"<<"{"<<m33<<"}"<<endl;
    }

    void toIdentity() {
    m00 = 1; m01 = 0; m02 = 0; m03 = 0;
    m10 = 0; m11 = 1; m12 = 0; m13 = 0;
    m20 = 0; m21 = 0; m22 = 1; m23 = 0;
    m30 = 0; m31 = 0; m32 = 0; m33 = 1;
    
    }
    
    void Translação(float tx, float ty, float tz) {
        float x = m03;  float y = m13;  float z = m23;
        
        toIdentity();
        
        m03 = x + tx;
        m13 = y + ty;
        m23 = z + tz;
        
        print();
        
    }
    
    void Escala(float sx, float sy, float sz) {
        float x = m00;  float y = m11; float z = m22;
        
        toIdentity();
        
        m00 = x + sx;
        m11 = y + sy;
        m22 = z + sz;
        
        print();
    }
    
    void Rotação(float rx, float ry, float rz) {
        
        //Deus existe né?
        
        m00 = (cos(ry) * cos(rz));
        m01 = (sin(rx) * sin(ry) * cos(rz) - cos(rx) * sin(rz));
        m02 = (cos(rx) * sin(ry) * cos(rz) + sin(rx) * sin(rz));
        
        m10 = (cos(ry) * sin(rz));
        m11 = (sin(rx) * sin(ry) * sin(rz) + cos(rx) * cos(rz));
        m12 = (cos(rx) * sin(ry) * sin(rz) - sin(rx) * cos(rz));
        
        m20 = -sin(ry);
        m21 = (sin(rx) * cos(ry));
        m22 = (cos(rx) * cos(ry));
        
        print();   
    }
};

class Camera {
  
  public:
  float camX;  float camY;  float camZ;
  float camRX; float camRY; float camRZ;
  
  Camera(float camX_, float camY_, float camZ_, float camRX_, float camRY_, float camRZ_) {
      this->camX = camX_;   this->camY = camY_;   this->camZ = camZ_;
      this->camRX = camRX_;   this->camRY = camRY_;   this->camRZ = camRZ_;
  }

    float matriz_camera[4][4];

    void Camera_Array(float camx, float camy, float camz, float camrx, float camry, float camrz) {
    
        matriz_camera[0][0] = 1;    matriz_camera[0][1] = 0;    matriz_camera[0][2] = 0;    matriz_camera[0][3] = -camx;
        matriz_camera[1][0] = 0;    matriz_camera[1][1] = 1;    matriz_camera[1][2] = 0;    matriz_camera[1][3] = -camy;
        matriz_camera[2][0] = 0;    matriz_camera[2][1] = 0;    matriz_camera[2][2] = 1;    matriz_camera[2][3] = -camz;
        matriz_camera[3][0] = 0;    matriz_camera[3][1] = 0;    matriz_camera[3][2] = 0;    matriz_camera[3][3] = 1;
}
    
};

int main()
{
    int menu = 0;
    int menu_choice = 0;
    
    while (menu == 0) {
        
cout<<". Menu Principal\n";
cout<<" 0. Testar o objeto\n 1. Manipular o objeto\n 2. Manipular a câmera\n 3. Modificar projeção\n 4. Modificar mapeamento\n"
    <<" 5. Visualizar objeto\n 6. Modificar Mapeamento\n 7. Visualizar o fdp\n 8. Tester o ncurses.h\n";
cin>>menu_choice;
    
    while (menu_choice < 0 || menu_choice > 8) {
        cin>>menu_choice;
    }
    
    if (menu_choice == 0) {
    
    cout<<"Hello World, Se liga só nessa porrenha aqui:\n\n";
    
    Esfera* Bola = new Esfera(0.10,0.20,0.30,0.40,0.50,0.60,0.70,0.80,0.90,0.11,0.22,0.33,0.44,0.55,0.66,0.77);
    
    cout<<"Esfera - Valores\n";
    Bola->print();
    
    cout<<"Esfera - Identidade\n";
    Bola->toIdentity(); Bola->print();
    
    cout<<"Esfera - Rotação em 45º\n";
    Bola->Rotação(45,0,0); //FUNCIONAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    
    cout<<"Esfera - Translação\n";
    Bola->Translação(1,2,3);
    
    cout<<"Esfera - Escala\n";
    Bola->Escala(6,6,6);
    
    menu = 1;
        }

        else if (menu_choice == 1) {
        
        cout<<"Ainda não implementado, tchau-tchau";
        menu = 1;
            
        }

            else if (menu_choice == 2) {
                
            cout<<"Ainda não implementado meu nobre";
            menu = 1;
                
            }
    
                else if (menu_choice == 3) {
                
                cout<<"Ainda não implementado, adios!";
                menu = 1;
                
            }
    
                    else if (menu_choice == 4) {
                    
                    cout<<"Ainda não implementado, flw!";
                    menu = 1;
                    
                }
    
                        else if (menu_choice == 5) {
                        
                        cout<<"Ainda não implementado o desgraça!";
                        menu = 1;
                        
                    }

                            else if (menu_choice == 6) {
                            
                            cout<<"Ainda não implementado filha da puta!!";
                            menu = 1;
                            
                            }
    
                                else if (menu_choice == 7) {
                                    
                                cout<<"AINDA. NÃO. FOI. IMPLEMENTADO. PORRA.";
                                menu = 1;
                                
                                }

                                    else if (menu_choice == 8) {
                                
    initscr();            // Inicializa a tela
    noecho();             // Não mostra a tecla digitada
    curs_set(0);          // Esconde o cursor

    mvaddch(10, 20, '*'); // Desenha um '*' na linha 10, coluna 20
    refresh();            // Atualiza a tela para mostrar a mudança
    getch();              // Aguarda entrada
    endwin();             // Finaliza o modo ncurses
    
    menu = 1;
    
                                
                            }
    
  }

   return 0;
}
