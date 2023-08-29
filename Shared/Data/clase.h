#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stdlib.h>
#include <fstream>
#include <assert.h>
#include <algorithm>

#ifndef CLASE_H
#define CLASE_H

class Game
{
private:
    std::string title, category;
    double size, price;

public:
    Game(std::string t = "", std::string c = "", double s = -1, double p = -1) : title{t}, category{c}, size{s}, price{p} {}
    Game(const Game &g) : title{g.title}, category{g.category}, size{g.size}, price{g.price} {}
    friend bool operator<(const Game &g1, const Game &g2) { return g1.title < g2.title; }
    Game &operator=(const Game &g)
    {
        if (this == &g)
            return *this;
        title = g.title;
        category = g.category;
        size = g.size;
        price = g.price;
        return *this;
    }

    std::string getTitle() { return title; }
    std::string getCategory() { return category; }
    double getSize() { return size; }
    double getPrice() { return price; }

    void setTitle(std::string s) { title = s; }
    void setCategory(std::string s) { category = s; }
    void setSize(double s) { size = s; }
    void setPrice(double s) { price = s; }

    virtual std::ostream &afisare(std::ostream &os) const
    {
        os << title << ";" << category << ";" << size << ";" << price << ";";
        return os;
    }
    friend std::ostream &operator<<(std::ostream &os, const Game &g) { return g.afisare(os); }
};

class Bundle
{
private:
    int discount;
    std::string title;
    std::vector<Game> games;
    double price;

public:
    Bundle(int d = -1, std::string t = "-", std::vector<Game> g = {}, double p = -1) : discount{d}, title{t}, games{g}, price{p} {}
    Bundle(const Bundle &b) : discount{b.discount}, title{b.title}, games{b.games}, price{b.price} {}
    Bundle &operator=(const Bundle &b)
    {
        if (this == &b)
            return *this;
        discount = b.discount;
        title = b.title;
        games = b.games;
        price = b.price;
        return *this;
    }

    int getDiscount() { return discount; }
    std::string getTitle() { return title; }
    std::vector<Game> getGames() { return games; }
    double getPrice() { return price; }

    void setDiscount(int s) { discount = s; }
    void setTitle(std::string &s) { title = s; }
    void setGames(std::vector<Game> s) { games = s; }
    void setPrice(double s) { price = s; }

    virtual std::ostream &afisare(std::ostream &os) const
    {
        os << title << ";" << price << ";" << discount << ";" << games.size() << ";";
        for (auto i : games)
            os << i;
        return os;
    }
    friend std::ostream &operator<<(std::ostream &os, const Bundle &b) { return b.afisare(os); }
};

class Catalog
{
private:
    std::vector<Game> games;
    std::vector<Bundle> bundles;

public:
    Catalog(std::vector<Game> g = {}, std::vector<Bundle> b = {}) : games{g}, bundles{b} {}
    Catalog(const Catalog &c) : games{c.games}, bundles{c.bundles} {}
    Catalog &operator=(const Catalog &c)
    {
        if (this == &c)
            return *this;
        games = c.games;
        bundles = c.bundles;
        return *this;
    }

    std::vector<Game> getGames() { return games; }
    std::vector<Bundle> getBundles() { return bundles; }

    void setGames(std::vector<Game> &g) { games = g; }
    void setBundles(std::vector<Bundle> &b) { bundles = b; }

    virtual std::ostream &afisare(std::ostream &os) const
    {
        os << games.size() << ";\n";
        for (auto i : games)
            os << i << '\n';
        os << bundles.size() << ";\n";
        for (auto i : bundles)
            os << i << '\n';
        return os;
    }
    friend std::ostream &operator<<(std::ostream &os, const Catalog &c) { return c.afisare(os); }
};

class Console
{
private:
    double storageLeft;
    std::vector<Game> games, gamesInstalled;

public:
    Console(double s = -1, std::vector<Game> g1 = {}, std::vector<Game> g2 = {}) : storageLeft{s}, gamesInstalled{g1}, games{g2} {}
    Console(const Console &c) : storageLeft{c.storageLeft}, games{c.games}, gamesInstalled{c.gamesInstalled} {}

    Console &operator=(const Console &c)
    {
        if (this == &c)
            return *this;

        storageLeft = c.storageLeft;
        games = c.games;
        gamesInstalled = c.gamesInstalled;
        return *this;
    }

    double getStorageLeft() { return storageLeft; }
    std::vector<Game> getGames() { return games; }
    std::vector<Game> getGamesInstalled() { return gamesInstalled; }

    void setStorageLeft(double s) { storageLeft = s; }
    void setGames(std::vector<Game> &s) { games = s; }
    void setGamesInstalled(std::vector<Game> &s) { gamesInstalled = s; }
};

#endif
