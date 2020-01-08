#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>

#endif
#include <SDL/SDL.h>
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;

using namespace std;

SDL_Surface *screen;
SDL_Surface *bmp;
int width = 512;
int height = 340;

string getNameOfFile();
int getOption();
void help();
void showChoices(int option, string &fileName);
bool checkIfFileExists(bool bmp);
bool isNumber(string &option);
int checkChoice(string &option);
void menu2(int conversionOption, string &fileName);
void menu();
void convertFromBMPip(string &fileName);
void convertFromBMPdp(string &fileName);
void convertFromBMPgs(string &fileName);
void convertToBMP(string &fileName);

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel (int x, int y);
SDL_Color getPixelSurface(int x, int y, SDL_Surface *surface);
void ladujBMP(int x, int y, SDL_Surface *surface);

string getNameOfFile()
{
    string fileName;
    cout << "Enter the name of file: ";
    cin.ignore();
    getline(cin, fileName);
    if (fileName[0] == '"') {
        for(int i = 0; i < fileName.size()-2; i++) {
            fileName[i] = fileName[i+1];
        }
        fileName.resize(fileName.size()-2);
    }

    for(int i = fileName.size()-1; i >= 0; i--) {
        if(fileName[i] == '.') {
            fileName.resize(i);
            break;
        }
    }
    cout << fileName << endl;
    return fileName;
}

int getOption()
{
    int option = 0;
    cout << "Choose the option of conversion: " << endl;
    cout << "1. Imposed palette" << endl;
    cout << "2. Dedicated palette" << endl;
    cout << "3. Gray scale" << endl;
    cout << "Choice: ";
    cin >> option;

    if(option != (1 && 2 && 3)) {
        cout << "Incorrect choice. Choose again" << endl;
        getOption();
    }
    return option;
}

void help()
{
    cout << "Program can convert bmp picture to 16 colors picture to reduce the size of file." << endl;
    cout << "There are three conversion options." << endl;
    cout << "1. Imposed palette - algorithm use predefined 16 colors and convert picture." << endl;
    cout << "2. Dedicated palette - algorithm is searching picture to find which colors appear most often\n";
    cout << "and use this colors in conversion." << endl;
    cout << "3. Gray scale - algorithm use 16 degree gray scale in conversion." << endl;
}

void showChoices(int option, string &fileName)
{
    cout << "Actual choices: " << endl;
    cout << "File name: ";
    if(fileName.empty()) {
        cout << "You must type file name first" << endl;
    }
    else
    {
        cout << fileName << endl;
    }
    cout << "Conversion option: ";

    switch (option)
    {
    case 0:
        cout << "You must choose option first" << endl;
        break;
    case 1:
        cout << "Imposed palette" << endl;
        break;
    case 2:
        cout << "Dedicated palette" << endl;
        break;
    case 3:
        cout << "Gray scale" << endl;
        break;
    default:
        cout << "Error. Bad conversionOption value" << endl;
        break;
    }
}

bool checkIfFileExists(bool bmp, string& fileName)
{
    fstream fileExists;
    if(bmp == false)
        fileName = fileName + ".lol";
    else
        fileName = fileName + ".bmp";

    fileExists.open(fileName.c_str());
    if(!fileExists.is_open()) {
        fileExists.close();
        return false;
    }
    fileExists.close();
    return true;
}

bool isNumber(string &option)
{
    for(int i = 0; i < option.size(); i++) {
        if(!isdigit(option[i])) {
            return false;
        }
    }
    return true;
}

int checkChoice(string &option)
{
    int optionInMenu2 = 0;
    while(!isNumber(option)) {
        cout << "Incorrect choice. Choose again." << endl;
        cout << "Choice: ";
        cin >> option;
    }
    optionInMenu2 = atoi(option.c_str());

    if(optionInMenu2 < 1 || optionInMenu2 > 9 || optionInMenu2 == 7 || optionInMenu2 == 8) {
        while(optionInMenu2 < 1 || optionInMenu2 > 9 || optionInMenu2 == 7 || optionInMenu2 == 8) {
            cout << "Incorrect choice. Choose again." << endl;
            cout << "Choice: ";
            cin >> option;

            while(!isNumber(option)) {
                cout << "Incorrect choice. Choose again." << endl;
                cout << "Choice: ";
                cin >> option;
            }

            optionInMenu2 = atoi(option.c_str());
        }
    }
    return optionInMenu2;
}

