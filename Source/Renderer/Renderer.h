#include "AirSim.h"

/**
 * @brief initialize the window
 * 
 * @param width 
 * @param height 
 */
void initWindow(int width, int height);
/**
 * @brief function that refreshes the airport display
 * 
 * @param simulation 
 */
void updateAirportRenderer(simulation *simulation);

/**
 * @fn void closeWindow()
 * @brief function that close the window
 * 
 */

/**
 * @fn void SetDrawColor(SDL_Color Color)
 * @brief Set the Draw Color object
 * 
 * @param Color 
 */

/**
 * @fn void printRectangleWithBorder(Anchor CSG, Anchor CID, SDL_Color couleur, int border)
 * @brief print a rectangle with a customized border into the renderer
 * @brief it actualy draw a rectangle with 4 little  rectangles. this way we can customize the width of the rectangle
 * 
 * @param CSG 
 * @param CID 
 * @param couleur 
 * @param border 
 */

/**
 * @fn void printProgress(Anchor CSG, Anchor CID, SDL_Color couleur, int border, float pourcentage)
 * @brief print the progress bar into the renderer
 * 
 * @param CSG 
 * @param CID 
 * @param couleur 
 * @param border 
 * @param pourcentage 
 */

/**
 * @fn void printText(char *text, int fontSize, SDL_Color color, Anchor origin, textAlign align)
 * @brief print the text you want into the renderer
 * 
 * @param text 
 * @param fontSize 
 * @param color 
 * @param origin 
 * @param align 
 */

/**
 * @fn void interf_AirportToRender(simulation simulation)
 * @brief print the airport's interface into the renderer
 * @brief it use 4 subfunction that print each element
 * 
 * @param simulation 
 */

/**
 * @fn void interf_Runway(simulation simulation,runway *runway, Anchor CSG, int w, int h)
 * @brief print a generic runway that is used 6times 
 * 
 * @param simulation 
 * @param runway 
 * @param CSG 
 * @param w 
 * @param h 
 */

/**
 * @fn void interf_Radar(airport *airport, Anchor left)
 * @brief print the radar square into the renderer
 * 
 * @param airport 
 * @param left 
 */

/**
 * @fn void interf_Radar_PrintLine(plane *plane, Anchor left)
 * @brief printf the radar intels into the radar square
 * 
 * @param plane 
 * @param left 
 */

/**
 * @fn void interf_Parking(simulation simulation, Anchor left)
 * @brief print the parking square into the renderer
 * 
 * @param simulation 
 * @param left 
 */

/**
 * @fn void interf_Parking_PrintLine(sim_planeActor *planeActor, Anchor left, bool isInWFTR)
 * @brief  printf the parking intels into the radar square
 * 
 * @param planeActor 
 * @param left 
 * @param isInWFTR 
 */

/**
 * @fn void interf_launchMenu(simulation *simulation)
 * @brief print the menu into the renderer and do the action when you click on a button
 * 
 * @param simulation 
 */

/**
 * @fn button newButton(Anchor center, int h, int w, char *text, menuAction action)
 * @brief creat a button with a text, a location and a size (width and heigth)
 * 
 * @param center 
 * @param h 
 * @param w 
 * @param text 
 * @param action 
 * @return button 
 */

/**
 * @fn void printButtons(button *buttons, int nButtons)
 * @brief printf that good looking button in the renderer
 * 
 * @param buttons 
 * @param nButtons 
 */

/**
 * @fn void updateHoverButtons(button *buttons, int nButtons,int mx,int my)
 * @brief add a cool effect (fill the background) when hovering the button 
 * 
 * @param buttons 
 * @param nButtons 
 * @param mx 
 * @param my 
 */

/**
 * @fn menuAction getActionButton(button *buttons, int nButtons)
 * @brief Get the right Action Button to perform when you click 
 * 
 * @param buttons 
 * @param nButtons 
 * @return menuAction 
 */

/**
 * @fn bool isButtonHover(int mx, int my, button button)
 * @brief tells you if your cursor is hovering a button or not
 * 
 * @param mx 
 * @param my 
 * @param button 
 * @return true 
 * @return false 
 */