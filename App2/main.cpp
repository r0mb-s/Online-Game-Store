#include "../Shared/Data/clase.h"
#include "../Shared/Data/functii.h"
#include <cstring>
#include <set>

using namespace std;

void modifica_joc(char **);
void vizualizare_catalog();
void cumpara_joc(char **);
void instalare_joc(char **);
void dezinstalare_joc(char **);
void cumpara_bundle(char **);

int main(int argc, char **argv)
{
    if (argc == 6)
    {
        if ((string)argv[1] == "modify_game")
        {
            modifica_joc(argv);
        }
        else
        {
            cout << "Comanda gresita!";
        }
    }
    if (argc == 2)
    {
        if ((string)argv[1] == "show_catalog")
        {
            vizualizare_catalog();
        }
        else
        {
            cout << "Comanda gresita!";
        }
    }
    if (argc == 3)
    {
        if ((string)argv[1] == "buy_game")
        {
            cumpara_joc(argv);
        }
        else if ((string)argv[1] == "install_game")
        {
            instalare_joc(argv);
        }
        else if ((string)argv[1] == "uninstall_game")
        {
            dezinstalare_joc(argv);
        }
        else if ((string)argv[1] == "buy_bundle")
        {
            cumpara_bundle(argv);
        }
        else
        {
            cout << "Comanda gresita!";
        }
    }

    return 0;
}

void vizualizare_catalog()
{
    vector<Game> jocuri;
    vector<Bundle> bundleuri;
    ifstream finGames("../Shared/Files/games.txt");
    ifstream finBundles("../Shared/Files/bundles.txt");
    string reader;
    set<string> categorii;
    if (finGames.is_open())
    {

        while (getline(finGames, reader))
        {

            jocuri.push_back(stringToGame(reader));
            categorii.insert(stringToGame(reader).getCategory());
        }
        finGames.close();
    }
    if (finBundles.is_open())
    {
        while (getline(finBundles, reader))
        {

            bundleuri.push_back(stringToBundle(reader));
        }
        finBundles.close();
    }
    cout << "Jocuri:" << endl
         << endl;
    for (auto i : categorii)
    {
        cout << i << ":" << endl
             << endl;
        for (auto j : jocuri)
        {
            if (j.getCategory() == i)
            {
                cout << "Nume: " << j.getTitle() << endl;
                cout << "Categorie: " << j.getCategory() << endl;
                cout << "Pret: " << j.getPrice() << "$" << endl;
                cout << "Dimensiune: " << j.getSize() << "GB" << endl;
            }
        }
        cout << endl;
    }
    cout << endl;
    cout << "Bundleuri:" << endl
         << endl;
    for (auto i : bundleuri)
    {
        cout << "Titlu: " << i.getTitle() << endl;
        cout << "Discount: " << i.getDiscount() << "%" << endl;
        double discount = i.getDiscount();
        cout << "Pret: " << i.getPrice() << "$" << endl;
        vector<Game> g = i.getGames();
        cout << endl
             << "Jocuri: " << endl;
        for (auto j : g)
        {
            cout << "Nume: " << j.getTitle() << endl;
            cout << "Categorie: " << j.getCategory() << endl;
            cout << "Pret: " << j.getPrice() << "$" << endl;
            cout << "Dimensiune: " << j.getSize() << "GB" << endl
                 << endl;
        }
        cout << endl;
    }
}

void modifica_joc(char **argv)
{
    string game = findGameOrBundleInFile((string)argv[2], "games");
    if (game != "-")
    {
        Game joc = stringToGame(game);
        joc.setCategory(argv[3]);
        joc.setSize(atof(argv[4]));
        joc.setPrice(atof(argv[5]));
        ifstream fin("../Shared/Files/bundles.txt");
        vector<Bundle> bundleuri;
        if (fin.is_open())
        {
            string reader;

            while (getline(fin, reader))
            {
                bundleuri.push_back(stringToBundle(reader));
            }
            fin.close();
        }

        for (auto &i : bundleuri)
        {
            vector<Game> jocuri = i.getGames();
            for (auto &j : jocuri)
            {
                if (j.getTitle() == argv[2])
                {
                    j = joc;
                }
            }

            i.setGames(jocuri);
        }

        deleteGameOrBundleFromFile(argv[2], "games");
        writeGameToFile(joc);
        ofstream fout("../Shared/Files/bundles.txt");
        if (fout.is_open())
        {
            for (auto i : bundleuri)
            {
                fout << bundleToString(i) << endl;
            }
            fout.close();
            cout << "Joc modificat cu succes!";
        }
    }
    else
    {
        cout << "Jocul nu exista";
    }
}

