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
 |   Datei        : client.cpp                               |
 |   Version      : 1.0.0                                    |
 |-----------------------------------------------------------|
 */
//==============================================================================

//==============================================================================
//Globale Includes
//==============================================================================
//==============================================================================

//==============================================================================
//Lokale Includes
//==============================================================================
#include "client.h"
//==============================================================================

//==============================================================================
//Direktiven und Definitionen
//==============================================================================
//==============================================================================

//==============================================================================
//Konstruktor
//Startet Winsock in Version 2 und generiert den Socket übergibt dann das Client
//Objekt, sollte keine Exception aufgetreten sein.
//==============================================================================
Client::Client()
{
    cout << "Initialisiere Client....." << endl;
    cout << "\t Generiere UDP Socket.....";
    sClient = socket(AF_INET,SOCK_DGRAM,0); //Verbindungssocket wird erstellt.
    if(sClient == -1)  //Prüfen auf Fehler bei Generierung
    {
        string cBuf = "Fehler bei Socketgenerierung: Code ";
        ostringstream sNumber;
        sNumber << errno; // letzten Fehler abfragen
        cBuf += sNumber.str();
        throw cBuf; // Exception werfen
    }
    else
    {
        cout << "ok" <<endl;
    }
    iCurrentPort = 50000;   //anfangsport für den "Ping"
    vData.clear();          //etwaige Speicherreste
    cout << "Client initialisiert." << endl << endl;
}
//==============================================================================

//==============================================================================
//Destruktor
//beendet die gestartete Winsock Schnittstelle für dieses Programm und den
//Verbindungssocket
//==============================================================================
Client::~Client()
{
    if(sClient != -1) close(sClient); // wenn Socket existiert wird Socket hier geschlossen
}
//==============================================================================

//====================================================p==========================
//void establish(unsigned short iStartPort)
//Bindet den UDP Socket an iStartPort bzw. inkrementiert iStartPort, wenn dieser
//Port bereits von einem Prozess, im Speziellen einem anderen Clienten belegt ist,
//==============================================================================
void Client::establish(unsigned short iStartPort)
{
    cout << "Starte Client....." << endl;
    int iError = 0;
    do  // suche solange nach freiem Port, bis erfolgreich gebindet werden kann
    {
        cout <<"\t Versuche UDP Socket an Port "<<iStartPort<<" zu binden.....";
        memset(&sAddr,0,sizeof(sockaddr_in));    // mit Nullen Adressraum "resetten"
        sAddr.sin_family = AF_INET;              //
        sAddr.sin_port = htons(iStartPort);      // Adressstruktur füllen (htons dreht die Bytes wegen Network Byte Mode
        sAddr.sin_addr.s_addr = INADDR_ANY;        //
        if(bind(sClient,(sockaddr*)&sAddr,sizeof(sockaddr)) == -1)
            //Socket binden, auf Fehler prüfen (hier besonders wichtig für die Schleife)
        {
            iError = errno;     // Fehler abfragen
            if(iError == EADDRINUSE)     //Port belegt, inkrementieren und erneut versuchen
            {
                cout <<"fehlgeschlagen. Port bereits gebindet." << endl;
                iStartPort ++;  // Einen Port weiter
            }
            else
            {
                string cBuf = "Fehler beim Binden des UDP Sockets: Code ";
                ostringstream sNumber;  // Int To string
                sNumber << errno;  //letzten Fehler abfragen
                cBuf += sNumber.str();
                throw cBuf; // Exception werfen
            }
        }
        else
        {
            cout << "ok" <<endl;
            iError = 0;
        }
    } while (iStartPort <= 50010 && iError == EADDRINUSE); //solange entweder Port belegt oder Port Range erreicht
    if(iStartPort > 50010)  //zu viele Clienten
    {
        string cBuf = "Kein freier Port mehr verf";
        cBuf += ue;
        cBuf += "gbar! Gegebenenfalls zu viele Clienten?";
        throw cBuf;
    }
    cout << "\t Versetze den Socket in den Non Blocking Mode.....";
    u_long iMode = 1;
    if(ioctl(sClient,FIONBIO, &iMode) == -1)  //recvfrom und sendto blocken nicht mehr!
    {
        string cBuf = "Fehler beim Binden des UDP Sockets: Code ";
        ostringstream sNumber;
        sNumber << errno;  //letzten Fehler abfragen
        cBuf += sNumber.str();
        throw cBuf; // Exception werfen
    }
    else
    {
        cout << "ok" << endl;
    }
    cout << "Client erfolgreich gestartet." << endl << endl;
}
//==============================================================================

