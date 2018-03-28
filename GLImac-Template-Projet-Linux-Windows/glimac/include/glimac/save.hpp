#ifndef SAVE_HPP
#define SAVE_HPP


#include <../include/glimac/tinyxml.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <../include/glimac/cube.hpp>
#include <../include/glimac/chunk.hpp>
#include <../include/glimac/map.hpp>
#include <../include/glimac/player.hpp>
#include <../include/glimac/camera.h>
#include <../include/glimac/character.hpp>
#include "glm.hpp"
#include <math.h>
#include <../include/glimac/visualLoad.hpp>
#include <../include/glimac/SDLWindowManager.hpp>

using namespace std;


void saveGame(Player player,Character charact, Map& map, VisualLoad* visualLoad, glimac::SDLWindowManager* windowManager )  //Player player, Map map
{
    int compteur = 0;

    TiXmlDocument doc;
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "UTF-8", "" );
    doc.LinkEndChild( decl );

    TiXmlElement * world = new TiXmlElement( "Game" );
    doc.LinkEndChild( world );

    /*TiXmlText * text = new TiXmlText( "World" );
    element->LinkEndChild( text );*/
    glm::vec3 pos=charact.getPosition();
    cout<<"camera pos x="<<pos.x<<" y="<<pos.y<<" z="<<pos.z<<endl;
    // player
    TiXmlElement * p;
    p = new TiXmlElement( "Player" );
    p->SetAttribute("Life", player.getLife());
    p->SetAttribute("PositionX", pos.x);
    p->SetAttribute("PositionY", pos.y);
    p->SetAttribute("PositionZ", pos.z);
    world->LinkEndChild(p);

    // map
    TiXmlElement * m;
    m = new TiXmlElement( "Map" );
   /* m->SetAttribute("seed", player.getLife());
    m->SetAttribute("PositionX", pos.x);
    m->SetAttribute("PositionY", pos.y);
    m->SetAttribute("PositionZ", pos.z);*/


    world->LinkEndChild(m);

    //chunk

    glm::vec3 NbChunkTmp = map.getNbChunk();
    int NbChunkTotal = (NbChunkTmp.x*NbChunkTmp.y)*NbChunkTmp.z;

   // int ChunkSize = pow(map.getNbCubeByRowByChunk(),3);
    vector<Chunk*> AllChunk = map.getAllChunk();
    TiXmlElement * ch;
    TiXmlElement * cube;
    for(int i=0; i<NbChunkTotal; i++)
    {
        glm::vec3 PosChunk = AllChunk[i]->getPosition();
        ch = new TiXmlElement( "Chunk" );
        ch->SetAttribute("PositionX", PosChunk.x);
        ch->SetAttribute("PositionY", PosChunk.y);
        ch->SetAttribute("PositionZ", PosChunk.z);
        ch->SetAttribute("NbCube",AllChunk[i]->getNbCube());
        ch->SetAttribute("NbCubeAff",AllChunk[i]->getNbCubeAff());
        ch->SetAttribute("NbTextureCube",AllChunk[i]->getNbTextureCube());
        m->LinkEndChild(ch);

        vector<Cube*> CubeByChunk = AllChunk[i]->getAllCube();
        //cube
        for(int j=0; j<CubeByChunk.size();j++)
        {
            cube = new TiXmlElement( "Cube" );

            if(CubeByChunk[j]!=NULL)
            {
                glm::vec3 PosCube=CubeByChunk[j]->getPosition();

                cube->SetAttribute("PositionX", PosCube.x);
                cube->SetAttribute("PositionY", PosCube.y);
                cube->SetAttribute("PositionZ", PosCube.z);
                cube->SetAttribute("IDText", CubeByChunk[j]->getTextureIndice());
                ch->LinkEndChild(cube);
            }
            else{
                cube->SetAttribute("NULL", "");
                ch->LinkEndChild(cube);
            }

            if(compteur%1000==0 && visualLoad != NULL && windowManager!=NULL)
            {
                visualLoad->Draw(((float)(compteur)/(float)(NbChunkTotal*CubeByChunk.size())), *windowManager, 1);
            }
            compteur++;

        }
    }





    doc.SaveFile( "save.xml" );
}
/*
int main(void)
{
    cout<<"ok"<<endl;
    saveGame();
    cout<<"ok2"<<endl;
    return 0;
}
*/



