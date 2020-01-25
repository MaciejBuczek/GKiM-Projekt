#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>

#endif
#include <SDL/SDL.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

vector<vector<SDL_Color> > sectors;
vector <SDL_Color> palette;
int amountOfColors = 16;

SDL_Surface *screen;
SDL_Surface *bmp;
enum Type{r,g,b};

Uint16 width;
Uint16 height;

string getNameOfFile();
int getOption();
void help();
void showChoices(int option, string &fileName);
bool checkIfFileExists(bool bmp);
bool isNumber(string &option);
int checkChoice(string &option);
void medianCut();

void menu2(int conversionOption, string &fileName);
void menu();

void convertFromBMPip(string &fileName, bool dithering);
void convertFromBMPdp(string &fileName, bool dithering);
void convertFromBMPgs(string &fileName, bool dithering);
void convertToBMP(string &fileName);

void ditheringRGBVisualize(vector <unsigned char> &bmpPixels);
void ditheringGrayScaleVisualize(vector <unsigned char> &bmpPixels);
void RGBVisualize(vector <unsigned char> &bmpPixels);
void GrayScaleVisualize(vector <unsigned char> &bmpPixels);

void mergePixels(int x, int y, int index, vector <unsigned char> &bmpPixels);

void compareCompressionSizes(vector <unsigned char> a, string &fileName, char paletteType);
vector <char> RLECompress(vector <char> a, int size);
vector <char> ByteRunCompress(vector <char> a, int length);
vector <char> ByteRunDecompress(vector <char> a, int length);
vector <char> RLEDecompress(vector <char> a, int size);
void saveFile(vector <char> data, string &fileName, char compressionType, char paletteType);

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
void setBMPPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel (int x, int y);
SDL_Color getPixelSurface(int x, int y, SDL_Surface *surface);
void ladujBMP(int x, int y, SDL_Surface *surface);

void delExtention(string &fileName) {
    for(int i = fileName.size()-1; i >= 0; i--) {
        if(fileName[i] == '.') {
            fileName.resize(i);
            break;
        }
    }
}

string getNameOfFile()
{
    string fileName;
    cout << "Enter the name of file or drop the file here: ";
    cin.ignore();
    getline(cin, fileName);
    if (fileName[0] == '"') {
        for(int i = 0; i < fileName.size()-2; i++) {
            fileName[i] = fileName[i+1];
        }
        fileName.resize(fileName.size()-2);
    }
    delExtention(fileName);

    cout << fileName << endl;
    return fileName;
}

