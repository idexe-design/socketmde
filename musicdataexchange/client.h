//==============================================================================
#ifndef CLIENT_H
#define CLIENT_H
//==============================================================================
/*
 |-----------------------------------------------------------|
 |                     UDPClient Info                        |
 |-----------------------------------------------------------|
 |   Beschreibung : Enthält zentrale Client Klasse, die einen|
 |                  rudimentären UDP Client nach den Anforder|
 |                  rungen der Aufgabenstellung bereitstellt,|
 |                  um mit dem Server zu kommunizieren       |
 |   Copyright By : Kevin Jakob                              |
 |   Datei        : client.h                                 |
 |   Version      : 1.0.0                                    |
 |-----------------------------------------------------------|
 */
//==============================================================================

//==============================================================================
//Globale Includes
//==============================================================================
#include <iostream>         // std Ein- und Ausgabestreams für die Konsole
#include <string>           // string Klasse der std
#include <sstream>          // Funktion für Str to int bei Fehlercode
#include <vector>           // dynamische Datenstruktur vector für die Datensätze
#include <curses.h>          // Tastatureingabe Funktionen
#include <sys/types.h>      //Deklariert Typ time_t für Zeitfunktionen
#include <arpa/inet.h>      //Definiert Internetfunktionen, z.B. in_addr
#include <sys/socket.h>     //Einbinden der Socket API
#include <netdb.h>          //Definiert Netzwerkdatenbankfunktionen, z.B: in_port
#include <netinet/in.h>     //Stellt Internet-Protokollfamilie bereit
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>
//==============================================================================

//==============================================================================
//Lokale Includes
//==============================================================================
//==============================================================================

//==============================================================================
//Direktiven und Definitionen
//==============================================================================
using namespace std;        // der Einfachheit halber um sich die Kennzeichnung
// von std vor jedem Funktionsaufruf zu sparen

//unschön aber defs für deutsche Sonderzeichen / Hass auf M$ weil kein UTF-8 in den Konsolen bzw. dem gesamten OS
#define AE (unsigned char)142
#define ae (unsigned char)132
#define OE (unsigned char)153
#define oe (unsigned char)148
#define UE (unsigned char)154
#define ue (unsigned char)129
#define ss (unsigned char)225
//==============================================================================
class Client
{
private:
    unsigned short iCurrentPort; //der nächst anzufragende Port
    int sClient;       // SOCKET der Daten empfängt und sendet UDP
    sockaddr_in sAddr;    // Struktur Infos über Socket Adressierung beim binden
    vector <string> vData; // Musikdatensätze
    void incPort();     //inkrementiert bis 50010 und fängt dann wieder
    //bei 50000 an
    void request(); //Anfrage senden an Port zur Datenübertragung
    void handleRequest(); //Anfrage beantworten
    #include <termios.h>
    
public:
    Client();     //Konstruktor (kümmert sich nur um Winsock)
    ~Client();    //Destruktor (kümmert sich im Grunde auch nur um Winsock)
    
    void  establish (unsigned short iStartPort); //Bindet UDP Socket auf iStartPort
    //oder inkrementiert iStartPort, bis
    //freier Port gefunden.
    void start ();    //Startet den Datenaustausch im Portbereich zwischen den
    //verschiedenen Clienten
    void collectData(); // Datensätze sammeln
    void showData(); //Zeigt die derzeit vorhandenen Datensätze
    int kbhit();
};
//==============================================================================
#endif
