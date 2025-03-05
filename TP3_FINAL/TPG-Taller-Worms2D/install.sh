#!/bin/bash
readonly TEMP=~/Downloads/Worms2DTemp
readonly TARGET_PATH=~/Desktop/Worms2D
readonly WORMS_PATH=$(echo "$(pwd)")

echo "
                                    _           _        _ _           
                                   (_)         | |      | | |          
__      _____  _ __ _ __ ___  ___   _ _ __  ___| |_ __ _| | | ___ _ __ 
\ \ /\ / / _ \|  __|  _   _ \/ __  | |  _ \/ __| __/ _  | | |/ _ \  __|
 \ V  V / (_) | |  | | | | | \__ \ | | | | \__ \ || (_| | | |  __/ |   
  \_/\_/ \___/|_|  |_| |_| |_|___/ |_|_| |_|___/\__\__,_|_|_|\___|_|   
                                                                     
                                                                     
"

echo "
                ██████                      ██████       
              ██░░░░▒▒██                  ██░░░░▒▒██    
            ██░░████░░░░██              ██░░████░░░░██  
          ██░░░░░░░░░░░░░░██          ██░░░░░░░░░░░░░░██
          ██░░░░░░    ░░  ██          ██░░░░░░    ░░  ██
          ██░░░░░░  ██  ████          ██░░░░░░  ██  ████
          ██░░░░░░  ██  ████          ██░░░░░░  ██  ████
          ██░░░░░░░░      ██          ██░░░░░░░░      ██
            ██░░░░░░░░░░██              ██░░░░░░░░░░██  
            ██░░░░██████                ██░░░░██████    
      ██████░░░░░░░░░░██          ██████░░░░░░░░░░██    
  ████░░░░░░░░░░░░░░░░██      ████░░░░░░░░░░░░░░░░██    
██░░░░░░░░░░░░░░░░░░░░██    ██░░░░░░░░░░░░░░░░░░░░██    
██░░░░░░████░░░░░░░░██      ██░░░░░░████░░░░░░░░██      
  ██████    ████████          ██████    ████████        
"

read -p "Do you want to install the game? (y/n) " confirm

if [[ $confirm =~ ^[Nn]$ ]]; then
    echo "Installation canceled."
    exit 0

fi
    mkdir ${TEMP}

    sudo apt update -qq

    echo "Installing essential tools"

    sudo apt install -qq build-essential git cmake -y

    echo "Installing dependencies..."
  
    echo "Installing Box2D"
        cd ${TEMP}
        sudo apt install -qq doxygen
        git clone git@github.com:erincatto/box2d.git > /dev/null
        cd box2d
        mkdir build
        cd build
        cmake .. > /dev/null
        make -j4 > /dev/null
        sudo make install > /dev/null

    echo "Installing yaml-cpp, google-test"
        sudo apt install -qq libgtest-dev libyaml-cpp-dev libyaml-cpp0.7 -y

    echo "Installing spdlog"
        cd ${TEMP}
        git clone -q --depth 1 --branch 10.1.1 git@github.com:fmtlib/fmt.git > /dev/null
        cd fmt
        mkdir build
        cd build
        cmake .. > /dev/null
        make -j4 > /dev/null
        sudo make install > /dev/null

        cd ${TEMP}
        git clone -q --depth 1 --branch v1.12.0 git@github.com:gabime/spdlog.git > /dev/null
        cd spdlog
        mkdir build
        cd build
        cmake .. > /dev/null
        make -j4 > /dev/null
        sudo make install > /dev/null


    echo "Installing QT6"
        sudo apt install -qq -y qmake6 qt6-base-dev qt6-base-dev-tools qt6-multimedia-dev qt6-tools-dev
        sudo apt install -qq -y qt6-tools-dev-tools qt6-wayland qt6-wayland-dev qt6-wayland-dev-tools
        sudo apt install -qq -y libqt6multimedia6 libqt5multimedia5-plugins

    echo "Installing SDL2pp, SDL2-image, SDL2-mixer & SDL2-ttf"
        sudo apt install -qq -y libjpeg-dev libpng-dev libfreetype-dev libopusfile-dev libflac-dev
        sudo apt install -qq -y libxmp-dev libfluidsynth-dev libwavpack-dev libmodplug-dev libsdl2-dev

        echo "Installing SDL2-image"
            cd ${TEMP}
            git clone -q --depth 1 --branch release-2.6.3 git@github.com:libsdl-org/SDL_image.git > /dev/null
            cd SDL_image
            mkdir build
            cd build
            cmake .. > /dev/null 
            make -j4 > /dev/null
            sudo make install > /dev/null

        echo "Installing SDL2-mixer"
            cd ${TEMP}
            git clone -q --depth 1 --branch release-2.6.3 git@github.com:libsdl-org/SDL_mixer.git > /dev/null
            cd SDL_mixer
            mkdir build
            cd build
            cmake .. > /dev/null
            make -j4 > /dev/null
            sudo make install > /dev/null

        echo "Installing SDL2-ttf"
            cd ${TEMP}
            git clone -q --depth 1 --branch release-2.20.2 git@github.com:libsdl-org/SDL_ttf.git > /dev/null
            cd SDL_ttf
            mkdir build
            cd build
            cmake .. > /dev/null
            make -j4 > /dev/null
            sudo make install > /dev/null

        echo "Installing SDL2pp"
            cd ${TEMP}
            git clone -q --depth 1 --branch 0.18.1 git@github.com:libSDL2pp/libSDL2pp.git > /dev/null
            cd libSDL2pp
            mkdir build
            cd build
            cmake .. > /dev/null
            make -j4 > /dev/null
            sudo make install > /dev/null

    echo "Installing Worms2D"
        #clean install
        rm -rf ${TARGET_PATH}
        cd ${WORMS_PATH}

        #clean build
        rm -rf build
        rm -rf target
        mkdir build
        cd build
        cmake ${WORMS_PATH} > /dev/null
        make -j4 > /dev/null

        #copy assets
        cd ${WORMS_PATH}
        mkdir ${TARGET_PATH}
        mv ./target ${TARGET_PATH}
        
        cp worms2D.sh ${TARGET_PATH}
        cp worms2Dserver.sh ${TARGET_PATH}
        chmod +x ${TARGET_PATH}/worms2D.sh
        chmod +x ${TARGET_PATH}/worms2Dserver.sh

        cp -r ./yaml ${TARGET_PATH}
        cp -r ./sprites ${TARGET_PATH}
        cp -r ./client/resources ${TARGET_PATH}

        #clean temp files
        rm -rf ${TEMP}

    echo "Installation complete, see ${TARGET_PATH}"