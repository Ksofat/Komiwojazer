#include <bits/stdc++.h> 
#include <random>         
#include <chrono>         

using namespace std;      
using namespace std::chrono;  

#define INF 1e9           
const int MAX = 17;      
int n;                    
int dist[MAX][MAX];       //Dwuwymiarowa tablica do przechowywania miast
int dp[MAX][1 << MAX];    // Dwuwymiarowa dablica do programowania dynamicznego



                                                  // Funkcja do rozwiązania Problemu Komiwojażera (TSP) 

int tsp(int pos, int Visited) {
    if (Visited == (1 << n) - 1) {
        return dist[pos][0]; // Zwraca odleglosc do miasta ) po odwiedzeniu wszystkich
    }

    if (dp[pos][Visited] != -1) {
        return dp[pos][Visited]; // Zwraca wynik po obliczeniu
    }

    int ans = INF;

    // Próbuje odwiedzić miasto
    for (int city = 0; city < n; city++) {
        // Sprawdza czy miasto odwiedzono
        if (!(Visited & (1 << city))) {
            int newAns = dist[pos][city] + tsp(city, Visited | (1 << city)); // Dodaje odległość i wywoływuje rekurencje
            ans = min(ans, newAns); // Aktualizuje odległośc na minimalną
        }
    }

    return dp[pos][Visited] = ans;
}



                                                    // Funkcja do losowania danych do macierzy

void initializeDistMatrix() {
    random_device rd;  
    mt19937 gen(rd());  
    uniform_int_distribution<> dis(1, 100);  

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                dist[i][j] = 0;  // Odległość dla siebie samego 0
            }
            else {
                dist[i][j] = dis(gen);  // Wygeneruj losową odległość między miastami
                dist[j][i] = dist[i][j];  // Ustaw odpowiednią odległość powrotną
            }
        }
    }
}



                                                        // Funkcja do mierzenia czasu 

int measureTime(int startNode, int visited, long long& timeTaken) {
    auto start = high_resolution_clock::now();  // Pobierz aktualny czas jako poczatek
    int result = tsp(startNode, visited);  // Wywołaj funkcję TSP 
    auto stop = high_resolution_clock::now();  // Pobierz aktualny czas jako koniec
    auto duration = duration_cast<microseconds>(stop - start);  // Oblicz czas trwania
    timeTaken = duration.count();  
    return result;  
}



                                                        // Funkcja do zapisywania do pliku

void saveToFile(const string& filename, long long averageTime) {
    ofstream file(filename, ios::app);  // Otwórz plik
    if (file.is_open()) {
        file << averageTime << endl;  // Zapisz czas
        file.close();  // Zamknij plik
    }
    else {
        cout << "Nie można otworzyć pliku: " << filename << endl;  
    }
}



                                                        // Funkcja do wgrywania z pliku

void readFromFile(const string& filename) {
    ifstream file(filename);  // Otwórz plik 
    if (file.is_open()) {
        file >> n;  // Wczytaj ilość miast z pliku
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                file >> dist[i][j];  // Wczytaj macierz z pliku
            }
        }
        file.close();  // Zamknij plik
    }
    else {
        cout << "Nie można otworzyć pliku: " << filename << endl; 
        exit(1); 
    }
}



                                                    // Funkcja do wyboru opcji

void selectOption() {
    cout << "Wybierz opcje:" << endl << "1 - Uruchom program" << endl << "2 - Przetestuj działanie (Wczytaj z pliku)" << endl;
    int option;
    cin >> option;  

    switch (option) {
    case 1: {
        cout << "Podaj ilość miejscowości: ";  
        cin >> n;

        for (int i = 0; i < MAX; i++) {  
            for (int j = 0; j < (1 << MAX); j++) {
                dp[i][j] = -1;
            }
        }

        long long total_time = 0;

        for (int proba = 0; proba < 100; proba++) { // 100 prob
            initializeDistMatrix();

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < (1 << n); j++) {
                    dp[i][j] = -1;
                }
            }

            long long timeTaken = 0;
            int minCost = measureTime(0, 1, timeTaken);
            total_time += timeTaken;
            cout << "Minimalna odległość: " << minCost << endl;
        }

        long long average_time = total_time / 100;
        cout << "Średni czas: " << average_time << " mikrosekundy" << endl;
        saveToFile("wyniki.txt", average_time);
        break;
    }
    case 2: {
        for (int i = 0; i < MAX; i++) {
            for (int j = 0; j < (1 << MAX); j++) {
                dp[i][j] = -1;
            }
        }

        long long total_time = 0;
        readFromFile("dane.txt");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < (1 << n); j++) {
                dp[i][j] = -1;
            }
        }

        long long timeTaken = 0;
        int minCost = measureTime(0, 1, timeTaken);
        total_time += timeTaken;
        cout << "Minimalna odległość: " << minCost << endl;

        long long average_time = total_time / 100;
        cout << "Średni czas: " << average_time << " mikrosekundy" << endl;
        saveToFile("wyniki.txt", average_time);
        break;
    }
    default: {
        cout << "Niepoprawna opcja" << endl;  
        break;
    }
    }
}




int main() {
    selectOption();
    return 0;
}