void cumpara_joc(char **argv)
{
    string find = findGameOrBundleInFile((string)argv[2], "games");
    if (find != "-")
    {
        Game joc = stringToGame(find);
        if (findGameOrBundleInFile((string)argv[2], "bought-games") == "-")
        {
            ofstream fout("../Shared/Files/bought-games.txt", std::ios::app);
            if (fout.is_open())
            {
                fout << find << endl;
                fout.close();
            }
        }
        ifstream fin("../Shared/Files/consola.txt");
        if (fin.is_open())
        {
            string reader;
            getline(fin, reader);
            Console consola = stringToConsole(reader);
            vector<Game> jocuriCumparate = consola.getGames();
            bool gasit = false;

            for (auto i : jocuriCumparate)
            {
                if (i.getTitle() == joc.getTitle())
                {
                    gasit = true;
                }
            }
            if (gasit == false)
            {
                jocuriCumparate.push_back(joc);
                consola.setGames(jocuriCumparate);
                fin.close();
                ofstream fout("../Shared/Files/consola.txt");
                if (fout.is_open())
                {
                    fout << consoleToString(consola);
                    fout.close();
                    cout << "Joc cumparat cu succes!";
                }
            }
            else
            {
                cout << "Detii deja acest joc!";
            }
        }
    }
    else
    {
        cout << "Jocul nu exista!";
    }
}

void instalare_joc(char **argv)
{
    ifstream fin("../Shared/Files/consola.txt");
    Console consola;
    if (fin.is_open())
    {
        string reader;
        getline(fin, reader);
        consola = stringToConsole(reader);
        fin.close();
    }
    vector<Game> jocuriCumparate = consola.getGames(), jocuriInstalate = consola.getGamesInstalled();
    bool dejainstalat = false;
    bool cumparat = false;
    Game joc;
    for (auto i : jocuriCumparate)
    {
        if (i.getTitle() == argv[2])
        {
            cumparat = true;
            joc = i;
            break;
        }
    }
    if (cumparat == false)
    {
        return;
    }
    for (auto i : jocuriInstalate)
    {
        if (i.getTitle() == argv[2])
        {
            dejainstalat = true;
            break;
        }
    }
    if (dejainstalat == true)
    {
        cout << "Jocul este deja instalat!";
        return;
    }
    if (consola.getStorageLeft() - joc.getSize() >= 0)
    {
        jocuriInstalate.push_back(joc);
        consola.setGamesInstalled(jocuriInstalate);
        consola.setStorageLeft(consola.getStorageLeft() - joc.getSize());
    }
    else
    {
        cout << "Spatiu insuficient pentru a instala acest joc!" << endl;
        cout << "Spatiu disponibil: " << consola.getStorageLeft() << "GB";
    }
    ofstream fout("../Shared/Files/consola.txt");
    if (fout.is_open())
    {
        fout << consoleToString(consola);
        fout.close();
        cout << "Joc instalat cu succes!";
    }
}

void dezinstalare_joc(char **argv)
{
    ifstream fin("../Shared/Files/consola.txt");
    Console consola;
    if (fin.is_open())
    {
        string reader;
        getline(fin, reader);
        consola = stringToConsole(reader);
        fin.close();
    }
    vector<Game> jocuriInstalate = consola.getGamesInstalled(), updateJocuriInstalate;
    Game joc;
    bool dezinstalat = false;
    for (auto i : jocuriInstalate)
    {
        if (i.getTitle() == argv[2])
        {
            dezinstalat = true;
            joc = i;
        }
        else
        {
            updateJocuriInstalate.push_back(i);
        }
    }
    if (dezinstalat == true)
    {
        consola.setGamesInstalled(updateJocuriInstalate);
        consola.setStorageLeft(consola.getStorageLeft() + joc.getSize());
        ofstream fout("../Shared/Files/consola.txt");
        if (fout.is_open())
        {
            fout << consoleToString(consola);
            fout.close();
            cout << "Joc dezinstalat cu succes!";
        }
    }
    else
    {
        cout << "Jocul nu este instalat!";
    }
}

void cumpara_bundle(char **argv)
{
    ifstream finBundleuri("../Shared/Files/bundles.txt");
    ifstream finConsola("../Shared/Files/consola.txt");
    Console consola;
    if (finConsola.is_open())
    {
        string reader;
        getline(finConsola, reader);
        consola = stringToConsole(reader);
        finConsola.close();
    }
    Bundle bundle;
    if (finBundleuri.is_open())
    {
        string cauta = findGameOrBundleInFile(argv[2], "bundles");
        if (cauta != "-")
        {
            bundle = stringToBundle(cauta);
        }
        else
        {
            cout << "Bundle inexistent!";
            return;
        }
        vector<Game> jocuriCumparateConsola = consola.getGames(), jocuriBundle = bundle.getGames();
        set<Game> finalJocuriCumparate;
        for (auto i : jocuriCumparateConsola)
        {
            finalJocuriCumparate.insert(i);
        }
        for (auto i : jocuriBundle)
        {
            finalJocuriCumparate.insert(i);
        }
        vector<Game> finalJocuriCumparateVector;
        for (auto i : finalJocuriCumparate)
        {
            finalJocuriCumparateVector.push_back(i);
        }
        consola.setGames(finalJocuriCumparateVector);
        finBundleuri.close();
    }
    ofstream fout("../Shared/Files/consola.txt");
    if (fout.is_open())
    {
        fout << consoleToString(consola);
        fout.close();
        cout << "Bundle cumparat cu succes!";
    }
}