int getOption()
{
    int option = 0;
    cout << "Choose the option of conversion: " << endl;
    cout << "1. Standard imposed palette" << endl;
    cout << "2. Imposed palette with dithering" << endl;
    cout << "3. Standard dedicated palette" << endl;
    cout << "4. Dedicated palette with dithering" << endl;
    cout << "5. Standard gray scale" << endl;
    cout << "6. Gray scale with dithering" << endl;
    cout << "Choice: ";
    cin >> option;

    if(option != 1 && option != 2 && option != 3 && option != 4 && option != 5 && option != 6) {
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
        cout << "Standard imposed palette" << endl;
        break;
    case 2:
        cout << "Imposed palette with dithering" << endl;
        break;
    case 3:
        cout << "Standard dedicated palette" << endl;
        break;
    case 4:
        cout << "Dedicated palette with dithering" << endl;
        break;
    case 5:
        cout << "Standard gray scale" << endl;
        break;
    case 6:
        cout << "Gray scale with dithering" << endl;
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
        fileName = fileName + ".yee";
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

void menu2(int conversionOption, string &fileName) {

    palette.clear();
    sectors.clear();
    width = 0;
    height = 0;

    string optionTemp;
    int optionInMenu2 = 0;
    cout << endl;
    cout << "1. Help" << endl;
    cout << "2. Enter file name" << endl;
    cout << "3. Pick color palette" << endl;
    cout << "4. Convert picture .bmp to .yee with actual choices" << endl;
    cout << "5. Convert picture .yee to .bmp with actual name" << endl;
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
                convertFromBMPip(fileName, false);
                break;
            case 2:
                convertFromBMPip(fileName, true);
                break;
            case 3:
                convertFromBMPdp(fileName, false);
                break;
            case 4:
                convertFromBMPdp(fileName, true);
                break;
            case 5:
                convertFromBMPgs(fileName, false);
                break;
            case 6:
                convertFromBMPgs(fileName, true);
                break;
            default:
                cout << "Error. Bad conversionOption value" << endl;
                menu2(conversionOption, fileName);
                break;
            }
        }
        else {
            cout << "File \"" << fileName << "\" does not exist!" << endl;
            delExtention(fileName);
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
            delExtention(fileName);
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

int nearestColor(SDL_Color rgb) {
    int colorIndex = 0;
    int *mistakeTab = new int[amountOfColors];

    for(int i = 0; i < amountOfColors; i++) {
        mistakeTab[i] = abs(palette[i].r - rgb.r) + abs(palette[i].g - rgb.g) + abs(palette[i].b - rgb.b);
    }

    for(int i = 0; i < amountOfColors; i++) {
        if(mistakeTab[colorIndex] > mistakeTab[i])
            colorIndex = i;
    }

    return colorIndex;
}

void convertFromBMPip(string &fileName, bool dithering) {

    bmp = SDL_LoadBMP(fileName.c_str());

    width = bmp->w;
    height = bmp->h;

    screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        printf("Unable to set video: %s\n", SDL_GetError());
        return;
    }
    SDL_WM_SetCaption( "GKiM2019 - Projekt" , NULL );

    SDL_Color rgb;

    vector <unsigned char> bmpPixels;

    int index = 0;
    for (int r = 0; r <= 255; r += 255) {
        rgb.r = r;
        for (int g = 0; g <= 255; g += 85) {
            rgb.g = g;
            for(int b = 0; b <= 255; b += 255){
                rgb.b = b;
                palette.push_back(rgb);
                index++;
            }
        }
    }

    if(dithering) {
        ditheringRGBVisualize(bmpPixels);
    }
    else {
        RGBVisualize(bmpPixels);
    }

    delExtention(fileName);
    compareCompressionSizes(bmpPixels, fileName, 0);
}

void mergePixels(int x, int y, int index, vector <unsigned char> &bmpPixels){
    unsigned char tempChar;
    if(x%2 == 1) {
        tempChar = bmpPixels[bmpPixels.size()-1];
        bmpPixels.pop_back();
        tempChar += index;
        bmpPixels.push_back(tempChar);
    }
    else {
        tempChar = index;
        tempChar <<= 4;
        bmpPixels.push_back(tempChar);
    }
}

void ditheringRGBVisualize(vector <unsigned char> &bmpPixels){
    SDL_Color RGB;
    SDL_Color newRGB;

    float ErrR[(bmp->w+2)][bmp->h+1];
    float ErrG[(bmp->w+2)][bmp->h+1];
    float ErrB[(bmp->w+2)][bmp->h+1];

    memset(ErrR, 0, sizeof(ErrR));
    memset(ErrG, 0, sizeof(ErrG));
    memset(ErrB, 0, sizeof(ErrB));

    int errR, errG, errB, R, G, B, index;
    int shift=1;

    for(int y=0; y<bmp->h; y++)
    {
        for(int x=0; x<bmp->w; x++)
        {
            RGB =getPixelSurface(x,y,bmp);

            R=RGB.r+ErrR[x+shift][y];
            G=RGB.g+ErrG[x+shift][y];
            B=RGB.b+ErrB[x+shift][y];
            if(R>255)
                R=255;
            if(R<0)
                R=0;
            if(G>255)
                G=255;
            if(G<0)
                G=0;
            if(B>255)
                B=255;
            if(B<0)
                B=0;
            RGB={R,G,B};

            index = nearestColor(RGB);
            errR=RGB.r-palette[index].r;
            if (errR>255)
                errR=255;
            errG=RGB.g-palette[index].g;
             if (errG>255)
                errG=255;
            errB=RGB.b-palette[index].b;
             if (errB>255)
                errB=255;
            newRGB=palette[index];

            mergePixels(x, y, index, bmpPixels);

            ErrR[x+shift+1][y  ]+=(errR*7.0/16.0);
            ErrR[x+shift+1][y+1]+=(errR*1.0/16.0);
            ErrR[x+shift  ][y+1]+=(errR*5.0/16.0);
            ErrR[x+shift-1][y+1]+=(errR*3.0/16.0);

            ErrG[x+shift+1][y  ]+=(errG*7.0/16.0);
            ErrG[x+shift+1][y+1]+=(errG*1.0/16.0);
            ErrG[x+shift  ][y+1]+=(errG*5.0/16.0);
            ErrG[x+shift-1][y+1]+=(errG*3.0/16.0);

            ErrB[x+shift+1][y  ]+=(errB*7.0/16.0);
            ErrB[x+shift+1][y+1]+=(errB*1.0/16.0);
            ErrB[x+shift  ][y+1]+=(errB*5.0/16.0);
            ErrB[x+shift-1][y+1]+=(errB*3.0/16.0);

            setPixel(x,y,newRGB.r, newRGB.g, newRGB.b);
        }
    }
    SDL_Flip(screen);
}

void RGBVisualize(vector <unsigned char> &bmpPixels){
    SDL_Color RGB;

    int errR, errG, errB, R, G, B, index;
    int shift=1;

    for(int y=0; y<bmp->h; y++)
    {
        for(int x=0; x<bmp->w; x++)
        {
            RGB = getPixelSurface(x,y,bmp);

            index = nearestColor(RGB);

            mergePixels(x, y, index, bmpPixels);

            setPixel(x, y, palette[index].r, palette[index].g, palette[index].b);
        }
    }
    SDL_Flip(screen);
}

void ditheringGrayScaleVisualize(vector <unsigned char> &bmpPixels){
    SDL_Color RGB;
    SDL_Color newRGB;

    float ErrGS[(bmp->w+2)][bmp->h+1];
    memset(ErrGS, 0, sizeof(ErrGS));
    int errGS, GS, index;
    int shift=1;

    for(int y=0; y<bmp->h; y++)
    {
        for(int x=0; x<bmp->w; x++)
        {
            RGB = getPixelSurface(x,y,bmp);

            GS = 0.299*RGB.r + 0.587*RGB.g + 0.114*RGB.b;
            GS+=ErrGS[x+shift][y];
            if(GS>255)
                GS=255;
            if(GS<0)
                GS=0;

            index=GS/17;
            errGS=GS-palette[index].r;
            if (errGS>255)
                errGS=255;

            newRGB=palette[index];

            mergePixels(x, y, index, bmpPixels);

            ErrGS[x+shift+1][y  ]+=(errGS*7.0/16.0);
            ErrGS[x+shift+1][y+1]+=(errGS*1.0/16.0);
            ErrGS[x+shift  ][y+1]+=(errGS*5.0/16.0);
            ErrGS[x+shift-1][y+1]+=(errGS*3.0/16.0);

            setPixel(x,y,newRGB.r, newRGB.r, newRGB.r);
        }
    }
    SDL_Flip(screen);
}

void grayScaleVisualize(vector <unsigned char> &bmpPixels) {

    SDL_Color RGB;
    int GS, index;

    for(int y=0; y<bmp->h; y++)
    {
        for(int x=0; x<bmp->w; x++)
        {
            RGB = getPixelSurface(x,y,bmp);

            GS = 0.299*RGB.r + 0.587*RGB.g + 0.114*RGB.b;
            index=GS/17;

            mergePixels(x, y, index, bmpPixels);

            setPixel(x,y,palette[index].r, palette[index].r, palette[index].r);
        }
    }
    SDL_Flip(screen);
}

compareR(const SDL_Color lhs, const SDL_Color rhs) {
    return lhs.r>rhs.r;
}

compareG(const SDL_Color lhs, const SDL_Color rhs) {
    return lhs.g>rhs.g;
}

compareB(const SDL_Color lhs, const SDL_Color rhs) {
    return lhs.b>rhs.b;
}

Type getType(vector<SDL_Color> sector) {
    int rangeR, rangeG, rangeB;
    int minR=255, maxR=0, minG=255, maxG=0, minB=255, maxB=0;
    SDL_Color rgb;
    for(int i=0; i<sector.size(); i++){
        rgb=sector[i];
        if(rgb.r<minR)
            minR=rgb.r;
        if(rgb.r>maxR)
            maxR=rgb.r;
        if(rgb.g<minG)
            minG=rgb.g;
        if(rgb.g>maxG)
            maxG=rgb.g;
        if(rgb.b<minB)
            minB=rgb.b;
        if(rgb.b>maxB)
            maxB=rgb.b;
    }
    rangeR=maxR-minR;
    rangeG=maxG-minG;
    rangeB=maxB-minB;
    if(rangeR==max(rangeR,max(rangeG,rangeB)))
        return r;
    else if(rangeG==max(rangeR,max(rangeG,rangeB)))
        return g;
    else
        return b;
}

void sortSector(vector<SDL_Color> &sector, Type t) {
    switch(t){
    case r:
        sort(sector.begin(), sector.end(), compareR);
        break;
    case g:
        sort(sector.begin(), sector.end(), compareG);
        break;
    case b:
        sort(sector.begin(), sector.end(), compareB);
        break;
    }
}

void initializeMediaCut(){
    SDL_Color rgb;
    Type t;
    vector<SDL_Color> sector;
    vector<SDL_Color> sector2;
    int median;
    for(int y=0; y<bmp->h; y++){
        for(int x=0; x<bmp->w; x++){
            rgb=getPixelSurface(x,y,bmp);
            sector.push_back(rgb);
        }
    }
    t=getType(sector);
    sortSector(sector, t);

    median = sector.size()/2;
    for(int i=sector.size()-1; i>=median; i--) {
        sector2.push_back(sector[i]);
        sector.pop_back();
    }
    sectors.push_back(sector);
    sectors.push_back(sector2);
}

vector<vector<SDL_Color> > divideSector(vector<SDL_Color> sector) {
    int median;
    Type t=getType(sector);
    vector<SDL_Color> sector2;
    vector<vector<SDL_Color> > temp;
    sortSector(sector, t);
    median = sector.size()/2;
    for(int i=sector.size()-1; i>=median; i--){
        sector2.push_back(sector[i]);
        sector.pop_back();
    }
    temp.push_back(sector);
    temp.push_back(sector2);
    return temp;
}

SDL_Color getAverageColor(vector<SDL_Color> sector){
    unsigned int r=0,g=0,b=0;
    SDL_Color color;
    for(int i=0; i<sector.size(); i++){
        r+=sector[i].r;
        g+=sector[i].g;
        b+=sector[i].b;
    }
    color.r=(double)r/sector.size();
    color.g=(double)g/sector.size();
    color.b=(double)b/sector.size();
    return color;
}

void medianCut(){
    vector<vector<SDL_Color> > temp;
    vector<vector<SDL_Color> > holder;
    initializeMediaCut();
    while(sectors.size()!=amountOfColors){
        for(int i=sectors.size()-1; i>=0; i--){
            temp=divideSector(sectors[i]);
            sectors.pop_back();
            for(int j=0; j<temp.size(); j++){
                holder.push_back(temp[j]);
            }
        }
        for(int i=0; i<holder.size(); i++){
            sectors.push_back(holder[i]);
        }
        holder.clear();
    }
    for(int i=0; i<sectors.size(); i++){
        palette.push_back(getAverageColor(sectors[i]));
    }
}

void convertFromBMPdp(string &fileName, bool dithering) {

    bmp = SDL_LoadBMP(fileName.c_str());
    width = bmp->w;
    height = bmp->h;

    screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        printf("Unable to set video: %s\n", SDL_GetError());
        return;
    }
    SDL_WM_SetCaption( "GKiM2019 - Projekt" , NULL );


    medianCut();

    vector <unsigned char> bmpPixels;

    if(dithering) {
        ditheringRGBVisualize(bmpPixels);
    }
    else {
        RGBVisualize(bmpPixels);
    }

    delExtention(fileName);
    compareCompressionSizes(bmpPixels, fileName, 1);
}

