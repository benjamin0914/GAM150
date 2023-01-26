#include "pch.h"
#include "System.h"
#include "Input.h"
#include "GameStateManager.h"

int main()
{
    System_Initialize(); //Initialize system

    GSM_Initialize(GS_LEVEL1); //Initialize GSM to game state Level 1

    while (current != GS_QUIT)
    {
        if (current != GS_RESTART)
        {
            GSM_Update(); //Update GSM
            fpLoad(); //Load current game state
        }
        else
        {
            next = previous;
            current = previous;
        }

        fpInitialize(); //Initialize current game state

        while (next == current)
        {
            Input_Handle(); //Input handle
            fpUpdate(); // Update current game state
            fpDraw(); //Draw in current game state
        }

        fpFree(); //Free current game state

        if (next != GS_RESTART)
        {
            fpUnload(); //Unload current game state
        }

        previous = current;
        current = next;
    }
    System_Exit(); //Exit system
    return 0;
}