#include "../Shared/Data/clase.h"
#include "../Shared/Data/functii.h"
#include <cstdlib>


int main(int argc, char const **argv){
    if(argc == 6){
        if((std::string) *(argv + 1) == "add_game"){
            std::string aux = (std::string) *(argv + 2) + ";" + (std::string) *(argv + 3) + ";" + (std::string) *(argv + 4) + ";" + (std::string) *(argv + 5) + ";";
            Game game = stringToGame(aux);
            writeGameToFile(game);
        }
    }

    if(argc == 3){
        if((std::string) *(argv + 1) == "delete_game"){
            deleteGameOrBundleFromFile((std::string) *(argv + 2), "games");
        }
        if((std::string) *(argv + 1) == "delete_bundle"){
            deleteGameOrBundleFromFile((std::string) *(argv + 2), "bundles");
        }
    }

    if(argc == 4){
        if((std::string) *(argv + 1) == "create_bundle"){
            std::vector<Game> aux = {};
            Bundle b = createBundle((std::string) *(argv + 2), std::atoi(*(argv + 3)), aux);
            writeBundleToFile(b);
        }
    }

    if(argc >= 5){
        if((std::string) *(argv + 1) == "add_to_bundle"){
            if(findGameOrBundleInFile((std::string) *(argv + 2), "bundles") != "-"){
                if(std::atoi(*(argv + 3)) == argc - 4){
                    for(int i = 0; i < std::atoi(*(argv + 3)); i++){
                        addGameToBundle(*(argv + 2), *(argv + 4 + i));
                    }
                }
            }
        }
        if((std::string) *(argv + 1) == "delete_from_bundle"){
            if(findGameOrBundleInFile((std::string) *(argv + 2), "bundles") != "-"){
                if(std::atoi(*(argv + 3)) == argc - 4){
                    for(int i = 0; i < std::atoi(*(argv + 3)); i++){
                        deleteGameFromBundle(*(argv + 2), *(argv + 4 + i));
                    }
                }
            }
        }
    }

    if(argc == 2){
        if((std::string) *(argv + 1) == "see_bought_games"){
            std::ifstream file;
            file.open("../Shared/Files/bought-games.txt");
            if(file.is_open()){
                std::string line;
                while(std::getline(file, line)){
                    std::cout << line << "\n";
                }
            }
        }
    }

    return 0;
}