void convertFromBMPgs(string &fileName, bool dithering) {

    bmp = SDL_LoadBMP(fileName.c_str());
    width = bmp->w;
    height = bmp->h;

    screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        printf("Unable to set video: %s\n", SDL_GetError());
        return;
    }
    SDL_WM_SetCaption( "GKiM2019 - Projekt" , NULL );

    vector <unsigned char> bmpPixels;

    SDL_Color rgb;
    int grayScale = 0;
    for (int i = 0; i < 16; i++) {
        rgb = {.r = grayScale, .g = grayScale, .b = grayScale};
        palette.push_back(rgb);
        grayScale += 17;
    }

    if(dithering) {
        ditheringGrayScaleVisualize(bmpPixels);
    }
    else {
        grayScaleVisualize(bmpPixels);
    }
    delExtention(fileName);
    compareCompressionSizes(bmpPixels, fileName, 2);
}

vector <char> RLECompress(vector <char> a, int size) {
    vector <char> temp;
    int pointer = 0, counter = 0;

    while (pointer != size - 1) {
        if (a[pointer] != a[pointer + 1]) {
            if (counter == 0) {
                temp.push_back(a[pointer]);
            }
            else {
                temp.push_back(a[pointer]);
                temp.push_back(a[pointer]);
                temp.push_back(counter - 1);
                counter = 0;
            }
        }
        else {
            if (counter == 255) {
                temp.push_back(a[pointer]);
                temp.push_back(counter);
                counter = 0;
                continue;
            }
            else {
                counter++;
            }
        }
        pointer++;
    }
    if (a[pointer - 1] != a[size - 1]) {
        temp.push_back(a[pointer]);
    }
    else {
        temp.push_back(a[pointer]);
        temp.push_back(a[pointer]);
        temp.push_back(counter - 1);
    }

    return temp;
}