//==============================================================================
// void start ()
// Startet den Netzwerkverkehr - den Austausch der Datensätze unter den Clienten
// im Portbereich 50000 - 50010
//==============================================================================
void Client::start()
{
    cin.ignore ();  // damit das Programm nicht gleich ins rudimentäre Menü springt
    if(iCurrentPort == htons(sAddr.sin_port)) incPort(); //inkrementiere Port wenn der eigene an der Reihe ist
    cout << "Gestartet!" << endl << endl;
    bool bEnd = false;
    while(!bEnd) //solange weiter, wie im Menü nicht Programm beenden gewählt wurde
    {
        handleRequest();
        request();
                       if(vData.size() == 6) //Spezialfall für diesen Anwendungsfall, wenn 6 verschiedene Einträge existieren
                                    //dann ist das Programm fertig mit dem Daten sammeln!
                                  //normalerweise würde es ewig weiter daten anfragen und beantworten
            {
              bEnd = true;
            }
        usleep(50*1000);
    }
    showData();
}
//==============================================================================

//==============================================================================
// void request()
// sendet eine Anfrage an einen Port auf localhost und wartet auf antwort
// nimmt eventuell Daten entgegen und sendet diese
//==============================================================================
void Client::request()
{
    sockaddr_in sRequest;
    memset(&sRequest,0,sizeof(sockaddr_in));
    sRequest.sin_family = AF_INET;                        //
    sRequest.sin_port = htons(iCurrentPort);              // Adressstruktur füllen (htons dreht die Bytes wegen Network Byte Mode
    sRequest.sin_addr.s_addr = inet_addr("127.0.0.1");    //
    char cBuf[] = "ready?";
    if(sendto(sClient,cBuf,sizeof(cBuf),0,(sockaddr*)&sRequest,sizeof(sockaddr_in)) == -1) //senden und auf Fehler prüfen
    {
        cout << "Prob1" << errno;
        exit(0);
    }
    else
    {
        //erfolgreich aber ohne Benachrichtung an den Nutzer, denn das passiert recht häufig
    }
    incPort();
}
//==============================================================================

//==============================================================================
// void handleRequest()
// verarbeitet Anfragen in Form von anliegenden Daten / Anfragen beantwortet
// diese und sendet daten / empfängt diese
//==============================================================================
void Client::handleRequest()
{
    char cBuf_r[512];
    sockaddr_in sRemote;
    socklen_t sRemoteLength = sizeof(sockaddr_in);
    if(recvfrom(sClient, cBuf_r, sizeof(cBuf_r), 0, (sockaddr*)&sRemote, &sRemoteLength) < 1) //empfangen und Fehler überprüfen
    {
        //keine Anfragen ausstehend aber gleiche Begründung wie oben, was gegen eine
        //Nutzerbenachrichtigung spricht
    }
    else
    {
        if(string(cBuf_r) == string("ready?"))
        {
            char cBuf_s [] = "ok!";
            if(sendto(sClient,cBuf_s,sizeof(cBuf_s),0,(sockaddr*)&sRemote,sizeof(sockaddr_in)) == -1) //senden und auf Fehler prüfen
            {
                cout << "Prob2" << errno;
                exit(0);
            }
            else
            {
                // erfolgreich gesandt
            }
        }
        else if(string(cBuf_r) == string("ok!")) // htons(sRemote.sin_port)
        {
            string cBufDaten = "Daten!#";
            cBufDaten += vData[0];
            cBufDaten += "#";
            cBufDaten += vData[1];
            if(sendto(sClient,cBufDaten.c_str(),512,0,(sockaddr*)&sRemote,sizeof(sockaddr_in)) == -1) //senden und auf Fehler prüfen
            {
                cout << "Prob3" << errno;
                exit(0);
            }
            else
            {
                // erfolgreich gesandt
            }
        }
        else
        {
            string cTmp = string(cBuf_r);
            if(cTmp.substr(0,6) == string("Daten!"))
            {
                //Stringoperationen um empfangenen Textblock in nutzbare Informationen zu zerlegen
                cTmp = cTmp.substr(7,cTmp.length()-8);  // anfang entfernen
                string cBuf1 = cTmp.substr(0,cTmp.find("#")); //an Trennzeichen # trennen
                string cBuf2 = cTmp.substr(cTmp.find("#") + 1, cTmp.length() - cTmp.find("#"));  //der andere Teil hinten
                //------------------------------
                bool bContain = false;
                bool bContain2 = false;
                for(vector<string>::iterator it = vData.begin(); it < vData.end(); it++) // Iterieren durch den Vektor
                {
                    if(*it == cBuf1) bContain = true;   // überprüfen ob neuer Datensatz 1 bereits vorhanden ist
                    if(*it == cBuf2) bContain2 = true;  // dasselbe für den 2.
                }
                if(!bContain || !bContain2)
                {
                    cout << "Es wurden Datens" << ae << "tze hinzugef" << ue << "gt!" << endl;
                    cout << "Client IP: " << inet_ntoa(sRemote.sin_addr) << " Port: " << htons(sRemote.sin_port) << endl << "------------------------" << endl;
                    if(!bContain)
                    {
                        cout << cBuf1 << endl;
                        vData.push_back(cBuf1); //Datensatz hinzufügen
                    }
                    if(!bContain2)
                    {
                        cout << cBuf2 << endl;
                        vData.push_back(cBuf2); // Datensatz hinzufügen
                    }
                    cout <<"------------------------" << endl <<endl;
                }
            }
        }
    }
}
//==============================================================================

