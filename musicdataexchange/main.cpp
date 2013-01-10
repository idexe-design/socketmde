//==============================================================================
/*
 |-----------------------------------------------------------|
 |                     UDPClient Info                        |
 |-----------------------------------------------------------|
 |   Beschreibung : Enthält main Funktion, übernimmt die     |
 |                  grundlegende Programminitialisierung und |
 |                  Finalisierung                            |
 |   Copyright By : Kevin Jakob                              |
 |   Datei        : main.cpp                                 |
 |   Version      : 1.0.0                                    |
 |-----------------------------------------------------------|
 */
//==============================================================================
//Globale Includes
//==============================================================================
#include <iostream>         // std Ein- und Ausgabestreams für die Konsole
#include <string>           // string Klasse der std
#include <curses.h>          // Funktionen zur Tastatureingabe (kbhit())
//==============================================================================

//==============================================================================
//Lokale Includes
//==============================================================================
#include "client.h"
//==============================================================================

//==============================================================================
//Direktiven und Definitionen
//==============================================================================
using namespace std;        // der Einfachheit halber um sich die Kennzeichnung
// von std vor jedem Funktionsaufruf zu sparen

#define PROGRAM_VERSION "1.0.0" //Programmversion als Konstante


//unschön aber defs für deutsche Sonderzeichen / Hass auf M$ weil kein UTF-8 in den Konsolen bzw. dem gesamten OS
#define AE (unsigned char)142
#define ae (unsigned char)132
#define OE (unsigned char)153
#define oe (unsigned char)148
#define UE (unsigned char)154
#define ue (unsigned char)129
#define ss (unsigned char)225

//==============================================================================
int main(int argc, char *argv[])
{
    cout << "Aufgabe 2 \n" << "UDP Client " <<PROGRAM_VERSION << endl << "----------------" << endl; //Programmkopfausgabe
    try
    {
        Client udpClient;               // Anlegen des Clients
        udpClient.establish(50000);     // auf Port 50000 binden
        udpClient.collectData();        // Eingaben zu den Datensätzen
        cout << "Programm starten mit beliebiger Taste,..." << endl << endl;
        //while(!udpClient.kbhit()); // unschön unschön aber praktisch
        udpClient.start();
        cout << endl << endl << endl;
        udpClient.showData();
        cout << "Beliebige Taste dr" <<ue<< "cken um fortzufahren." << endl;
        //while(!udpClient.kbhit());        //nicht ganz schön aber adäquat um auf Tasteneingabe zu warten.
        return 0;    //oder auch return 0, dass ist ne wxDev-C++ Konstante
    }
    catch(string &cException) // einfaches Exception Handling via std::string, global !
    {
        cout << cException << endl << "Programm wird beendet..." << endl;
        return 1; //oder auch return -1 etc., dass ist ne wxDev-C++ Konstante
    }
}
//==============================================================================