vector <char> ByteRunCompress(vector <char> a, int length) {
    vector<char> temp;
    int i = 0;
    while (i < length) {
        if (i < length - 1 && a[i] == a[i + 1]) {
            int j = 0;
            while (i + j < length - 1 && a[i + j] == a[i + j + 1] && j < 127) {
                j++;
            }
            temp.push_back(-j);
            temp.push_back(a[i + j]);
            i += j + 1;
        }
        else {
            int j = 0;
            while (i + j < length - 1 && a[i + j] != a[j + i + 1] && j < 128) {
                j++;
            }
            if (i + j == length - 1 && j < 128) {
                j++;
            }
            temp.push_back(j - 1);
            for (int k = 0; k < j; k++) {
                temp.push_back(a[i + k]);
            }
            i += j;
        }
    }
    return temp;
}

vector <char> ByteRunDecompress(vector <char> a, int length) {
    int i = 0;
    vector<char> temp;
    while (i < length) {
        if (a[i] == -128) {
            i++;
        }
        else if (a[i] < 0) {
            for (int j = 0; j < -(a[i] - 1); j++) {
                temp.push_back(a[i + 1]);
            }
            i += 2;
        }
        else {
            for (int j = 0; j < (a[i] + 1); j++) {
                temp.push_back(a[i + 1 + j]);
            }
            i += a[i] + 2;
        }
    }
    return temp;
}