void menu2(int conversionOption, string &fileName)
{
    string optionTemp;
    int optionInMenu2 = 0;
    cout << endl;
    cout << "1. Help" << endl;
    cout << "2. Enter file name" << endl;
    cout << "3. Pick color palette" << endl;
    cout << "4. Convert picture .bmp to .lol with actual choices" << endl;
    cout << "5. Convert picture .lol to .bmp with actual name" << endl;
    cout << "6. Show actual choices" << endl;
    cout << "9. Exit" << endl;
    cout << "Choice: ";
    cin >> optionTemp;

    optionInMenu2 = checkChoice(optionTemp);

    cout << endl << endl;

    switch(optionInMenu2)
    {
    case 1:
        help();
        menu2(conversionOption, fileName);
        break;
    case 2:
        fileName = getNameOfFile();
        menu2(conversionOption, fileName);
        break;
    case 3:
        conversionOption = getOption();
        menu2(conversionOption, fileName);
        break;
    case 4:
        if(fileName.empty() == true) {
            cout << "You must type a name of file to convert!\n\n";
            fileName = getNameOfFile();
            menu2(conversionOption, fileName);
        }

        if(checkIfFileExists(true, fileName))
        {
            switch(conversionOption)
            {
            case 0:
                cout << "You must select a color palette!\n\n";
                conversionOption = getOption();
                menu2(conversionOption, fileName);
                break;
            case 1:
                convertFromBMPip(fileName);
                break;
            case 2:
                convertFromBMPdp(fileName);
                break;
            case 3:
                convertFromBMPgs(fileName);
                break;
            default:
                cout << "Error. Bad conversionOption value" << endl;
                menu2(conversionOption, fileName);
                break;
            }
        }
        else
        {
            cout << "File \"" << fileName << "\" does not exist!" << endl;
            menu2(conversionOption, fileName);
        }
        break;
    case 5:
        if(fileName.empty() == true) {
            cout << "You must type a name of file to convert!\n\n";
            fileName = getNameOfFile();
            menu2(conversionOption, fileName);
        }

        if(checkIfFileExists(false, fileName))
        {
            convertToBMP(fileName);
        }
        else
        {
            cout << "File \"" << fileName << "\" does not exist!" << endl;
            menu2(conversionOption, fileName);
        }
        break;
    case 6:
        showChoices(conversionOption, fileName);
        menu2(conversionOption, fileName);
        break;
    case 9:
        exit(0);
        break;
    default:
        break;
    }
}

void menu()
{
    cout << "Welcome in pictures converter. You can find informations in help." << endl;
    int conversionOption = 0;
    string fileName;
    menu2(conversionOption, fileName);
}

void convertFromBMPip(string &fileName)
{
    //Load bmp surface
    bmp = SDL_LoadBMP(fileName.c_str());
    ladujBMP(0,0, bmp);
}

void convertFromBMPdp(string &fileName)
{
    //Load bmp surface
    bmp = SDL_LoadBMP(fileName.c_str());
    ladujBMP(0,0, bmp);
}

void convertFromBMPgs(string &fileName)
{
    //Load bmp surface
    bmp = SDL_LoadBMP(fileName.c_str());
    ladujBMP(0,0, bmp);
}

void convertToBMP(string &fileName)
{

}

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
  if ((x>=0) && (x<width) && (y>=0) && (y<height))
  {
    /* Zamieniamy poszczeg�lne sk�adowe koloru na format koloru piksela */
    Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

    /* Pobieramy informacj� ile bajt�w zajmuje jeden piksel */
    int bpp = screen->format->BytesPerPixel;

    /* Obliczamy adres piksela */
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

    /* Ustawiamy warto�� piksela, w zale�no�ci od formatu powierzchni*/
    switch(bpp)
    {
        case 1: //8-bit
            *p = pixel;
            break;

        case 2: //16-bit
            *(Uint16 *)p = pixel;
            break;

        case 3: //24-bit
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32 *)p = pixel;
            break;

    }
         /* ewentualna aktualizacja obrazu (aka double buffering) */
  }
}


SDL_Color getPixel (int x, int y) {
    SDL_Color color ;
    Uint32 col = 0 ;
    if ((x>=0) && (x<width) && (y>=0) && (y<height)) {
        //okre�lamy pozycj�
        char* pPosition=(char*)screen->pixels ;
        //przesuni�cie wzgl�dem y
        pPosition+=(screen->pitch*y) ;
        //przesuni�cie wzgl�dem x
        pPosition+=(screen->format->BytesPerPixel*x);
        //kopiujemy dane piksela
        memcpy(&col, pPosition, screen->format->BytesPerPixel);
        //konwertujemy kolor
        SDL_GetRGB(col, screen->format, &color.r, &color.g, &color.b);
    }
    return ( color ) ;
}

SDL_Color getPixelSurface(int x, int y, SDL_Surface *surface) {
    SDL_Color color ;
    Uint32 col = 0 ;
    //okre�lamy pozycj�
    char* pPosition=(char*)surface->pixels ;
    //przesuni�cie wzgl�dem y
    pPosition+=(surface->pitch*y) ;
    //przesuni�cie wzgl�dem x
    pPosition+=(surface->format->BytesPerPixel*x);
    //kopiujemy dane piksela
    memcpy(&col, pPosition, surface->format->BytesPerPixel);
    //konwertujemy kolor
    SDL_GetRGB(col, surface->format, &color.r, &color.g, &color.b);
    return ( color ) ;
}

void ladujBMP(int x, int y, SDL_Surface *surface)
{
    if (!surface)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
    }
    else
    {
        SDL_Color kolor;
        for (int yy=0; yy<surface->h; yy++) {
			for (int xx=0; xx<surface->w; xx++) {
				kolor = getPixelSurface(xx, yy, surface);
				setPixel(x+xx, y+yy, kolor.r, kolor.g, kolor.b);
			}
        }
        SDL_Flip(screen);
    }

}

int main ( int argc, char** argv )
{
    freopen( "CON", "wt", stdout );
    freopen( "CON", "wt", stderr );

    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        printf("Unable to set video: %s\n", SDL_GetError());
        return 1;
    }
    SDL_WM_SetCaption( "GKiM2019 - Projekt" , NULL );

    SDL_Surface *screen2 = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
        if ( !screen2 )
    {
        printf("Unable to set video: %s\n", SDL_GetError());
        return 1;
    }
    SDL_WM_SetCaption( "GKiM2019 - Projekt" , NULL );

    menu();

    // program main loop
    bool done = false;
    while (!done)
    {
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                done = true;
                break;

                // check for keypresses
            case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE){
                        done = true;
                    }
                    break;
                }
            } // end switch
        } // end of message processing
    } // end main loop

    // free loaded bitmap
    SDL_FreeSurface(bmp);

    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}