/*#include <include/tinyxml.h>
#include <iostream>
#include <stdio.h>
#include <vector>


using namespace std;

class user{
    public:
        string name, pass;
        int droits;
};


int main(void)
{
vector<user> user_list;


    TiXmlDocument doc("../test_xml/tinyFile.xml");
    if(!doc.LoadFile()){
        cerr << "erreur lors du chargement" << endl;
        cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << endl;
        return 1;
    }


    TiXmlHandle hdl(&doc);
    TiXmlElement *elem = hdl.FirstChildElement().FirstChildElement().Element();

    user cl;
    if(!elem){
        cerr << "le noeud à atteindre n'existe pas" << endl;
        return 2;
    }

    while (elem){
        cl.name = elem->Attribute("name");
        cl.pass = elem->Attribute("pass");
        elem->QueryIntAttribute("indice", &cl.droits);
        user_list.push_back(cl);

        elem = elem->NextSiblingElement(); // iteration
    }

    for(int i=0; i<user_list.size(); i++)
    {
        cout << user_list[i].name << " " << user_list[i].pass << " " << user_list[i].droits << endl;
    }
    return 0;
}

*/
/*
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "include/rapidxml.hpp"

using namespace rapidxml;
using namespace std;

int main(void)
{
    cout << "Parsing my beer journal..." << endl;
    xml_document<> doc;
    xml_node<> * root_node;

    // Read the xml file into a vector
    ifstream theFile("../test_xml/file.xml");

    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    // Parse the buffer using the xml file parsing library into doc
    doc.parse<0>(&buffer[0]);

    // Find our root node
    root_node = doc.first_node("MyBeerJournal");
    xml_node<> * brewery_node = root_node->first_node("Brewery");

    // Iterate over the brewerys
    for (xml_node<> *brewery_node = root_node->first_node("Brewery"); brewery_node; brewery_node = brewery_node->next_sibling())
    {
        cout<<"I have visited "<<brewery_node->first_attribute("name")->value()<<" in "<<brewery_node->first_attribute("location")->value()<<endl;
        // Interate over the beers
        for(xml_node<> * beer_node = brewery_node->first_node("Beer"); beer_node; beer_node = beer_node->next_sibling())
        {
            printf("On %s, I tried their %s which is a %s. ",
            beer_node->first_attribute("dateSampled")->value(),
            beer_node->first_attribute("name")->value(),
            beer_node->first_attribute("description")->value());
            printf("I gave it the following review: %s", beer_node->value());
        }
        cout << endl;
    }
}*/




/*

#include <iostream>
#include <fstream>
#include <strstream>
#include "include/rapidxml.hpp"
#include "include/rapidxml_utils.hpp"

using namespace std;

int main()
{
    int tab[4][4];

    for(int j = 0; j<4; j++)
    {
        for(int i = 0; i< 4; i++)
        {
            tab[i][j]=i;
        }
    }

    for(int j = 0; j<4; j++)
    {
        for(int i = 0; i< 4; i++)
        {
          cout << tab[i][j];
        }
        cout << endl;
    }

   rapidxml::xml_document<> doc;
    std::ifstream file("file_xml.xml",ios::in);
    std::stringstream buffer (stringstream::in | stringstream::out);;
    buffer << file.rdbuf();
    file.close();
    std::string content(buffer.str());
    doc.parse<0>(&content[0]);

    rapidxml::xml_document<> xmlFile("file_xml.xml"); // Default template is char
        rapidxml::xml_document<> xmlFile;
        doc.parse<0>(xmlFile.data());


    cout << "Hello World!" << endl;
    return 0;
}*/



#endif // SAVE_HPP
