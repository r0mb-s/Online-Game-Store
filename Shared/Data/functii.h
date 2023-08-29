#include "clase.h"
// 

std::string findGameOrBundleInFile(std::string, std::string);

Game stringToGame(std::string);
std::string gameToString(Game);
Bundle stringToBundle(std::string);
std::string bundleToString(Bundle);
void deleteGameOrBundleFromFile(std::string, std::string);

void writeGameToFile(Game);

std::vector<Game> createGameVector(std::vector<std::string>);
Bundle createBundle(std::string, int, std::vector<Game>);
void writeBundleToFile(Bundle);
void addGameToBundle(std::string, std::string);
void deleteGameFromBundle(std::string, std::string);

// Auxiliary

std::string findGameOrBundleInFile(std::string title, std::string gamesORbundles)
{
    std::ifstream FileIn;
    std::string line;

    FileIn.open("../Shared/Files/" + gamesORbundles + ".txt");
    if (FileIn.is_open())
    {
        while (std::getline(FileIn, line))
        {
            auto pos = line.find(";");
            if (line.substr(0, pos) == title)
            {
                FileIn.close();
                return line;
            }
        }
        FileIn.close();
    }
    return "-";
}

Game stringToGame(std::string s)
{
    std::string title, category, size, price;

    std::size_t pos = s.find(';');
    title = s.substr(0, pos);

    pos = s.find(';', pos + 1);
    category = std::string(&s[title.length() + 1], &s[pos]);

    pos = s.find(";", pos + 1);
    size = std::string(&s[title.length() + category.length() + 2], &s[pos]);

    pos = s.find(";", pos + 1);
    price = std::string(&s[title.length() + category.length() + size.length() + 3], &s[pos]);

    return Game(title, category, std::stod(size), std::stod(price));
}

std::string gameToString(Game g)
{
    std::string game, aux;
    char cifra;

    game = g.getTitle() + ";";
    game += g.getCategory() + ";";
    aux = std::to_string(g.getSize()).erase(std::to_string(g.getSize()).length() - 4);
    while (*(aux.end() - 1) == '0' || *(aux.end() - 1) == '.')
    {
        if (*(aux.end() - 1) == '.')
        {
            aux.pop_back();
            break;
        }
        aux.pop_back();
    }
    game += aux + ";";
    aux = std::to_string(g.getPrice()).erase(std::to_string(g.getPrice()).length() - 4);
    while (*(aux.end() - 1) == '0' || *(aux.end() - 1) == '.')
    {
        if (*(aux.end() - 1) == '.')
        {
            aux.pop_back();
            break;
        }
        aux.pop_back();
    }
    game += aux + ";";

    return game;
}

Bundle stringToBundle(std::string s)
{
    int numberOfGames, lengthToJump = 0;
    std::string discount, title, price;

    std::size_t pos = s.find(';');
    title = std::string(&s[lengthToJump], &s[pos]);
    lengthToJump += title.length() + 1;

    pos = s.find(';', pos + 1);
    price = std::string(&s[lengthToJump], &s[pos]);
    lengthToJump += price.length() + 1;

    pos = s.find(";", pos + 1);
    discount = std::string(&s[lengthToJump], &s[pos]);
    lengthToJump += discount.length() + 1;

    pos = s.find(";", pos + 1);
    numberOfGames = std::stoi(std::string(&s[lengthToJump], &s[pos]));
    lengthToJump += std::string(&s[lengthToJump], &s[pos]).length() + 1;

    std::vector<Game> games;
    for (int i = 0; i < numberOfGames; i++)
    {
        for (int j = 0; j < 4; j++)
            pos = s.find(";", pos + 1);
        std::string game = std::string(&s[lengthToJump], &s[pos + 1]);
        games.push_back(stringToGame(game));
        lengthToJump += std::string(&s[lengthToJump], &s[pos]).length() + 1;
    }

    return Bundle(std::stod(discount), title, games, std::stod(price));
}