vector <char> RLEDecompress(vector <char> a, int size) {
    vector <char> temp;
    int pointer = 0, counter = 0;

    while (pointer < size - 1) {
        if (a[pointer] != a[pointer + 1]) {
            temp.push_back(a[pointer]);
        }
        else {
            temp.push_back(a[pointer]);
            temp.push_back(a[pointer]);
            counter = a[pointer + 2];
            for(int i = 0; i < counter; i++)
                temp.push_back(a[pointer]);
            pointer += 2;
            counter = 0;
        }
        pointer++;
    }
    if (a[pointer - 1] != a[size - 1]) {
        temp.push_back(a[pointer]);
    }

    return temp;
}

void compareCompressionSizes(vector <unsigned char> a, string &fileName, char paletteType) {

    vector <char> RLE;
    vector <char> ByteRun;
    vector <char> aNew;
    for(int i = 0; i < a.size(); i++) {
        aNew.push_back((char)a[i]);
    }
    RLE = RLECompress(aNew, aNew.size());
    ByteRun = ByteRunCompress(aNew, aNew.size());

    if(aNew.size() < min(RLE.size(), ByteRun.size())) {
        saveFile(aNew, fileName, 0, paletteType);
    }
    else if (RLE.size() < ByteRun.size()) {
        saveFile(RLE, fileName, 1, paletteType);
    }
    else {
        saveFile(ByteRun, fileName, 2, paletteType);
    }
}

