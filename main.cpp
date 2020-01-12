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
SDL_Color **pictureTab;

int width = 1024;
int height = 680;

int picWidth;
int picHeight;

int palette_counter = 0;
SDL_Color *palette;

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
int partition(int p, int r, int r_max, int r_red, int r_green);
void quicksort(int p, int r, int r_max, int r_red, int r_green);
void mediancut(int a, int b, int glebokosc);
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

    if(option != 1 && option != 2 && option != 3) {
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
    bmp = SDL_LoadBMP(fileName.c_str());
    picWidth = bmp->w;
    picHeight = bmp->h;

    SDL_Color rgb;
    SDL_Color palete[16];
    int index = 0;
    for (int r = 0; r <= 255; r += 255) {
        rgb.r = r;
        for (int g = 0; g <= 255; g += 85) {
            rgb.g = g;
            for(int b = 0; b <= 255; b += 255) {
                rgb.b = b;
                palete[index] = rgb;
                index++;
            }
        }
    }

    int mistakeTab[16];
    int red, green, blue;
    for(int x = 0; x < bmp->w; x++) {
        for(int y = 0; y < bmp->h; y++) {
            rgb = getPixelSurface(x, y, bmp);

            for(int i = 0; i < 16; i++) {

                if(rgb.r > palete[i].r) {
                    red = rgb.r - palete[i].r;
                }
                else {
                    red = palete[i].r - rgb.r;
                }

                if(rgb.g > palete[i].g) {
                    green = rgb.g - palete[i].g;
                }
                else {
                    green = palete[i].g - rgb.g;
                }

                if(rgb.b > palete[i].b) {
                    blue = rgb.b - palete[i].b;
                }
                else {
                    blue = palete[i].b - rgb.b;
                }

                mistakeTab[i] = red + green + blue;
            }

            int minMistakeIndex = 0;
            for(int i = 0; i < 16; i++) {

                if(mistakeTab[minMistakeIndex] > mistakeTab[i])
                    minMistakeIndex = i;
            }

            setPixel(x, y, rgb.r, rgb.g, rgb.b);
            setPixel(x+bmp->w, y, palete[minMistakeIndex].r, palete[minMistakeIndex].g, palete[minMistakeIndex].b);
        }
    }



    SDL_Flip(screen);
}

void convertFromBMPdp(string &fileName)
{
    bmp = SDL_LoadBMP(fileName.c_str());
    picWidth = bmp->w;
    picHeight = bmp->h;

    int amountOfColors = 16;

    SDL_Color rgb;
    palette = new SDL_Color[amountOfColors];
    pictureTab = new SDL_Color*[picWidth];

    for(int i = 0; i < picWidth; i++) {
        pictureTab[i] = new SDL_Color[picHeight];
    }

    for(int i = 0; i < picWidth; i++) {
        for(int j = 0; j < picHeight; j++) {
            pictureTab[i][j] = getPixelSurface(i,j,bmp);
        }
    }

    mediancut(0,(picWidth) * (picHeight),0);

    for(int i = 0; i < amountOfColors; i++) {
        cout << i << ": " << (int)palette[i].r << " " << (int)palette[i].g << " " << (int)palette[i].b << endl;
    }


    int mistakeTab[amountOfColors];
    int red, green, blue;
    for(int x = 0; x < picWidth; x++) {
        for(int y = 0; y < picHeight; y++) {
            rgb = getPixelSurface(x, y, bmp);

            for(int i = 0; i < amountOfColors; i++) {

                if(rgb.r > palette[i].r) {
                    red = rgb.r - (int)palette[i].r;
                }
                else {
                    red = (int)palette[i].r - rgb.r;
                }

                if(rgb.g > palette[i].g) {
                    green = rgb.g - (int)palette[i].g;
                }
                else {
                    green = (int)palette[i].g - rgb.g;
                }

                if(rgb.b > palette[i].b) {
                    blue = rgb.b - (int)palette[i].b;
                }
                else {
                    blue = (int)palette[i].b - rgb.b;
                }

                mistakeTab[i] = red + green + blue;
            }

            int minMistakeIndex = 0;
            for(int i = 0; i < amountOfColors; i++) {
                if(mistakeTab[minMistakeIndex] > mistakeTab[i])
                    minMistakeIndex = i;
            }

            setPixel(x, y, rgb.r, rgb.g, rgb.b);
            setPixel(x+picWidth, y, palette[minMistakeIndex].r, palette[minMistakeIndex].g, palette[minMistakeIndex].b);
        }
    }


    SDL_Flip(screen);
}