std::string bundleToString(Bundle b)
{
    std::vector<Game> games = b.getGames();
    std::string bundle;

    bundle = b.getTitle() + ";";
    bundle += std::to_string(b.getPrice()).erase(std::to_string(b.getPrice()).length() - 4) + ";";
    bundle += std::to_string(b.getDiscount()) + ";";
    bundle += std::to_string(games.size()) + ";";

    for (auto i : games)
        bundle += gameToString(i);

    return bundle;
}

std::vector<Game> createGameVector(std::vector<std::string> games)
{
    std::vector<Game> out;

    for (auto i : games)
    {
        std::ifstream gamesFileIn;
        gamesFileIn.open("../Shared/Files/games.txt");
        if (gamesFileIn.is_open())
        {
            std::string line;
            while (std::getline(gamesFileIn, line))
            {
                auto pos = line.find(";");
                if (line.substr(0, pos) == i)
                {
                    out.push_back(stringToGame(line));
                }
            }
            gamesFileIn.close();
        }
    }
    return out;
}

void deleteGameOrBundleFromFile(std::string title, std::string gameORbundle)
{
    std::list<std::string> obiecte;
    int contorObiecte;

    std::ifstream fileIn;
    fileIn.open("../Shared/Files/" + gameORbundle + ".txt");
    if (fileIn.is_open())
    {
        std::string line;
        while (std::getline(fileIn, line))
        {
            contorObiecte += 1;
            auto pos = line.find(";");
            if (line.substr(0, pos) != title)
                obiecte.push_back(line);
        }
        fileIn.close();
    }

    if (contorObiecte != obiecte.size())
    {
        if (gameORbundle == "games")
        {
            std::ifstream bundlesFileIn;
            bundlesFileIn.open("../Shared/Files/bundles.txt");
            if (bundlesFileIn.is_open())
            {
                std::vector<std::string> bundles;

                std::string line;
                while (std::getline(bundlesFileIn, line))
                    bundles.push_back(line.substr(0, line.find(';')));
                bundlesFileIn.close();

                for (auto i : bundles)
                {

                    deleteGameFromBundle(i, title);
                }
            }
        }

        std::ofstream fileOut;
        fileOut.open("../Shared/Files/" + gameORbundle + ".txt");
        if (fileOut.is_open())
        {
            for (auto i : obiecte)
                fileOut << i << '\n';
            fileOut.close();
        }
    }
}

// Game

void writeGameToFile(Game game)
{
    if (findGameOrBundleInFile(game.getTitle(), "games") == "-")
    {
        std::ofstream gamesFileOut;
        gamesFileOut.open("../Shared/Files/games.txt", std::ios::app);
        if (gamesFileOut.is_open())
        {
            gamesFileOut << gameToString(game) << '\n';
            gamesFileOut.close();
        }
    }
}

// Bundle

Bundle createBundle(std::string title, int discount, std::vector<Game> games)
{
    double sum = 0;
    for (auto i : games)
        sum += i.getPrice();
    return Bundle(discount, title, games, sum);
}

void writeBundleToFile(Bundle bundle)
{
    if (findGameOrBundleInFile(bundle.getTitle(), "bundles") == "-")
    {
        std::ofstream bundlesFileOut;
        bundlesFileOut.open("../Shared/Files/bundles.txt", std::ios::app);
        if (bundlesFileOut.is_open())
        {
            bundlesFileOut << bundleToString(bundle) << '\n';
            bundlesFileOut.close();
        }
    }
}