void saveFile(vector <char> data, string &fileName, char compressionType, char paletteType) {

    string entireFileName = fileName + ".yee";
    ofstream zapis(entireFileName.c_str(), ios::binary);

    char ID[] = "YE";
    width = bmp->w;
    height = bmp->h;


    zapis.write((char *)&ID, sizeof(char)*2);
    zapis.write((char *)&width, sizeof(Uint16));
    zapis.write((char *)&height, sizeof(Uint16));
    zapis.write((char *)&compressionType, sizeof(char));

    if((int)paletteType == 0) {
        char temp = 32;
        zapis.write((char *)&temp, sizeof(char));
    }
    else if((int)paletteType == 1) {
        char temp = (char)palette.size();
        zapis.write((char *)&temp, sizeof(char));

        for(int i = 0; i < palette.size(); i++) {
            zapis.write((char *)&palette[i].r, sizeof(char));
            zapis.write((char *)&palette[i].g, sizeof(char));
            zapis.write((char *)&palette[i].b, sizeof(char));
        }
    }
    else if((int)paletteType == 2) {
        char temp = 64;
        zapis.write((char *)&temp, sizeof(char));
    }


    for(int i = 0; i < data.size(); i++) {
        zapis.write((char *)&data[i], sizeof(char));
    }

    zapis.close();

    fileName.clear();
    SDL_FreeSurface(bmp);

    cout << "Press SPACE to go back to menu or ESC to exit (in active GKiM2019-Projekt window)" << endl;
}

