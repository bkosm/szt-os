#pragma once
#include <SFML/Graphics.hpp>


const sf::String APP_TITLE{"sztOS"};
const sf::VideoMode RESOLUTION{1280, 720};

const sf::String FONT_PATH{"./resources/arial.ttf"};
const sf::String CONSOLE_FONT_PATH{"./resources/consoleFont.png"};
const sf::String LOGO_PATH{"./resources/logoLowRes.png"};
const sf::String TERM_ICON_PATH{"./resources/termIcon.png"};
const sf::String PLAY_BUTTON_PATH{"./resources/playButton.png"};

const sf::String WALLPAPER_PATH_1{"./resources/winWall1.jpg"};
const sf::String WALLPAPER_PATH_2{"./resources/winWall2.jpg"};
const sf::String SZTOS_WALLPAPER_PATH_1{"./resources/sztOSwall1.png"};
const sf::String SZTOS_WALLPAPER_PATH_2{"./resources/sztOSwall2.png"};

const sf::String BOOT_SOUND_PATH{"./resources/boot.wav"};
const sf::String ERROR_SOUND_PATH{"./resources/error.wav"};
const sf::String EXIT_SOUND_PATH{"./resources/shutdown.wav"};

const std::string BIKE{ R"(
                                           %%( %###                         
                                            %  %%%#                         
                                                ,,*                         
                                                 #*,                        
                                             *,,,,(**                       
                                           *,,,,,,*#*,                      
                                         *,,,,#,,,,*(/,                     
                 ,*%&&@@&%(//*,        ,,,,#,,,,*,///*                    
                      */./(%&@@@* .,,,,%%/(,,,,,.  ../,                   
                         /,#***** /,,,,%,,,,,,        #.* .###.             
      /&&&&&&&&&%         ****//,,/,#%,,/,            %&/,&&&&&%%         
    &&&&&&&&&&&&&&&&   ,*/*,**/(,,#,,,,,            %&&%&,.&%%%%&&%%%       
  &%&&%&  ,    (&&@@,,,,,,,,,***,,,&,,            #&&&&&*/** .(  #%&%%(     
 &&%&%      %((,,,,,,,,,******#/,,,,             (%%&&/  (***      %&%%     
&&&@(  .  .****,,,,,,,*,****,,,,,,,              &&&%    ((@**    ,*&&%% 
&&&%     .,,****,,,,,*,*,,,,,****,               &&&&     %&(*//#.  &&%% 
&&&&*%   ##,******(@@@&.                         &&&%.     # #/ %  %#(  
(&&@% /     .   ( /(%&&                          ,&&&&*    (   /  #&%   
 &&&@((,  % .  , %@&&&                            ,%&&&&.  *    %&@&%%   
   &%&@&&%#,*%&&@@@%%                               %&&%@&%#*/%@&%&&     
     &@&@@&@@&@&%                                    &@&%%%%%%%%%%             
         .&&&%,                                         *********
)" };

const sf::Vector2f START_STATE_BUTTON_SIZE{250, 80};

const float BUTTON_LABEL_TOP_INDENT{10.0};
const float INPUT_FIELD_LABEL_HEIGHT{12.0};

const unsigned SHUTDOWN_DURATION{2500};