void addGameToBundle(std::string bundleTitle, std::string gameTitle)
{
    std::string game = findGameOrBundleInFile(gameTitle, "games");
    std::string bundle = findGameOrBundleInFile(bundleTitle, "bundles");

    if (game != "-" && bundle != "-")
    {
        Bundle bundleObj = stringToBundle(bundle);
        Game gameObj = stringToGame(game);

        std::vector<Game> bundleObjGames = bundleObj.getGames();

        bool ok = true;
        for (auto i : bundleObjGames)
        {
            if (i.getTitle() == gameObj.getTitle())
            {
                ok = false;
                break;
            }
        }
        if (ok)
        {
            bundleObjGames.push_back(gameObj);
            bundleObj.setGames(bundleObjGames);
            deleteGameOrBundleFromFile(bundleTitle, "bundles");
            bundleObj.setPrice(bundleObj.getPrice() + gameObj.getPrice());
            writeBundleToFile(bundleObj);
        }
    }
}

void deleteGameFromBundle(std::string bundleTitle, std::string gameTitle)
{
    std::string game = findGameOrBundleInFile(gameTitle, "games");
    std::string bundle = findGameOrBundleInFile(bundleTitle, "bundles");

    if (game != "-" && bundle != "-")
    {
        Bundle bundleObj = stringToBundle(bundle);
        Game gameObj = stringToGame(game);

        std::vector<Game> bundleObjGames = bundleObj.getGames();

        bool ok = false;
        for (auto i = bundleObjGames.begin(); i < bundleObjGames.end(); i++)
        {
            if ((*i).getTitle() == gameObj.getTitle())
            {
                bundleObjGames.erase(i);
                ok = true;
                break;
            }
        }
        if (ok)
        {
            bundleObj.setGames(bundleObjGames);
            deleteGameOrBundleFromFile(bundleTitle, "bundles");
            bundleObj.setPrice(bundleObj.getPrice() - gameObj.getPrice());
            writeBundleToFile(bundleObj);
        }
    }
}

Console stringToConsole(std::string s)
{
    std::string storageLeft;
    int numberOfInstalledGames, numberOfBoughtGames, lengthToJump = 0;
    std::size_t pos = s.find(';');
    storageLeft = std::string(&s[lengthToJump], &s[pos]);
    lengthToJump += storageLeft.length() + 1;

    pos = s.find(";", pos + 1);
    numberOfInstalledGames = std::stoi(std::string(&s[lengthToJump], &s[pos]));
    lengthToJump += std::string(&s[lengthToJump], &s[pos]).length() + 1;

    std::vector<Game> installedGames;
    for (int i = 0; i < numberOfInstalledGames; i++)
    {
        for (int j = 0; j < 4; j++)
            pos = s.find(";", pos + 1);
        std::string game = std::string(&s[lengthToJump], &s[pos + 1]);
        installedGames.push_back(stringToGame(game));
        lengthToJump += std::string(&s[lengthToJump], &s[pos]).length() + 1;
    }
    pos = s.find(";", pos + 1);
    numberOfBoughtGames = std::stoi(std::string(&s[lengthToJump], &s[pos]));
    lengthToJump += std::string(&s[lengthToJump], &s[pos]).length() + 1;

    std::vector<Game> BoughtGames;
    for (int i = 0; i < numberOfBoughtGames; i++)
    {
        for (int j = 0; j < 4; j++)
            pos = s.find(";", pos + 1);
        std::string game = std::string(&s[lengthToJump], &s[pos + 1]);
        BoughtGames.push_back(stringToGame(game));
        lengthToJump += std::string(&s[lengthToJump], &s[pos]).length() + 1;
    }

    return Console(std::stoi(storageLeft), installedGames, BoughtGames);
}

std::string consoleToString(Console c)
{
    std::string consola;

    consola = std::to_string(c.getStorageLeft()).erase(std::to_string(c.getStorageLeft()).length() - 4) + ";";
    consola += std::to_string(c.getGamesInstalled().size()) + ";";
    for (auto i : c.getGamesInstalled())
    {
        consola += gameToString(i);
    }
    consola += std::to_string(c.getGames().size()) + ";";
    for (auto i : c.getGames())
    {
        consola += gameToString(i);
    }
    return consola;
}