void convertToBMP(string &fileName) {
    ifstream odczyt(fileName.c_str(), ios::binary);

    char ID[] = "  ";
    char compressionType;
    char paletteType;

    vector <char> data;

    odczyt.read((char *)&ID, sizeof(char)*2);

    if(ID[0] != 'Y' || ID[1] != 'E') {
        cout << "Bad .yee id" << endl;
        exit(0);
    }

    odczyt.read((char *)&width, sizeof(Uint16));
    odczyt.read((char *)&height, sizeof(Uint16));
    odczyt.read((char *)&compressionType, sizeof(char));
    odczyt.read((char *)&paletteType, sizeof(char));

    if(paletteType == 32) {
        SDL_Color rgb;
        palette.clear();
        int index = 0;
        for (int r = 0; r <= 255; r += 255) {
            rgb.r = r;
            for (int g = 0; g <= 255; g += 85) {
                rgb.g = g;
                for(int b = 0; b <= 255; b += 255){
                    rgb.b = b;
                    palette.push_back(rgb);
                    index++;
                }
            }
        }
    }
    else if(paletteType == 64) {
        palette.clear();
        SDL_Color rgb;
        int grayScale = 0;
        for (int i = 0; i < 16; i++) {
            rgb = {.r = grayScale, .g = grayScale, .b = grayScale};
            palette.push_back(rgb);
            grayScale += 17;
        }
    }
    else if(paletteType > 0 && paletteType <= 16) {
        palette.clear();
        char r, g, b;
        for(int i = 0; i < paletteType; i++) {
            odczyt.read((char *)&r, sizeof(char));
            odczyt.read((char *)&g, sizeof(char));
            odczyt.read((char *)&b, sizeof(char));

            palette.push_back({.r = r, .g = g, .b = b});
        }
    }
    else {
        cout << "Read error" << endl;
        exit(0);
    }

    for(int i = 0; i < (width/2)*height; i++) {
        char temp;
        odczyt.read((char *)&temp, sizeof(char));

        data.push_back(temp);
    }
    odczyt.close();

    vector <char> clearData;
    if(compressionType == 0) {
        clearData = data;
    }
    else if(compressionType == 1) {
        clearData = RLEDecompress(data, data.size());
    }
    else if(compressionType == 2) {
        clearData = ByteRunDecompress(data, data.size());
    }
    else {
        cout << "Write error" << endl;
        exit(0);
    }

    data.clear();
    unsigned char temp, temp1, temp2;
    int index = 0;
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width/2; x++) {
            temp = (unsigned int)clearData[index];
            temp1 = temp & 0b11110000;
            temp1 >>= 4;
            temp2 = temp & 0b00001111;
            data.push_back(temp1);
            data.push_back(temp2);

            index++;
        }
    }

    bmp = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

    screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        printf("Unable to set video: %s\n", SDL_GetError());
        return;
    }
    SDL_WM_SetCaption( "GKiM2019 - Projekt" , NULL );

    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            temp = data[x+y*width];
            setPixel(x, y, palette[(int)temp].r, palette[(int)temp].g, palette[(int)temp].b);
            setBMPPixel(x, y, palette[(int)temp].r, palette[(int)temp].g, palette[(int)temp].b);
        }
    }

    delExtention(fileName);
    fileName += "2.bmp";

    int err = SDL_SaveBMP(bmp, fileName.c_str());

    if(err != 0) {
        cout << "Cannot save " << fileName << endl;
        exit(0);
    }

    SDL_Flip(screen);

    fileName.clear();
    SDL_FreeSurface(bmp);

    cout << "Press SPACE to go back to menu or ESC to exit (in active GKiM2019-Projekt window)" << endl;
}

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B) {
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
  }
}

void setBMPPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B) {
  if ((x>=0) && (x<width) && (y>=0) && (y<height))
  {
    /* Zamieniamy poszczeg�lne sk�adowe koloru na format koloru piksela */
    Uint32 pixel = SDL_MapRGB(bmp->format, R, G, B);

    /* Pobieramy informacj� ile bajt�w zajmuje jeden piksel */
    int bpp = bmp->format->BytesPerPixel;

    /* Obliczamy adres piksela */
    Uint8 *p = (Uint8 *)bmp->pixels + y * bmp->pitch + x * bpp;

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

void ladujBMP(int x, int y, SDL_Surface *surface) {
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

int main ( int argc, char** argv ) {

    freopen( "CON", "wt", stdout );
    freopen( "CON", "wt", stderr );

    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    atexit(SDL_Quit);

    string fileName;
    menu();

    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                done = true;
                break;

            case SDL_KEYDOWN:
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE){
                        done = true;
                    }
                    if (event.key.keysym.sym == SDLK_SPACE){
                        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
                        SDL_Flip(screen);

                        menu2(0, fileName);
                    }
                    break;
                }
            }
        }
    }
    printf("Exited cleanly\n");
    return 0;
}