//==============================================================================
// incPort ()
// inkrementiert den Wert der member Variable iCurrentPort bis 50010, fängt wieder bei
// 50000 an.
//==============================================================================
void Client::incPort ()
{
    if(iCurrentPort < 50010) iCurrentPort ++;
    else iCurrentPort = 50000;
    if(iCurrentPort == htons(sAddr.sin_port)) iCurrentPort ++;
}
//==============================================================================

//==============================================================================
//void showData ()
//Zeigt die aktuell verfügbaren Datensätze untereinander an
//==============================================================================
void Client::showData ()
{
    cout << "Folgende Datens" << ae << "tze sind diesem Clienten bekannt:" << endl << "------------------------" << endl;
    for(int i = 0; i < vData.size(); i++) // alle Datensätze anzeigen (wegen der Übersicht)
    {
        cout << i+1 << "# - " << vData[i] << endl;
    }
    cout <<  "------------------------" << endl;
}
//==============================================================================

//==============================================================================
//void CollectData ()
//rumdimentäre Eingabe der beiden Datensätze und einfügen in den Vektor!
//==============================================================================
void Client::collectData ()
{
    cout << "Eingabe der Datens" << ae << "tze:" << endl;
    char cBuf1[512],cBuf2[512];
    cout << "Datensatz 1 - Interpret: ";
    cin.getline(cBuf1,512);
    cout << "Datensatz 1 - Titel: ";
    cin.getline(cBuf2,512);
    string cBuf = "Interpret: ";
    cBuf += cBuf1;
    cBuf += " Titel: ";
    cBuf += cBuf2;
    vData.push_back(cBuf);  // Einfügen des Datensatzes / Strings
    memset(&cBuf1,0,512);   // Buffer freiräumen
    memset(&cBuf2,0,512);   // Buffer freiräumen
    cout << "Datensatz 2 - Interpret: ";
    cin.getline(cBuf1,512);
    cout << "Datensatz 2 - Titel: ";
    cin.getline(cBuf2,512);
    cBuf = "Interpret: ";
    cBuf += cBuf1;
    cBuf += " Titel: ";
    cBuf += cBuf2;
    vData.push_back(cBuf);   // Einfügen des Datensatzes / Strings
    cout << endl;
    cout << "------------------------" << endl << "#1 - " << vData[0] << endl << "#2 - " << vData[1] << endl << "----------" << endl << endl;
}
//==============================================================================


//==============================================================================
//void CollectData ()
//rumdimentäre Eingabe der beiden Datensätze und einfügen in den Vektor!
//==============================================================================
int Client::kbhit(void) {
    struct termios term, oterm;
    int fd = 0;
    int c = 0;
    tcgetattr(fd, &oterm);
    memcpy(&term, &oterm, sizeof(term));
    term.c_lflag = term.c_lflag & (!ICANON);
    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 1;
    tcsetattr(fd, TCSANOW, &term);
    c = getchar();
    tcsetattr(fd, TCSANOW, &oterm);
    if (c != -1)
        ungetc(c, stdin);
    return ((c != -1) ? 1 : 0);
}
//==============================================================================