int partition(int p, int r, int r_max, int r_red, int r_green) { // dzielimy tablice na dwie czesci, w pierwszej wszystkie liczby sa mniejsze badz rowne x, w drugiej wieksze lub rowne od x
    SDL_Color x;
    x = pictureTab[p%picWidth][p/picWidth];
    int i = p, j = r; // i, j - indeksy w tabeli
    SDL_Color w, u;
    while (true) // petla nieskonczona - wychodzimy z niej tylko przez return j
    {
        w = pictureTab[i%picWidth][i/picWidth];
        u = pictureTab[j%picWidth][j/picWidth];
        if(r_max == r_red) {
            while (u.r > x.r) { j--; u = pictureTab[j%picWidth][j/picWidth]; }
            while (w.r < x.r) { i++; w = pictureTab[i%picWidth][i/picWidth]; }
        }
        else if(r_max == r_green) {
            while (u.g > x.g) { j--; u = pictureTab[j%picWidth][j/picWidth]; }
            while (w.g < x.g) { i++; w = pictureTab[i%picWidth][i/picWidth]; }
        }
        else {
            while (u.b > x.b) { j--; u = pictureTab[j%picWidth][j/picWidth]; }
            while (w.b < x.b) { i++; w = pictureTab[i%picWidth][i/picWidth]; }
        }
        if (i < j) // zamieniamy miejscami gdy i < j
        {
            w = pictureTab[i%picWidth][i/picWidth];
            u = pictureTab[j%picWidth][j/picWidth];
            pictureTab[i%picWidth][i/picWidth] = u;
            pictureTab[j%picWidth][j/picWidth] = w;
            i++;
            j--;
        }
        else // gdy i >= j zwracamy j jako punkt podzialu tablicy
            return j;
    }
}
void quicksort(int p, int r, int r_max, int r_red, int r_green) { // sortowanie szybkie
    int q;
    if (p < r)
    {
        q = partition(p, r, r_max, r_red, r_green); // dzielimy tablice na dwie czesci; q oznacza punkt podzialu
        quicksort(p, q, r_max, r_red, r_green); // wywolujemy rekurencyjnie quicksort dla pierwszej czesci tablicy
        quicksort(q+1, r, r_max, r_red, r_green); // wywolujemy rekurencyjnie quicksort dla drugiej czesci tablicy
    }
}

void mediancut(int a, int b, int glebokosc) {
    if (glebokosc == 5) {
        SDL_Color kolor;
        kolor = pictureTab[(a+(b-a)/2)%picWidth][(a+(b-a)/2)/picWidth];
        palette[palette_counter] = kolor;
        palette_counter++;
    }
    else {
        SDL_Color kolor_min, kolor_max, kolor;
        kolor_max = pictureTab[0][0];
        kolor_min = pictureTab[0][0];
        for(int y = 0; y < height; y++) {
            for(int x = 1; x < picWidth; x++) {
                kolor = getPixelSurface(0, 0, bmp);
                if(kolor.r > kolor_max.r) kolor_max.r = kolor.r;
                if(kolor.g > kolor_max.g) kolor_max.g = kolor.g;
                if(kolor.b > kolor_max.b) kolor_max.b = kolor.b;
                if(kolor.r < kolor_min.r) kolor_min.r = kolor.r;
                if(kolor.g < kolor_min.g) kolor_min.g = kolor.g;
                if(kolor.b < kolor_min.b) kolor_min.b = kolor.b;
            }
        }
        int r_red = kolor_max.r - kolor_min.r;
        int r_green = kolor_max.g - kolor_min.g;
        int r_blue = kolor_max.b - kolor_min.b;
        int r_max = max(r_red, max(r_green, r_blue));
        quicksort(a, b, r_max, r_red, r_green);
        mediancut(a, a+(b-a)/2, glebokosc+1);
        mediancut(a+(b-a)/2+1, b, glebokosc+1);
    }
}

void convertFromBMPgs(string &fileName)
{
    bmp = SDL_LoadBMP(fileName.c_str());


    SDL_Color rgb;
    Uint8 palete[16];
    int grayScale = 0;
    for (int i = 0; i < 16; i++) {
        palete[i] = grayScale;
        grayScale += 17;
    }

    int BW = 0;
    for(int x = 0; x < bmp->w; x++) {
        for(int y = 0; y < bmp->h; y++) {
            rgb = getPixelSurface(x, y, bmp);
            BW = 0.299*rgb.r + 0.587*rgb.g + 0.114*rgb.b;
            int tempIndex = BW / 17;

            setPixel(x, y, rgb.r, rgb.g, rgb.b);
            setPixel(x+bmp->w, y, palete[tempIndex], palete[tempIndex], palete[tempIndex]);
        }
    }




    SDL_Flip(screen);
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
    char* pPosition=(char*)surface->pixels;
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
