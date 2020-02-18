/**
 * Copyright 2016 Tim Lindquist,
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Purpose: AlbumsCollection is a class defining the interface between clients
 * and the server. The server implementation of AlbumsCollection
 * provides storage for description of multiple Albumss
 *
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version July 2016
 */

#include "AlbumsCollection.hpp"
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>

using namespace std;
AlbumsCollection::AlbumsCollection(){
   resetFromJsonFile("media.json");
}

AlbumsCollection::~AlbumsCollection() {
   //cout << "Albums collection destructor.\n";
   library.clear();
}

bool AlbumsCollection::resetFromJsonFile(string jsonFileName){
   bool ret = false;
   Json::Reader reader;
   Json::Value root;
   std::ifstream json(jsonFileName.c_str(), std::ifstream::binary);
   bool parseSuccess = reader.parse(json,root,false);
   if(parseSuccess){
      Json::Value::Members mbr = root.getMemberNames();
      for(vector<string>::const_iterator i = mbr.begin(); i!= mbr.end(); i++){
         //cout << *i << " " << endl;
        Json::Value jsonMedia = root[*i];
         Albums * aAlbums = new Albums(jsonMedia);
         library[*i] = *aAlbums;
         //cout << "adding ";
         //aAlbums->print();
      }
      ret = true;
   }
   return ret;
}

bool AlbumsCollection::saveToJsonFile(string jsonFileName){
   bool ret = false;
   Json::Value jsonLib;
   for(std::map<string,Albums>::iterator i = library.begin();
                                              i != library.end(); i++){
      string key = i->first;
      //cout << key << " " << endl;
      Albums aAlbums = library[key];
      Json::Value jsonAlbums = aAlbums.toJson();
      jsonLib[key] = jsonAlbums;
   }
   ret = true;
   Json::StyledStreamWriter ssw("  ");
   std::ofstream jsonOutFile(jsonFileName.c_str(), std::ofstream::binary);
   ssw.write(jsonOutFile, jsonLib);
   return ret;
}

bool AlbumsCollection::add(const Json::Value& aAlbumsJson){
   bool ret = false;
   Albums aAlbums(aAlbumsJson);
   // The map container does not allow duplicates so adding will replace existing
   // value with new Albums instance.
   //aAlbums.print();
   library[aAlbums.name]=aAlbums;
   return true;
}

bool AlbumsCollection::remove(string aName){
   bool ret = false;
   // does the library contain this Albums? if so remove, if not return false
   if(library.count(aName)>0){
      library.erase(aName);
      ret = true;
   }
   return true;
}

Json::Value AlbumsCollection::get(string aName){
   Albums aAlbums("{\"name\":\""+aName+" Unknown\",\"Albumsid\":-1,\"takes\":[\"None\" ]}");
   if(library.count(aName)>0){
         aAlbums = library[aName];
   }
   return aAlbums.toJson();
}

Json::Value AlbumsCollection::getNames(){
   Json::Value ret(Json::arrayValue);
   vector<string> myVec;
   for(map<string,Albums>::iterator it = library.begin();
                                     it != library.end(); ++it){
      myVec.push_back(it->first);
   }
   for(std::vector<string>::iterator it = myVec.begin(); it!=myVec.end();++it) {
      ret.append(Json::Value(*it));
   }
   return ret;
}

std::string AlbumsCollection::getById(int album_id){
   std::string ret="";
   for(map<string,Albums>::iterator it = library.begin();
                                              it != library.end();++it){
      Albums stud = library[it->first];
      if(stud.album_id == album_id){
         ret = stud.name;
      }
   }
   return ret;
}
